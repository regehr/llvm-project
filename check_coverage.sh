#!/usr/bin/env bash
# Run WidthOpt tests and report coverage of WidthOpt.cpp
set -e
cd "$(dirname "$0")"

PROFRAW_PATTERN="/tmp/widthopt_cov_%m.profraw"
PROFDATA="/tmp/widthopt_merged.profdata"
WIDTHOPT_OBJ="build-with-coverage/lib/Transforms/Scalar/CMakeFiles/LLVMScalarOpts.dir/WidthOpt.cpp.o"

# Clean old profiles
rm -f /tmp/widthopt_cov_*.profraw "$PROFDATA"

# Run tests with coverage
LLVM_PROFILE_FILE="$PROFRAW_PATTERN" build-with-coverage/bin/llvm-lit -q llvm/test/Transforms/WidthOpt 2>&1 | tail -5

# Merge profiles
build-with-coverage/bin/llvm-profdata merge /tmp/widthopt_cov_*.profraw -o "$PROFDATA"

# Report coverage for WidthOpt.cpp only
build-with-coverage/bin/llvm-cov report "$WIDTHOPT_OBJ" -instr-profile="$PROFDATA" 2>/dev/null | grep "WidthOpt.cpp"
