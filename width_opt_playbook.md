# Improving WidthOpt.cpp

## Introduction
    
First, familiarize yourself with our new LLVM width optimization pass,
you can find the implementation in `llvm/lib/Transforms/Scalar/WidthOpt.cpp`.

## Finding a Missed Optimization
    
You will be given a file containing one or more functions in LLVM
IR. These can be found in width-opt-tests. You are to optimize the
given file using `./build/bin/opt -O2` and then closely inspect the
resulting IR. Your goal is to see if you are able to optimize it
further. The optimization that you are looking for:

- Must result in the function becoming a total of at least one
  instruction smaller. But larger improvements are better.
- Must primarily involve the removal of LLVM's width-changing
  instructions: sext, zext, and trunc. Other instructions may be
  modified as necessary. You may even add new instructions but,
  again, your optimization must end up making the function
  smaller by at least one instruction, considering *all* instructions
  in the function, not just the instructions for width changes.
- Must be validated by `alive-tv --disable-undef-input`. This
  tool is in the path.
- *Must not* make unnecessary changes, such as changing a
  zext to a sext. This kind of change is only permitted if it
  if necessary to support removing an instruction.
- Should *NOT* involve removing a width-change that is going to
  be fed directly to a GEP instruction. This is not expected to
  be profitable.

If you cannot find such an optimization in the file you have been
given, then say so and return control to the user.

On the other hand, if you can find such an optimization, then you are
to devise a plan for implementing it. This implementation should be as
simple as possible and it should fit into the existing architecture of
WidthOpt.cpp. This is the only file that you are allowed to change.
You should take particular care when performing cost modeling. The
cost model is the part of the code that determines whether a proposed
optimization will be profitable. Every optimization that you implement
must either be obviously, structurally profitable, or else it must be
protected by a cost model check. Be particularly careful about extra
uses of SSA values; a common feature of cost model checks is calls to
hasOneUse().

Whenever possible, your new optimization should apply to vector values
as well as scalars. It is fine if there are some optimizations are
only apply to vectors, or only apply to scalars, but that is not the
expected case. Present this plan the user and then stop.

## Implementing the New Optimization
    
If you are in doubt about the actual profitability of a
transformation, you may use `llc` to lower both the original function
and also the transformed version to x86_64, riscv64, and aarch64
assembly. By looking at the generated code, you will be able to get
some information about whether the proposed transformation is a good
one.

If you are asked to proceed, then you should carry out the
implementation plan. Keep the code simple and when there are
conditions that you believe are impossible, place an assertion on that
condition.

After you have implemented the optimization, then you should add a new
unit test to the existing collection in llvm/test/Transforms/WidthOpt,
that is based upon the original file you were given.

## Extra Information

You may rapidly build just the part of LLVM that we care about using:
```
ninja -C build LLVMScalarOpts
ninja -C build opt
```

You may run just the tests for our new pass like this:
```
./build/bin/llvm-lit build/test/Transforms/WidthOpt/
```

To validate a transformation, save the optimized output to a file and
compare against the original:
```
./build/bin/opt -passes=width-opt -S input.ll -o /tmp/after.ll
alive-tv --disable-undef-input input.ll /tmp/after.ll
```

A correct transformation will print `Transformation seems to be
correct!`.
