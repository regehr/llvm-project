
define i32 @widen_add_non_dominating_zext(i1 %cond, i8 %x) {
entry:
  br i1 %cond, label %side, label %main

side:
  ; This zext has the right type but is in a non-dominating block.
  %x32_side = zext i8 %x to i32
  br label %main

main:
  ; Pattern: zext(add(zext(%x, i16), 3), i32) — should be widened.
  %x16 = zext i8 %x to i16
  %add16 = add i16 %x16, 3
  %wide = zext i16 %add16 to i32
  ret i32 %wide
}

