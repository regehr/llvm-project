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

template <size_t Arity> struct CRRow {
  std::array<ConstantRange, Arity> Args;
  ConstantRange Out;
};

template <size_t Arity, size_t NumRows>
inline ConstantRange lookupCR(const std::array<ConstantRange, Arity> &Args,
                              const std::array<CRRow<Arity>, NumRows> &Table,
                              ConstantRange::PreferredRangeType Preferred) {
  unsigned BW = Args[0].getBitWidth();
  ConstantRange Out = ConstantRange::getFull(BW);
  for (const CRRow<Arity> &Row : Table) {
    bool Match = true;
    for (size_t I = 0; I < Arity; ++I) {
      if (!Row.Args[I].contains(Args[I])) {
        Match = false;
        break;
      }
    }
    if (Match)
      Out = Out.intersectWith(Row.Out, Preferred);
  }
  return Out;
}

template <size_t Arity, size_t NumRows>
inline ConstantRange lookupSCR(const std::array<ConstantRange, Arity> &Args,
                               const std::array<CRRow<Arity>, NumRows> &Table) {
  return lookupCR(Args, Table, ConstantRange::Signed);
}

template <size_t Arity, size_t NumRows>
inline ConstantRange lookupUCR(const std::array<ConstantRange, Arity> &Args,
                               const std::array<CRRow<Arity>, NumRows> &Table) {
  return lookupCR(Args, Table, ConstantRange::Unsigned);
}

} // namespace ConstantRangePatterns

} // namespace llvm

#endif
