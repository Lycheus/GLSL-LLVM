//===-- NewTargetMachine.cpp - Define TargetMachine for New -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Top-level implementation for the New target.
//
//===----------------------------------------------------------------------===//

#include "New.h"
#include "NewMCAsmInfo.h"
#include "NewTargetMachine.h"
#include "llvm/PassManager.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetRegistry.h"
#include "llvm/Support/FormattedStream.h"
using namespace llvm;

static MCAsmInfo *createMCAsmInfo(const Target &T, StringRef TT) {
  Triple TheTriple(TT);
  bool isNew64 = TheTriple.getArch() == Triple::ppc64;
  if (TheTriple.getOS() == Triple::Darwin)
    return new NewMCAsmInfoDarwin(isNew64);
  return new NewLinuxMCAsmInfo(isNew64);
  
}

extern "C" void LLVMInitializeNewTarget() {
  // Register the targets
  RegisterTargetMachine<New32TargetMachine> A(TheNew32Target);  
  RegisterTargetMachine<New64TargetMachine> B(TheNew64Target);
  
  RegisterAsmInfoFn C(TheNew32Target, createMCAsmInfo);
  RegisterAsmInfoFn D(TheNew64Target, createMCAsmInfo);
}


NewTargetMachine::NewTargetMachine(const Target &T, const std::string &TT,
                                   const std::string &FS, bool is64Bit)
  : LLVMTargetMachine(T, TT),
    Subtarget(TT, FS, is64Bit),
    DataLayout(Subtarget.getTargetDataString()), InstrInfo(*this),
    FrameInfo(*this, is64Bit), JITInfo(*this, is64Bit),
    TLInfo(*this), TSInfo(*this),
    InstrItins(Subtarget.getInstrItineraryData()) {

  if (getRelocationModel() == Reloc::Default) {
    if (Subtarget.isDarwin())
      setRelocationModel(Reloc::DynamicNoPIC);
    else
      setRelocationModel(Reloc::Static);
  }
}

/// Override this for New.  Tail merging happily breaks up instruction issue
/// groups, which typically degrades performance.
bool NewTargetMachine::getEnableTailMergeDefault() const { return false; }

New32TargetMachine::New32TargetMachine(const Target &T, const std::string &TT, 
                                       const std::string &FS) 
  : NewTargetMachine(T, TT, FS, false) {
}


New64TargetMachine::New64TargetMachine(const Target &T, const std::string &TT, 
                                       const std::string &FS)
  : NewTargetMachine(T, TT, FS, true) {
}


//===----------------------------------------------------------------------===//
// Pass Pipeline Configuration
//===----------------------------------------------------------------------===//

bool NewTargetMachine::addInstSelector(PassManagerBase &PM,
                                       CodeGenOpt::Level OptLevel) {
  // Install an instruction selector.
  PM.add(createNewISelDag(*this));
  return false;
}

bool NewTargetMachine::addPreEmitPass(PassManagerBase &PM,
                                      CodeGenOpt::Level OptLevel) {
  // Must run branch selection immediately preceding the asm printer.
  PM.add(createNewBranchSelectionPass());
  return false;
}

bool NewTargetMachine::addCodeEmitter(PassManagerBase &PM,
                                      CodeGenOpt::Level OptLevel,
                                      JITCodeEmitter &JCE) {
  // The JIT should use the static relocation model in ppc32 mode, PIC in ppc64.
  // FIXME: This should be moved to TargetJITInfo!!
  if (Subtarget.isNew64()) {
    // We use PIC codegen in ppc64 mode, because otherwise we'd have to use many
    // instructions to materialize arbitrary global variable + function +
    // constant pool addresses.
    setRelocationModel(Reloc::PIC_);
    // Temporary workaround for the inability of New64 JIT to handle jump
    // tables.
    DisableJumpTables = true;      
  } else {
    setRelocationModel(Reloc::Static);
  }
  
  // Inform the subtarget that we are in JIT mode.  FIXME: does this break macho
  // writing?
  Subtarget.SetJITMode();
  
  // Machine code emitter pass for New.
  PM.add(createNewJITCodeEmitterPass(*this, JCE));

  return false;
}
