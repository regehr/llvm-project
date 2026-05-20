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
#include "llvm/lib/Analysis/DAGSlicer.h"

#include <algorithm>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {
llvm::cl::OptionCategory SlicerOptions("Slicer options");

llvm::cl::opt<std::string>
    InputFilename("input", llvm::cl::desc("Input LLVM IR or bitcode file"),
                  llvm::cl::Required, llvm::cl::value_desc("filename"),
                  llvm::cl::cat(SlicerOptions));

llvm::cl::opt<std::string>
    OutputFilename("output", llvm::cl::desc("TSV file for pattern counts"),
                   llvm::cl::value_desc("filename"),
                   llvm::cl::init("patterns.tsv"),
                   llvm::cl::cat(SlicerOptions));

// llvm::cl::opt<int> SliceDepth("depth", llvm::cl::desc("Backward slice depth"),
//                               llvm::cl::value_desc("N"),
//                               llvm::cl::cat(SlicerOptions), llvm::cl::init(5));
//
// llvm::cl::opt<bool> SymbolizeConstants(
//     "slice-constant",
//     llvm::cl::desc("Treat integer constants as constant placeholders when "
//                    "matching patterns"),
//     llvm::cl::value_desc("bool"), llvm::cl::cat(SlicerOptions),
//     llvm::cl::init(false));
//
// llvm::cl::list<unsigned>
//     PatternSizes("pattern-size",
//                  llvm::cl::desc("Operation counts for enumerated subpatterns "
//                                 "(default: every size from 2 through --depth)"),
//                  llvm::cl::value_desc("s1,s2,s3,..."), llvm::cl::CommaSeparated,
//                  llvm::cl::cat(SlicerOptions), llvm::cl::ZeroOrMore);

} // namespace

static llvm::ExitOnError ExitOnErr;

static std::unique_ptr<llvm::Module> openInputFile(llvm::LLVMContext &Context,
                                            const std::string &Filename) {
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

class PatternSlicer {
public:
  PatternSlicer(llvm::Module &Module) : Module(Module) {}

  void run() {
    for (llvm::Function &SourceFunction : Module) {
      for (llvm::Instruction &Inst : llvm::instructions(SourceFunction)) {
        llvm::DAGSlicer::enumeratePatterns(
            &Inst, [&](unsigned, llvm::StringRef Pattern) {
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

  std::string Usage = "Extract integer DAG patterns from LLVM IR.\n";

  llvm::cl::HideUnrelatedOptions(SlicerOptions);
  llvm::cl::ParseCommandLineOptions(argc, argv, Usage);

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

  PatternSlicer Slicer(*ModuleOwner);
  Slicer.run();
  Slicer.writePatternCounts(OutputFilename);

  llvm::errs() << "Pattern occurrences: " << Slicer.getPatternOccurrenceCount()
               << "\n";
  llvm::errs() << "Unique patterns: " << Slicer.getUniquePatternCount() << "\n";
  return 0;
}
