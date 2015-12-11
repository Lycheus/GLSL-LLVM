//===-- NewTargetMachine.h - Define TargetMachine for PowerPC -----*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the PowerPC specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef New_TARGETMACHINE_H
#define New_TARGETMACHINE_H

#include "NewFrameInfo.h"
#include "NewSubtarget.h"
#include "NewJITInfo.h"
#include "NewInstrInfo.h"
#include "NewISelLowering.h"
#include "NewSelectionDAGInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetData.h"

namespace llvm {
class PassManager;
class GlobalValue;

/// NewTargetMachine - Common code between 32-bit and 64-bit PowerPC targets.
///
class NewTargetMachine : public LLVMTargetMachine {
  NewSubtarget        Subtarget;
  const TargetData    DataLayout;       // Calculates type size & alignment
  NewInstrInfo        InstrInfo;
  NewFrameInfo        FrameInfo;
  NewJITInfo          JITInfo;
  NewTargetLowering   TLInfo;
  NewSelectionDAGInfo TSInfo;
  InstrItineraryData  InstrItins;

public:
  NewTargetMachine(const Target &T, const std::string &TT,
                   const std::string &FS, bool is64Bit);

  virtual const NewInstrInfo     *getInstrInfo() const { return &InstrInfo; }
  virtual const NewFrameInfo     *getFrameInfo() const { return &FrameInfo; }
  virtual       NewJITInfo       *getJITInfo()         { return &JITInfo; }
  virtual const NewTargetLowering *getTargetLowering() const { 
   return &TLInfo;
  }
  virtual const NewSelectionDAGInfo* getSelectionDAGInfo() const {
    return &TSInfo;
  }
  virtual const NewRegisterInfo  *getRegisterInfo() const {
    return &InstrInfo.getRegisterInfo();
  }
  
  virtual const TargetData    *getTargetData() const    { return &DataLayout; }
  virtual const NewSubtarget  *getSubtargetImpl() const { return &Subtarget; }
  virtual const InstrItineraryData getInstrItineraryData() const {  
    return InstrItins;
  }

  // Pass Pipeline Configuration
  virtual bool addInstSelector(PassManagerBase &PM, CodeGenOpt::Level OptLevel);
  virtual bool addPreEmitPass(PassManagerBase &PM, CodeGenOpt::Level OptLevel);
  virtual bool addCodeEmitter(PassManagerBase &PM, CodeGenOpt::Level OptLevel,
                              JITCodeEmitter &JCE);
  virtual bool getEnableTailMergeDefault() const;
};

/// New32TargetMachine - PowerPC 32-bit target machine.
///
class New32TargetMachine : public NewTargetMachine {
public:
  New32TargetMachine(const Target &T, const std::string &TT,
                     const std::string &FS);
};

/// New64TargetMachine - PowerPC 64-bit target machine.
///
class New64TargetMachine : public NewTargetMachine {
public:
  New64TargetMachine(const Target &T, const std::string &TT,
                     const std::string &FS);
};

} // end namespace llvm

#endif
