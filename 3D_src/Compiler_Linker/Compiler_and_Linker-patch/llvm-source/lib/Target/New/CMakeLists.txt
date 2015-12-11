set(LLVM_TARGET_DEFINITIONS New.td)

tablegen(NewGenInstrNames.inc -gen-instr-enums)
tablegen(NewGenRegisterNames.inc -gen-register-enums)
tablegen(NewGenAsmWriter.inc -gen-asm-writer)
tablegen(NewGenCodeEmitter.inc -gen-emitter)
tablegen(NewGenRegisterInfo.h.inc -gen-register-desc-header)
tablegen(NewGenRegisterInfo.inc -gen-register-desc)
tablegen(NewGenInstrInfo.inc -gen-instr-desc)
tablegen(NewGenDAGISel.inc -gen-dag-isel)
tablegen(NewGenCallingConv.inc -gen-callingconv)
tablegen(NewGenSubtarget.inc -gen-subtarget)

add_llvm_target(NewCodeGen
  NewBranchSelector.cpp
  NewCodeEmitter.cpp
  NewHazardRecognizers.cpp
  NewInstrInfo.cpp
  NewISelDAGToDAG.cpp
  NewISelLowering.cpp
  NewJITInfo.cpp
  NewMCAsmInfo.cpp
  NewPredicates.cpp
  NewRegisterInfo.cpp
  NewSubtarget.cpp
  NewTargetMachine.cpp
  NewSelectionDAGInfo.cpp
  )

target_link_libraries (LLVMNewCodeGen LLVMSelectionDAG)
