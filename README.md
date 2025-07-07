# WarCache
This repository is an open-source artifact of WarCache. WarCache is an automated code refactoring framework based on specific runtime cache features.

## Installation
1. Install the required WebAssembly environment. You can refer to the following links to install the environment one by one.
- [Wasmer](https://github.com/wasmerio/wasmer)
- [Wasmtime](https://github.com/bytecodealliance/wasmtime)
- [Wasm3](https://github.com/wasm3/wasm3)
- [WAMR](https://github.com/bytecodealliance/wasm-micro-runtime)
- [WasmEdge](https://github.com/WasmEdge/WasmEdge)
- [wasi-sdk](https://github.com/WebAssembly/wasi-sdk)
2. Clone the repository
```bash
git clone https://github.com/lianzhilu/warcache.git
cd warcache
```

3. Install the required dependencies
```bash
pip install -r requirements.txt
```

4. Set the environment variables
```bash
export API_KEY=your_api_key
export WASI_PATH=/path/to/wasi-sdk/bin
```

## Usage
### Cache Friendliness Scoring
To score programs based on a specific cache feature, run the following command:
```bash
python score.py --feature <cache_feature> --input <program_dir> --output <score_output_dir>
```
- <cache_feature>: The cache feature to evaluate against (e.g., "lru", "lfu", "mru", "fifo").
- <program_dir>: The directory containing the programs to be scored.
- <score_output_dir>: The directory to store the scoring results.

### Code Refactoring
To refactor programs to be more cache-friendly and compatible with a specific WASM runtime, run the following command:
```bash
python refactor.py --feature <cache_feature> --runtime <wasm_runtime> --input <program_dir> --output <refactor_output_dir>
```
- <cache_feature>: The cache feature to optimize for (e.g., "lru", "lfu", "mru", "fifo").
- <wasm_runtime>: The target WASM runtime (e.g., "wasmer", "wasmtime", "wasm3", "wamr", "wasmedge").
- <program_dir>: The directory containing the programs to be refactored.
- <refactor_output_dir>: The directory to store the refactored programs and response files.
