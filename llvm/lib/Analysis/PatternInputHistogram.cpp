#include "llvm/Analysis/PatternInputHistogram.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include <cassert>
#include <system_error>

using namespace llvm;

static cl::opt<std::string> PatternHistFile(
    "value-tracking-pattern-histogram", cl::Hidden, cl::init(""),
    cl::desc("Write a TSV histogram of pattern-transformer inputs to this file "
             "at exit (domain + id + bw + abstract operands)."));

static PatternInputHistogram &getPatternInputHistogram() {
  static PatternInputHistogram Histogram;
  return Histogram;
}

template <typename T> static unsigned getInputBitWidth(ArrayRef<T> Inputs) {
  unsigned BW = 1;
  for (const T &In : Inputs) {
    unsigned InputBW = In.getBitWidth();
    if (InputBW == 1)
      continue;
    assert((BW == 1 || BW == InputBW) && "heterogeneous pattern input widths");
    BW = InputBW;
  }
  return BW;
}

static void printSConstRange(raw_ostream &OS, const ConstantRange &CR) {
  unsigned BW = CR.getBitWidth();
  if (CR.isEmptySet()) {
    OS << "(bottom)";
    return;
  }
  if (CR.isFullSet() || CR.isSignWrappedSet()) {
    OS << '[';
    APInt::getSignedMinValue(BW).print(OS, true);
    OS << ", ";
    APInt::getSignedMaxValue(BW).print(OS, true);
    OS << ']';
    return;
  }
  OS << '[';
  CR.getSignedMin().print(OS, true);
  OS << ", ";
  CR.getSignedMax().print(OS, true);
  OS << ']';
}

static void printUConstRange(raw_ostream &OS, const ConstantRange &CR) {
  unsigned BW = CR.getBitWidth();
  if (CR.isEmptySet()) {
    OS << "(bottom)";
    return;
  }
  if (CR.isFullSet() || CR.isWrappedSet()) {
    OS << '[';
    APInt::getZero(BW).print(OS, false);
    OS << ", ";
    APInt::getMaxValue(BW).print(OS, false);
    OS << ']';
    return;
  }
  OS << '[';
  CR.getUnsignedMin().print(OS, false);
  OS << ", ";
  CR.getUnsignedMax().print(OS, false);
  OS << ']';
}

void PatternInputHistogram::record(StringRef OutFile, StringRef Domain,
                                   unsigned ID, ArrayRef<KnownBits> Inputs,
                                   unsigned BitsAdded, bool Bottom) {
  std::lock_guard<std::mutex> Lock(Mtx);
  // Capture the output path on first use; reading the cl::opt here (during
  // compilation) is safe, whereas reading it in the destructor would race
  // static-destruction order.
  if (Path.empty())
    Path = OutFile.str();
  std::string Key;
  raw_string_ostream OS(Key);
  OS << Domain << '\t' << ID << '\t' << getInputBitWidth(Inputs);
  for (const KnownBits &In : Inputs) {
    OS << '\t';
    In.print(OS);
  }
  Row &R = Rows[OS.str()];
  ++R.Count;
  R.BitsAdded += BitsAdded;
  R.Bottom += Bottom ? 1 : 0;
}

void PatternInputHistogram::record(StringRef OutFile, StringRef Domain,
                                   unsigned ID, ArrayRef<ConstantRange> Inputs,
                                   bool ForSigned, unsigned BitsAdded,
                                   bool Bottom) {
  std::lock_guard<std::mutex> Lock(Mtx);
  if (Path.empty())
    Path = OutFile.str();
  std::string Key;
  raw_string_ostream OS(Key);
  OS << Domain << '\t' << ID << '\t' << getInputBitWidth(Inputs);
  for (const ConstantRange &In : Inputs) {
    OS << '\t';
    if (ForSigned)
      printSConstRange(OS, In);
    else
      printUConstRange(OS, In);
  }
  Row &R = Rows[OS.str()];
  ++R.Count;
  R.BitsAdded += BitsAdded;
  R.Bottom += Bottom ? 1 : 0;
}

PatternInputHistogram::~PatternInputHistogram() {
  if (Path.empty() || Rows.empty())
    return;
  std::error_code EC;
  raw_fd_ostream Out(Path, EC, sys::fs::OF_Text);
  if (EC)
    return;
  // <domain> <id> <bw> <arg0> <arg1> ... <count> <bits_added> <bottom>
  for (const auto &KV : Rows)
    Out << KV.first << '\t' << KV.second.Count << '\t' << KV.second.BitsAdded
        << '\t' << KV.second.Bottom << '\n';
}

void llvm::recordKBPatternHistogram(unsigned ID, ArrayRef<KnownBits> Inputs,
                                    unsigned BitsAdded, bool Bottom) {
  if (PatternHistFile.empty())
    return;
  getPatternInputHistogram().record(PatternHistFile, "kb", ID, Inputs,
                                    BitsAdded, Bottom);
}

void llvm::recordCRPatternHistogram(unsigned ID, ArrayRef<ConstantRange> Inputs,
                                    bool ForSigned, unsigned BitsAdded,
                                    bool Bottom) {
  if (PatternHistFile.empty())
    return;
  getPatternInputHistogram().record(PatternHistFile, ForSigned ? "scr" : "ucr",
                                    ID, Inputs, ForSigned, BitsAdded, Bottom);
}
