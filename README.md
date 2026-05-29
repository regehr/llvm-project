# LLVM Pattern Mining Notes

This fork contains pattern mining support for integer expression DAGs seen by LLVM analysis code.

## DAGSlicer

The mining implementation lives in `llvm/lib/Analysis/DAGSlicer.cpp`.
This code is hooked and run from selected analyses in
`llvm/lib/Analysis/ValueTracking.cpp`.

`DAGSlicer` walks backward from an LLVM IR value and renders supported instruction DAGs as canonical pattern strings.
It supports scalar integer operations such as:

- integer binary operators, including `nuw`, `nsw`, `exact`, and `disjoint` flags;
- selected integer intrinsics such as min/max, saturating add/sub/shift, popcount, and ctlz;
- `icmp`, `select`, and bool/int casts needed to represent `i1` values inside integer DAGs.
- See `dsl.md` for a full list of supported ops

Pattern depth is measured as the maximum number of expanded instruction nodes on any root-to-leaf path.
Boundary values are rendered as `argN`.

Pattern mining allows `i1` values and one integer type `iN`.
Patterns that would require multiple distinct integer widths are rejected.

## ValueTracking Hooks

`ValueTracking.cpp` calls the miner from both `computeKnownBits` and
`computeConstantRange`:

```c++
DAGSlicer::recordPatterns(V, Depth, 2, MaxAnalysisRecursionDepth);
```

The effective mining depth is: `MaxAnalysisRecursionDepth - Depth`
This keeps mined patterns within the recursion budget that the current analysis
would use from the current query.
Such that the pattern miner only "sees" the instructions that the analysis
would during normal execution.

Both hooks are disabled by default. To enable known-bits mining, pass both the
known-bits mining flag and LLVM debug output for the slicer:

```bash
opt -O3 input.ll                     \
    -enable-knownbits-pattern-mining \
    -debug-only=dag-slicer           \
    -disable-output 2> patterns.txt
```

To enable constant-range mining instead:

```bash
opt -O3 input.ll                         \
    -enable-constantrange-pattern-mining \
    -debug-only=dag-slicer               \
    -disable-output 2> patterns.txt
```

Both flags can be used in the same run.
The debug output prints one pattern string per line.

## `pattern-miner`

`pattern-miner` is a standalone tool for mining patterns from every instruction in an input LLVM IR or bitcode module.
It aggregates pattern counts into a TSV file.
This is mostly used for testing the mining code.

Example:

```bash
pattern-miner --input=input.ll --output=patterns.tsv --depth=6
```

## Building

The tool is integrated into LLVM's CMake tool tree under `llvm/tools/pattern-miner`.

After configuring LLVM, build it with:

```sh
ninja -C build pattern-miner
```

It is also part of the normal LLVM tools build configured from this source tree.
