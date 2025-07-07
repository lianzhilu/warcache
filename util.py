from pathlib import Path
from typing import List, Tuple


def get_files_with_suffix(directory: str, suffix: str) -> List[str]:
    return [str(f) for f in Path(directory).rglob(f'*{suffix}') if f.is_file()]

def get_files_tuple_with_suffix(directory: str, suffix: str) -> List[Tuple[str, str, str]]:
    return [
        (str(file), file.name, file.stem)
        for file in Path(directory).rglob(f'*{suffix}')
        if file.is_file()
    ]

if __name__ == "__main__":
    result = get_files_tuple_with_suffix("./program", "c")
    print(result)