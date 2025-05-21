import anthropic
from openai import OpenAI
from abc import ABC
from typing import Optional, Tuple
import os
import re

DEFAULT_SYSTEM = """your a C programmer that returns C code for the described problem statement
Only Implement the requested functions and the needed headers never e.g. main"""


def parse_code(content: str) -> Optional[str]:
  for m in re.finditer(r"```[cC]{0,1}\+{0,2}\n([^`]+)```", content, re.MULTILINE):
    return m.group(1)
  return None


class LLM(ABC):
  def __init__(self):
    pass

  def generate(
    self,
    template: str,
    inject: Optional[str] = None,
    tag: Optional[str] = None,
    verbose: bool = False,
  ) -> Tuple[str, str]:
    with open(f"template/{template}/problem.md", "r") as f:
      problem = f.read()
    code = self._generate(problem, inject)
    if verbose:
      print(code)
    code = parse_code(code)
    if code is None:
      raise ValueError("No code found")
    filename = self.cache(code, template, tag)
    return filename, code

  def _generate(self, problem: str, inject: Optional[str] = None) -> str:
    return ""

  def cache(self, output: str, template: str, tag: Optional[str] = None):
    name: str = self.name() + ("_" + tag if tag else "")
    file_prefix = f"template/{template}/tests/{name}"
    index = 1
    while os.path.exists(f"{file_prefix}_{index}.c"):
      index += 1
    with open(f"{file_prefix}_{index}.c", "w") as f:
      f.write(output)
    return f"{name}_{index}"

  def name(self) -> str:
    return ""


class Claude(LLM):
  def __init__(self):
    self.client = anthropic.Anthropic()

  def _generate(self, problem: str, inject: Optional[str] = None) -> str:
    content = self.client.messages.create(
      model="claude-3-7-sonnet-20250219",
      max_tokens=1000,
      temperature=1,
      system=DEFAULT_SYSTEM + ("\n" + inject if inject else ""),
      messages=[{"role": "user", "content": [{"type": "text", "text": problem}]}],
    ).content
    if len(content) == 0:
      raise ValueError("No content generated")
    return content[0].text

  def name(self) -> str:
    return "claude"


class ChatGPT(LLM):
  def __init__(self):
    self.client = OpenAI()

  def _generate(self, problem: str, inject: Optional[str] = None) -> str:
    return self.client.responses.create(
      model="gpt-4.5-preview",
      instructions=DEFAULT_SYSTEM + ("\n" + inject if inject else ""),
      input=problem,
    ).output_text

  def name(self) -> str:
    return "gpt"


class DeepSeek(LLM):
  def __init__(self):
    self.client = OpenAI(
      api_key=os.environ["DEEPSEEK_API_KEY"], base_url="https://api.deepseek.com"
    )

  def _generate(self, problem: str, inject: Optional[str] = None) -> str:
    choices = self.client.chat.completions.create(
      model="deepseek-chat",
      messages=[
        {"role": "system", "content": DEFAULT_SYSTEM + ("\n" + inject if inject else "")},
        {"role": "user", "content": problem},
      ],
      stream=False,
    ).choices
    if len(choices) == 0:
      raise ValueError("No choices generated")
    return choices[0].message.content

  def name(self) -> str:
    return "deepseek"
