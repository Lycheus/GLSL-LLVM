include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_llvm_library(LLVMNewInfo
  NewTargetInfo.cpp
  )

add_dependencies(LLVMNewInfo NewCodeGenTable_gen)
