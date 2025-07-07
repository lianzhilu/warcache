import os

from jinja2 import Template

# api_key
API_KEY = os.environ.get("API_KEY")

# base system_prompt
BASE_SCORE_SYSTEM_PROMPT = """
You are an expert in evaluating the cache friendliness of programs.
Your task is to analyze a user-provided program and assess whether its behavior aligns with workloads characterized by {{cache_feature}}, assigning a cache friendliness score accordingly.

The score should be an integer between 0 and 10, where a higher score indicates that the program exhibits stronger characteristics of {{cache_feature}}.

Use the following criteria to guide your evaluation:
- If the program clearly exhibits cache-unfriendly patterns, assign a score between 0 and 2.
- If the program demonstrates poor locality that could be improved through structural refactoring, assign a score between 3 and 5.
- If the program is generally compatible with the cache feature but requires minor adjustments, assign a score between 6 and 7.
- If the program fully utilizes the cache feature with no need for further optimization, assign a score between 8 and 10.

Based on this evaluation, provide a final score from 0 to 10. 

Please follow the format below in your response:
<score>score</score>
<reason>reason</reason>
"""

BASE_REFACTOR_SYSTEM_PROMPT = """
You are a code refactoring expert specializing in transforming programs to align with {{cache_feature}} characteristics.
Your task is to refactor a given program to make it more friendly to the {{wasm_runtime}} runtime environment.

The target runtime, {{wasm_runtime}}, has the following properties:
- Empirical studies show that code patterns resembling LRU-like workload behavior tend to perform better on this runtime. You should leverage the {{cache_rule}} guidelines to guide your refactoring accordingly.
- The runtime inherently follows the WebAssembly (WASM) memory model, including linear memory management, explicit allocation. Your refactoring must take these into account to ensure memory correctness, efficiency, and safety.

The user will provide a complete, runnable program. Your task is to refactor it with the following constraints:
- Do not modify the function signatures, including their names, types, or arguments.
- Do not use any transformations or operations that are not specified in the given rules.

Apply the refactoring conservatively and ensure the result remains functionally equivalent while being more compatible with the target runtime.
In the end, you only need to output the complete refactoring code without outputting other content
"""

# model
SCORE_MODEL = "deepseek-reasoner"
REFACTOR_MODEL = "deepseek-reasoner"

# cache feature
CACHE_FEATURE = ["LRU", "LFU", "MRU", "FIFO"]

# cache rule
CACHE_RULE = {
    "LRU": [
        "Merge the recurrent layer",
        "Reduce spanning access",
        "Separate the processing paths of cold and hot data",
    ],
    "LFU": [
        "Centralize high-frequency access data",
        "Reduce interpenetrating access to cold data",
        "Optimize the handling of burst access",
    ],
    "FIFO": [
        "Distribute hotspot data access",
        "Avoid continuously updating the same memory area",
        "Use access spacing strategies"
    ],
    "MRU": [
        "Reduce out-of-order access",
        "Increase queued data preload",
        "Sequential access optimization",
    ]
}

# default dir
PROGRAM_PATH = "./program"
SCORE_RESULT_PATH = "./score"
REFACTOR_RESULT_PATH = "./refactor"

# runtime command
RUNTIME_COMMANDS = {
    "wasmer": ["wasmer", "run"],
    "wasmtime": ["wasmtime"],
    "wasm3": ["wasm3"],
    "wamr": ["./iwasm"],
    "wasmedge": ["wasmedge"]
}

# export WASI_PATH. e.g. /xxx/xxx/wasi-sdk-xxx/bin
WASI_PATH = os.environ.get("WASI_PATH")


def get_cache_rule(feature: str) -> str:
    rules = CACHE_RULE.get(feature.upper())
    if not rules:
        return ""
    rule_str = ""
    for i, rule in enumerate(rules):
        rule_str += f"({i}) {rule} "

    return rule_str


def get_score_system_prompt(feature: str) -> str :
    return Template(BASE_SCORE_SYSTEM_PROMPT).render(
        cache_feature=feature,
    )

def get_refactor_system_prompt(feature: str, runtime: str) -> str:
    return Template(BASE_REFACTOR_SYSTEM_PROMPT).render(
        cache_feature=feature,
        wasm_runtime=runtime,
        cache_rule=get_cache_rule(feature),
    )