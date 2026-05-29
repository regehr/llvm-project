## Unary ops

| DSL op          |       Type | LLVM IR op                               |
| --------------- | ---------: | ---------------------------------------- |
| Abs             | (iN) -> iN | `call iN @llvm.abs.iN(iN %x, i1 false)`  |
| AbsUndef        | (iN) -> iN | `call iN @llvm.abs.iN(iN %x, i1 true)`   |
| PopCount        | (iN) -> iN | `call iN @llvm.ctpop.iN(iN %x)`          |
| CountLZero      | (iN) -> iN | `call iN @llvm.ctlz.iN(iN %x, i1 false)` |
| CountLZeroUndef | (iN) -> iN | `call iN @llvm.ctlz.iN(iN %x, i1 true)`  |
| CountRZero      | (iN) -> iN | `call iN @llvm.cttz.iN(iN %x, i1 false)` |
| CountRZeroUndef | (iN) -> iN | `call iN @llvm.cttz.iN(iN %x, i1 true)`  |

## Binary ops

| DSL op     |           Type | LLVM IR op                                           |
| ---------- | -------------: | ---------------------------------------------------- |
| Add        | (iN, iN) -> iN | `add iN %x, %y`                                      |
| AddNsw     | (iN, iN) -> iN | `add nsw iN %x, %y`                                  |
| AddNuw     | (iN, iN) -> iN | `add nuw iN %x, %y`                                  |
| AddNswNuw  | (iN, iN) -> iN | `add nsw nuw iN %x, %y`                              |
| And        | (iN, iN) -> iN | `and iN %x, %y`                                      |
| Ashr       | (iN, iN) -> iN | `ashr iN %x, %y`                                     |
| AshrExact  | (iN, iN) -> iN | `ashr exact iN %x, %y`                               |
| Lshr       | (iN, iN) -> iN | `lshr iN %x, %y`                                     |
| LshrExact  | (iN, iN) -> iN | `lshr exact iN %x, %y`                               |
| Mul        | (iN, iN) -> iN | `mul iN %x, %y`                                      |
| MulNsw     | (iN, iN) -> iN | `mul nsw iN %x, %y`                                  |
| MulNuw     | (iN, iN) -> iN | `mul nuw iN %x, %y`                                  |
| MulNswNuw  | (iN, iN) -> iN | `mul nsw nuw iN %x, %y`                              |
| Or         | (iN, iN) -> iN | `or iN %x, %y`                                       |
| OrDisjoint | (iN, iN) -> iN | `or disjoint iN %x, %y`                              |
| Sdiv       | (iN, iN) -> iN | `sdiv iN %x, %y`                                     |
| SdivExact  | (iN, iN) -> iN | `sdiv exact iN %x, %y`                               |
| Shl        | (iN, iN) -> iN | `shl iN %x, %y`                                      |
| ShlNsw     | (iN, iN) -> iN | `shl nsw iN %x, %y`                                  |
| ShlNuw     | (iN, iN) -> iN | `shl nuw iN %x, %y`                                  |
| ShlNswNuw  | (iN, iN) -> iN | `shl nsw nuw iN %x, %y`                              |
| Mods       | (iN, iN) -> iN | `srem iN %x, %y`                                     |
| Sub        | (iN, iN) -> iN | `sub iN %x, %y`                                      |
| SubNsw     | (iN, iN) -> iN | `sub nsw iN %x, %y`                                  |
| SubNuw     | (iN, iN) -> iN | `sub nuw iN %x, %y`                                  |
| SubNswNuw  | (iN, iN) -> iN | `sub nsw nuw iN %x, %y`                              |
| Udiv       | (iN, iN) -> iN | `udiv iN %x, %y`                                     |
| UdivExact  | (iN, iN) -> iN | `udiv exact iN %x, %y`                               |
| Modu       | (iN, iN) -> iN | `urem iN %x, %y`                                     |
| Xor        | (iN, iN) -> iN | `xor iN %x, %y`                                      |
| Umax       | (iN, iN) -> iN | `call iN @llvm.umax.iN(iN %x, iN %y)`                |
| Umin       | (iN, iN) -> iN | `call iN @llvm.umin.iN(iN %x, iN %y)`                |
| Smax       | (iN, iN) -> iN | `call iN @llvm.smax.iN(iN %x, iN %y)`                |
| Smin       | (iN, iN) -> iN | `call iN @llvm.smin.iN(iN %x, iN %y)`                |
| SaddSat    | (iN, iN) -> iN | `call iN @llvm.sadd.sat.iN(iN %x, iN %y)`            |
| UaddSat    | (iN, iN) -> iN | `call iN @llvm.uadd.sat.iN(iN %x, iN %y)`            |
| SsubSat    | (iN, iN) -> iN | `call iN @llvm.ssub.sat.iN(iN %x, iN %y)`            |
| UsubSat    | (iN, iN) -> iN | `call iN @llvm.usub.sat.iN(iN %x, iN %y)`            |
| SmulSat    | (iN, iN) -> iN | `call iN @llvm.smul.fix.sat.iN(iN %x, iN %y, i32 0)` |
| UmulSat    | (iN, iN) -> iN | `call iN @llvm.umul.fix.sat.iN(iN %x, iN %y, i32 0)` |
| SshlSat    | (iN, iN) -> iN | `call iN @llvm.sshl.sat.iN(iN %x, iN %y)`            |
| UshlSat    | (iN, iN) -> iN | `call iN @llvm.ushl.sat.iN(iN %x, iN %y)`            |

## ICmp ops

| DSL op  |           Type | LLVM IR op           |
| ------- | -------------: | -------------------- |
| ICmpEq  | (iN, iN) -> i1 | `icmp eq iN %x, %y`  |
| ICmpNe  | (iN, iN) -> i1 | `icmp ne iN %x, %y`  |
| ICmpSlt | (iN, iN) -> i1 | `icmp slt iN %x, %y` |
| ICmpSle | (iN, iN) -> i1 | `icmp sle iN %x, %y` |
| ICmpSgt | (iN, iN) -> i1 | `icmp sgt iN %x, %y` |
| ICmpSge | (iN, iN) -> i1 | `icmp sge iN %x, %y` |
| ICmpUlt | (iN, iN) -> i1 | `icmp ult iN %x, %y` |
| ICmpUle | (iN, iN) -> i1 | `icmp ule iN %x, %y` |
| ICmpUgt | (iN, iN) -> i1 | `icmp ugt iN %x, %y` |
| ICmpUge | (iN, iN) -> i1 | `icmp uge iN %x, %y` |

## Other ops

| DSL op      |               Type | LLVM IR op                      |
| ----------- | -----------------: | ------------------------------- |
| TruncToBool | (iN) -> i1         | `trunc iN %x to i1`             |
| ZextBool    | (i1) -> iN         | `zext i1 %x to iN`              |
| SextBool    | (i1) -> iN         | `sext i1 %x to iN`              |
| Select      | (i1, iN, iN) -> iN | `select i1 %cond, iN %x, iN %y` |
