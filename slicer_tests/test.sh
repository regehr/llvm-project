#!/usr/bin/env bash

dir="${1:-slicer_tests}"

shopt -s nullglob

total=0
passed=0
failed=0

for input in "$dir"/*.ll; do
  total=$((total + 1))

  name="$(basename "$input" .ll)"
  expected="$dir/$name.expected.tsv"
  output="/tmp/$name.tsv"

  echo "==> $name"

  if [[ ! -f "$expected" ]]; then
    echo "MISSING expected file: $expected"
    failed=$((failed + 1))
    echo
    continue
  fi

  if ! ./slicer --input="$input" --output="$output"; then
    echo "FAIL: slicer exited non-zero"
    failed=$((failed + 1))
    echo
    continue
  fi

  if diff -u "$expected" "$output"; then
    echo "PASS"
    passed=$((passed + 1))
  else
    echo "FAIL: output differs"
    failed=$((failed + 1))
  fi

  echo
done

echo "Summary: $passed passed, $failed failed, $total total"

if [[ "$failed" -ne 0 ]]; then
  exit 1
fi
