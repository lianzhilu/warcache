import argparse
import csv
import re

from openai import OpenAI

from config import API_KEY, PROGRAM_PATH, SCORE_RESULT_PATH, SCORE_MODEL, get_score_system_prompt
from util import get_files_tuple_with_suffix

client = OpenAI(api_key=API_KEY, base_url="https://api.deepseek.com")

parser = argparse.ArgumentParser(description="score")
parser.add_argument("--feature","-f", type=str, default="lru", help="cache feature")
parser.add_argument("--input","-i", type=str, default=PROGRAM_PATH, help="program dir")
parser.add_argument("--output","-o", type=str, default=SCORE_RESULT_PATH, help="cache scoring output dir")



if __name__ == "__main__":
    args = parser.parse_args()
    feature = args.feature
    input_dir = args.input
    output_dir = args.output

    file_tuples = get_files_tuple_with_suffix(input_dir, "c")

    with open(f"{output_dir}/score_results.csv", mode="w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["program", "score"])

    for file_path, file_name, program_name in file_tuples:
        with open(file_path, "r") as f:
            program_content = f.read()

        response = client.chat.completions.create(
            model=SCORE_MODEL,
            messages=[
                {
                    "role": "system",
                    "content": get_score_system_prompt(feature),
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

        score_response = f"<think>{thinking_content}</think> \n\n {response_content}"
        score_match = re.search(r"<score>(.*?)</score>", response_content)
        if score_match:
            score = score_match.grup(1)
        else:
            score = 0

        with open(f"{output_dir}/score_results.csv", mode="w", newline="", encoding="utf-8") as f:
            writer = csv.writer(f)
            writer.writerow([file_name, score])

        with open(f"{output_dir}/{program_name}.score_response", "w", newline="") as f:
            f.write(score_response)

