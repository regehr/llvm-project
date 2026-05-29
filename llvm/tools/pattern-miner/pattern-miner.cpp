#include "DAGSlicer.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include <algorithm>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {
llvm::cl::OptionCategory PatternMinerOptions("Pattern miner options");

llvm::cl::opt<std::string>
    InputFilename("input", llvm::cl::desc("Input LLVM IR or bitcode file"),
                  llvm::cl::Required, llvm::cl::value_desc("filename"),
                  llvm::cl::cat(PatternMinerOptions));

llvm::cl::opt<std::string>
    OutputFilename("output", llvm::cl::desc("TSV file for pattern counts"),
                   llvm::cl::value_desc("filename"),
                   llvm::cl::init("patterns.tsv"),
                   llvm::cl::cat(PatternMinerOptions));

llvm::cl::opt<unsigned>
    PatternDepth("depth", llvm::cl::desc("Maximum pattern depth to mine"),
                 llvm::cl::value_desc("N"),
                 llvm::cl::init(llvm::MaxAnalysisRecursionDepth),
                 llvm::cl::cat(PatternMinerOptions));

} // namespace

static llvm::ExitOnError ExitOnErr;

static std::unique_ptr<llvm::Module>
openInputFile(llvm::LLVMContext &Context, const std::string &Filename) {
  auto MB = ExitOnErr(errorOrToExpected(llvm::MemoryBuffer::getFile(Filename)));
  llvm::SMDiagnostic Diag;
  auto M = getLazyIRModule(std::move(MB), Diag, Context,
                           /*ShouldLazyLoadMetadata=*/true);
  if (!M) {
    Diag.print("", llvm::errs(), false);
    return nullptr;
  }
  ExitOnErr(M->materializeAll());
  return M;
}

static std::string getOutputDirectory() {
  return llvm::sys::path::parent_path(OutputFilename).str();
}

class PatternMiner {
public:
  PatternMiner(llvm::Module &Module) : Module(Module) {}

  void run() {
    for (llvm::Function &SourceFunction : Module) {
      for (llvm::Instruction &Inst : llvm::instructions(SourceFunction)) {
        llvm::DAGSlicer::enumeratePatterns(&Inst, 2, PatternDepth,
                                           [&](llvm::StringRef Pattern) {
                                             ++PatternCounts[Pattern.str()];
                                             ++PatternOccurrenceCount;
                                           });
      }
    }
  }

  void writePatternCounts(const std::string &OutputFilename) const {
    std::vector<std::pair<std::string, unsigned>> Counts(PatternCounts.begin(),
                                                         PatternCounts.end());
    std::sort(Counts.begin(), Counts.end(),
              [](const auto &LHS, const auto &RHS) {
                if (LHS.second != RHS.second)
                  return LHS.second > RHS.second;
                return LHS.first < RHS.first;
              });

    std::ofstream Out(OutputFilename);
    if (!Out) {
      llvm::errs() << "Error: cannot open counts file: " << OutputFilename
                   << "\n";
      return;
    }

    Out << "count\tpattern\n";
    for (const auto &Entry : Counts)
      Out << Entry.second << '\t' << Entry.first << '\n';
  }

  unsigned getPatternOccurrenceCount() const { return PatternOccurrenceCount; }

  std::size_t getUniquePatternCount() const { return PatternCounts.size(); }

private:
  llvm::Module &Module;
  std::unordered_map<std::string, unsigned> PatternCounts;
  unsigned PatternOccurrenceCount = 0;
};

int main(int argc, char **argv) {
  llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);
  llvm::InitLLVM X(argc, argv);
  llvm::EnableDebugBuffering = true;
  llvm::LLVMContext Context;

  std::string Usage = "Mine integer DAG patterns from LLVM IR.\n";

  llvm::cl::HideUnrelatedOptions(PatternMinerOptions);
  llvm::cl::ParseCommandLineOptions(argc, argv, Usage);

  if (PatternDepth < 2) {
    llvm::errs() << "Error: --depth must be at least 2\n";
    return -1;
  }

  std::string OutputDirectory = getOutputDirectory();
  if (!OutputDirectory.empty()) {
    std::error_code EC = llvm::sys::fs::create_directories(OutputDirectory);
    if (EC) {
      llvm::errs() << "Could not create output directory '" << OutputDirectory
                   << "': " << EC.message() << "\n";
      return -1;
    }
  }

  auto ModuleOwner = openInputFile(Context, InputFilename);
  if (!ModuleOwner) {
    llvm::errs() << "Could not read input file from '" << InputFilename
                 << "'\n";
    return -1;
  }

  PatternMiner Miner(*ModuleOwner);
  Miner.run();
  Miner.writePatternCounts(OutputFilename);

  llvm::errs() << "Pattern occurrences: " << Miner.getPatternOccurrenceCount()
               << "\n";
  llvm::errs() << "Unique patterns: " << Miner.getUniquePatternCount() << "\n";
  return 0;
}
