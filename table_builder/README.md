# table_builder

Tooling that turns a set of DAG patterns into the generated KnownBits
transfer-function dispatcher that `llvm/lib/Analysis/ValueTracking.cpp`
`#include`s (`Generated/KnownBitsPatternDispatch.inc`).

A *pattern* is an expression like `Add(arg0, And(arg1, arg2))`. Each pattern
gets a C++ `Pattern<id>::solution()` that, given the known bits of its leaves,
returns sharper known bits for the whole expression. A **stub** transformer is
one whose `solution()` just returns top (all bits unknown) — it wires the
pattern into the dispatcher without improving precision, which is enough to
measure which patterns fire on real code.

`generate_pattern_dispatcher.py` and `gen_optimized.py` live at the repo root,
not in this folder; run all commands from the repo root.

## Generate stub transformers and compile with them

```bash
# 1) pattern list -> transformer folder (patterns.json + inc/pattern_*.inc),
#    every solution() a stub that returns top.
python3 table_builder/build_stub_xfer.py \
    --tsv table_builder/tests/top_10_pattern.tsv \
    --out-dir /tmp/xfer

# 2) transformer folder -> KnownBitsPatternDispatch.inc, copied into
#    llvm/lib/Analysis/Generated/ (omit --generated-dir to write that real tree;
#    pass one to write a scratch copy instead).
python3 generate_pattern_dispatcher.py /tmp/xfer

# 3) rebuild opt with the regenerated dispatcher.
ninja -C build opt

# 4) run -O3 over the benchmark suite; aggregate per-pattern stats.
python3 gen_optimized.py ~/repos/llvm-opt-benchmark \
    --opt build/bin/opt --stats outputs/stats.json
```


Smoke test for steps 1–2:

```bash
python3 table_builder/tests/test_stub_xfer_dispatch.py
```

## ⚠️ Work in progress

**`build_xfer_from_table.py`** and **`prune_table.py`** are the lookup-table solutions instead of stubs and are **not wired into the working
flow yet**. 
