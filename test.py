#!/usr/bin/env python3
import shutil
from time import sleep
import tempfile
from typing import Tuple
import subprocess
from rich.progress import (
  Progress,
  SpinnerColumn,
  BarColumn,
  TextColumn,
  TimeElapsedColumn,
)
import typer
import os
import csv
from llm import LLM, ChatGPT, Claude, DeepSeek
from enum import Enum


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
    bug &= failed
    crpt &= failed
    self.problem = problem
    self.test = test
    self.tag = tag
    self.bug = bug
    self.crpt = crpt
    self.failed = failed

  def save(self):
    if not os.path.exists(STAT_FILE):
      with open(STAT_FILE, "w") as f:
        writer = csv.writer(f)
        writer.writerow(["Problem", "Test", "Tag", "Bug", "Crpt", "Failed"])
    with open(STAT_FILE, "a") as f:
      writer = csv.writer(f)
      writer.writerow([self.problem, self.test, self.tag, self.bug, self.crpt, self.failed])


app = typer.Typer(context_settings={"help_option_names": ["-h", "--help"]})
DOCKER_TEMPLATE = """FROM klee/klee:3.0@sha256:8f76ff8a820e255b5b6503eb84dfb2e9e48dae52f46b6d82d1e67e492f63f2d2 AS builder

RUN mkdir /home/klee/build
WORKDIR /home/klee/build/
COPY ./template/{template}/ ./
COPY ./template/{template}/tests/{file}.c ./test.c
COPY ./Makefile ./

RUN make

FROM scratch AS export
COPY --from=builder /home/klee/build/out /"""


def check_klee():
  try:
    subprocess.run("klee --version", shell=True, check=True)
    return True
  except subprocess.CalledProcessError:
    return False


def run_klee(template: str, file: str):
  dockerfile = DOCKER_TEMPLATE.format(template=template, file=file)
  verbose(f"Dockerfile:\n{dockerfile}")
  with open("Dockerfile", "w") as f:
    f.write(dockerfile)
  if VERBOSE:
    rc = os.system("make docker-verbose")
  else:
    rc = os.system("make docker 2>&1 > /dev/null")
  if rc != 0:
    raise KleeException("Failed to run KLEE")


KLEE_LOG = "./out/bugs/messages.txt"


def validate_output() -> Tuple[bool, bool]:
  if not os.path.exists(KLEE_LOG):
    raise KleeException("Failed to generate output")

  with open(KLEE_LOG, "r") as f:
    content = f.read()

  crpt = False
  if content.find("memory error") != -1:
    crpt = True

  bug = False
  if content.find("ASSERTION FAIL") != -1:
    bug = True

  return bug, crpt


def run_test(progress: Progress, template: str, file: str, tag: str = ""):
  if not VERBOSE:
    task = progress.add_task("klee", total=1, transient=True)
  try:
    run_klee(template, file)
  except KleeException as e:
    raise e
  finally:
    if not VERBOSE:
      progress.update(task, completed=1, visible=False)
  bug, crpt = validate_output()
  if not os.path.exists(f"klee/{template}"):
    os.makedirs(f"klee/{template}", exist_ok=True)
  shutil.copytree("out/bugs", f"klee/{template}/{file}")
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


RETRIES = 3
def generate(progress: Progress, llm: LLM, template: str, inject: str = "", tag: str = "") -> None:
  file = None
  try:
    if not os.path.exists(f"template/{template}"):
      raise ValueError(f"Template '{template}' not found")
    if not VERBOSE:
      task = progress.add_task(f"{llm.name()}", total=1, transient=True)
    for _ in range(RETRIES):
      try:
        file, code = llm.generate(template, inject=inject, tag=tag)
        break
      except Exception:
        sleep(60)
      finally:
        if not VERBOSE:
          progress.update(task, completed=1, visible=False)
    else:
      return
    verbose(f"Generated code:\n{code}")
    if not test_build(code):
      RuntimeError("Failed to build")
    run_test(progress, template, file, tag)
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
  global VERBOSE
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
):
  """
  run tests on cached entry in template
  """

  total = 1 if test else len(os.listdir(f"template/{template}/tests"))
  with Progress(
    SpinnerColumn(),
    TextColumn("[bold blue]{task.description}"),
    BarColumn(),
    TextColumn("[progress.percentage]{task.percentage:>3.0f}%"),
    TimeElapsedColumn(),
  ) as progress:
    if not VERBOSE:
      task = progress.add_task(f"Round {template}", total=total)
    if test:
      try:
        run_test(progress, template, test)
      except Exception as e:
        if VERBOSE:
          typer.echo(f"Error running test {test}: {e}")
      if not VERBOSE:
        progress.update(task, advance=1)
      return

    for test in os.listdir(f"template/{template}/tests"):
      test = test[: test.find(".c")]
      try:
        run_test(progress, template, test)
      except Exception as e:
        if VERBOSE:
          typer.echo(f"Error running test {test}: {e}")
      if not VERBOSE:
        progress.update(task, advance=1)


class Model(str, Enum):
  claude = "claude"
  chatgpt = "chatgpt"
  deepseek = "deepseek"


CLIENTS = {
  Model.claude: Claude(),
  Model.chatgpt: ChatGPT(),
  Model.deepseek: DeepSeek(),
}


@app.command()
def run(
  template: str = typer.Argument(
    help="specify template to run cache on",
  ),
  llm: Model = typer.Option(
    None,
    "--llm",
    "-l",
    help="specify LLM to use",
  ),
  rounds: int = typer.Option(
    1,
    "--rounds",
    "-r",
    help="specify number of rounds to run",
  ),
) -> None:
  total = rounds * (1 if llm else len(CLIENTS))
  with Progress(
    SpinnerColumn(),
    TextColumn("[bold blue]{task.description}"),
    BarColumn(),
    TextColumn("[progress.percentage]{task.percentage:>3.0f}%"),
    TimeElapsedColumn(),
  ) as progress:
    if not VERBOSE:
      task = progress.add_task(f"Round {template}", total=total)

    for _ in range(rounds):
      if llm:
        generate(progress, CLIENTS[llm], template)
        if not VERBOSE:
          progress.update(task, advance=1)
      else:
        for client in CLIENTS.values():
          generate(progress, client, template)
          if not VERBOSE:
            progress.update(task, advance=1)


def cli():
  app()


if __name__ == "__main__":
  cli()
