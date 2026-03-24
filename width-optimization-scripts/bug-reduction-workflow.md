# Bug Reduction Workflow

This documents the process for reducing, fixing, and writing regression tests
for crashes found by the Csmith-based differential fuzzer (`csmith_fuzz.py`).

## 1. Obtain IR for the crashing input

When clang crashes on a Csmith-generated file, it leaves a preprocessed copy in
`/tmp/` (e.g. `/tmp/test-88083e.c`).  We need LLVM IR that reproduces the crash
under `opt` so that `llvm-reduce` can work on it.

Do **not** use `-O0` alone — that attaches `optnone` to every function, which
prevents the optimizer from running.  Do **not** use `-O1 -Xclang
-disable-llvm-passes` — that produces IR shaped by higher-level optimizations.
The right incantation is:

```sh
clang -w -O0 -Xclang -disable-O0-optnone -emit-llvm -S \
  -I/home/regehr/csmith/runtime \
  -I/home/regehr/csmith/build/runtime \
  /tmp/test-XXXXXX.c -o /tmp/test-XXXXXX-pre.ll
```

This emits IR without `optnone`, so the optimizer can run on it later.

## 2. Confirm the crash reproduces under opt

Our crashes occur inside `width-opt`, which runs as part of the standard
optimization pipeline.  Running the pass in isolation (`-passes=width-opt`)
often does not reproduce the crash because the pass expects earlier passes
(e.g. `mem2reg`) to have prepared the IR.  Use the full pipeline instead:

```sh
opt -passes='default<O3>' -S /tmp/test-XXXXXX-pre.ll -o /dev/null
```

Check that you see the same assertion message before proceeding.

**Shortcut check**: also try `opt -passes='width-opt'` on the pre-reduced IR
directly.  If it reproduces without the full pipeline, you can use that as the
interestingness test (faster reduction).  If it only fires under `default<O3>`,
use `-print-changed=diff-quiet` to identify which upstream pass transformed the
IR into the triggering form:

```sh
opt -passes='default<O3>' --print-changed=diff-quiet -S input.ll -o /dev/null 2>&1 \
  | grep -E "^\*\*\* IR Dump After|Running pass.*width-opt"
```

Once you know the upstream culprit (e.g. IPSCCPPass replacing a load with
`undef`), you can often craft a simpler interestingness-test IR directly — e.g.
replace `load i8, ptr null` with `undef` in the source — and verify that
`-passes='width-opt'` alone reproduces the crash.

## 3. Write an interestingness test

Create a small shell script that returns 0 when the crash is present and
non-zero otherwise.  Grep for a distinctive string from the assertion message:

```sh
#!/bin/bash
opt -passes='default<O3>' -S "$1" -o /dev/null 2>&1 | grep -q "Assertion.*cast"
```

Make it executable and verify it fires on the pre-reduced IR:

```sh
chmod +x /tmp/check-crash.sh
bash /tmp/check-crash.sh /tmp/test-XXXXXX-pre.ll && echo "works"
```

## 4. Run llvm-reduce

```sh
llvm-reduce --test=/tmp/check-crash.sh /tmp/test-XXXXXX-pre.ll -o /tmp/reduced.ll
```

`llvm-reduce` will iterate through a series of reduction passes (removing
functions, blocks, instructions, operands, etc.) and keep the smallest IR that
still satisfies the interestingness test.

## 5. Understand the reduced trigger

Read the reduced IR and the stack trace together.  The stack trace names the
function in `WidthOpt.cpp` where the crash occurs; the reduced IR shows the
minimal pattern that exercises it.

The build is optimized (`-O3 -UNDEBUG`), so function names in the stack trace
are often inlined away and only `WidthOptPass::run` is visible.  Use
`llvm-symbolizer` on the shared-library offsets:

```sh
llvm-symbolizer --obj=build/lib/libLLVMScalarOpts.so.23.0git <offset>
```

If the result is still `??:0:0` (debug info stripped), fall back to reasoning
from the reduced IR: identify which worklist (ZExts, SExts, Truncs, Compares,
component widening) would process the instructions in the reduced IR and trace
through the relevant functions manually.

**Example 1** — `zext i24→i32→i64` chain:
`tryShrinkZExtOfZeroBounded` called `B.CreateZExt(NarrowSrc, WideType)`.  When
the IRBuilder constant-folded the result to a non-`Instruction`, `cast<Instruction>`
asserted.

**Example 2** — `sext i8 undef to i16 → sext i16 to i32` chain:
The singleton component widening path called `B.CreateSExt(undef, TargetTy)`,
which folded to `UndefValue`.  The `undef` was introduced by IPSCCPPass
replacing a `load i8, ptr null` — spotted via `--print-changed=diff-quiet`.

## 6. Fix the crash

The common root cause for these crashes is that the pass was originally compiled
in release mode (assertions disabled), so `cast<T>` on an incompatible type
silently returned garbage.  Running as an internal pass with assertions enabled
surfaces these.

**Root causes seen so far:**

1. **`cast<Instruction>(B.Create*(...))` on a folded result.**  IRBuilder
   constant-folds results when operands are constants or `undef`.  For example:
   - `B.CreateZExt(undef, i32)` → `UndefValue` (not a `ZExtInst`)
   - `B.CreateSExt(undef, i32)` → `UndefValue`
   - `B.CreateICmp(pred, C1, C2)` → `ConstantInt` (not an `ICmpInst`)
   Fix: store the result as `Value *` and use `dyn_cast<Instruction>` only where
   the `Instruction*` interface is needed (e.g. `setDebugLoc`).

2. **`getValueWidth()` or `->getIntegerBitWidth()` called on vector types.**
   Both assert `isa<IntegerType>`.  Csmith programs include vector operations.
   Fix: add `if (!isIntegerValue(&inst)) return false;` before any such call.

3. **`isIntegerValue` check ordered after `getValueWidth` call.**  If
   `getValueWidth` appears before the `isIntegerValue` guard, vector types crash
   before the guard can fire.
   Fix: reorder so `isIntegerValue` is checked first.

**`undef` as a trigger**: upstream passes like IPSCCPPass replace loads from
null pointers with `undef`.  Any path in the pass that materializes a value from
`undef` and then passes it to `B.Create*` may fold to a non-`Instruction`.

After editing `WidthOpt.cpp`, rebuild and confirm:

```sh
ninja -C build LLVMScalarOpts
opt -passes='default<O3>' -S /tmp/reduced.ll -o /dev/null && echo "no crash"
```

## 7. Add a regression test

Create a `.ll` file under `llvm/test/Transforms/WidthOpt/` using the reduced IR
as a starting point.  Prefer a clean version that exercises the same code path
without relying on specific upstream-pass behavior:

- Replace `ptr null` with a pointer argument (avoids UB from null dereference).
- If the crash only fires after e.g. IPSCCPPass introduces `undef`, write the
  `undef` directly in the test IR — this makes the test self-contained and
  runnable with just `-passes='width-opt'`.

A no-crash test only needs a `RUN` line — no `CHECK` lines required.  Use
`-passes='width-opt'` when possible (faster, more targeted) and fall back to
`-passes='default<O3>'` only if the simpler pipeline does not exercise the bug:

```llvm
; RUN: opt -passes='width-opt' -S %s -o /dev/null
```

Run the new test to confirm it passes:

```sh
llvm-lit llvm/test/Transforms/WidthOpt/width-opt-YOUR-test-no-crash.ll
```

Then run the full suite to confirm no regressions:

```sh
llvm-lit -j$(nproc) llvm/test/Transforms/WidthOpt/
```

## 8. Working directory

Keep bug reduction artifacts in
`width-optimization-scripts/bugs/` (under the repo root) rather than `/tmp/`,
so the sandbox does not interrupt with permission prompts and files persist
across sessions.
