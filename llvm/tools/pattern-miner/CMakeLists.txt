set(LLVM_LINK_COMPONENTS
  Analysis
  BitReader
  Core
  IRReader
  Support
  )

add_llvm_tool(pattern-miner
  pattern-miner.cpp
  )

target_include_directories(pattern-miner PRIVATE
  ${LLVM_MAIN_SRC_DIR}/lib/Analysis
  )
