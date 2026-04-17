
define i1 @ptr_icmp_eq(ptr %p, ptr %q) {
entry:
  %cmp = icmp eq ptr %p, %q
  ret i1 %cmp
}

