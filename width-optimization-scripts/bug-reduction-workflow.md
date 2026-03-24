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
minimal pattern that exercises it.  In the example case:

```llvm
define i64 @main() {
entry:
  %load = load i24, ptr null, align 1
  %ext1 = zext i24 %load to i32
  %ext2 = zext i32 %ext1 to i64
  ret i64 %ext2
}
```

A chain of two `zext`s through an intermediate width (`i24→i32→i64`) reached
`tryShrinkZExtOfZeroBounded`, where `IRBuilder::CreateZExt` was folded to a
non-`Instruction` value but the result was immediately passed to
`cast<Instruction>`, triggering the assertion.

## 6. Fix the crash

The common root cause for these crashes is that the pass was originally compiled
in release mode (assertions disabled), so `cast<T>` on an incompatible type
silently returned garbage.  Running as an internal pass with assertions enabled
surfaces these.

Typical fixes:

- Replace `cast<Instruction>(builder_call(...))` with the result stored as
  `Value *`, then use `dyn_cast<Instruction>` only where the `Instruction*`
  interface is actually needed (e.g. `setDebugLoc`).
- Add `if (!isIntegerValue(&inst)) return false;` guards before any call to
  `getValueWidth()` or `->getIntegerBitWidth()` when vector types could appear.

After editing `WidthOpt.cpp`, rebuild and confirm:

```sh
ninja -C build LLVMScalarOpts
opt -passes='default<O3>' -S /tmp/reduced.ll -o /dev/null && echo "no crash"
```

## 7. Add a regression test

Create a `.ll` file under `llvm/test/Transforms/WidthOpt/` using the reduced IR
as a starting point.  Prefer a clean, non-UB version (e.g. replace `ptr null`
with a pointer argument).  A no-crash test only needs a `RUN` line — no
`CHECK` lines required:

```llvm
; RUN: opt -passes='default<O3>' -S %s -o /dev/null
```

Run the new test to confirm it passes:

```sh
llvm-lit llvm/test/Transforms/WidthOpt/width-opt-YOUR-test-no-crash.ll
```

Then run the full suite to confirm no regressions:

```sh
llvm-lit -j$(nproc) llvm/test/Transforms/WidthOpt/
```
