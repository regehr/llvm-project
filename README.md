# Good Company

This fork is based off of LLVM 22.1.4
And contains changes for the Eval of GoodCompany

## Pattern Mining

Mining Patterns from LLVM proceeds in several steps:

1. Slicing
2. Deduplication
3. Enumerate up to some size (right now 6 I think)
4. Refinement

### Slicing

The slicing implementation lives in `llvm/lib/Analysis/DAGSlicer.cpp`.
The hook to call the silcer is in `ValueTracking.cpp` which may call `DAGSlicer` from both `computeKnownBits` or `computeConstantRange`,
with `-enable-knownbits-pattern-mining`, or `-enable-constantrange-pattern-mining` flags respectivly.

`DAGSlicer` walks backward from an LLVM IR value and renders supported instruction DAGs as canonical pattern strings.
All supported operations are documented in [dsl.md](dsl.md).
The patterns should follow a canonical form, documented in [canonicalization_contract.md](canonicalization_contract.md).

Pattern depth is measured as the maximum number of expanded instruction nodes on any root-to-leaf path.
Boundary values are rendered as `argN`.

Pattern mining allows `i1` values and one integer type `iN`.
Patterns that would require multiple distinct integer widths are rejected.

To run this part of the pipeline:
```bash
python synth_xfer/llvm_eval/run_opt_benchmark.py \
    --bench-path ~/git/llvm-opt-benchmark        \
    --opt-path ~/git/llvm/llvm/build/bin/opt     \
    --slice-kb
```
or alternatively use `--slice-cr`

### Deduplication

**TODO** Xuanyu add the dedup script to synth-xfer
and explain the dedup algo

### Enumeration

 **TODO** Xuanyu add the enum script to synth-xfer
and explain the enum algo

### Refinement 

**TODO** Dominic add the refinement script to synth-xfer
and explain the refine algo

## Table Building

1. Select patterns to mine abstact inputs for, and generate stubs:
**TODO** Xuanyu, add table building script
2. Rebuild LLVM
3. Run the opt benchmark with the `--pattern-hist` flag:
```bash
python synth_xfer/llvm_eval/run_opt_benchmark.py \
    --bench-path ~/git/llvm-opt-benchmark        \
    --opt-path ~/git/llvm/llvm/build/bin/opt     \
    --pattern-hist outputs/pattern-hist          \
```
4. Calculate the max precise value for all table values: **TODO**
5. Shrink the tables: **TODO**

## Final Eval

Once the table transformers have been generated, and LLVM opt has been rebuilt run:
```bash
python synth_xfer/llvm_eval/run_opt_benchmark.py \
    --bench-path ~/git/llvm-opt-benchmark        \
    --opt-path ~/git/llvm/llvm/build/bin/opt     \
    --stats stats.json
```
for a breakdown of the KnownBits added.
