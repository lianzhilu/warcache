import subprocess
from pathlib import Path
from typing import List, Tuple

from config import RUNTIME_COMMANDS, PROGRAM_PATH, REFACTOR_RESULT_PATH, WASI_PATH


def get_files_with_suffix(directory: str, suffix: str) -> List[str]:
    return [str(f) for f in Path(directory).rglob(f'*{suffix}') if f.is_file()]

def get_files_tuple_with_suffix(directory: str, suffix: str) -> List[Tuple[str, str, str]]:
    return [
        (str(file), file.name, file.stem)
        for file in Path(directory).rglob(f'*{suffix}')
        if file.is_file()
    ]

def run_compile_command(program_path: str, program_name: str):
    if program_path.endswith("c"):
        wasi_path = f"{WASI_PATH}/clang"
    else:
        wasi_path = f"{WASI_PATH}/clang++"
    cmds = [wasi_path, program_path, "-o", f"{REFACTOR_RESULT_PATH}/{program_name}.wasm"]
    result = subprocess.run(cmds, capture_output=True, text=True)
    return result

def run_execute_command(program_path: str, runtime: str):
    cmds = RUNTIME_COMMANDS.get(runtime.lower())
    cmds.append(program_path)
    result = subprocess.run(cmds, capture_output=True, text=True)
    return result

def format_compile_result(program_path: str, program_name: str) -> (bool, str):
    result = run_compile_command(program_path, program_name)
    if result.returncode != 0:
        return False, f"compile failed due to: {result.stderr}"
    return True, "compile success"

def format_execute_result_comparison(program_name: str, runtime: str) -> (bool, str):
    reference_output_path = f"{PROGRAM_PATH}/{program_name}.reference_output"
    with open(reference_output_path, mode="r") as f:
        reference_output = f.read()
    result = run_execute_command(f"{REFACTOR_RESULT_PATH}/{program_name}.wasm", runtime)
    if result.returncode != 0:
        return False, f"execute failed due to: {result.stderr}"
    refactor_output = result.stdout
    if refactor_output not in reference_output:
        return False, f"execute result is not expected. reference output is {reference_output}, actual output is {refactor_output}"
    return True, "refactor success"


if __name__ == "__main__":
    # result = get_files_tuple_with_suffix("./program", "c")
    # print(result)

    # result = run_execute_command("wasmer", "refactor/fib.wasm")
    # print(result)

    flag, msg = format_execute_result_comparison("fib2", "wasmer")
    print(flag, msg)