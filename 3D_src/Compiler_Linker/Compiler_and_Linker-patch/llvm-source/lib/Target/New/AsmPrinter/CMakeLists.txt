include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_llvm_library(LLVMNewAsmPrinter
  PPCAsmPrinter.cpp
  )
add_dependencies(LLVMNewAsmPrinter NewCodeGenTable_gen)
