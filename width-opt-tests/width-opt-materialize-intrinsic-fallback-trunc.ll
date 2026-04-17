

declare i32 @llvm.ctpop.i32(i32)

define i8 @and_ctpop_zext(i32 %x, i8 %a) {
  %cp = call i32 @llvm.ctpop.i32(i32 %x)
  %za = zext i8 %a to i32
  %and = and i32 %cp, %za
  %t = trunc i32 %and to i8
  ret i8 %t
}
