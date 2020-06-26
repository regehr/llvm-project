This directory contains scripts used to qualify a hacked LLVM before
it is used by Souper.

The LLVM is hacked to disable some unsound optimizations (that
increase the amount of undefined behavior) and also to disable
peephole optimizations that Souper competes with, in order to answer
the research question "can Souper replace InstCombine and some of its
friends."

The strategy is to build three LLVMs:
1. all peepholes and unsound optimizations enabled by default
2. only unsound disabled by default
3. both peepholes and unsound disabled by default

Then to make sure that each of these can build a working LLVM that
passes all of its tests, and to make sure that each can build
benchmarks from SPEC CPU 2017, and those pass all their test.

TODO
- also test debug builds?

