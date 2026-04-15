#!/usr/bin/env bash
set -e
cd "$(dirname "$0")"

echo "=== Alive2 verification ==="
python3 run_alive2.py "$@"

echo ""
echo "=== lit tests ==="
../build/bin/llvm-lit -sv ../llvm/test/Transforms/WidthOpt
