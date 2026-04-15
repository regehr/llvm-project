# Improving WidthOpt.cpp

You will be given a file containing one or more functions in LLVM
IR. You are to inspect the file, and see if you are able to optimize it. The optimization that you are looking for:

- Must result in the function becoming a total of at least one
  instruction smaller. But larger improvements are better.
- Must primarily involve the removal of LLVM's width-changing
  instructions: sext, zext, and trunc. Other instructions may be
  modified as necessary. You may even add new instructions but,
  again, your optimization must end up making the function
  smaller by at least one instruction.
- Must be validated by `alive-tv --disable-undef-input`. This
  tool is in the path.
- *Must not* make unnecessary changes, such as changing a
  zext to a sext. This kind of change is only permitted if it
  if necessary to support removing an instruction.
- Should *NOT* involve removing a width-change that is going to
  be fed directly to a GEP instruction. This not not expected to
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
protected by a cost model check. Whenever possible, your new
optimization should apply to vector values as well as scalars. It is
fine if there are some optimizations are only apply to vectors, or
only apply to scalars, but that is not the expected case. Present
this plan the user and then stop.

If you are asked to proceed, then you should carry out the
implementation plan. Keep the code simple and when there are
conditions that you believe are impossible, place an assertion on that
condition.

After you have implemented the optimization, then you should add a new
unit test, that is based upon the original file you were given.
