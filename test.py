#!/usr/bin/env python3
from rich.progress import (
  Progress,
  SpinnerColumn,
  BarColumn,
  TextColumn,
  TimeElapsedColumn,
)
from rich.console import Console
from rich.panel import Panel
from rich.syntax import Syntax
from rich import box
from typing import Tuple, List, Optional
from multiprocessing import Pool
from filelock import FileLock
from functools import partial
from time import sleep
import subprocess
import tempfile
import random
import shutil
import typer
import csv
import os
import re

from inject import INJECTS, Inject
from llm import Model, LLM, ChatGPT, Claude, DeepSeek


class KleeException(Exception):
  pass


VERBOSE = False


def verbose(msg: str):
  if VERBOSE:
    typer.echo(msg)


STAT_FILE = "statistic.csv"
CACHING = True


class TestResult:
  def __init__(self, problem: str, test: str, tag: str, bug: bool, crpt: bool, failed: bool):
    bug |= crpt
    bug &= not failed
    crpt &= not failed
    self.problem = problem
    self.llm = test[: test.find("_")]
    self.test = test
    self.tag = tag
    self.bug = bug
    self.crpt = crpt
    self.failed = failed

  def save(self):
    lock = FileLock(STAT_FILE + ".lock")

    with lock.acquire(timeout=10):
      if not os.path.exists(STAT_FILE):
        with open(STAT_FILE, "w") as f:
          writer = csv.writer(f)
          writer.writerow(["Problem", "LLM", "Test", "Tag", "Bug", "Crpt", "Failed"])
      with open(STAT_FILE, "a") as f:
        writer = csv.writer(f)
        writer.writerow(
          [self.problem, self.llm, self.test, self.tag, self.bug, self.crpt, self.failed]
        )


app = typer.Typer(context_settings={"help_option_names": ["-h", "--help"]})
DOCKER_TEMPLATE = """FROM klee/klee:3.0@sha256:8f76ff8a820e255b5b6503eb84dfb2e9e48dae52f46b6d82d1e67e492f63f2d2 AS builder

RUN mkdir /home/klee/build
WORKDIR /home/klee/build/
COPY ./template/{template}/ ./
COPY ./template/{template}/tests/{file}.c ./test.c
COPY ./Makefile ./

RUN make

FROM scratch AS export
COPY --from=builder /home/klee/build/out/bugs /"""


def check_klee():
  try:
    subprocess.run("klee --version", shell=True, check=True)
    return True
  except subprocess.CalledProcessError:
    return False


def run_klee(template: str, file: str):
  dockerfile = DOCKER_TEMPLATE.format(template=template, file=file)
  verbose(f"Dockerfile:\n{dockerfile}")
  if not os.path.exists(f"klee/{template}/{file}/template/{template}/tests"):
    os.makedirs(f"klee/{template}/{file}/template/{template}/tests")
    shutil.copy("Makefile", f"klee/{template}/{file}/")
    shutil.copy(
      f"template/{template}/tests/{file}.c", f"klee/{template}/{file}/template/{template}/tests"
    )
    shutil.copy(f"template/{template}/bugs.c", f"klee/{template}/{file}/template/{template}")
    shutil.copy(f"template/{template}/oracle.c", f"klee/{template}/{file}/template/{template}")

  with open(f"klee/{template}/{file}/Dockerfile", "w") as f:
    f.write(dockerfile)
  if VERBOSE:
    rc = os.system(
      f"make docker-verbose OUTDIR=klee/{template}/{file}/out DOCKERFILE=klee/{template}/{file}"
    )
  else:
    rc = os.system(
      f"make docker OUTDIR=klee/{template}/{file}/out DOCKERFILE=klee/{template}/{file} 2>&1 > /dev/null"
    )
  if rc != 0:
    raise KleeException("Failed to run KLEE")


def validate_output(file: str) -> Tuple[bool, bool]:
  if not os.path.exists(file):
    raise KleeException("Failed to generate output")

  with open(file, "r") as f:
    content = f.read()

  crpt = False
  if content.find("memory error") != -1:
    crpt = True

  bug = False
  if content.find("ASSERTION FAIL") != -1:
    bug = True

  return bug, crpt


def run_test(template: str, file: str, tag: str = ""):
  if not os.path.exists(f"klee/{template}/{file}/out"):
    try:
      run_klee(template, file)
    except KleeException as e:
      result = TestResult(template, file, tag, False, False, True)
      result.save()
      raise e
    if not os.path.exists(f"klee/{template}"):
      os.makedirs(f"klee/{template}", exist_ok=True)

  bug, crpt = validate_output(f"klee/{template}/{file}/out/messages.txt")
  if crpt and VERBOSE:
    typer.echo(f"{template}: corruption detected in {file}")
  if bug and VERBOSE:
    typer.echo(f"{template}: bug detected in {file}")
  result = TestResult(template, file, tag, bug, crpt, False)
  result.save()


COMPILE_TEMPLATE = "gcc -fPIC -fno-stack-protector {file} -shared -Wl,-z,relro,-z,now -o /dev/null"


def test_build(code: str) -> bool:
  tmp = tempfile.NamedTemporaryFile()
  tmp.write(code.encode())

  try:
    subprocess.run(COMPILE_TEMPLATE.format(file=tmp.name), shell=True, check=True)
    return True
  except subprocess.CalledProcessError:
    return False


RETRIES = 10


def generate(llm: LLM, template: str, inject: str = "", tag: str = "", skip: bool = False) -> None:
  file = None
  try:
    if not os.path.exists(f"template/{template}"):
      raise ValueError(f"Template '{template}' not found")
    for _ in range(RETRIES):
      try:
        file, code = llm.generate(template, inject=inject, tag=tag, verbose=VERBOSE)
        break
      except Exception:
        sleep(60 + random.randint(0, 60))
    else:
      return
    if not test_build(code):
      RuntimeError("Failed to build")
    try:
      if not skip:
        run_test(template, file, tag)
    except Exception as e:
      if VERBOSE:
        typer.echo(f"Failed to generate test: {e}")
  except Exception as e:
    if VERBOSE:
      typer.echo(f"Failed to generate test: {e}")
    if file != None:
      result = TestResult(template, file, tag, False, False, True)
      result.save()


@app.callback()
def main(
  verbose: bool = typer.Option(
    VERBOSE,
    "--verbose",
    "-v",
    help="verbose output",
  ),
  stats: str = typer.Option(
    STAT_FILE,
    "--stats",
    "-s",
    help="generate statistics",
  ),
) -> None:
  global VERBOSE, STAT_FILE
  VERBOSE = verbose
  STAT_FILE = stats


@app.command()
def list(
  template: str = typer.Option(
    None,
    "--template",
    "-t",
    help="specify template to list",
  ),
) -> None:
  """
  List available templates or tests within a template.
  """
  if not template:
    typer.echo("Available templates:")
    for template in os.listdir("template"):
      typer.echo(f"- {template}")
  else:
    print("Available Tests:")
    for test in os.listdir(f"template/{template}/tests"):
      typer.echo(f"- {test}")


def cache_worker(template: str, test: str):
  try:
    run_test(template, test, tag=test[test.find("_") + 1 : test.rfind("_")])
    return (test, None)
  except Exception as e:
    return (test, str(e))


@app.command()
def cache(
  template: str = typer.Argument(
    help="specify template to run cache on",
  ),
  test: str = typer.Option(
    None,
    "--test",
    "-t",
    help="specify test to run",
  ),
  forks: int = typer.Option(
    1,
    "--forks",
    "-f",
    help="specify number of forks to run",
  ),
):
  """
  run tests on cached entry in template
  """

  tests = [test] if test else [f[: f.find(".c")] for f in os.listdir(f"template/{template}/tests")]
  total = len(tests)

  with Progress(
    SpinnerColumn(),
    TextColumn("[bold blue]{task.description}"),
    BarColumn(),
    TextColumn("[progress.percentage]{task.percentage:>3.0f}%"),
    TimeElapsedColumn(),
  ) as progress:
    if not VERBOSE:
      task = progress.add_task(f"Round {template}", total=total)

    with Pool(processes=forks) as pool:
      for t, err in pool.imap_unordered(partial(cache_worker, template), tests):
        if err and VERBOSE:
          typer.echo(f"Error running test {t}: {err}")
        if not VERBOSE:
          progress.update(task, advance=1)




LLMS = {Model.claude: Claude, Model.chatgpt: ChatGPT, Model.deepseek: DeepSeek}

CLIENTS = dict()


def init_clients(llms: Optional[List[Model]] = None):
  if llms is None:
    for llm in LLMS.keys():
      CLIENTS[llm] = LLMS[llm]()
    return

  for llm in llms:
    CLIENTS[llm] = LLMS[llm]()


def run_worker(template: str, skip: bool, tags: List[Inject], name: Model = Model.chatgpt):
  try:
    generate(
      CLIENTS[name],
      template,
      inject="\n".join(INJECTS[tag] for tag in tags),
      tag="_".join(tag.value for tag in tags),
      skip=skip,
    )
    return (name, None)
  except Exception as e:
    return (name, str(e))


@app.command()
def run(
  template: str = typer.Argument(
    help="specify template to run cache on",
  ),
  llms: Optional[List[Model]] = typer.Option(
    None,
    "--llm",
    "-l",
    help="specify LLM to use",
  ),
  tags: Optional[List[Inject]] = typer.Option(
    [],
    "--tag",
    "-t",
    help="specify which injection to use",
  ),
  rounds: int = typer.Option(
    1,
    "--rounds",
    "-r",
    help="specify number of rounds to run",
  ),
  forks: int = typer.Option(
    1,
    "--forks",
    "-f",
    help="specify number of forks to run",
  ),
  skip: bool = typer.Option(
    False,
    "--skip",
    "-s",
    help="skip running tests",
  ),
) -> None:
  """
  Run tests for a template.
  """
  init_clients(llms)
  if llms:
    tasks = [llm for _ in range(rounds) for llm in llms]
  else:
    tasks = [name for _ in range(rounds) for name in CLIENTS.keys()]
  total = len(tasks)

  with Progress(
    SpinnerColumn(),
    TextColumn("[bold blue]{task.description}"),
    BarColumn(),
    TextColumn("[progress.percentage]{task.percentage:>3.0f}%"),
    TimeElapsedColumn(),
  ) as progress:
    task = None
    if not VERBOSE:
      task = progress.add_task(f"Running {template}", total=total)

    # Use pool to parallelize generate calls
    with Pool(processes=forks) as pool:
      for t, err in pool.imap_unordered(partial(run_worker, template, skip, tags), tasks):
        if err and VERBOSE:
          typer.echo(f"Error generating with {name}: {err}")
        if not VERBOSE:
          progress.update(task, advance=1)


def show_memory_corruption(result: TestResult, console: Console):
  if not os.path.exists(f"template/{result.problem}/tests/{result.test}.c"):
    console.print(f"[bold red]Test {result.test} not found[/]")
    return

  if not os.path.exists(f"klee/{result.problem}/{result.test}/out/messages.txt"):
    console.print(f"[bold red]KLEE output not found for {result.test}[/]")
    return

  console.print(
    Panel(
      f"[bold cyan]{result.test}[/]",
      title="Test Details",
      title_align="left",
      border_style="cyan",
      box=box.ROUNDED,
    )
  )

  error_found = False
  with open(f"klee/{result.problem}/{result.test}/out/messages.txt", "r") as f:
    for line in f:
      line_re = r"test.c:(\d+): memory error: (.*)\n$"
      m = re.search(line_re, line)
      if not m:
        continue

      error_found = True
      line_num = int(m.group(1))
      crpt = m.group(2)

      console.print(
        Panel(
          f"[bold red]{crpt}[/]",
          title="Memory Error",
          title_align="left",
          border_style="red",
          box=box.ROUNDED,
        )
      )

      # Read the entire file to show context
      with open(f"template/{result.problem}/tests/{result.test}.c", "r") as src_file:
        source_lines = src_file.readlines()

      # Get context (lines before and after)
      start_line = max(0, line_num - 3)
      end_line = min(len(source_lines), line_num + 2)

      # Extract the relevant lines with context
      context_lines = source_lines[start_line:end_line]

      # Create syntax highlighted code
      code = "".join(context_lines)
      syntax = Syntax(
        code, "c", line_numbers=True, start_line=start_line + 1, highlight_lines=[line_num]
      )

      console.print(
        Panel(
          syntax,
          title="Source Code Context",
          title_align="left",
          border_style="blue",
          box=box.ROUNDED,
        )
      )

  if not error_found:
    console.print("[yellow]No specific memory error details found in the output.[/]")


@app.command()
def analyze(
  template: str = typer.Argument(
    help="specify template to run cache on",
  ),
  llms: Optional[List[Model]] = typer.Option(
    None,
    "--llm",
    "-l",
    help="specify LLM to filter on",
  ),
  tags: Optional[List[Inject]] = typer.Option(
    [],
    "--tag",
    "-t",
    help="specify which injection to filter on",
  ),
):
  """
  Analyze memory corruption issues for a template.
  """
  console = Console()

  console.print(
    Panel(
      f"[bold]Analyzing memory corruption issues for [cyan]{template}[/cyan][/]",
      border_style="green",
    )
  )

  with open(STAT_FILE, "r") as f:
    reader = csv.reader(f)
    results = []
    # Skip header row
    next(reader, None)
    for row in reader:
      try:
        problem, llm_name, test, tag, bug, crpt, failed = row
        test = TestResult(
          problem,
          test,
          tag,
          bug == "True",
          crpt == "True",
          failed == "True",
        )

        if template != test.problem:
          continue

        if "_".join(t.value for t in tags) != test.tag:
          continue

        if llms and test.llm not in [l.value for l in llms]:
          continue

        if not test.crpt:
          continue

        results.append(test)
      except Exception as e:
        if VERBOSE:
          console.print(f"[red]Error processing row: {e}[/]")
          console.print(f"[dim]{row}[/]")

  if not results:
    console.print("[bold yellow]No memory corruption issues found matching the criteria.[/]")
    exit(0)

  console.print(f"[green]Found [bold]{len(results)}[/] memory corruption issues.[/]")

  console.print("[dim]Press enter to start[/]", end="")
  input()

  for i, result in enumerate(results):
    console.clear()
    console.rule(f"[bold]Issue {i + 1}/{len(results)}: {result.llm} - {result.tag}")
    show_memory_corruption(result, console)

    # Don't prompt for the last item
    if i < len(results) - 1:
      console.print("[dim]Press enter to continue...[/]", end="")
      input()

  console.print("[dim]Press enter to finish[/]", end="")
  input()

  console.print(Panel("[bold green]Analysis complete![/]", border_style="green"))


def cli():
  app()


if __name__ == "__main__":
  cli()
