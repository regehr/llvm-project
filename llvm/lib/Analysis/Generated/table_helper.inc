// Auto-generated runtime support for KnownBits pattern lookup tables.
// Included once from KnownBitsPatternDispatch.inc, before per-pattern files.
//
// Each pattern's .inc holds a per-bitwidth byte blob with the layout:
//   row 0: arg0.Zero | arg0.One | arg1.Zero | arg1.One | ... | out.Zero | out.One
//   row 1: ...
// where each field is `(bw + 7) / 8` little-endian bytes.
//
// lookupKB() picks the subtable matching the runtime bitwidth, accumulates
// (bitwise OR of Zero/One masks) the ideal output of every row that subsumes
// the runtime input KnownBits, and returns the meet. If no row subsumes, it
// returns top.

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
                                      const unsigned char *oBytes,
                                      unsigned bw, unsigned maskBytes) {
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
                                     const BwTable *tables,
                                     size_t numTables) {
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

  // A bitwidth may be split across several subtables (blobs are chunked to keep
  // each string literal within the limit C++ guarantees). Scan every subtable
  // matching the runtime bitwidth and meet their outputs.
  for (size_t t = 0; t < numTables; ++t) {
    if (tables[t].bw != bw)
      continue;
    if (!packed) {
      for (unsigned a = 0; a < Arity; ++a)
        detail::packArg(args[a], maskBytes,
                        inZ + a * maskBytes, inO + a * maskBytes);
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
