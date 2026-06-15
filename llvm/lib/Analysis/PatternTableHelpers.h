#ifndef LLVM_LIB_ANALYSIS_PATTERNTABLEHELPERS_H
#define LLVM_LIB_ANALYSIS_PATTERNTABLEHELPERS_H

#include "llvm/ADT/APInt.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/ConstantRange.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace llvm {

namespace KnownBitsPatterns {

struct BwTable {
  unsigned bw;
  uint32_t numRows;
  const unsigned char *blob;
};

namespace detail {

inline constexpr unsigned kMaxMaskBytes = 128; // up to 1024-bit KnownBits

inline void packArg(const std::array<APInt, 2> &arg, unsigned maskBytes,
                    unsigned char *zBuf, unsigned char *oBuf) {
  uint64_t tmpZ[kMaxMaskBytes / sizeof(uint64_t)] = {};
  uint64_t tmpO[kMaxMaskBytes / sizeof(uint64_t)] = {};
  unsigned argWords = arg[0].getNumWords();
  unsigned needWords = (maskBytes + sizeof(uint64_t) - 1) / sizeof(uint64_t);
  unsigned copyWords = argWords < needWords ? argWords : needWords;
  const uint64_t *zData = arg[0].getRawData();
  const uint64_t *oData = arg[1].getRawData();
  for (unsigned w = 0; w < copyWords; ++w) {
    tmpZ[w] = zData[w];
    tmpO[w] = oData[w];
  }
  std::memcpy(zBuf, tmpZ, maskBytes);
  std::memcpy(oBuf, tmpO, maskBytes);
}

inline std::array<APInt, 2> unpackOut(const unsigned char *zBytes,
                                      const unsigned char *oBytes, unsigned bw,
                                      unsigned maskBytes) {
  uint64_t tmpZ[kMaxMaskBytes / sizeof(uint64_t)] = {};
  uint64_t tmpO[kMaxMaskBytes / sizeof(uint64_t)] = {};
  std::memcpy(tmpZ, zBytes, maskBytes);
  std::memcpy(tmpO, oBytes, maskBytes);
  unsigned numWords = (bw + 63) / 64;
  return {APInt(bw, ArrayRef<uint64_t>(tmpZ, numWords)),
          APInt(bw, ArrayRef<uint64_t>(tmpO, numWords))};
}

} // namespace detail

template <unsigned Arity>
inline std::array<APInt, 2> lookupKB(const std::array<APInt, 2> *args,
                                     const BwTable *tables, size_t numTables) {
  unsigned bw = args[0][0].getBitWidth();
  unsigned maskBytes = (bw + 7) / 8;
  if (maskBytes > detail::kMaxMaskBytes)
    return {APInt(bw, 0), APInt(bw, 0)};
  unsigned rowBytes = 2 * (Arity + 1) * maskBytes;

  unsigned char inZ[Arity * detail::kMaxMaskBytes];
  unsigned char inO[Arity * detail::kMaxMaskBytes];
  bool packed = false;

  unsigned char outZ[detail::kMaxMaskBytes] = {};
  unsigned char outO[detail::kMaxMaskBytes] = {};

  for (size_t t = 0; t < numTables; ++t) {
    if (tables[t].bw != bw)
      continue;
    if (!packed) {
      for (unsigned a = 0; a < Arity; ++a)
        detail::packArg(args[a], maskBytes, inZ + a * maskBytes,
                        inO + a * maskBytes);
      packed = true;
    }

    const BwTable &T = tables[t];
    for (uint32_t r = 0; r < T.numRows; ++r) {
      const unsigned char *row = T.blob + r * rowBytes;
      bool match = true;
      for (unsigned a = 0; a < Arity; ++a) {
        const unsigned char *rZ = row + (2 * a) * maskBytes;
        const unsigned char *rO = row + (2 * a + 1) * maskBytes;
        const unsigned char *bZ = inZ + a * maskBytes;
        const unsigned char *bO = inO + a * maskBytes;
        for (unsigned i = 0; i < maskBytes; ++i) {
          if ((rZ[i] & ~bZ[i]) | (rO[i] & ~bO[i])) {
            match = false;
            break;
          }
        }
        if (!match)
          break;
      }
      if (!match)
        continue;

      const unsigned char *outRowZ = row + (2 * Arity) * maskBytes;
      const unsigned char *outRowO = row + (2 * Arity + 1) * maskBytes;
      for (unsigned i = 0; i < maskBytes; ++i) {
        outZ[i] |= outRowZ[i];
        outO[i] |= outRowO[i];
      }
    }
  }

  return detail::unpackOut(outZ, outO, bw, maskBytes);
}

} // namespace KnownBitsPatterns

namespace ConstantRangePatterns {

struct CRTable {
  unsigned bw;
  uint32_t numRows;
  const unsigned char *blob;
};

namespace detail {

inline constexpr unsigned kMaxRangeBytes = 128; // up to 1024-bit ConstantRange

inline uint64_t maskForBW(unsigned BW) {
  return BW == 64 ? ~uint64_t(0) : ((uint64_t(1) << BW) - 1);
}

inline unsigned getMaxHighByte(unsigned BW) {
  unsigned Rem = BW % 8;
  return Rem == 0 ? 0xffu : ((1u << Rem) - 1u);
}

inline bool isZeroValue(const unsigned char *Bytes, unsigned NumBytes) {
  for (unsigned I = 0; I < NumBytes; ++I)
    if (Bytes[I] != 0)
      return false;
  return true;
}

inline bool isMaxValue(const unsigned char *Bytes, unsigned BW,
                       unsigned NumBytes) {
  if (NumBytes == 0)
    return false;
  for (unsigned I = 0; I + 1 < NumBytes; ++I)
    if (Bytes[I] != 0xff)
      return false;
  return Bytes[NumBytes - 1] == getMaxHighByte(BW);
}

inline bool equalValue(const unsigned char *A, const unsigned char *B,
                       unsigned NumBytes) {
  return std::memcmp(A, B, NumBytes) == 0;
}

inline int compareUnsigned(const unsigned char *A, const unsigned char *B,
                           unsigned NumBytes) {
  for (unsigned I = NumBytes; I > 0; --I) {
    unsigned CharIdx = I - 1;
    if (A[CharIdx] < B[CharIdx])
      return -1;
    if (A[CharIdx] > B[CharIdx])
      return 1;
  }
  return 0;
}

inline bool isEmptyRange(const unsigned char *Lower, const unsigned char *Upper,
                         unsigned, unsigned NumBytes) {
  return equalValue(Lower, Upper, NumBytes) && isZeroValue(Lower, NumBytes);
}

inline bool isFullRange(const unsigned char *Lower, const unsigned char *Upper,
                        unsigned BW, unsigned NumBytes) {
  return equalValue(Lower, Upper, NumBytes) && isMaxValue(Lower, BW, NumBytes);
}

inline bool isUpperWrapped(const unsigned char *Lower, const unsigned char *Upper,
                           unsigned NumBytes) {
  return compareUnsigned(Lower, Upper, NumBytes) > 0;
}

inline bool containsRange(const unsigned char *OuterLower,
                          const unsigned char *OuterUpper,
                          const unsigned char *InnerLower,
                          const unsigned char *InnerUpper, unsigned BW,
                          unsigned NumBytes) {
  if (isFullRange(OuterLower, OuterUpper, BW, NumBytes) ||
      isEmptyRange(InnerLower, InnerUpper, BW, NumBytes))
    return true;
  if (isEmptyRange(OuterLower, OuterUpper, BW, NumBytes) ||
      isFullRange(InnerLower, InnerUpper, BW, NumBytes))
    return false;

  bool OuterWrapped = isUpperWrapped(OuterLower, OuterUpper, NumBytes);
  bool InnerWrapped = isUpperWrapped(InnerLower, InnerUpper, NumBytes);
  if (!OuterWrapped) {
    if (InnerWrapped)
      return false;
    return compareUnsigned(OuterLower, InnerLower, NumBytes) <= 0 &&
           compareUnsigned(InnerUpper, OuterUpper, NumBytes) <= 0;
  }

  if (!InnerWrapped)
    return compareUnsigned(InnerUpper, OuterUpper, NumBytes) <= 0 ||
           compareUnsigned(OuterLower, InnerLower, NumBytes) <= 0;

  return compareUnsigned(InnerUpper, OuterUpper, NumBytes) <= 0 &&
         compareUnsigned(OuterLower, InnerLower, NumBytes) <= 0;
}

inline uint64_t loadU64(const unsigned char *Bytes, unsigned NumBytes,
                        unsigned BW) {
  uint64_t Value = 0;
  std::memcpy(&Value, Bytes, NumBytes);
  return Value & maskForBW(BW);
}

inline bool isEmptyRange64(uint64_t Lower, uint64_t Upper) {
  return Lower == Upper && Lower == 0;
}

inline bool isFullRange64(uint64_t Lower, uint64_t Upper, unsigned BW) {
  uint64_t Max = maskForBW(BW);
  return Lower == Upper && Lower == Max;
}

inline bool isUpperWrapped64(uint64_t Lower, uint64_t Upper) {
  return Lower > Upper;
}

inline bool containsRange64(uint64_t OuterLower, uint64_t OuterUpper,
                            uint64_t InnerLower, uint64_t InnerUpper,
                            unsigned BW) {
  if (isFullRange64(OuterLower, OuterUpper, BW) ||
      isEmptyRange64(InnerLower, InnerUpper))
    return true;
  if (isEmptyRange64(OuterLower, OuterUpper) ||
      isFullRange64(InnerLower, InnerUpper, BW))
    return false;

  bool OuterWrapped = isUpperWrapped64(OuterLower, OuterUpper);
  bool InnerWrapped = isUpperWrapped64(InnerLower, InnerUpper);
  if (!OuterWrapped) {
    if (InnerWrapped)
      return false;
    return OuterLower <= InnerLower && InnerUpper <= OuterUpper;
  }

  if (!InnerWrapped)
    return InnerUpper <= OuterUpper || OuterLower <= InnerLower;

  return InnerUpper <= OuterUpper && OuterLower <= InnerLower;
}

inline void packRange64(const ConstantRange &Range, unsigned BW,
                        uint64_t &Lower, uint64_t &Upper) {
  Lower = Range.getLower().getZExtValue() & maskForBW(BW);
  Upper = Range.getUpper().getZExtValue() & maskForBW(BW);
}

inline ConstantRange unpackRange64(uint64_t Lower, uint64_t Upper, unsigned BW) {
  if (isEmptyRange64(Lower, Upper))
    return ConstantRange::getEmpty(BW);
  if (isFullRange64(Lower, Upper, BW))
    return ConstantRange::getFull(BW);
  return ConstantRange(APInt(BW, Lower), APInt(BW, Upper));
}

inline void packAPInt(const APInt &Value, unsigned BW, unsigned NumBytes,
                      unsigned char *Out) {
  uint64_t Tmp[kMaxRangeBytes / sizeof(uint64_t)] = {};
  unsigned NeedWords = (NumBytes + sizeof(uint64_t) - 1) / sizeof(uint64_t);
  unsigned CopyWords =
      Value.getNumWords() < NeedWords ? Value.getNumWords() : NeedWords;
  const uint64_t *Data = Value.getRawData();
  for (unsigned W = 0; W < CopyWords; ++W)
    Tmp[W] = Data[W];
  std::memcpy(Out, Tmp, NumBytes);
  if (unsigned Rem = BW % 8)
    Out[NumBytes - 1] &= (1u << Rem) - 1u;
}

inline void packRange(const ConstantRange &Range, unsigned BW, unsigned NumBytes,
                      unsigned char *Lower, unsigned char *Upper) {
  packAPInt(Range.getLower(), BW, NumBytes, Lower);
  packAPInt(Range.getUpper(), BW, NumBytes, Upper);
}

inline APInt unpackAPInt(const unsigned char *Bytes, unsigned BW,
                         unsigned NumBytes) {
  uint64_t Tmp[kMaxRangeBytes / sizeof(uint64_t)] = {};
  std::memcpy(Tmp, Bytes, NumBytes);
  unsigned NumWords = (BW + 63) / 64;
  return APInt(BW, ArrayRef<uint64_t>(Tmp, NumWords));
}

inline ConstantRange unpackRange(const unsigned char *Lower,
                                 const unsigned char *Upper, unsigned BW,
                                 unsigned NumBytes) {
  if (isEmptyRange(Lower, Upper, BW, NumBytes))
    return ConstantRange::getEmpty(BW);
  if (isFullRange(Lower, Upper, BW, NumBytes))
    return ConstantRange::getFull(BW);
  return ConstantRange(unpackAPInt(Lower, BW, NumBytes),
                       unpackAPInt(Upper, BW, NumBytes));
}

} // namespace detail

template <size_t Arity>
inline ConstantRange lookupCR64(unsigned BW,
                                const std::array<ConstantRange, Arity> &Args,
                                const CRTable *Tables, size_t NumTables,
                                ConstantRange::PreferredRangeType Preferred) {
  unsigned NumBytes = (BW + 7) / 8;
  unsigned RowBytes = 2 * (Arity + 1) * NumBytes;
  uint64_t ArgLower[Arity ? Arity : 1];
  uint64_t ArgUpper[Arity ? Arity : 1];
  bool Packed = false;
  ConstantRange Out = ConstantRange::getFull(BW);

  for (size_t TIdx = 0; TIdx < NumTables; ++TIdx) {
    if (Tables[TIdx].bw != BW)
      continue;
    if (!Packed) {
      for (size_t A = 0; A < Arity; ++A)
        detail::packRange64(Args[A], BW, ArgLower[A], ArgUpper[A]);
      Packed = true;
    }

    const CRTable &T = Tables[TIdx];
    for (uint32_t R = 0; R < T.numRows; ++R) {
      const unsigned char *Row = T.blob + R * RowBytes;
      bool Match = true;
      for (size_t A = 0; A < Arity; ++A) {
        uint64_t RowLower =
            detail::loadU64(Row + (2 * A) * NumBytes, NumBytes, BW);
        uint64_t RowUpper =
            detail::loadU64(Row + (2 * A + 1) * NumBytes, NumBytes, BW);
        if (!detail::containsRange64(RowLower, RowUpper, ArgLower[A],
                                     ArgUpper[A], BW)) {
          Match = false;
          break;
        }
      }
      if (!Match)
        continue;

      uint64_t OutLower =
          detail::loadU64(Row + (2 * Arity) * NumBytes, NumBytes, BW);
      uint64_t OutUpper =
          detail::loadU64(Row + (2 * Arity + 1) * NumBytes, NumBytes, BW);
      ConstantRange RowOut = detail::unpackRange64(OutLower, OutUpper, BW);
      Out = Out.intersectWith(RowOut, Preferred);
    }
  }

  return Out;
}

template <size_t Arity>
inline ConstantRange lookupCRWide(unsigned BW,
                                  const std::array<ConstantRange, Arity> &Args,
                                  const CRTable *Tables, size_t NumTables,
                                  ConstantRange::PreferredRangeType Preferred) {
  unsigned NumBytes = (BW + 7) / 8;
  if (NumBytes > detail::kMaxRangeBytes)
    return ConstantRange::getFull(BW);

  unsigned RowBytes = 2 * (Arity + 1) * NumBytes;
  unsigned char ArgLower[(Arity ? Arity : 1) * detail::kMaxRangeBytes];
  unsigned char ArgUpper[(Arity ? Arity : 1) * detail::kMaxRangeBytes];
  bool Packed = false;
  ConstantRange Out = ConstantRange::getFull(BW);

  for (size_t TIdx = 0; TIdx < NumTables; ++TIdx) {
    if (Tables[TIdx].bw != BW)
      continue;
    if (!Packed) {
      for (size_t A = 0; A < Arity; ++A)
        detail::packRange(Args[A], BW, NumBytes, ArgLower + A * NumBytes,
                          ArgUpper + A * NumBytes);
      Packed = true;
    }

    const CRTable &T = Tables[TIdx];
    for (uint32_t R = 0; R < T.numRows; ++R) {
      const unsigned char *Row = T.blob + R * RowBytes;
      bool Match = true;
      for (size_t A = 0; A < Arity; ++A) {
        const unsigned char *RowLower = Row + (2 * A) * NumBytes;
        const unsigned char *RowUpper = Row + (2 * A + 1) * NumBytes;
        const unsigned char *InLower = ArgLower + A * NumBytes;
        const unsigned char *InUpper = ArgUpper + A * NumBytes;
        if (!detail::containsRange(RowLower, RowUpper, InLower, InUpper, BW,
                                   NumBytes)) {
          Match = false;
          break;
        }
      }
      if (!Match)
        continue;

      const unsigned char *OutLower = Row + (2 * Arity) * NumBytes;
      const unsigned char *OutUpper = Row + (2 * Arity + 1) * NumBytes;
      ConstantRange RowOut =
          detail::unpackRange(OutLower, OutUpper, BW, NumBytes);
      Out = Out.intersectWith(RowOut, Preferred);
    }
  }

  return Out;
}

template <size_t Arity>
inline ConstantRange lookupCR(unsigned BW,
                              const std::array<ConstantRange, Arity> &Args,
                              const CRTable *Tables, size_t NumTables,
                              ConstantRange::PreferredRangeType Preferred) {
  if (BW <= 64)
    return lookupCR64(BW, Args, Tables, NumTables, Preferred);
  return lookupCRWide(BW, Args, Tables, NumTables, Preferred);
}

template <size_t Arity>
inline ConstantRange lookupSCR(unsigned BW,
                               const std::array<ConstantRange, Arity> &Args,
                               const CRTable *Tables, size_t NumTables) {
  return lookupCR(BW, Args, Tables, NumTables, ConstantRange::Signed);
}

template <size_t Arity>
inline ConstantRange lookupUCR(unsigned BW,
                               const std::array<ConstantRange, Arity> &Args,
                               const CRTable *Tables, size_t NumTables) {
  return lookupCR(BW, Args, Tables, NumTables, ConstantRange::Unsigned);
}

} // namespace ConstantRangePatterns

} // namespace llvm

#endif
