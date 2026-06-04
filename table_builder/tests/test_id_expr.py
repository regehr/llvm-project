#!/usr/bin/env python3
"""Unit test for the id<->expression codec in table_builder/util.py.

expr_to_id and decode_id_to_expr must be exact inverses: encoding an expression
to a filename id and decoding it back has to yield the original text verbatim.
This is what lets the <id>.inc filename be the sole record of the pattern, with
no patterns.json and no marker comment.

Run directly:  python3 table_builder/tests/test_id_expr.py
Or via pytest: pytest table_builder/tests/test_id_expr.py
"""
import csv
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent          # table_builder/tests
TABLE_BUILDER = HERE.parent                      # table_builder
sys.path.insert(0, str(TABLE_BUILDER))

from util import decode_id_to_expr, expr_to_id  # noqa: E402

FIXTURE = HERE / "top_10_pattern.tsv"

# A handful of shapes covering flat, left-nested, right-nested, repeated args,
# and multi-word op names.
SAMPLES = [
    "Add(arg0, arg1)",
    "Add(Add(arg0, arg1), arg2)",
    "Add(arg0, And(arg1, arg2))",
    "Or(Lshr(arg0, arg1), arg0)",
    "AshrExact(Sub(arg0, arg1), arg2)",
    "AddNswNuw(Or(arg0, arg1), Xor(arg2, arg0))",
]


def _roundtrip(expr: str) -> None:
    pid = expr_to_id(expr)
    back = decode_id_to_expr(pid)
    assert back == expr, f"round-trip changed {expr!r} -> {pid!r} -> {back!r}"


def test_samples_roundtrip() -> None:
    for expr in SAMPLES:
        _roundtrip(expr)


def test_fixture_roundtrips() -> None:
    with FIXTURE.open(newline="") as f:
        for row in csv.DictReader(f, delimiter="\t"):
            expr = row["pattern"].strip()
            if expr:
                _roundtrip(expr)


def test_decode_validates_ops() -> None:
    # With a supported-op set, an unknown op token is rejected up front.
    try:
        decode_id_to_expr("Bogus_arg0_arg1", valid_ops={"Add"})
    except ValueError:
        pass
    else:
        raise AssertionError("decode_id_to_expr accepted an unknown op token")


if __name__ == "__main__":
    test_samples_roundtrip()
    test_fixture_roundtrips()
    test_decode_validates_ops()
    print("PASS: id<->expression codec round-trips")
