#!/usr/bin/env python3
import tempfile
from typing import Tuple
import subprocess
from rich.progress import Progress, SpinnerColumn, TextColumn
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
  def __init__(self, problem: str, test: str, bug: bool, crpt: bool, tag: str = ""):
    bug |= crpt
    self.problem = problem
    self.test = test
    self.tag = tag
    self.bug = bug
    self.crpt = crpt

  def save(self):
    if not os.path.exists(STAT_FILE):
      with open(STAT_FILE, "w") as f:
        writer = csv.writer(f)
        writer.writerow(["Problem", "Test", "Bug", "Crpt", "Tag"])
    with open(STAT_FILE, "a") as f:
      writer = csv.writer(f)
      writer.writerow([self.problem, self.test, self.bug, self.crpt, self.tag])


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
  with Progress(
    SpinnerColumn(),
    TextColumn("[progress.description]{task.description}"),
    transient=True,
  ) as progress:
    task = progress.add_task(f"{template}: running test {os.path.basename(file)}", total=None)
    rc = os.system("make docker 2>&1 > /dev/null")
    progress.update(task, completed=1)
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


def run_test(template: str, file: str, tag: str = ""):
  run_klee(template, file)
  bug, crpt = validate_output()
  if crpt:
    typer.echo(f"{template}: corruption detected in {file}")
  if bug:
    typer.echo(f"{template}: bug detected in {file}")
  result = TestResult(template, file, bug, crpt, tag)
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


def generate(llm: LLM, template: str, inject: str = "", tag: str = "") -> None:
  for _ in range(RETRIES):
    try:
      if not os.path.exists(f"template/{template}"):
        raise ValueError(f"Template '{template}' not found")
      file, code = llm.generate(template, inject=inject, tag=tag)
      verbose(f"Generated code:\n{code}")
      if not test_build(code):
        RuntimeError("Failed to build")
      run_test(template, file, tag)
    except Exception as e:
      typer.echo(f"Failed to generate test: {e}")
      continue
  else:
    typer.echo(f"Failed to generate test after {RETRIES} attempts")


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

  if test:
    try:
      run_test(template, test)
    except Exception as e:
      typer.echo(f"Error running test {test}: {e}")
    return

  for test in os.listdir(f"template/{template}/tests"):
    try:
      run_test(template, test)
    except Exception as e:
      typer.echo(f"Error running test {test}: {e}")


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
  for _ in range(rounds):
    if llm:
      generate(CLIENTS[llm], template)
    else:
      for client in CLIENTS.values():
        generate(client, template)


def cli():
  app()


if __name__ == "__main__":
  cli()
