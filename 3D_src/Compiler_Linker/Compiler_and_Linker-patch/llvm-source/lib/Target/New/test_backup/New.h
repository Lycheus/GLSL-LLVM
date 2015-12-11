//===-- New.h - Top-level interface for PowerPC Target ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// PowerPC back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TARGET_POWERPC_H
#define LLVM_TARGET_POWERPC_H

// GCC #defines New on Linux but we use it as our namespace name
#undef New

#include "llvm/Target/TargetMachine.h"

namespace llvm {
  class NewTargetMachine;
  class FunctionPass;
  class formatted_raw_ostream;
  
FunctionPass *createNewBranchSelectionPass();
FunctionPass *createNewISelDag(NewTargetMachine &TM);
FunctionPass *createNewJITCodeEmitterPass(NewTargetMachine &TM,
                                          JITCodeEmitter &MCE);

extern Target TheNew32Target;
extern Target TheNew64Target;

} // end namespace llvm;

// Defines symbolic names for PowerPC registers.  This defines a mapping from
// register name to register number.
//
#include "NewGenRegisterNames.inc"

// Defines symbolic names for the PowerPC instructions.
//
#include "NewGenInstrNames.inc"

#endif
