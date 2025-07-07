import argparse

from openai import OpenAI

from config import API_KEY, PROGRAM_PATH, REFACTOR_RESULT_PATH, SCORE_MODEL, get_refactor_system_prompt
from util import get_files_tuple_with_suffix

client = OpenAI(api_key=API_KEY, base_url="https://api.deepseek.com")

parser = argparse.ArgumentParser(description="score")
parser.add_argument("--feature","-f", type=str, default="lru", help="cache feature")
parser.add_argument("--runtime","-r", type=str, default="wasmer", help="wasm runtime")
parser.add_argument("--input","-i", type=str, default=PROGRAM_PATH, help="program dir")
parser.add_argument("--output","-o", type=str, default=REFACTOR_RESULT_PATH, help="refactor output dir")

if __name__ == "__main__":
    args = parser.parse_args()
    feature = args.feature
    runtime = args.runtime
    input_dir = args.input
    output_dir = args.output

    file_tuples = get_files_tuple_with_suffix(input_dir, "c")

    for file_path, file_name, program_name in file_tuples:
        with open(file_path, "r") as f:
            program_content = f.read()

        response = client.chat.completions.create(
            model=SCORE_MODEL,
            messages=[
                {
                    "role": "system",
                    "content": get_refactor_system_prompt(feature, runtime),
                },
                {
                    "role": "user",
                    "content": program_content,
                },
            ],
            stream=True
        )

        thinking_content = ""
        response_content = ""

        for chunk in response:
            if not chunk.choices:
                continue
            if chunk.choices[0].delta.reasoning_content:
                thinking_content += chunk.choices[0].delta.reasoning_content
                print(chunk.choices[0].delta.reasoning_content, end="")
            else:
                response_content += chunk.choices[0].delta.content
                print(chunk.choices[0].delta.content, end="")

        refactor_response = f"<think>{thinking_content}</think> \n\n {response_content}"
        refactor_program = response_content

        with open(f"{output_dir}/{file_name}", "w", newline="") as f:
            f.write(refactor_program)

        with open(f"{output_dir}/{program_name}.refactor_response", "w", newline="") as f:
            f.write(refactor_response)

