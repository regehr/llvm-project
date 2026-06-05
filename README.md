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

TODO give the expected `gen_optimized.py` call to get the patterns

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

Once patterns are mined from LLVM, there is a pipeline to the final patterns.

TODO fill these out
### Deduplication
TODO add the dedup script to synth-xfer
### Enumeration
TODO add the enum script to synth-xfer
### Refinement 
TODO add the refinement script to synth-xfer

## Table Building

TODO

## Final Eval

TODO
