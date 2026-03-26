# Miscompilation Bug Playbook: C → IR → Fix

This document records the end-to-end workflow for finding, reducing, and fixing a
miscompilation in a custom LLVM-based compiler, using the following tools:

- `cvise` — source-level C reducer
- `llvm-reduce` — IR-level reducer
- `alive-tv` — translation-validation oracle
- `opt -passes='...'` — run individual LLVM passes (new pass manager only)
- `clang` (system) and `gcc` — reference compilers

The workflow was developed and validated on a real bug in `widthopt::WidthOptPass`
that incorrectly narrowed `trunc(smin.i32(x, 0))` to `smin.i8(trunc(x), 0)`.

---

## 0. Environment

```
LOCAL_CLANG=/path/to/build/bin/clang      # compiler under test
LOCAL_OPT=/path/to/build/bin/opt          # opt from the same build
ALIVE_TV=/path/to/alive2/build/alive-tv
```

System `clang` and `gcc` serve as reference compilers throughout.

---

## 1. Verify the Mismatch

Before doing anything else, confirm the miscompile is real and stable.
For Csmith-generated programs, always pass `-w` to suppress warnings.

```bash
CSMITH_INC=/path/to/csmith/runtime
CSMITH_BUILD=/path/to/csmith/build/runtime

$LOCAL_CLANG -w -O3 -I$CSMITH_INC -I$CSMITH_BUILD test.c -o prog_local
gcc           -w -O3 -I$CSMITH_INC -I$CSMITH_BUILD test.c -o prog_gcc

./prog_local > out_local.txt
./prog_gcc   > out_gcc.txt

diff out_local.txt out_gcc.txt
```

If `clang` (system) and `gcc` agree and both differ from `LOCAL_CLANG`, the signal
is almost certainly a real bug in the pass under development.

---

## 2. Reduce the C Test Case with C-Vise

### 2a. Preprocess First — No Exceptions

Never run `cvise` on raw source. Always preprocess to a single self-contained file.
This makes the reducer faster and avoids header-dependency problems.

```bash
mkdir -p /tmp/reduce && cd /tmp/reduce

# For a single-file csmith program:
$LOCAL_CLANG -E -P -std=c99 -I$CSMITH_INC -I$CSMITH_BUILD test.c > merged.pre.c

# Remove _Float* typedefs that can confuse some compilers:
sed -i \
  -e '/^typedef float _Float32;$/d' \
  -e '/^typedef double _Float64;$/d' \
  -e '/^typedef double _Float32x;$/d' \
  -e '/^typedef long double _Float64x;$/d' \
  merged.pre.c
```

### 2b. Write `interesting.sh`

The script must:

1. **Warning gates** — reject UB from format errors, uninitialized reads,
   pointer/integer comparisons.
2. **Sanitizer gates** — run system clang with ASan+UBSan; run gcc with UBSan.
   Both must be clean.
3. **Output comparison** — system clang and gcc must agree; local clang must differ.
4. **Use the local filename** `merged.pre.c`, not an argument or absolute path.
   (C-Vise runs the script from a temp directory with a copy of the candidate.)

```bash
#!/usr/bin/env bash
set -euo pipefail

LOCAL_CLANG="/path/to/build/bin/clang"
CAND="merged.pre.c"

rm -f prog_clang prog_gcc prog_gcc_ubsan prog_local \
  out_clang.txt out_gcc.txt out_local.txt \
  err_clang.txt err_gcc.txt err_gcc_ubsan.txt err_local.txt \
  warn_clang.log warn_gcc.log warn_analyze.log warn_gcc_analyze.log \
  warn_gcc.o warn_gcc_analyze.o \
  warn_ptrint_clang.log warn_ptrint_gcc.log warn_ptrint_gcc.o

# ── Warning gate: format + uninitialized (clang) ──────────────────────────────
timeout 30s clang -x c -std=c99 -O3 -fsyntax-only \
  -Wno-everything \
  -Wformat -Wformat-security -Wformat-extra-args \
  -Wformat-insufficient-args -Wformat-invalid-specifier \
  -Wformat-signedness \
  -Wuninitialized -Wconditional-uninitialized \
  -Wincompatible-library-redeclaration \
  -Wdeprecated-non-prototype \
  -Werror=format \
  -Werror=uninitialized -Werror=conditional-uninitialized \
  -Werror=incompatible-library-redeclaration \
  -Werror=deprecated-non-prototype \
  "$CAND" > warn_clang.log 2>&1 || exit 1

# ── Warning gate: uninitialized (gcc) ─────────────────────────────────────────
timeout 30s gcc -x c -std=c99 -O3 -c \
  -Wuninitialized -Wmaybe-uninitialized \
  -Werror=uninitialized -Werror=maybe-uninitialized \
  "$CAND" -o warn_gcc.o > warn_gcc.log 2>&1 || exit 1

# ── Analyzer gate: uninitialized (gcc -fanalyzer) ─────────────────────────────
timeout 45s gcc -x c -std=c99 -O0 -fanalyzer -c \
  -Wanalyzer-use-of-uninitialized-value \
  -Werror=analyzer-use-of-uninitialized-value \
  "$CAND" -o warn_gcc_analyze.o > warn_gcc_analyze.log 2>&1 || exit 1

# ── Warning gate: pointer-vs-integer (clang + gcc) ────────────────────────────
timeout 30s clang -x c -std=c99 -O3 -fsyntax-only \
  -Wall "$CAND" > warn_ptrint_clang.log 2>&1 || exit 1
timeout 30s gcc -x c -std=c99 -O3 -c \
  -Wall "$CAND" -o warn_ptrint_gcc.o > warn_ptrint_gcc.log 2>&1 || exit 1

if grep -qi \
  "ordered comparison between pointer and integer\|comparison between pointer and integer" \
  warn_ptrint_clang.log warn_ptrint_gcc.log; then
  exit 1
fi

# ── Clang static analyzer: uninitialized ──────────────────────────────────────
timeout 30s clang -x c -std=c99 -O0 --analyze \
  "$CAND" > warn_analyze.log 2>&1 || exit 1
if grep -q "core\.uninitialized\." warn_analyze.log; then
  exit 1
fi

# ── Build: system clang with ASan+UBSan (reference) ──────────────────────────
timeout 30s clang -x c -std=c99 -w -O3 \
  -fsanitize=address,undefined -fno-sanitize-recover=all \
  "$CAND" -o prog_clang > /dev/null 2>err_clang.txt || exit 1

# ── Build: gcc (reference) ────────────────────────────────────────────────────
timeout 30s gcc -x c -std=c99 -w -O3 \
  "$CAND" -o prog_gcc > /dev/null 2>err_gcc.txt || exit 1

# ── Build: gcc UBSan gate ─────────────────────────────────────────────────────
timeout 30s gcc -x c -std=c99 -w -O3 \
  -fsanitize=undefined -fno-sanitize=alignment -fno-sanitize-recover=all \
  "$CAND" -o prog_gcc_ubsan > /dev/null 2>err_gcc_ubsan.txt || exit 1

# ── Build: compiler under test ────────────────────────────────────────────────
timeout 30s "$LOCAL_CLANG" -x c -std=c99 -w -O3 \
  "$CAND" -o prog_local > /dev/null 2>err_local.txt || exit 1

# ── Run all binaries ──────────────────────────────────────────────────────────
timeout 5s env ASAN_OPTIONS=detect_leaks=0:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 \
  ./prog_clang > out_clang.txt 2>>err_clang.txt || exit 1
timeout 5s env UBSAN_OPTIONS=halt_on_error=1 \
  ./prog_gcc_ubsan > /dev/null 2>>err_gcc_ubsan.txt || exit 1
timeout 5s ./prog_gcc   > out_gcc.txt   2>>err_gcc.txt   || exit 1
timeout 5s ./prog_local > out_local.txt 2>>err_local.txt || exit 1

# ── All stderr must be empty ──────────────────────────────────────────────────
[ ! -s err_clang.txt ]     || exit 1
[ ! -s err_gcc.txt ]       || exit 1
[ ! -s err_gcc_ubsan.txt ] || exit 1
[ ! -s err_local.txt ]     || exit 1

# ── Reference compilers must agree ───────────────────────────────────────────
cmp -s out_clang.txt out_gcc.txt || exit 1

# ── Compiler under test must differ ──────────────────────────────────────────
! cmp -s out_gcc.txt out_local.txt || exit 1
```

### 2c. Validate Before Running C-Vise

Always validate in-place **and** in a temp directory before launching `cvise`:

```bash
chmod +x interesting.sh
./interesting.sh && echo "OK in-place"

DIR=$(mktemp -d)
cp merged.pre.c "$DIR/"
(cd "$DIR" && /path/to/reduce/interesting.sh) && echo "OK in temp dir"
rm -rf "$DIR"
```

Both must exit 0. If the temp-dir check fails, the script uses absolute paths or
assumes the working directory — fix it before proceeding.

### 2d. Run C-Vise

```bash
cvise --n 8 --timeout 30 ./interesting.sh merged.pre.c
```

Use `--n` to match your core count. Let it run to completion.

**Important:** When invoking `cvise` using the Claude Code Bash tool with
`run_in_background=true`, do **not** append `&` to the command. The tool handles
backgrounding. Adding `&` causes the shell to exit and kills `cvise`.
Also, prefix the command with `cd /your/reduce/dir &&` because the shell cwd
resets to the project root between tool calls.

---

## 3. Convert to LLVM IR and Reduce Further

### 3a. Generate Unoptimized IR

Use `-O1 -Xclang -disable-llvm-passes` to get mem2reg/canonicalization without
any of the optimization pipeline:

```bash
$LOCAL_CLANG -x c -std=c99 -w -O1 -Xclang -disable-llvm-passes \
  -emit-llvm -S reduced.c -o orig.ll
```

### 3b. Run the Optimization Pipeline and Check with Alive-TV

Strip metadata before feeding to `alive-tv` (TBAA etc. causes "Unsupported
metadata" errors):

```bash
# Generate optimized IR
$LOCAL_OPT -S -passes='default<O3>' orig.ll -o opt.ll

# Strip inline metadata references (Python one-liner):
python3 -c "
import re, sys
lines = open('orig.ll').readlines()
out = [re.sub(r',\s*!\w+ !\d+', '', l) for l in lines]
print(''.join(l for l in out if not l.startswith('!')), end='')
" > orig_clean.ll
# (repeat for opt.ll)

$ALIVE_TV orig_clean.ll opt_clean.ll
```

Alternatively, write a minimal IR function directly (no printf, no globals,
just the computation) — this avoids all metadata issues and makes alive-tv
output clean.

### 3c. Identify the Offending Pass

Use `-print-changed` to find where the bad transformation first appears:

```bash
$LOCAL_OPT -S -passes='default<O3>' -print-changed orig_clean.ll \
  -o /dev/null 2>&1 | grep -B5 "smin\.i8\|<pattern-of-interest>"
```

Look for the `*** IR Dump After <PassName> on <function> ***` header immediately
before the problematic IR. This tells you exactly which pass to investigate.

**Common pattern:** A pass introduces an intermediate form, then a later run of
the same pass (or InstCombine) completes the miscompilation. Check both.

### 3d. Write a Minimal IR Input for the Offending Pass

Isolate the bug to a single pass and the simplest IR that triggers it:

```bash
cat > input.ll << 'EOF'
declare i32 @llvm.smin.i32(i32, i32)

define i8 @f(i32 %x) {
  %smin = call i32 @llvm.smin.i32(i32 %x, i32 0)
  %trunc = trunc i32 %smin to i8
  ret i8 %trunc
}
EOF

$LOCAL_OPT -S -passes='width-opt' input.ll   # shows the wrong output
$ALIVE_TV input.ll <(opt -S -passes='width-opt' input.ll)  # confirms the bug
```

### 3e. Write `interesting.sh` for llvm-reduce

**Critical difference from the C-vise script:** `llvm-reduce` passes the
candidate filename as `$1`. The script **must** use `$1`, not a hardcoded name.
Forgetting this causes llvm-reduce to always test the original file and either
never reduce or over-reduce to an empty file.

```bash
#!/usr/bin/env bash
set -euo pipefail

LOCAL_OPT="/path/to/build/bin/opt"
ALIVE_TV="/path/to/alive2/build/alive-tv"
CAND="${1:-input.ll}"
OUT="${CAND%.ll}.out.ll"

rm -f "$OUT"

# Run the suspect pass; fail if opt itself errors
$LOCAL_OPT -S -passes='width-opt' "$CAND" -o "$OUT" 2>/dev/null || exit 1

# Must actually change the IR (otherwise the pass did nothing — not interesting)
diff -q "$CAND" "$OUT" > /dev/null 2>&1 && exit 1

# alive-tv must report the transformation as incorrect
result=$($ALIVE_TV "$CAND" "$OUT" 2>&1)
echo "$result" | grep -q "incorrect transformations" && \
  ! echo "$result" | grep -q "0 incorrect transformations"
```

### 3f. Run llvm-reduce

```bash
llvm-reduce --test=./interesting.sh input.ll
cat reduced.ll
```

The result is typically just a handful of instructions.

---

## 4. Fix the Bug

### Understand the Pattern First

Before editing, identify whether the transformation is:

- **Completely wrong** — the transformation is never valid.
- **Missing a guard** — valid when a precondition holds, but the code never
  checks that precondition.

The `smin`/`smax` narrowing bug was the second kind: the code had a comment
saying "narrowable when both args are sext-bounded at TargetWidth" but no code
enforcing it.

### The Signed Bounded Check Pattern

For signed min/max intrinsics, narrowing `trunc(sminN(a, b))` to `sminM(trunc(a), trunc(b))`
(M < N) is only valid when both `a` and `b` are **sext-bounded** at M bits —
i.e., `trunc(a)` sign-extends back to `a`.

Compare to the unsigned case which requires **zero-bounded** args (no high bits set).

Pattern for adding such a guard:

```cpp
// Returns true when V's value fits in a Width-bit signed integer,
// i.e. sext(trunc(V, Width)) == V.
bool isSextBoundedAtWidth(Value *V, unsigned Width) {
  if (auto Ext = getExtOperandInfo(V))
    return Ext->Kind == ExtKind::SExt && Ext->NarrowWidth <= Width;
  if (auto *C = dyn_cast<ConstantInt>(V))
    return C->getValue().isSignedIntN(Width);
  return false;
}
```

Then gate the transformation:

```cpp
case Intrinsic::smin:
case Intrinsic::smax:
  if (!isSextBoundedAtWidth(II->getArgOperand(0), TargetWidth) ||
      !isSextBoundedAtWidth(II->getArgOperand(1), TargetWidth))
    return false;
  // ... proceed with cost check and materialization
```

### Validate the Fix with Alive-TV

After rebuilding:

```bash
cmake --build build --target opt -j$(nproc)

$LOCAL_OPT -S -passes='width-opt' reduced.ll
$ALIVE_TV reduced.ll <($LOCAL_OPT -S -passes='width-opt' reduced.ll)
# Should say: "Transformation seems to be correct!"
```

Also re-run the original C test case end-to-end to confirm.

---

## 5. Add a Regression Test

### Test File Location

Tests live in `llvm/test/Transforms/<PassDir>/`. For `width-opt`, that is
`llvm/test/Transforms/WidthOpt/`. Add to the most relevant existing file or
create a new one.

### Test Format

Use FileCheck. Every test file needs a RUN line:

```
; RUN: opt -passes='width-opt' -S %s | FileCheck %s
```

**Note: always use new pass manager syntax** (`-passes='...'`). The legacy
pass manager flags (e.g., `-width-opt`) are gone.

For a "no change" (regression) test:

```llvm
; Regression: trunc(smin.i32(x, 0)) must NOT be narrowed to smin.i8(trunc(x), 0)
; when x is an unbounded i32. The i32 sign bit may differ from the i8 sign bit
; after truncation (e.g. x=129: smin.i32(129,0)=0 but smin.i8(-127,0)=-127).
define i8 @trunc_smin_unbounded_nochange(i32 %x) {
  %m = call i32 @llvm.smin.i32(i32 %x, i32 0)
  %t = trunc i32 %m to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @trunc_smin_unbounded_nochange(
; CHECK: call i32 @llvm.smin.i32(i32 %x, i32 0)
; CHECK: trunc i32
; CHECK: ret i8
```

For a "should transform" test (valid case still works):

```llvm
define i8 @trunc_smin_sext_bounded(i8 %a) {
  %a32 = sext i8 %a to i32
  %m = call i32 @llvm.smin.i32(i32 %a32, i32 42)
  %t = trunc i32 %m to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @trunc_smin_sext_bounded(
; CHECK-NOT: sext
; CHECK-NOT: smin i32
; CHECK-NOT: trunc
; CHECK: call i8 @llvm.smin.i8(i8 %a, i8 42)
; CHECK: ret i8
```

Run the test:

```bash
build/bin/llvm-lit llvm/test/Transforms/WidthOpt/width-opt-trunc-minmax-narrow.ll -v
```

---

## 6. Common Pitfalls and Notes

### C-Vise

- **cvise cwd:** When using Claude Code's `run_in_background` Bash tool, always
  prefix with `cd /your/workdir &&`. The shell cwd resets to the project root
  between tool calls. Forgetting this means cvise runs in the wrong directory and
  either silently no-ops or immediately exits.

- **`&` in background commands:** Do not use `&` when `run_in_background=true`.
  The `&` causes the shell to exit and kills cvise.

- **Temp-dir validation:** C-Vise workers run the interestingness test in temp
  directories. Always validate with a temp-dir `cp && cd && ./interesting.sh` test
  before launching cvise. If this fails, the reducer will make no progress.

- **Preprocess before reducing, always.** Running cvise on `#include`-containing
  source leads to reducer working on headers, not the actual code.

- **`-w` flag for Csmith programs.** Csmith-generated code always triggers warnings.
  Every compilation of a Csmith program must use `-w`.

### Alive-TV

- **Metadata causes errors.** TBAA and other metadata (`!tbaa`, `!range`, etc.)
  trigger "Unsupported metadata" in alive-tv. Either strip them or write minimal
  IR without metadata for the oracle test.

- **Unknown libcalls.** Calls to `printf` and other libc functions cause alive-tv
  to approximate semantics, which can suppress real bugs. Write the minimal IR
  using only the computation, with function arguments instead of globals/printf.

- **The oracle for llvm-reduce** should check `alive-tv input tgt` and grep for
  `"incorrect transformations"` with a count > 0. A simple exit-code check is
  insufficient because alive-tv exits 0 for errors too.

### LLVM Pass Manager

- **Always use new pass manager syntax.** Legacy flags like `--instcombine` or
  `-instcombine` are gone. Use `-passes='instcombine'` or
  `-passes='instcombine,simplifycfg'`.

- **Pass names** are in `llvm/lib/Passes/PassRegistry.def`. When in doubt, grep
  there. Examples: `width-opt`, `instcombine`, `simplifycfg`, `sroa`.

### llvm-reduce

- **The candidate is `$1`.** Unlike cvise (which uses a fixed local filename),
  llvm-reduce passes the candidate as the first argument. The interestingness
  script must use `$1`. If it doesn't, llvm-reduce either makes no progress or
  reduces to an empty file.

- **Verify the pass actually changes the candidate.** Add a `diff -q "$CAND" "$OUT" && exit 1`
  check so that llvm-reduce doesn't keep candidates where the pass is a no-op.

### Finding the Offending Pass

1. Use `-print-changed` (not `-print-after-all`) to see only IR that changed.
2. Search for the characteristic pattern of the miscompilation (e.g., `smin\.i8`).
3. Look at the `*** IR Dump After <PassName> ***` header above the first match.
4. The real bug may be in a *second* invocation of a pass (after InstCombine has
   set up the pattern), not the first. Check carefully.

### Debugging `collectTruncRootedValueCost`-style Cost Functions

Many narrowing transformations follow this pattern:

1. A cost/feasibility function (`collectTruncRootedValueCost`) walks the
   expression tree and returns true if it's safe and profitable to narrow.
2. A materialization function (`materializeTruncRootedValueAtWidth`) actually
   performs the narrowing.

Bugs arise when the cost function has a **too-permissive fallback**: a catch-all
`if (Width > TargetWidth) return true;` that allows arbitrary wide values through.
This is correct for values that can be freely truncated (low-bits-preserving ops),
but wrong for signed operations where the sign must be preserved.

Whenever adding a new signed operation to such a cost function, ask:
*Does truncation preserve the signed comparison order?* If the answer depends on
the value range, add an `isSextBoundedAtWidth` (or equivalent) check rather than
relying on the fallback.

---

## 7. Full Example: The smin Narrowing Bug

**Miscompile trigger (C):**

```c
int g_66 = 2696858675;  // 0xA0BECC33 — negative as int32
char g_143;
int main() {
  long tmp = g_66 < 0 ? g_66 : 0;
  g_143 |= tmp;
  printf("checksum = %X\n", g_143);
}
// Correct: checksum = 33
// Buggy local clang -O3: checksum = 0
```

**Reduced LLVM IR:**

```llvm
declare i32 @llvm.smin.i32(i32, i32)

define i8 @f(i32 %x) {
  %smin = call i32 @llvm.smin.i32(i32 %x, i32 0)
  %trunc = trunc i32 %smin to i8
  ret i8 %trunc
}
```

**What `width-opt` wrongly produced:**

```llvm
define i8 @f(i32 %x) {
  %1 = trunc i32 %x to i8
  %smin.narrow = call i8 @llvm.smin.i8(i8 %1, i8 0)  ; WRONG
  ret i8 %smin.narrow
}
```

**Counterexample (from alive-tv):** `%x = 129` (0x81, positive in i32):
- Source: `smin.i32(129, 0) = 0`, `trunc(0) = 0`
- Target: `trunc(129) = -127` (i8), `smin.i8(-127, 0) = -127`

**Root cause:** `collectTruncRootedValueCost` had a correct comment
("sext-bounded at TargetWidth") but no code enforcing it. The fallback
`Width > TargetWidth → return true` let unbounded i32 args through.

**Fix:** Added `isSextBoundedAtWidth()` and gated the `smin`/`smax` case on it.
