
define float @f(i1 %c, float %x, float %y) {
entry:
  %s = select i1 %c, float %x, float %y
  ret float %s
}

