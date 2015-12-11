//===- NewInstrInfo.h - PowerPC Instruction Information ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the PowerPC implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef POWERPC32_INSTRUCTIONINFO_H
#define POWERPC32_INSTRUCTIONINFO_H

#include "New.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "NewRegisterInfo.h"

namespace llvm {

// Little helper function for new GPU_BCC instruction detection.  -- Julian                
namespace New {
  inline bool isBranchInstruction(unsigned opcode) {
    return !(opcode != New::BCC && opcode != New::GPU_BCC &&
         opcode != New::GPU_BCC_I8 && opcode != New::GPU_BCC_F4 &&
         opcode != New::GPU_BCC_F8);
  }
}


/// NewII - This namespace holds all of the PowerPC target-specific
/// per-instruction flags.  These must match the corresponding definitions in
/// New.td and NewInstrFormats.td.
namespace NewII {
enum {
  // New970 Instruction Flags.  These flags describe the characteristics of the
  // PowerPC 970 (aka G5) dispatch groups and how they are formed out of
  // raw machine instructions.

  /// New970_First - This instruction starts a new dispatch group, so it will
  /// always be the first one in the group.
  New970_First = 0x1,
  
  /// New970_Single - This instruction starts a new dispatch group and
  /// terminates it, so it will be the sole instruction in the group.
  New970_Single = 0x2,

  /// New970_Cracked - This instruction is cracked into two pieces, requiring
  /// two dispatch pipes to be available to issue.
  New970_Cracked = 0x4,
  
  /// New970_Mask/Shift - This is a bitmask that selects the pipeline type that
  /// an instruction is issued to.
  New970_Shift = 3,
  New970_Mask = 0x07 << New970_Shift
};
enum New970_Unit {
  /// These are the various New970 execution unit pipelines.  Each instruction
  /// is one of these.
  New970_Pseudo = 0 << New970_Shift,   // Pseudo instruction
  New970_FXU    = 1 << New970_Shift,   // Fixed Point (aka Integer/ALU) Unit
  New970_LSU    = 2 << New970_Shift,   // Load Store Unit
  New970_FPU    = 3 << New970_Shift,   // Floating Point Unit
  New970_CRU    = 4 << New970_Shift,   // Control Register Unit
  New970_VALU   = 5 << New970_Shift,   // Vector ALU
  New970_VPERM  = 6 << New970_Shift,   // Vector Permute Unit
  New970_BRU    = 7 << New970_Shift    // Branch Unit
};
}
  
  
class NewInstrInfo : public TargetInstrInfoImpl {
  NewTargetMachine &TM;
  const NewRegisterInfo RI;

  bool StoreRegToStackSlot(MachineFunction &MF,
                           unsigned SrcReg, bool isKill, int FrameIdx,
                           const TargetRegisterClass *RC,
                           SmallVectorImpl<MachineInstr*> &NewMIs) const;
  void LoadRegFromStackSlot(MachineFunction &MF, DebugLoc DL, 
                            unsigned DestReg, int FrameIdx,
                            const TargetRegisterClass *RC,
                            SmallVectorImpl<MachineInstr*> &NewMIs) const;
public:
  explicit NewInstrInfo(NewTargetMachine &TM);

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  ///
  virtual const NewRegisterInfo &getRegisterInfo() const { return RI; }

  unsigned isLoadFromStackSlot(const MachineInstr *MI,
                               int &FrameIndex) const;
  unsigned isStoreToStackSlot(const MachineInstr *MI,
                              int &FrameIndex) const;

  // commuteInstruction - We can commute rlwimi instructions, but only if the
  // rotate amt is zero.  We also have to munge the immediates a bit.
  virtual MachineInstr *commuteInstruction(MachineInstr *MI, bool NewMI) const;
  
  virtual void insertNoop(MachineBasicBlock &MBB, 
                          MachineBasicBlock::iterator MI) const;


  // Branch analysis.
  virtual bool AnalyzeBranch(MachineBasicBlock &MBB, MachineBasicBlock *&TBB,
                             MachineBasicBlock *&FBB,
                             SmallVectorImpl<MachineOperand> &Cond,
                             bool AllowModify) const;
  virtual unsigned RemoveBranch(MachineBasicBlock &MBB) const;
  virtual unsigned InsertBranch(MachineBasicBlock &MBB, MachineBasicBlock *TBB,
                                MachineBasicBlock *FBB,
                                const SmallVectorImpl<MachineOperand> &Cond,
                                DebugLoc DL) const;
  virtual void copyPhysReg(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator I, DebugLoc DL,
                           unsigned DestReg, unsigned SrcReg,
                           bool KillSrc) const;
  
  virtual void storeRegToStackSlot(MachineBasicBlock &MBB,
                                   MachineBasicBlock::iterator MBBI,
                                   unsigned SrcReg, bool isKill, int FrameIndex,
                                   const TargetRegisterClass *RC,
                                   const TargetRegisterInfo *TRI) const;

  virtual void loadRegFromStackSlot(MachineBasicBlock &MBB,
                                    MachineBasicBlock::iterator MBBI,
                                    unsigned DestReg, int FrameIndex,
                                    const TargetRegisterClass *RC,
                                    const TargetRegisterInfo *TRI) const;
  
  virtual MachineInstr *emitFrameIndexDebugValue(MachineFunction &MF,
                                                 int FrameIx,
                                                 uint64_t Offset,
                                                 const MDNode *MDPtr,
                                                 DebugLoc DL) const;

  virtual
  bool ReverseBranchCondition(SmallVectorImpl<MachineOperand> &Cond) const;
  
  /// GetInstSize - Return the number of bytes of code the specified
  /// instruction may be.  This returns the maximum number of bytes.
  ///
  virtual unsigned GetInstSizeInBytes(const MachineInstr *MI) const;
};

}

#endif
