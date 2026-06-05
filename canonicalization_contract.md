# Pattern Canonicalization Contract

Patterns are canonicalized as LLVM matcher patterns, not as algebraic expressions.

- Patterns are rendered as trees. Repeated subexpressions are expanded
  structurally, not printed as shared DAG bindings.

- Children are canonicalized recursively before their parent is canonicalized.

- For local commutative operations, children are sorted by canonical subtree key.
  This mirrors LLVM `m_c_*` matcher semantics. Only the operand order of that
  one operation is changed; associativity is not canonicalized.

- For noncommutative operations, child order is preserved, except for the ICmp
  normalization rule below.

- Ordered ICmp predicates are canonicalized to the matcher-generator-supported
  less-than / less-or-equal predicate families. This mirrors LLVM
  `m_c_SpecificICmp` style matching while keeping canonical pattern names in the
  supported DSL:
  - `ICmpSgt(a, b)` canonicalizes to `ICmpSlt(b, a)`
  - `ICmpSge(a, b)` canonicalizes to `ICmpSle(b, a)`
  - `ICmpUgt(a, b)` canonicalizes to `ICmpUlt(b, a)`
  - `ICmpUge(a, b)` canonicalizes to `ICmpUle(b, a)`
  - `ICmpSlt(a, b)`, `ICmpSle(a, b)`, `ICmpUlt(a, b)`, and `ICmpUle(a, b)`
    keep their predicate and operand order.
  Equality and inequality ICmps are treated as commutative.

- Alpha-renaming is applied after canonical child ordering. Argument names are
  reassigned by first occurrence in the final canonical tree: the first distinct
  original argument becomes `arg0`, the next becomes `arg1`, etc.

- Alpha-renaming preserves equality constraints. Multiple occurrences of the
  same original argument are rendered as the same canonical `argN`. Distinct
  original arguments are never merged.

- Alpha-renaming does not change operation structure or noncommutative child
  order. For example, `Sub(arg1, arg0)` canonicalizes to `Sub(arg0, arg1)` only
  as a matcher-binder renaming: lhs and rhs remain distinct captures. But
  `Sub(arg0, arg0)` remains a repeated-argument pattern and is not canonicalized
  to `Sub(arg0, arg1)`.

- We intentionally do not canonicalize associativity, distributivity, identities,
  constants, or other algebraic equivalences.
