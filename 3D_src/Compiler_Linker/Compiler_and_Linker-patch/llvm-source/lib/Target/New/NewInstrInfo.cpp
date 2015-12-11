//===- NewInstrInfo.cpp - PowerPC32 Instruction Information -----*- C++ -*-===//
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

#include "NewInstrInfo.h"
#include "NewInstrBuilder.h"
#include "NewMachineFunctionInfo.h"
#include "NewPredicates.h"
#include "NewGenInstrInfo.inc"
#include "NewTargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/PseudoSourceValue.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/MC/MCAsmInfo.h"

#include <iostream>
using std::cout;

namespace llvm {
extern cl::opt<bool> EnableNew32RS;  // FIXME (64-bit): See NewRegisterInfo.cpp.
extern cl::opt<bool> EnableNew64RS;  // FIXME (64-bit): See NewRegisterInfo.cpp.
}

using namespace llvm;

NewInstrInfo::NewInstrInfo(NewTargetMachine &tm)
  : TargetInstrInfoImpl(NewInsts, array_lengthof(NewInsts)), TM(tm),
    RI(*TM.getSubtargetImpl(), *this) {}

unsigned NewInstrInfo::isLoadFromStackSlot(const MachineInstr *MI, 
                                           int &FrameIndex) const {
  switch (MI->getOpcode()) {
  default: break;
  case New::LD:
  case New::LWZ:
  case New::LFS:
  case New::LFD:
    if (MI->getOperand(1).isImm() && !MI->getOperand(1).getImm() &&
        MI->getOperand(2).isFI()) {
      FrameIndex = MI->getOperand(2).getIndex();
      return MI->getOperand(0).getReg();
    }
    break;
  }
  return 0;
}

unsigned NewInstrInfo::isStoreToStackSlot(const MachineInstr *MI, 
                                          int &FrameIndex) const {
  switch (MI->getOpcode()) {
  default: break;
  case New::STD:
  case New::STW:
  case New::STFS:
  case New::STFD:
    if (MI->getOperand(1).isImm() && !MI->getOperand(1).getImm() &&
        MI->getOperand(2).isFI()) {
      FrameIndex = MI->getOperand(2).getIndex();
      return MI->getOperand(0).getReg();
    }
    break;
  }
  return 0;
}

// commuteInstruction - We can commute rlwimi instructions, but only if the
// rotate amt is zero.  We also have to munge the immediates a bit.
MachineInstr *
NewInstrInfo::commuteInstruction(MachineInstr *MI, bool NewMI) const {
  MachineFunction &MF = *MI->getParent()->getParent();

  // Normal instructions can be commuted the obvious way.

  /*
  if (MI->getOpcode() != New::RLWIMI)
    return TargetInstrInfoImpl::commuteInstruction(MI, NewMI);
  */ //EXCESS4

  // Cannot commute if it has a non-zero rotate count.
  if (MI->getOperand(3).getImm() != 0)
    return 0;
  
  // If we have a zero rotate count, we have:
  //   M = mask(MB,ME)
  //   Op0 = (Op1 & ~M) | (Op2 & M)
  // Change this to:
  //   M = mask((ME+1)&31, (MB-1)&31)
  //   Op0 = (Op2 & ~M) | (Op1 & M)

  // Swap op1/op2
  unsigned Reg0 = MI->getOperand(0).getReg();
  unsigned Reg1 = MI->getOperand(1).getReg();
  unsigned Reg2 = MI->getOperand(2).getReg();
  bool Reg1IsKill = MI->getOperand(1).isKill();
  bool Reg2IsKill = MI->getOperand(2).isKill();
  bool ChangeReg0 = false;
  // If machine instrs are no longer in two-address forms, update
  // destination register as well.
  if (Reg0 == Reg1) {
    // Must be two address instruction!
    assert(MI->getDesc().getOperandConstraint(0, TOI::TIED_TO) &&
           "Expecting a two-address instruction!");
    Reg2IsKill = false;
    ChangeReg0 = true;
  }

  // Masks.
  unsigned MB = MI->getOperand(4).getImm();
  unsigned ME = MI->getOperand(5).getImm();

  if (NewMI) {
    // Create a new instruction.
    unsigned Reg0 = ChangeReg0 ? Reg2 : MI->getOperand(0).getReg();
    bool Reg0IsDead = MI->getOperand(0).isDead();
    return BuildMI(MF, MI->getDebugLoc(), MI->getDesc())
      .addReg(Reg0, RegState::Define | getDeadRegState(Reg0IsDead))
      .addReg(Reg2, getKillRegState(Reg2IsKill))
      .addReg(Reg1, getKillRegState(Reg1IsKill))
      .addImm((ME+1) & 31)
      .addImm((MB-1) & 31);
  }

  if (ChangeReg0)
    MI->getOperand(0).setReg(Reg2);
  MI->getOperand(2).setReg(Reg1);
  MI->getOperand(1).setReg(Reg2);
  MI->getOperand(2).setIsKill(Reg1IsKill);
  MI->getOperand(1).setIsKill(Reg2IsKill);
  
  // Swap the mask around.
  MI->getOperand(4).setImm((ME+1) & 31);
  MI->getOperand(5).setImm((MB-1) & 31);
  return MI;
}

void NewInstrInfo::insertNoop(MachineBasicBlock &MBB, 
                              MachineBasicBlock::iterator MI) const {
  DebugLoc DL;
  BuildMI(MBB, MI, DL, get(New::NOP));
}


// Branch analysis.
bool NewInstrInfo::AnalyzeBranch(MachineBasicBlock &MBB,MachineBasicBlock *&TBB,
                                 MachineBasicBlock *&FBB,
                                 SmallVectorImpl<MachineOperand> &Cond,
                                 bool AllowModify) const {
  // If the block has no terminators, it just falls into the block after it.
  MachineBasicBlock::iterator I = MBB.end();
  if (I == MBB.begin())
    return false;
  --I;
  while (I->isDebugValue()) {
    if (I == MBB.begin())
      return false;
    --I;
  }
  if (!isUnpredicatedTerminator(I))
    return false;

  // Get the last instruction in the block.
  MachineInstr *LastInst = I;
  
  // If there is only one terminator instruction, process it.
  if (I == MBB.begin() || !isUnpredicatedTerminator(--I)) {
    if (LastInst->getOpcode() == New::B) {
      if (!LastInst->getOperand(0).isMBB())
        return true;
      TBB = LastInst->getOperand(0).getMBB();
      return false;
    } else if (LastInst->getOpcode() == New::BCC) {
      if (!LastInst->getOperand(2).isMBB())
        return true;
      // Block ends with fall-through condbranch.
      TBB = LastInst->getOperand(2).getMBB();
      Cond.push_back(LastInst->getOperand(0));
      Cond.push_back(LastInst->getOperand(1));
      return false;
    }
    // Otherwise, don't know what this is.
    return true;
  }
  
  // Get the instruction before it if it's a terminator.
  MachineInstr *SecondLastInst = I;

  // If there are three terminators, we don't know what sort of block this is.
  if (SecondLastInst && I != MBB.begin() &&
      isUnpredicatedTerminator(--I))
    return true;
  
  // If the block ends with New::B and New:BCC, handle it.
  if (SecondLastInst->getOpcode() == New::BCC && 
      LastInst->getOpcode() == New::B) {
    if (!SecondLastInst->getOperand(2).isMBB() ||
        !LastInst->getOperand(0).isMBB())
      return true;
    TBB =  SecondLastInst->getOperand(2).getMBB();
    Cond.push_back(SecondLastInst->getOperand(0));
    Cond.push_back(SecondLastInst->getOperand(1));
    FBB = LastInst->getOperand(0).getMBB();
    return false;
  }
  
  // If the block ends with two New:Bs, handle it.  The second one is not
  // executed, so remove it.
  if (SecondLastInst->getOpcode() == New::B && 
      LastInst->getOpcode() == New::B) {
    if (!SecondLastInst->getOperand(0).isMBB())
      return true;
    TBB = SecondLastInst->getOperand(0).getMBB();
    I = LastInst;
    if (AllowModify)
      I->eraseFromParent();
    return false;
  }

  // Otherwise, can't handle this.
  return true;
}

unsigned NewInstrInfo::RemoveBranch(MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator I = MBB.end();
  if (I == MBB.begin()) return 0;
  --I;
  while (I->isDebugValue()) {
    if (I == MBB.begin())
      return 0;
    --I;
  }
  if (I->getOpcode() != New::B && I->getOpcode() != New::BCC)
    return 0;
  
  // Remove the branch.
  I->eraseFromParent();
  
  I = MBB.end();

  if (I == MBB.begin()) return 1;
  --I;
  if (I->getOpcode() != New::BCC)
    return 1;
  
  // Remove the branch.
  I->eraseFromParent();
  return 2;
}

unsigned
NewInstrInfo::InsertBranch(MachineBasicBlock &MBB, MachineBasicBlock *TBB,
                           MachineBasicBlock *FBB,
                           const SmallVectorImpl<MachineOperand> &Cond,
                           DebugLoc DL) const {
  // Shouldn't be a fall through.
  assert(TBB && "InsertBranch must not be told to insert a fallthrough");
  assert((Cond.size() == 2 || Cond.size() == 0) && 
         "New branch conditions have two components!");
  
  // One-way branch.
  if (FBB == 0) {
    if (Cond.empty())   // Unconditional branch
      BuildMI(&MBB, DL, get(New::B)).addMBB(TBB);
    else                // Conditional branch
      BuildMI(&MBB, DL, get(New::BCC))
        .addImm(Cond[0].getImm()).addReg(Cond[1].getReg()).addMBB(TBB);
    return 1;
  }
  
  // Two-way Conditional Branch.
  BuildMI(&MBB, DL, get(New::BCC))
    .addImm(Cond[0].getImm()).addReg(Cond[1].getReg()).addMBB(TBB);
  BuildMI(&MBB, DL, get(New::B)).addMBB(FBB);
  return 2;
}

void NewInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                               MachineBasicBlock::iterator I, DebugLoc DL,
                               unsigned DestReg, unsigned SrcReg,
                               bool KillSrc) const {
  unsigned Opc;
  //dondum
  //int flag=0;
  //cout<<"in here??\n";
  if (New::GPRCRegClass.contains(DestReg, SrcReg))
    Opc = New::OR;
  //dondum 2010-11-07. 
  //if DestReg=GPRC & SrcReg=VRRC, Build Machine Instruction for vector OR directly.
  else if (New::GPRCRegClass.contains(DestReg, DestReg)&& New::VRRCRegClass.contains(SrcReg, SrcReg))
    Opc = New::VOR;
  //if DestReg=VPRC & SrcReg=GRRC, Build Machine Instruction for vector OR directly.
  else if (New::VRRCRegClass.contains(DestReg, DestReg)&& New::GPRCRegClass.contains(SrcReg, SrcReg))
    Opc = New::VOR;
  //dondum 2011-02-24.
  else if (New::GPRCRegClass.contains(SrcReg, SrcReg)&& New::F4RCRegClass.contains(DestReg, DestReg))
    Opc = New::FMR;
  else if (New::GPRCRegClass.contains(DestReg, DestReg)&& New::F4RCRegClass.contains(SrcReg, SrcReg))
    Opc = New::OR;
  //end of this addition.
  else if (New::G8RCRegClass.contains(DestReg, SrcReg))
    Opc = New::OR8;
  else if (New::F4RCRegClass.contains(DestReg, SrcReg))
    Opc = New::FMR;
  else if (New::CRRCRegClass.contains(DestReg, SrcReg))
    Opc = New::MCRF;
  else if (New::VRRCRegClass.contains(DestReg, SrcReg))
    Opc = New::VOR;
  else if (New::CRBITRCRegClass.contains(DestReg, SrcReg))
    Opc = New::CROR;
  else if (New::VRRCRegClass.contains(SrcReg,SrcReg)&&New::F4RCRegClass.contains(DestReg))//Kaymon
    Opc = New::CROR;
  else if (New::VRRCRegClass.contains(DestReg)&&New::F4RCRegClass.contains(SrcReg,SrcReg))//Kaymon
    Opc = New::CROR;
  else{
    //Opc = New::FMR;
    /*
    cout<<DestReg <<" "<<New::GPRCRegClass.contains(DestReg)<<" G dst\n";
    cout<<SrcReg<<" "<<New::F4RCRegClass.contains(SrcReg, SrcReg)<<" F src\n";
    cout<<(New::GPRCRegClass.contains(DestReg)&&New::F4RCRegClass.contains(SrcReg, SrcReg) )<<" final \n";
    cout<<DestReg<<" "<<
    cout<<SrcReg <<" "<<New::GPRCRegClass.contains(SrcReg,SrcReg)<<" G dst\n";
    cout<<SrcReg <<" "<<New::VRRCRegClass.contains(SrcReg,SrcReg)<<" V dst\n";
    cout<<(New::GPRCRegClass.contains(SrcReg,SrcReg)&&New::F4RCRegClass.contains(DestReg) )<<" final2 \n";
    cout<<(New::GPRCRegClass.contains(SrcReg)&&New::F4RCRegClass.contains(DestReg) )<<" final3 \n";
    cout<<(New::VRRCRegClass.contains(SrcReg)&&New::F4RCRegClass.contains(DestReg) )<<" final4 \n";
    cout<<(New::VRRCRegClass.contains(SrcReg,SrcReg)&&New::F4RCRegClass.contains(DestReg) )<<" final4 \n";
    cout<<"here error message \n";
    cout.flush();*/
    llvm_unreachable("Impossible reg-to-reg copy");
  }
  //cout<<"middle ??\n";
  //cout.flush();
  //if(!flag){ //dondum
  const TargetInstrDesc &TID = get(Opc);
  if (TID.getNumOperands() == 3)
    BuildMI(MBB, I, DL, TID, DestReg)
      .addReg(SrcReg).addReg(SrcReg, getKillRegState(KillSrc));
  else
    BuildMI(MBB, I, DL, TID, DestReg).addReg(SrcReg, getKillRegState(KillSrc));
    //}

  //cout<<"exit NewInstrInfo ??\n";
  //cout.flush();
}

bool
NewInstrInfo::StoreRegToStackSlot(MachineFunction &MF,
                                  unsigned SrcReg, bool isKill,
                                  int FrameIdx,
                                  const TargetRegisterClass *RC,
                                  SmallVectorImpl<MachineInstr*> &NewMIs) const{
  DebugLoc DL;
  if (RC == New::GPRCRegisterClass) {
    if (SrcReg != New::LR) {
      NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::STW))
                                         .addReg(SrcReg,
                                                 getKillRegState(isKill)),
                                         FrameIdx));
    } else {
      // FIXME: this spills LR immediately to memory in one step.  To do this,
      // we use R11, which we know cannot be used in the prolog/epilog.  This is
      // a hack.
      NewMIs.push_back(BuildMI(MF, DL, get(New::MFLR), New::R11));
      NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::STW))
                                         .addReg(New::R11,
                                                 getKillRegState(isKill)),
                                         FrameIdx));
    }
  } else if (RC == New::G8RCRegisterClass) {
    if (SrcReg != New::LR8) {
      NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::STD))
                                         .addReg(SrcReg,
                                                 getKillRegState(isKill)),
                                         FrameIdx));
    } else {
      // FIXME: this spills LR immediately to memory in one step.  To do this,
      // we use R11, which we know cannot be used in the prolog/epilog.  This is
      // a hack.
      NewMIs.push_back(BuildMI(MF, DL, get(New::MFLR8), New::X11));
      NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::STD))
                                         .addReg(New::X11,
                                                 getKillRegState(isKill)),
                                         FrameIdx));
    }
  } else if (RC == New::F8RCRegisterClass) {
    NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::STFD))
                                       .addReg(SrcReg,
                                               getKillRegState(isKill)),
                                       FrameIdx));
  } else if (RC == New::F4RCRegisterClass) {
    NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::STFS))
                                       .addReg(SrcReg,
                                               getKillRegState(isKill)),
                                       FrameIdx));
  } else if (RC == New::CRRCRegisterClass) {
    if ((EnableNew32RS && !TM.getSubtargetImpl()->isNew64()) ||
        (EnableNew64RS && TM.getSubtargetImpl()->isNew64())) {
      // FIXME (64-bit): Enable
      NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::SPILL_CR))
                                         .addReg(SrcReg,
                                                 getKillRegState(isKill)),
                                         FrameIdx));
      return true;
    } else {
      // FIXME: We need a scatch reg here.  The trouble with using R0 is that
      // it's possible for the stack frame to be so big the save location is
      // out of range of immediate offsets, necessitating another register.
      // We hack this on Darwin by reserving R2.  It's probably broken on Linux
      // at the moment.

      // We need to store the CR in the low 4-bits of the saved value.  First,
      // issue a MFCR to save all of the CRBits.
      /*      unsigned ScratchReg = TM.getSubtargetImpl()->isDarwinABI() ? 
                                                           New::R2 : New::R0;
      NewMIs.push_back(BuildMI(MF, DL, get(New::MFCRpseud), ScratchReg)
                               .addReg(SrcReg, getKillRegState(isKill)));
      */
      NewMIs.push_back(BuildMI(MF, DL, get(New::MFCR), New::R0));

      // If the saved register wasn't CR0, shift the bits left so that they are
      // in CR0's slot.
      if (SrcReg != New::CR0) {
	/*        unsigned ShiftBits = NewRegisterInfo::getRegisterNumbering(SrcReg)*4;
        // rlwinm scratch, scratch, ShiftBits, 0, 31.
        NewMIs.push_back(BuildMI(MF, DL, get(New::RLWINM), ScratchReg)
                       .addReg(ScratchReg).addImm(ShiftBits)
                       .addImm(0).addImm(31));*/
	//Julian, added by dondum
      }
    
      NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::STW))
					 //.addReg(ScratchReg,
					 .addReg(New::R0,
                                                 getKillRegState(isKill)),
                                         FrameIdx));
    }
  } else if (RC == New::CRBITRCRegisterClass) {
    // FIXME: We use CRi here because there is no mtcrf on a bit. Since the
    // backend currently only uses CR1EQ as an individual bit, this should
    // not cause any bug. If we need other uses of CR bits, the following
    // code may be invalid.
    unsigned Reg = 0;
    if (SrcReg == New::CR0LT || SrcReg == New::CR0GT ||
        SrcReg == New::CR0EQ || SrcReg == New::CR0UN)
      Reg = New::CR0;
    else if (SrcReg == New::CR1LT || SrcReg == New::CR1GT ||
             SrcReg == New::CR1EQ || SrcReg == New::CR1UN)
      Reg = New::CR1;
    else if (SrcReg == New::CR2LT || SrcReg == New::CR2GT ||
             SrcReg == New::CR2EQ || SrcReg == New::CR2UN)
      Reg = New::CR2;
    else if (SrcReg == New::CR3LT || SrcReg == New::CR3GT ||
             SrcReg == New::CR3EQ || SrcReg == New::CR3UN)
      Reg = New::CR3;
    else if (SrcReg == New::CR4LT || SrcReg == New::CR4GT ||
             SrcReg == New::CR4EQ || SrcReg == New::CR4UN)
      Reg = New::CR4;
    else if (SrcReg == New::CR5LT || SrcReg == New::CR5GT ||
             SrcReg == New::CR5EQ || SrcReg == New::CR5UN)
      Reg = New::CR5;
    else if (SrcReg == New::CR6LT || SrcReg == New::CR6GT ||
             SrcReg == New::CR6EQ || SrcReg == New::CR6UN)
      Reg = New::CR6;
    else if (SrcReg == New::CR7LT || SrcReg == New::CR7GT ||
             SrcReg == New::CR7EQ || SrcReg == New::CR7UN)
      Reg = New::CR7;

    return StoreRegToStackSlot(MF, Reg, isKill, FrameIdx, 
                               New::CRRCRegisterClass, NewMIs);

  } else if (RC == New::VRRCRegisterClass) {
    // We don't have indexed addressing for vector loads.  Emit:
    // R0 = ADDI FI#
    // STVX VAL, 0, R0
    // 
    // FIXME: We use R0 here, because it isn't available for RA.
    /*sheng-chang 
    NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::ADDI), New::R0),
                                       FrameIdx, 0, 0));
    NewMIs.push_back(BuildMI(MF, DL, get(New::STVX))
                     .addReg(SrcReg, getKillRegState(isKill))
                     .addReg(New::R0)
                     .addReg(New::R0));
    */
    //sheng-chang add
    
    NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::ADDISC), New::R0),
                                       FrameIdx, 0, 0));
    NewMIs.push_back(BuildMI(MF, DL, get(New::STVXS))
                     .addReg(SrcReg, getKillRegState(isKill))
                     .addReg(New::R0)
                     .addReg(New::R0));
    
  } else {
    llvm_unreachable("Unknown regclass!");
  }

  return false;
}

void
NewInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator MI,
                                  unsigned SrcReg, bool isKill, int FrameIdx,
                                  const TargetRegisterClass *RC,
                                  const TargetRegisterInfo *TRI) const {
  MachineFunction &MF = *MBB.getParent();
  SmallVector<MachineInstr*, 4> NewMIs;

  if (StoreRegToStackSlot(MF, SrcReg, isKill, FrameIdx, RC, NewMIs)) {
    NewFunctionInfo *FuncInfo = MF.getInfo<NewFunctionInfo>();
    FuncInfo->setSpillsCR();
  }

  for (unsigned i = 0, e = NewMIs.size(); i != e; ++i)
    MBB.insert(MI, NewMIs[i]);

  const MachineFrameInfo &MFI = *MF.getFrameInfo();
  MachineMemOperand *MMO =
    MF.getMachineMemOperand(PseudoSourceValue::getFixedStack(FrameIdx),
                            MachineMemOperand::MOStore, /*Offset=*/0,
                            MFI.getObjectSize(FrameIdx),
                            MFI.getObjectAlignment(FrameIdx));
  NewMIs.back()->addMemOperand(MF, MMO);
}

void
NewInstrInfo::LoadRegFromStackSlot(MachineFunction &MF, DebugLoc DL,
                                   unsigned DestReg, int FrameIdx,
                                   const TargetRegisterClass *RC,
                                   SmallVectorImpl<MachineInstr*> &NewMIs)const{
  if (RC == New::GPRCRegisterClass) {
    if (DestReg != New::LR) {
      NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::LWZ),
                                                 DestReg), FrameIdx));
    } else {
      NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::LWZ),
                                                 New::R11), FrameIdx));
      NewMIs.push_back(BuildMI(MF, DL, get(New::MTLR)).addReg(New::R11));
    }
  } else if (RC == New::G8RCRegisterClass) {
    if (DestReg != New::LR8) {
      NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::LD), DestReg),
                                         FrameIdx));
    } else {
      NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::LD),
                                                 New::R11), FrameIdx));
      NewMIs.push_back(BuildMI(MF, DL, get(New::MTLR8)).addReg(New::R11));
    }
  } else if (RC == New::F8RCRegisterClass) {
    NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::LFD), DestReg),
                                       FrameIdx));
  } else if (RC == New::F4RCRegisterClass) {
    NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::LFS), DestReg),
                                       FrameIdx));
  } else if (RC == New::CRRCRegisterClass) {
    // FIXME: We need a scatch reg here.  The trouble with using R0 is that
    // it's possible for the stack frame to be so big the save location is
    // out of range of immediate offsets, necessitating another register.
    // We hack this on Darwin by reserving R2.  It's probably broken on Linux
    // at the moment.
    unsigned ScratchReg = TM.getSubtargetImpl()->isDarwinABI() ?
                                                          New::R2 : New::R0;
    NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::LWZ), 
                                       ScratchReg), FrameIdx));
    
    // If the reloaded register isn't CR0, shift the bits right so that they are
    // in the right CR's slot.
    if (DestReg != New::CR0) {
      unsigned ShiftBits = NewRegisterInfo::getRegisterNumbering(DestReg)*4;
      // rlwinm r11, r11, 32-ShiftBits, 0, 31.
      NewMIs.push_back(BuildMI(MF, DL, get(New::RLWINM), ScratchReg)
		       .addReg(ScratchReg).addImm(32-ShiftBits).addImm(0)
		       .addImm(31));
    }
    
    NewMIs.push_back(BuildMI(MF, DL, get(New::MTCRF), DestReg)
                     .addReg(ScratchReg));
  } else if (RC == New::CRBITRCRegisterClass) {
   
    unsigned Reg = 0;
    if (DestReg == New::CR0LT || DestReg == New::CR0GT ||
        DestReg == New::CR0EQ || DestReg == New::CR0UN)
      Reg = New::CR0;
    else if (DestReg == New::CR1LT || DestReg == New::CR1GT ||
             DestReg == New::CR1EQ || DestReg == New::CR1UN)
      Reg = New::CR1;
    else if (DestReg == New::CR2LT || DestReg == New::CR2GT ||
             DestReg == New::CR2EQ || DestReg == New::CR2UN)
      Reg = New::CR2;
    else if (DestReg == New::CR3LT || DestReg == New::CR3GT ||
             DestReg == New::CR3EQ || DestReg == New::CR3UN)
      Reg = New::CR3;
    else if (DestReg == New::CR4LT || DestReg == New::CR4GT ||
             DestReg == New::CR4EQ || DestReg == New::CR4UN)
      Reg = New::CR4;
    else if (DestReg == New::CR5LT || DestReg == New::CR5GT ||
             DestReg == New::CR5EQ || DestReg == New::CR5UN)
      Reg = New::CR5;
    else if (DestReg == New::CR6LT || DestReg == New::CR6GT ||
             DestReg == New::CR6EQ || DestReg == New::CR6UN)
      Reg = New::CR6;
    else if (DestReg == New::CR7LT || DestReg == New::CR7GT ||
             DestReg == New::CR7EQ || DestReg == New::CR7UN)
      Reg = New::CR7;

    return LoadRegFromStackSlot(MF, DL, Reg, FrameIdx, 
                                New::CRRCRegisterClass, NewMIs);

  } else if (RC == New::VRRCRegisterClass) {
    // We don't have indexed addressing for vector loads.  Emit:
    // R0 = ADDI FI#
    // Dest = LVX 0, R0
    // 
    // FIXME: We use R0 here, because it isn't available for RA.
    /* original code mark by sheng-chih
    NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::ADDI), New::R0),
                                       FrameIdx, 0, 0));
    NewMIs.push_back(BuildMI(MF, DL, get(New::LVX),DestReg).addReg(New::R0)
                     .addReg(New::R0));

    NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::ADDI), New::R0),
                                       FrameIdx, 0, 0));

    NewMIs.push_back(BuildMI(MF, DL, get(New::STVX))
                     .addReg(SrcReg, getKillRegState(isKill))
                     .addReg(New::R0)
                     .addReg(New::R0));
    */
    NewMIs.push_back(addFrameReference(BuildMI(MF, DL, get(New::ADDISC), New::R0),
                                       FrameIdx, 0, 0));
    NewMIs.push_back(BuildMI(MF, DL, get(New::LVXS),DestReg).addReg(New::R0)
                     .addReg(New::R0));

  } else {
    llvm_unreachable("Unknown regclass!");
  }
}

void
NewInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                   MachineBasicBlock::iterator MI,
                                   unsigned DestReg, int FrameIdx,
                                   const TargetRegisterClass *RC,
                                   const TargetRegisterInfo *TRI) const {
  MachineFunction &MF = *MBB.getParent();
  SmallVector<MachineInstr*, 4> NewMIs;
  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();
  LoadRegFromStackSlot(MF, DL, DestReg, FrameIdx, RC, NewMIs);
  for (unsigned i = 0, e = NewMIs.size(); i != e; ++i)
    MBB.insert(MI, NewMIs[i]);

  const MachineFrameInfo &MFI = *MF.getFrameInfo();
  MachineMemOperand *MMO =
    MF.getMachineMemOperand(PseudoSourceValue::getFixedStack(FrameIdx),
                            MachineMemOperand::MOLoad, /*Offset=*/0,
                            MFI.getObjectSize(FrameIdx),
                            MFI.getObjectAlignment(FrameIdx));
  NewMIs.back()->addMemOperand(MF, MMO);
}

MachineInstr*
NewInstrInfo::emitFrameIndexDebugValue(MachineFunction &MF,
                                       int FrameIx, uint64_t Offset,
                                       const MDNode *MDPtr,
                                       DebugLoc DL) const {
  MachineInstrBuilder MIB = BuildMI(MF, DL, get(New::DBG_VALUE));
  addFrameReference(MIB, FrameIx, 0, false).addImm(Offset).addMetadata(MDPtr);
  return &*MIB;
}

bool NewInstrInfo::
ReverseBranchCondition(SmallVectorImpl<MachineOperand> &Cond) const {
  assert(Cond.size() == 2 && "Invalid New branch opcode!");
  Cond[0].setImm(New::InvertPredicate((New::Predicate)Cond[0].getImm()));
  return false;
}

/// GetInstSize - Return the number of bytes of code the specified
/// instruction may be.  This returns the maximum number of bytes.
///
unsigned NewInstrInfo::GetInstSizeInBytes(const MachineInstr *MI) const {
  switch (MI->getOpcode()) {
  case New::INLINEASM: {       // Inline Asm: Variable size.
    const MachineFunction *MF = MI->getParent()->getParent();
    const char *AsmStr = MI->getOperand(0).getSymbolName();
    return getInlineAsmLength(AsmStr, *MF->getTarget().getMCAsmInfo());
  }
  case New::PROLOG_LABEL:
  case New::EH_LABEL:
  case New::GC_LABEL:
  case New::DBG_VALUE:
    return 0;
  default:
    return 4; // PowerPC instructions are all 4 bytes
  }
}
