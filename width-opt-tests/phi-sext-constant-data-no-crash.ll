
define i32 @phi_sext_constant_no_crash(i1 %cond, i16 %x) {
entry:
  br i1 %cond, label %then, label %else

then:
  ; sext of a negative ConstantInt — stays as SExt, NarrowValue is ConstantData
  ; (i8 -1) which has no use list.
  %ext_const = sext i8 -1 to i32
  br label %merge

else:
  ; sext of a variable with a wider source; drives Info.NarrowWidth to 16,
  ; so the constant incoming needs materialization to i16, triggering the
  ; users() scan on i8 -1 (ConstantData) without the fix.
  %ext_val = sext i16 %x to i32
  br label %merge

merge:
  %phi = phi i32 [ %ext_const, %then ], [ %ext_val, %else ]
  ret i32 %phi
}

