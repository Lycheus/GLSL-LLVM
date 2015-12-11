//===- NewRegisterInfo.cpp - PowerPC Register Information -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the PowerPC implementation of the TargetRegisterInfo
// class.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "reginfo"
#include "New.h"
#include "NewInstrBuilder.h"
#include "NewMachineFunctionInfo.h"
#include "NewRegisterInfo.h"
#include "NewFrameInfo.h"
#include "NewSubtarget.h"
#include "llvm/CallingConv.h"
#include "llvm/Constants.h"
#include "llvm/Function.h"
#include "llvm/Type.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineLocation.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/Target/TargetFrameInfo.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"
#include <cstdlib>

#include <iostream>
using std::cout;
extern int FloatVariable;
extern int structure_flag;
// FIXME This disables some code that aligns the stack to a boundary
// bigger than the default (16 bytes on Darwin) when there is a stack local
// of greater alignment.  This does not currently work, because the delta
// between old and new stack pointers is added to offsets that reference
// incoming parameters after the prolog is generated, and the code that 
// does that doesn't handle a variable delta.  You don't want to do that
// anyway; a better approach is to reserve another register that retains
// to the incoming stack pointer, and reference parameters relative to that.
#define ALIGN_STACK 0

// FIXME (64-bit): Eventually enable by default.
namespace llvm {
cl::opt<bool> EnableNew32RS("enable-new32-regscavenger",
                                   cl::init(false),
                                   cl::desc("Enable New32 register scavenger"),
                                   cl::Hidden);
cl::opt<bool> EnableNew64RS("enable-new64-regscavenger",
                                   cl::init(false),
                                   cl::desc("Enable New64 register scavenger"),
                                   cl::Hidden);
}

using namespace llvm;

#define EnableRegisterScavenging \
  ((EnableNew32RS && !Subtarget.isNew64()) || \
   (EnableNew64RS && Subtarget.isNew64()))

//added by sheng-chih
extern cl::opt<bool> UseGPUStackAddress;
//end added

// FIXME (64-bit): Should be inlined.
bool
NewRegisterInfo::requiresRegisterScavenging(const MachineFunction &) const {
  return EnableRegisterScavenging;
}

/// getRegisterNumbering - Given the enum value for some register, e.g.
/// New::F14, return the number that it corresponds to (e.g. 14).
unsigned NewRegisterInfo::getRegisterNumbering(unsigned RegEnum) {
  using namespace New;
  switch (RegEnum) {
    
  case 0: return 0;
  case R0 :  case X0 :  /*case F0 :*/  case V0 : case CR0:  case CR0LT: return  0;
  case R1 :  case X1 :  /*case F1 :*/  case V1 : case CR1:  case CR0GT: return  1;
  case R2 :  case X2 :  /*case F2 :*/  case V2 : case CR2:  case CR0EQ: return  2;
    /*case R3 :  case X3 :*/  /*case F3 :*/  /*case V3 :*/ case CR3:  case CR0UN: return  3;
  case R4 :  case X4 :  case F4 :  case V4 : case CR4:  case CR1LT: return  4;
  case R5 :  case X5 :  case F5 :  case V5 : case CR5:  case CR1GT: return  5;
  case R6 :  case X6 :  case F6 :  case V6 : case CR6:  case CR1EQ: return  6;
  case R7 :  case X7 :  case F7 :  case V7 : case CR7:  case CR1UN: return  7;
  case R8 :  case X8 :  case F8 :  case V8 : case CR2LT: return  8;
  case R9 :  case X9 :  case F9 :  case V9 : case CR2GT: return  9;
  case R10:  case X10:  case F10:  case V10: case CR2EQ: return 10;
  case R11:  case X11:  case F11:  case V11: case CR2UN: return 11;
  case R12:  case X12:  case F12:  case V12: case CR3LT: return 12;
  case R13:  case X13:  case F13:  case V13: case CR3GT: return 13;
  case R14:  case X14:  case F14:  case V14: case CR3EQ: return 14;
    /*  case R15:  case X15: */ case F15:  case V15: case CR3UN: return 15;
    /*  case R16:  case X16: */ case F16:  case V16: case CR4LT: return 16;
    /*  case R17:  case X17: */ case F17:  case V17: case CR4GT: return 17;
    /*  case R18:  case X18: */ case F18:  case V18: case CR4EQ: return 18;
    /*  case R19:  case X19: */ case F19:  case V19: case CR4UN: return 19;
    /*  case R20:  case X20: */ case F20:  case V20: case CR5LT: return 20;
    /*  case R21:  case X21: */ case F21:  case V21: case CR5GT: return 21;
    /*  case R22:  case X22: */ case F22:  case V22: case CR5EQ: return 22;
    /*  case R23:  case X23: */ case F23:  case V23: case CR5UN: return 23;
    /*  case R24:  case X24: */ /*case F24:  case V24: case CR6LT: return 24;
  case R25:  case X25:  case F25:  case V25: case CR6GT: return 25;
  case R26:  case X26:  case F26:  case V26: case CR6EQ: return 26;
  case R27:  case X27:  case F27:  case V27: case CR6UN: return 27;
  case R28:  case X28:  case F28:  case V28: case CR7LT: return 28;
  case R29:  case X29:  case F29:  case V29: case CR7GT: return 29;
  case R30:  case X30:  case F30:  case V30: case CR7EQ: return 30;*/
  case R31:  case X31:  /*case F31:  case V31:*/ case CR7UN: return 31; //Kaymon
  default:
    llvm_unreachable("Unhandled reg in NewRegisterInfo::getRegisterNumbering!");
  }
}

NewRegisterInfo::NewRegisterInfo(const NewSubtarget &ST,
                                 const TargetInstrInfo &tii)
  : NewGenRegisterInfo(New::ADJCALLSTACKDOWN, New::ADJCALLSTACKUP),
    Subtarget(ST), TII(tii) {
  ImmToIdxMap[New::LD]   = New::LDX;    ImmToIdxMap[New::STD]  = New::STDX;
  ImmToIdxMap[New::LBZ]  = New::LBZX;   ImmToIdxMap[New::STB]  = New::STBX;
  ImmToIdxMap[New::LHZ]  = New::LHZX;   ImmToIdxMap[New::LHA]  = New::LHAX;
  ImmToIdxMap[New::LWZ]  = New::LWZX;   ImmToIdxMap[New::LWA]  = New::LWAX;
  ImmToIdxMap[New::LFS]  = New::LFSX;   ImmToIdxMap[New::LFD]  = New::LFDX;
  ImmToIdxMap[New::STH]  = New::STHX;   ImmToIdxMap[New::STW]  = New::STWX;
  ImmToIdxMap[New::STFS] = New::STFSX;  ImmToIdxMap[New::STFD] = New::STFDX;
  ImmToIdxMap[New::ADDI] = New::ADD4;

  // 64-bit
  ImmToIdxMap[New::LHA8] = New::LHAX8; ImmToIdxMap[New::LBZ8] = New::LBZX8;
  ImmToIdxMap[New::LHZ8] = New::LHZX8; ImmToIdxMap[New::LWZ8] = New::LWZX8;
  ImmToIdxMap[New::STB8] = New::STBX8; ImmToIdxMap[New::STH8] = New::STHX8;
  ImmToIdxMap[New::STW8] = New::STWX8; ImmToIdxMap[New::STDU] = New::STDUX;
  ImmToIdxMap[New::ADDI8] = New::ADD8; ImmToIdxMap[New::STD_32] = New::STDX_32;
}

/// getPointerRegClass - Return the register class to use to hold pointers.
/// This is used for addressing modes.
const TargetRegisterClass *
NewRegisterInfo::getPointerRegClass(unsigned Kind) const {
  if (Subtarget.isNew64())
    return &New::G8RCRegClass;
  return &New::GPRCRegClass;
}

const unsigned*
NewRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  // 32-bit Darwin calling convention. 
  static const unsigned Darwin32_CalleeSavedRegs[] = {
              New::R13, New::R14, /*New::R15, //for vector used
    New::R16, New::R17, New::R18, New::R19,
    New::R20, New::R21, New::R22, New::R23,
    New::R24,*//* New::R25, New::R26, New::R27,//Kaymon
		  New::R28, New::R29, New::R30, New::R31,*/

    New::F14, New::F15, New::F16, New::F17,
    New::F18, New::F19, New::F20, New::F21,
    New::F22, New::F23,/* New::F24, New::F25,//Kaymon
    New::F26, New::F27, New::F28, New::F29,
    New::F30, New::F31,*/
    
    New::CR2, New::CR3, New::CR4,
    New::V20, New::V21, New::V22, New::V23,
	      /*New::V24, New::V25, New::V26, New::V27,//Kaymon
		New::V28, New::V29, New::V30, New::V31,*/
    
    New::CR2LT, New::CR2GT, New::CR2EQ, New::CR2UN,
    New::CR3LT, New::CR3GT, New::CR3EQ, New::CR3UN,
    New::CR4LT, New::CR4GT, New::CR4EQ, New::CR4UN,
    
    New::LR,  0
  };

  // 32-bit SVR4 calling convention.
  static const unsigned SVR4_CalleeSavedRegs[] = {
                        New::R14, /*New::R15, //for vector used
    New::R16, New::R17, New::R18, New::R19,
    New::R20, New::R21, New::R22, New::R23,
    New::R24,*/ /*New::R25, New::R26, New::R27, //Kaymon
		  New::R28, New::R29, New::R30, New::R31,*/

    New::F14, New::F15, New::F16, New::F17,
    New::F18, New::F19, New::F20, New::F21,
    New::F22, New::F23,/* New::F24, New::F25,//Kaymon
    New::F26, New::F27, New::F28, New::F29,
    New::F30, New::F31,*/
    
    New::CR2, New::CR3, New::CR4,
    
    New::VRSAVE,
    
    New::V20, New::V21, New::V22, New::V23,
			/*New::V24, New::V25, New::V26, New::V27,//Kaymon
			  New::V28, New::V29, New::V30, New::V31,*/
    
    New::CR2LT, New::CR2GT, New::CR2EQ, New::CR2UN,
    New::CR3LT, New::CR3GT, New::CR3EQ, New::CR3UN,
    New::CR4LT, New::CR4GT, New::CR4EQ, New::CR4UN,
    
    0
  };
  // 64-bit Darwin calling convention. 
  static const unsigned Darwin64_CalleeSavedRegs[] = {
    New::X14, /*New::X15, //for vector used
    New::X16, New::X17, New::X18, New::X19,
    New::X20, New::X21, New::X22, New::X23,
    New::X24,*/ /*New::X25, New::X26, New::X27,//Kaymon
		  New::X28, New::X29, New::X30, New::X31,*/
    
    New::F14, New::F15, New::F16, New::F17,
    New::F18, New::F19, New::F20, New::F21,
    New::F22, New::F23, /*New::F24, New::F25,
    New::F26, New::F27, New::F28, New::F29,
    New::F30, New::F31,*///Kaymon
    
    New::CR2, New::CR3, New::CR4,
    New::V20, New::V21, New::V22, New::V23,
    /*New::V24, New::V25, New::V26, New::V27,
      New::V28, New::V29, New::V30, New::V31,*///Kaymon
    
    New::CR2LT, New::CR2GT, New::CR2EQ, New::CR2UN,
    New::CR3LT, New::CR3GT, New::CR3EQ, New::CR3UN,
    New::CR4LT, New::CR4GT, New::CR4EQ, New::CR4UN,
    
    New::LR8,  0
  };

  // 64-bit SVR4 calling convention.
  static const unsigned SVR4_64_CalleeSavedRegs[] = {
    New::X14, /*New::X15, //for vector used
    New::X16, New::X17, New::X18, New::X19,
    New::X20, New::X21, New::X22, New::X23,
    New::X24,*/ /*New::X25, New::X26, New::X27,
		  New::X28, New::X29, New::X30, New::X31,*///Kaymon

    New::F14, New::F15, New::F16, New::F17,
    New::F18, New::F19, New::F20, New::F21,
    New::F22, New::F23,/* New::F24, New::F25,//Kaymon
    New::F26, New::F27, New::F28, New::F29,
    New::F30, New::F31,*/

    New::CR2, New::CR3, New::CR4,

    New::VRSAVE,

    New::V20, New::V21, New::V22, New::V23,
    /* New::V24, New::V25, New::V26, New::V27,
       New::V28, New::V29, New::V30, New::V31,*///Kaymon

    New::CR2LT, New::CR2GT, New::CR2EQ, New::CR2UN,
    New::CR3LT, New::CR3GT, New::CR3EQ, New::CR3UN,
    New::CR4LT, New::CR4GT, New::CR4EQ, New::CR4UN,

    0
  };
  
  if (Subtarget.isDarwinABI())
    return Subtarget.isNew64() ? Darwin64_CalleeSavedRegs :
                                 Darwin32_CalleeSavedRegs;

  return Subtarget.isNew64() ? SVR4_64_CalleeSavedRegs : SVR4_CalleeSavedRegs;
}

// needsFP - Return true if the specified function should have a dedicated frame
// pointer register.  This is true if the function has variable sized allocas or
// if frame pointer elimination is disabled.
//
static bool needsFP(const MachineFunction &MF) {
  const MachineFrameInfo *MFI = MF.getFrameInfo();
  // Naked functions have no stack frame pushed, so we don't have a frame pointer.
  if (MF.getFunction()->hasFnAttr(Attribute::Naked))
    return false;
  return DisableFramePointerElim(MF) || MFI->hasVarSizedObjects() ||
    (GuaranteedTailCallOpt && MF.getInfo<NewFunctionInfo>()->hasFastCall());
}

static bool spillsCR(const MachineFunction &MF) {
  const NewFunctionInfo *FuncInfo = MF.getInfo<NewFunctionInfo>();
  return FuncInfo->isCRSpilled();
}

BitVector NewRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  Reserved.set(New::R0);
  Reserved.set(New::R1);
  Reserved.set(New::LR);
  Reserved.set(New::LR8);
  Reserved.set(New::RM);

  // The SVR4 ABI reserves r2 and r13
  if (Subtarget.isSVR4ABI()) {
    Reserved.set(New::R2);  // System-reserved register
    Reserved.set(New::R13); // Small Data Area pointer register
  }
  // Reserve R2 on Darwin to hack around the problem of save/restore of CR
  // when the stack frame is too big to address directly; we need two regs.
  // This is a hack.
  if (Subtarget.isDarwinABI()) {
    Reserved.set(New::R2);
  }
  
  // On New64, r13 is the thread pointer. Never allocate this register.
  // Note that this is over conservative, as it also prevents allocation of R31
  // when the FP is not needed.
  if (Subtarget.isNew64()) {
    Reserved.set(New::R13);
    Reserved.set(New::R31);

    if (!EnableRegisterScavenging)
      Reserved.set(New::R0);    // FIXME (64-bit): Remove

    Reserved.set(New::X0);
    Reserved.set(New::X1);
    Reserved.set(New::X13);
    Reserved.set(New::X31);

    // The 64-bit SVR4 ABI reserves r2 for the TOC pointer.
    if (Subtarget.isSVR4ABI()) {
      Reserved.set(New::X2);
    }
    // Reserve R2 on Darwin to hack around the problem of save/restore of CR
    // when the stack frame is too big to address directly; we need two regs.
    // This is a hack.
    if (Subtarget.isDarwinABI()) {
      Reserved.set(New::X2);
    }
  }

  if (needsFP(MF))
    Reserved.set(New::R31);

  return Reserved;
}

//===----------------------------------------------------------------------===//
// Stack Frame Processing methods
//===----------------------------------------------------------------------===//

// hasFP - Return true if the specified function actually has a dedicated frame
// pointer register.  This is true if the function needs a frame pointer and has
// a non-zero stack size.
bool NewRegisterInfo::hasFP(const MachineFunction &MF) const {
  const MachineFrameInfo *MFI = MF.getFrameInfo();
  return MFI->getStackSize() && needsFP(MF);
}

/// MustSaveLR - Return true if this function requires that we save the LR
/// register onto the stack in the prolog and restore it in the epilog of the
/// function.
static bool MustSaveLR(const MachineFunction &MF, unsigned LR) {
  const NewFunctionInfo *MFI = MF.getInfo<NewFunctionInfo>();
  
  // We need a save/restore of LR if there is any def of LR (which is
  // defined by calls, including the PIC setup sequence), or if there is
  // some use of the LR stack slot (e.g. for builtin_return_address).
  // (LR comes in 32 and 64 bit versions.)
  MachineRegisterInfo::def_iterator RI = MF.getRegInfo().def_begin(LR);
  return RI !=MF.getRegInfo().def_end() || MFI->isLRStoreRequired();
}



void NewRegisterInfo::
eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                              MachineBasicBlock::iterator I) const {
  if (GuaranteedTailCallOpt && I->getOpcode() == New::ADJCALLSTACKUP) {
    // Add (actually subtract) back the amount the callee popped on return.
    if (int CalleeAmt =  I->getOperand(1).getImm()) {
      bool is64Bit = Subtarget.isNew64();
      CalleeAmt *= -1;
      unsigned StackReg = is64Bit ? New::X1 : New::R1;
      unsigned TmpReg = is64Bit ? New::X0 : New::R0;
      unsigned ADDIInstr = is64Bit ? New::ADDI8 : New::ADDI;
      unsigned ADDInstr = is64Bit ? New::ADD8 : New::ADD4;
      unsigned LISInstr = is64Bit ? New::LIS8 : New::LIS;
      //unsigned ORIInstr = is64Bit ? New::ORI8 : New::ORI;
      unsigned ORIInstr = New::ORI8;
      //EXCESS4
      MachineInstr *MI = I;
      DebugLoc dl = MI->getDebugLoc();

      if (isInt<16>(CalleeAmt)) {
        BuildMI(MBB, I, dl, TII.get(ADDIInstr), StackReg).addReg(StackReg).
          addImm(CalleeAmt);
      } else {
        MachineBasicBlock::iterator MBBI = I;
        BuildMI(MBB, MBBI, dl, TII.get(LISInstr), TmpReg)
          .addImm(CalleeAmt >> 16);
        BuildMI(MBB, MBBI, dl, TII.get(ORIInstr), TmpReg)
          .addReg(TmpReg, RegState::Kill)
          .addImm(CalleeAmt & 0xFFFF);
        BuildMI(MBB, MBBI, dl, TII.get(ADDInstr))
          .addReg(StackReg)
          .addReg(StackReg)
          .addReg(TmpReg);
      }
    }
  }
  // Simply discard ADJCALLSTACKDOWN, ADJCALLSTACKUP instructions.
  MBB.erase(I);
}

/// findScratchRegister - Find a 'free' New register. Try for a call-clobbered
/// register first and then a spilled callee-saved register if that fails.
static
unsigned findScratchRegister(MachineBasicBlock::iterator II, RegScavenger *RS,
                             const TargetRegisterClass *RC, int SPAdj) {
  assert(RS && "Register scavenging must be on");
  unsigned Reg = RS->FindUnusedReg(RC);
  // FIXME: move ARM callee-saved reg scan to target independent code, then 
  // search for already spilled CS register here.
  if (Reg == 0)
    Reg = RS->scavengeRegister(RC, II, SPAdj);
  return Reg;
}

/// lowerDynamicAlloc - Generate the code for allocating an object in the
/// current frame.  The sequence of code with be in the general form
///
///   addi   R0, SP, \#frameSize ; get the address of the previous frame
///   stwxu  R0, SP, Rnegsize   ; add and update the SP with the negated size
///   addi   Rnew, SP, \#maxCalFrameSize ; get the top of the allocation
///
void NewRegisterInfo::lowerDynamicAlloc(MachineBasicBlock::iterator II,
                                        int SPAdj, RegScavenger *RS) const {
  // Get the instruction.
  MachineInstr &MI = *II;
  // Get the instruction's basic block.
  MachineBasicBlock &MBB = *MI.getParent();
  // Get the basic block's function.
  MachineFunction &MF = *MBB.getParent();
  // Get the frame info.
  MachineFrameInfo *MFI = MF.getFrameInfo();
  // Determine whether 64-bit pointers are used.
  bool LP64 = Subtarget.isNew64();
  DebugLoc dl = MI.getDebugLoc();

  // Get the maximum call stack size.
  unsigned maxCallFrameSize = MFI->getMaxCallFrameSize();
  // Get the total frame size.
  unsigned FrameSize = MFI->getStackSize();
  
  // Get stack alignments.
  unsigned TargetAlign = MF.getTarget().getFrameInfo()->getStackAlignment();
  unsigned MaxAlign = MFI->getMaxAlignment();
  if (MaxAlign > TargetAlign)
    report_fatal_error("Dynamic alloca with large aligns not supported");

  // Determine the previous frame's address.  If FrameSize can't be
  // represented as 16 bits or we need special alignment, then we load the
  // previous frame's address from 0(SP).  Why not do an addis of the hi? 
  // Because R0 is our only safe tmp register and addi/addis treat R0 as zero. 
  // Constructing the constant and adding would take 3 instructions. 
  // Fortunately, a frame greater than 32K is rare.
  const TargetRegisterClass *G8RC = &New::G8RCRegClass;
  const TargetRegisterClass *GPRC = &New::GPRCRegClass;
  const TargetRegisterClass *RC = LP64 ? G8RC : GPRC;

  // FIXME (64-bit): Use "findScratchRegister"
  unsigned Reg;
  if (EnableRegisterScavenging)
    Reg = findScratchRegister(II, RS, RC, SPAdj);
  else
    Reg = New::R0;
  
  if (MaxAlign < TargetAlign && isInt<16>(FrameSize)) {
    BuildMI(MBB, II, dl, TII.get(New::ADDI), Reg)
      .addReg(New::R31)
      .addImm(FrameSize);
  } else if (LP64) {
    if (EnableRegisterScavenging) // FIXME (64-bit): Use "true" part.
      BuildMI(MBB, II, dl, TII.get(New::LD), Reg)
        .addImm(0)
        .addReg(New::X1);
    else
      BuildMI(MBB, II, dl, TII.get(New::LD), New::X0)
        .addImm(0)
        .addReg(New::X1);
  } else {
    BuildMI(MBB, II, dl, TII.get(New::LWZ), Reg)
      .addImm(0)
      .addReg(New::R1);
  }
  
  // Grow the stack and update the stack pointer link, then determine the
  // address of new allocated space.
  if (LP64) {
    if (EnableRegisterScavenging) // FIXME (64-bit): Use "true" part.
      BuildMI(MBB, II, dl, TII.get(New::STDUX))
        .addReg(Reg, RegState::Kill)
        .addReg(New::X1)
        .addReg(MI.getOperand(1).getReg());
    else
      BuildMI(MBB, II, dl, TII.get(New::STDUX))
        .addReg(New::X0, RegState::Kill)
        .addReg(New::X1)
        .addReg(MI.getOperand(1).getReg());

    if (!MI.getOperand(1).isKill())
      BuildMI(MBB, II, dl, TII.get(New::ADDI8), MI.getOperand(0).getReg())
        .addReg(New::X1)
        .addImm(maxCallFrameSize);
    else
      // Implicitly kill the register.
      BuildMI(MBB, II, dl, TII.get(New::ADDI8), MI.getOperand(0).getReg())
        .addReg(New::X1)
        .addImm(maxCallFrameSize)
        .addReg(MI.getOperand(1).getReg(), RegState::ImplicitKill);
  } else {
    BuildMI(MBB, II, dl, TII.get(New::STWUX))
      .addReg(Reg, RegState::Kill)
      .addReg(New::R1)
      .addReg(MI.getOperand(1).getReg());

    if (!MI.getOperand(1).isKill())
      BuildMI(MBB, II, dl, TII.get(New::ADDI), MI.getOperand(0).getReg())
        .addReg(New::R1)
        .addImm(maxCallFrameSize);
    else
      // Implicitly kill the register.
      BuildMI(MBB, II, dl, TII.get(New::ADDI), MI.getOperand(0).getReg())
        .addReg(New::R1)
        .addImm(maxCallFrameSize)
        .addReg(MI.getOperand(1).getReg(), RegState::ImplicitKill);
  }
  
  // Discard the DYNALLOC instruction.
  MBB.erase(II);
}

/// lowerCRSpilling - Generate the code for spilling a CR register. Instead of
/// reserving a whole register (R0), we scrounge for one here. This generates
/// code like this:
///
///   mfcr rA                  ; Move the conditional register into GPR rA.
///   rlwinm rA, rA, SB, 0, 31 ; Shift the bits left so they are in CR0's slot.
///   stw rA, FI               ; Store rA to the frame.
///
void NewRegisterInfo::lowerCRSpilling(MachineBasicBlock::iterator II,
                                      unsigned FrameIndex, int SPAdj,
                                      RegScavenger *RS) const {
  /*
  // Get the instruction.
  MachineInstr &MI = *II;       // ; SPILL_CR <SrcReg>, <offset>, <FI>
  // Get the instruction's basic block.
  MachineBasicBlock &MBB = *MI.getParent();
  DebugLoc dl = MI.getDebugLoc();

  const TargetRegisterClass *G8RC = &New::G8RCRegClass;
  const TargetRegisterClass *GPRC = &New::GPRCRegClass;
  const TargetRegisterClass *RC = Subtarget.isNew64() ? G8RC : GPRC;
  unsigned Reg = findScratchRegister(II, RS, RC, SPAdj);
  unsigned SrcReg = MI.getOperand(0).getReg();

  // We need to store the CR in the low 4-bits of the saved value. First, issue
  // an MFCRpsued to save all of the CRBits and, if needed, kill the SrcReg.
  BuildMI(MBB, II, dl, TII.get(New::MFCRpseud), Reg)
          .addReg(SrcReg, getKillRegState(MI.getOperand(0).isKill()));
    
  // If the saved register wasn't CR0, shift the bits left so that they are in
  // CR0's slot.
  if (SrcReg != New::CR0)
    // rlwinm rA, rA, ShiftBits, 0, 31.
    BuildMI(MBB, II, dl, TII.get(New::RLWINM), Reg)
      .addReg(Reg, RegState::Kill)
      .addImm(NewRegisterInfo::getRegisterNumbering(SrcReg) * 4)
      .addImm(0)
      .addImm(31);

  addFrameReference(BuildMI(MBB, II, dl, TII.get(New::STW))
                    .addReg(Reg, getKillRegState(MI.getOperand(1).getImm())),
                    FrameIndex);

  // Discard the pseudo instruction.
  MBB.erase(II);
  */
  //Julian removed it. //dondum edit
}

void
NewRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                     int SPAdj, RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");

  // Get the instruction.
  MachineInstr &MI = *II;
  // Get the instruction's basic block.
  MachineBasicBlock &MBB = *MI.getParent();
  // Get the basic block's function.
  MachineFunction &MF = *MBB.getParent();
  // Get the frame info.
  MachineFrameInfo *MFI = MF.getFrameInfo();
  DebugLoc dl = MI.getDebugLoc();

  // Find out which operand is the frame index.
  unsigned FIOperandNo = 0;
  while (!MI.getOperand(FIOperandNo).isFI()) {
    ++FIOperandNo;
    assert(FIOperandNo != MI.getNumOperands() &&
           "Instr doesn't have FrameIndex operand!");
  }
  // Take into account whether it's an add or mem instruction
  unsigned OffsetOperandNo = (FIOperandNo == 2) ? 1 : 2;
  if (MI.isInlineAsm())
    OffsetOperandNo = FIOperandNo-1;

  // Get the frame index.
  int FrameIndex = MI.getOperand(FIOperandNo).getIndex();

  // Get the frame pointer save index.  Users of this index are primarily
  // DYNALLOC instructions.
  NewFunctionInfo *FI = MF.getInfo<NewFunctionInfo>();
  int FPSI = FI->getFramePointerSaveIndex();
  // Get the instruction opcode.
  unsigned OpC = MI.getOpcode();

  // Special case for dynamic alloca.
  if (FPSI && FrameIndex == FPSI &&
      (OpC == New::DYNALLOC || OpC == New::DYNALLOC8)) {
    lowerDynamicAlloc(II, SPAdj, RS);
    return;
  }

  // Special case for pseudo-op SPILL_CR.
  if (EnableRegisterScavenging) // FIXME (64-bit): Enable by default.
    if (OpC == New::SPILL_CR) {
      lowerCRSpilling(II, FrameIndex, SPAdj, RS);
      return;
    }

  // Replace the FrameIndex with base register with GPR1 (SP) or GPR31 (FP).
  MI.getOperand(FIOperandNo).ChangeToRegister(hasFP(MF) ? New::R31 : New::R1,
                                              false);

  // Figure out if the offset in the instruction is shifted right two bits. This
  // is true for instructions like "STD", which the machine implicitly adds two
  // low zeros to.
  bool isIXAddr = false;
  switch (OpC) {
  case New::LWA:
  case New::LD:
  case New::STD:
  case New::STD_32:
    isIXAddr = true;
    break;
  }
  
  // Now add the frame object offset to the offset from r1.
  int Offset = MFI->getObjectOffset(FrameIndex);

  if (!isIXAddr)
    Offset += MI.getOperand(OffsetOperandNo).getImm();
  else
    Offset += MI.getOperand(OffsetOperandNo).getImm() << 2;


  int y = MI.getOperand(OffsetOperandNo).getImm()<<2;

  // If we're not using a Frame Pointer that has been set to the value of the
  // SP before having the stack size subtracted from it, then add the stack size
  // to Offset to get the correct offset.
  // Naked functions have stack size 0, although getStackSize may not reflect that
  // because we didn't call all the pieces that compute it for naked functions.
  if (!MF.getFunction()->hasFnAttr(Attribute::Naked))
    Offset += MFI->getStackSize();

  // If we can, encode the offset directly into the instruction.  If this is a
  // normal New "ri" instruction, any 16-bit value can be safely encoded.  If
  // this is a New64 "ix" instruction, only a 16-bit value with the low two bits
  // clear can be encoded.  This is extremely uncommon, because normally you
  // only "std" to a stack slot that is at least 4-byte aligned, but it can
  // happen in invalid code.
  if (isInt<16>(Offset) && (!isIXAddr || (Offset & 3) == 0)) {
    if (isIXAddr)
      Offset >>= 2;    // The actual encoded value has the low two bits zero.

    //added by sheng-chih

    if(UseGPUStackAddress) //Kaymon fixed it for float array.
      {
	if(OpC == New::LFS)//Kaymon add this thing
	  {
	    Offset = MFI->getObjectOffset(FrameIndex) + MFI->getStackSize() ;

	    char gpu_mask[5] = {'-','-','-','-','\0'};
	    if(FloatVariable&&MI.Check_array_for_LFS_instruction()){
	      //FloatVariable = 0;
	      Offset += MI.getOperand(OffsetOperandNo).getImm(); //add this item to fix FloatingPoint array.
	      gpu_mask[0]='x';
	      MI.addOperand(MachineOperand::CreateGPUMask(gpu_mask));
	    }
	    else if(structure_flag){//structure Kaymon. if there is an array in structure, follow above function Check_array_for_LFS_instruction.
	      //cout<<"in here\n";
	      Offset += MI.getOperand(OffsetOperandNo).getImm(); //add this item to fix structure.
	      //cout<<Offset<<" off\n";
	      gpu_mask[0]='x';
	      MI.addOperand(MachineOperand::CreateGPUMask(gpu_mask));
	      //structure_flag=0;
	    }
	    else{
	      switch(MI.getOperand(OffsetOperandNo).getImm())
		{
		case 0:
		  gpu_mask[0] = 'x';
		  break;
		case 4:
		  gpu_mask[1] = 'y';
		  break;
		case 8:
		  gpu_mask[2] = 'z';
		  break;
		case 12:
		  gpu_mask[3] = 'w';
		  break;
		default:
		  assert("eliminateFrameIndex stack imm");
		}
	      MI.addOperand(MachineOperand::CreateGPUMask(gpu_mask));
	    }
	  }
      }

    //end added
    if(OpC == New::LWZ){ //Kaymon add one more operand to record the component of LWZ. e.g. 0->x 4->y 8->z 12 ->w.
      if(structure_flag){//structure Kaymon
	Offset =MFI->getObjectOffset(FrameIndex) + MFI->getStackSize() +  MI.getOperand(OffsetOperandNo).getImm(); //add this item to fix structure.
	//cout<<Offset<< " offset\n";
	//cout<<MI.getOperand(OffsetOperandNo).getImm()<< " IMM\n";
	MI.addOperand(MI.getOperand(OffsetOperandNo));
	//cout<<MI.getNumOperands() <<" num\n";
	//structure_flag=0;
      }
      else{
	Offset = MFI->getObjectOffset(FrameIndex) + MFI->getStackSize() ;
	MI.addOperand(MI.getOperand(OffsetOperandNo));
      }
    }
    MI.getOperand(OffsetOperandNo).ChangeToImmediate(Offset);

    return;
  }

  // The offset doesn't fit into a single register, scavenge one to build the
  // offset in.
  // FIXME: figure out what SPAdj is doing here.

  // FIXME (64-bit): Use "findScratchRegister".
  unsigned SReg;
  if (EnableRegisterScavenging)
    SReg = findScratchRegister(II, RS, &New::GPRCRegClass, SPAdj);
  else
    SReg = New::R0;

  // Insert a set of rA with the full offset value before the ld, st, or add
  BuildMI(MBB, II, dl, TII.get(New::LIS), SReg)
    .addImm(Offset >> 16);
  /*
  BuildMI(MBB, II, dl, TII.get(New::ORI), SReg)
    .addReg(SReg, RegState::Kill)
    .addImm(Offset);
  */
  //EXCESS4
  // Convert into indexed form of the instruction:
  // 
  //   sth 0:rA, 1:imm 2:(rB) ==> sthx 0:rA, 2:rB, 1:r0
  //   addi 0:rA 1:rB, 2, imm ==> add 0:rA, 1:rB, 2:r0
  unsigned OperandBase;

  if (OpC != TargetOpcode::INLINEASM) {
    assert(ImmToIdxMap.count(OpC) &&
           "No indexed form of load or store available!");
    unsigned NewOpcode = ImmToIdxMap.find(OpC)->second;
    MI.setDesc(TII.get(NewOpcode));
    OperandBase = 1;
  } else {
    OperandBase = OffsetOperandNo;
  }

  unsigned StackReg = MI.getOperand(FIOperandNo).getReg();
  MI.getOperand(OperandBase).ChangeToRegister(StackReg, false);
  MI.getOperand(OperandBase + 1).ChangeToRegister(SReg, false);
}

/// VRRegNo - Map from a numbered VR register to its enum value.
///
static const unsigned short VRRegNo[] = {
  New::V0 , New::V1 , New::V2 , /*New::V3 */ //dondum 20110726
  New::V4 , New::V5 , New::V6 , New::V7 ,
  New::V8 , New::V9 , New::V10, New::V11, New::V12, New::V13, New::V14, New::V15,
  New::V16, New::V17, New::V18, New::V19, New::V20, New::V21, New::V22, New::V23,
  /*New::V24, New::V25, New::V26, New::V27, New::V28, New::V29, New::V30, New::V31*///Kaymon
};

/// RemoveVRSaveCode - We have found that this function does not need any code
/// to manipulate the VRSAVE register, even though it uses vector registers.
/// This can happen when the only registers used are known to be live in or out
/// of the function.  Remove all of the VRSAVE related code from the function.
static void RemoveVRSaveCode(MachineInstr *MI) {
  MachineBasicBlock *Entry = MI->getParent();
  MachineFunction *MF = Entry->getParent();

  // We know that the MTVRSAVE instruction immediately follows MI.  Remove it.
  MachineBasicBlock::iterator MBBI = MI;
  ++MBBI;
  assert(MBBI != Entry->end() && MBBI->getOpcode() == New::MTVRSAVE);
  MBBI->eraseFromParent();
  
  bool RemovedAllMTVRSAVEs = true;
  // See if we can find and remove the MTVRSAVE instruction from all of the
  // epilog blocks.
  for (MachineFunction::iterator I = MF->begin(), E = MF->end(); I != E; ++I) {
    // If last instruction is a return instruction, add an epilogue
    if (!I->empty() && I->back().getDesc().isReturn()) {
      bool FoundIt = false;
      for (MBBI = I->end(); MBBI != I->begin(); ) {
        --MBBI;
        if (MBBI->getOpcode() == New::MTVRSAVE) {
          MBBI->eraseFromParent();  // remove it.
          FoundIt = true;
          break;
        }
      }
      RemovedAllMTVRSAVEs &= FoundIt;
    }
  }

  // If we found and removed all MTVRSAVE instructions, remove the read of
  // VRSAVE as well.
  if (RemovedAllMTVRSAVEs) {
    MBBI = MI;
    assert(MBBI != Entry->begin() && "UPDATE_VRSAVE is first instr in block?");
    --MBBI;
    assert(MBBI->getOpcode() == New::MFVRSAVE && "VRSAVE instrs wandered?");
    MBBI->eraseFromParent();
  }
  
  // Finally, nuke the UPDATE_VRSAVE.
  MI->eraseFromParent();
}

// HandleVRSaveUpdate - MI is the UPDATE_VRSAVE instruction introduced by the
// instruction selector.  Based on the vector registers that have been used,
// transform this into the appropriate ORI instruction.
static void HandleVRSaveUpdate(MachineInstr *MI, const TargetInstrInfo &TII) {
  MachineFunction *MF = MI->getParent()->getParent();
  DebugLoc dl = MI->getDebugLoc();

  unsigned UsedRegMask = 0;
  for (unsigned i = 0; i != 32; ++i)
    if (MF->getRegInfo().isPhysRegUsed(VRRegNo[i]))
      UsedRegMask |= 1 << (31-i);
  
  // Live in and live out values already must be in the mask, so don't bother
  // marking them.
  for (MachineRegisterInfo::livein_iterator
       I = MF->getRegInfo().livein_begin(),
       E = MF->getRegInfo().livein_end(); I != E; ++I) {
    unsigned RegNo = NewRegisterInfo::getRegisterNumbering(I->first);
    if (VRRegNo[RegNo] == I->first)        // If this really is a vector reg.
      UsedRegMask &= ~(1 << (31-RegNo));   // Doesn't need to be marked.
  }
  for (MachineRegisterInfo::liveout_iterator
       I = MF->getRegInfo().liveout_begin(),
       E = MF->getRegInfo().liveout_end(); I != E; ++I) {
    unsigned RegNo = NewRegisterInfo::getRegisterNumbering(*I);
    if (VRRegNo[RegNo] == *I)              // If this really is a vector reg.
      UsedRegMask &= ~(1 << (31-RegNo));   // Doesn't need to be marked.
  }
  
  // If no registers are used, turn this into a copy.
  if (UsedRegMask == 0) {
    // Remove all VRSAVE code.
    RemoveVRSaveCode(MI);
    return;
  }

  unsigned SrcReg = MI->getOperand(1).getReg();
  unsigned DstReg = MI->getOperand(0).getReg();
  /*
  if ((UsedRegMask & 0xFFFF) == UsedRegMask) {
    if (DstReg != SrcReg)
      BuildMI(*MI->getParent(), MI, dl, TII.get(New::ORI), DstReg)
        .addReg(SrcReg)
        .addImm(UsedRegMask);
    else
      BuildMI(*MI->getParent(), MI, dl, TII.get(New::ORI), DstReg)
        .addReg(SrcReg, RegState::Kill)
        .addImm(UsedRegMask);
  } else if ((UsedRegMask & 0xFFFF0000) == UsedRegMask) {
    if (DstReg != SrcReg)
      BuildMI(*MI->getParent(), MI, dl, TII.get(New::ORIS), DstReg)
        .addReg(SrcReg)
        .addImm(UsedRegMask >> 16);
    else
      BuildMI(*MI->getParent(), MI, dl, TII.get(New::ORIS), DstReg)
        .addReg(SrcReg, RegState::Kill)
        .addImm(UsedRegMask >> 16);
  } else {
    if (DstReg != SrcReg)
      BuildMI(*MI->getParent(), MI, dl, TII.get(New::ORIS), DstReg)
        .addReg(SrcReg)
        .addImm(UsedRegMask >> 16);
    else
      BuildMI(*MI->getParent(), MI, dl, TII.get(New::ORIS), DstReg)
        .addReg(SrcReg, RegState::Kill)
        .addImm(UsedRegMask >> 16);

    BuildMI(*MI->getParent(), MI, dl, TII.get(New::ORI), DstReg)
      .addReg(DstReg, RegState::Kill)
      .addImm(UsedRegMask & 0xFFFF);
  }
  *///EXCESS4
  // Remove the old UPDATE_VRSAVE instruction.
  MI->eraseFromParent();
}

/// determineFrameLayout - Determine the size of the frame and maximum call
/// frame size.
void NewRegisterInfo::determineFrameLayout(MachineFunction &MF) const {
  MachineFrameInfo *MFI = MF.getFrameInfo();

  // Get the number of bytes to allocate from the FrameInfo
  unsigned FrameSize = MFI->getStackSize();
  
  // Get the alignments provided by the target, and the maximum alignment
  // (if any) of the fixed frame objects.
  unsigned MaxAlign = MFI->getMaxAlignment();
  unsigned TargetAlign = MF.getTarget().getFrameInfo()->getStackAlignment();
  unsigned AlignMask = TargetAlign - 1;  //

  // If we are a leaf function, and use up to 224 bytes of stack space,
  // don't have a frame pointer, calls, or dynamic alloca then we do not need
  // to adjust the stack pointer (we fit in the Red Zone).
  bool DisableRedZone = MF.getFunction()->hasFnAttr(Attribute::NoRedZone);
  // FIXME SVR4 The 32-bit SVR4 ABI has no red zone.
  if (!DisableRedZone &&
      FrameSize <= 224 &&                          // Fits in red zone.
      !MFI->hasVarSizedObjects() &&                // No dynamic alloca.
      !MFI->adjustsStack() &&                      // No calls.
      (!ALIGN_STACK || MaxAlign <= TargetAlign)) { // No special alignment.
    // No need for frame
    MFI->setStackSize(0);
    return;
  }
  
  // Get the maximum call frame size of all the calls.
  unsigned maxCallFrameSize = MFI->getMaxCallFrameSize();
  
  // Maximum call frame needs to be at least big enough for linkage and 8 args.
  unsigned minCallFrameSize =
    NewFrameInfo::getMinCallFrameSize(Subtarget.isNew64(), 
                                      Subtarget.isDarwinABI());
  maxCallFrameSize = std::max(maxCallFrameSize, minCallFrameSize);

  // If we have dynamic alloca then maxCallFrameSize needs to be aligned so
  // that allocations will be aligned.
  if (MFI->hasVarSizedObjects())
    maxCallFrameSize = (maxCallFrameSize + AlignMask) & ~AlignMask;
  
  // Update maximum call frame size.
  MFI->setMaxCallFrameSize(maxCallFrameSize);
  
  // Include call frame size in total.
  FrameSize += maxCallFrameSize;
  
  // Make sure the frame is aligned.
  FrameSize = (FrameSize + AlignMask) & ~AlignMask;

  // Update frame info.
  MFI->setStackSize(FrameSize);
}

void
NewRegisterInfo::processFunctionBeforeCalleeSavedScan(MachineFunction &MF,
                                                      RegScavenger *RS) const {
  //  Save and clear the LR state.
  NewFunctionInfo *FI = MF.getInfo<NewFunctionInfo>();
  unsigned LR = getRARegister();
  FI->setMustSaveLR(MustSaveLR(MF, LR));
  MF.getRegInfo().setPhysRegUnused(LR);

  //  Save R31 if necessary
  int FPSI = FI->getFramePointerSaveIndex();
  bool isNew64 = Subtarget.isNew64();
  bool isDarwinABI  = Subtarget.isDarwinABI();
  MachineFrameInfo *MFI = MF.getFrameInfo();
 
  // If the frame pointer save index hasn't been defined yet.
  if (!FPSI && needsFP(MF)) {
    // Find out what the fix offset of the frame pointer save area.
    int FPOffset = NewFrameInfo::getFramePointerSaveOffset(isNew64,
                                                           isDarwinABI);
    // Allocate the frame index for frame pointer save area.
    FPSI = MF.getFrameInfo()->CreateFixedObject(isNew64? 8 : 4, FPOffset, true);
    // Save the result.
    FI->setFramePointerSaveIndex(FPSI);                      
  }

  // Reserve stack space to move the linkage area to in case of a tail call.
  int TCSPDelta = 0;
  if (GuaranteedTailCallOpt && (TCSPDelta = FI->getTailCallSPDelta()) < 0) {
    MF.getFrameInfo()->CreateFixedObject(-1 * TCSPDelta, TCSPDelta, true);
  }
  
  // Reserve a slot closest to SP or frame pointer if we have a dynalloc or
  // a large stack, which will require scavenging a register to materialize a
  // large offset.
  // FIXME: this doesn't actually check stack size, so is a bit pessimistic
  // FIXME: doesn't detect whether or not we need to spill vXX, which requires
  //        r0 for now.

  if (EnableRegisterScavenging) // FIXME (64-bit): Enable.
    if (needsFP(MF) || spillsCR(MF)) {
      const TargetRegisterClass *GPRC = &New::GPRCRegClass;
      const TargetRegisterClass *G8RC = &New::G8RCRegClass;
      const TargetRegisterClass *RC = isNew64 ? G8RC : GPRC;
      RS->setScavengingFrameIndex(MFI->CreateStackObject(RC->getSize(),
                                                         RC->getAlignment(),
                                                         false));
    }
}

void
NewRegisterInfo::processFunctionBeforeFrameFinalized(MachineFunction &MF)
                                                     const {
  // Early exit if not using the SVR4 ABI.
  if (!Subtarget.isSVR4ABI()) {
    return;
  }

  // Get callee saved register information.
  MachineFrameInfo *FFI = MF.getFrameInfo();
  const std::vector<CalleeSavedInfo> &CSI = FFI->getCalleeSavedInfo();

  // Early exit if no callee saved registers are modified!
  if (CSI.empty() && !needsFP(MF)) {
    return;
  }
  
  unsigned MinGPR = New::R31;
  unsigned MinG8R = New::X31;
  unsigned MinFPR = 100;//New::F31;Kaymon
  unsigned MinVR = 100;//New::V31;Kaymon
  
  bool HasGPSaveArea = false;
  bool HasG8SaveArea = false;
  bool HasFPSaveArea = false;
  bool HasCRSaveArea = false;
  bool HasVRSAVESaveArea = false;
  bool HasVRSaveArea = false;
  
  SmallVector<CalleeSavedInfo, 18> GPRegs;
  SmallVector<CalleeSavedInfo, 18> G8Regs;
  SmallVector<CalleeSavedInfo, 18> FPRegs;
  SmallVector<CalleeSavedInfo, 18> VRegs;
  
  for (unsigned i = 0, e = CSI.size(); i != e; ++i) {
    unsigned Reg = CSI[i].getReg();
    if (New::GPRCRegisterClass->contains(Reg)) {
      HasGPSaveArea = true;
      
      GPRegs.push_back(CSI[i]);
      
      if (Reg < MinGPR) {
        MinGPR = Reg;
      }
    } else if (New::G8RCRegisterClass->contains(Reg)) {
      HasG8SaveArea = true;

      G8Regs.push_back(CSI[i]);

      if (Reg < MinG8R) {
        MinG8R = Reg;
      }
    } else if (New::F8RCRegisterClass->contains(Reg)) {
      HasFPSaveArea = true;
      
      FPRegs.push_back(CSI[i]);
      
      if (Reg < MinFPR) {
        MinFPR = Reg;
      }
// FIXME SVR4: Disable CR save area for now.
    } else if (New::CRBITRCRegisterClass->contains(Reg)
               || New::CRRCRegisterClass->contains(Reg)) {
//      HasCRSaveArea = true;
    } else if (New::VRSAVERCRegisterClass->contains(Reg)) {
      HasVRSAVESaveArea = true;
    } else if (New::VRRCRegisterClass->contains(Reg)) {
      HasVRSaveArea = true;
      
      VRegs.push_back(CSI[i]);
      
      if (Reg < MinVR) {
        MinVR = Reg;
      }
    } else {
      llvm_unreachable("Unknown RegisterClass!");
    }
  }

  NewFunctionInfo *PFI = MF.getInfo<NewFunctionInfo>();
  
  int64_t LowerBound = 0;

  // Take into account stack space reserved for tail calls.
  int TCSPDelta = 0;
  if (GuaranteedTailCallOpt && (TCSPDelta = PFI->getTailCallSPDelta()) < 0) {
    LowerBound = TCSPDelta;
  }

  // The Floating-point register save area is right below the back chain word
  // of the previous stack frame.
  if (HasFPSaveArea) {
    for (unsigned i = 0, e = FPRegs.size(); i != e; ++i) {
      int FI = FPRegs[i].getFrameIdx();
      
      FFI->setObjectOffset(FI, LowerBound + FFI->getObjectOffset(FI));
    }
    
    LowerBound -= (31 - getRegisterNumbering(MinFPR) + 1) * 8; 
  }

  // Check whether the frame pointer register is allocated. If so, make sure it
  // is spilled to the correct offset.
  if (needsFP(MF)) {
    HasGPSaveArea = true;
    
    int FI = PFI->getFramePointerSaveIndex();
    assert(FI && "No Frame Pointer Save Slot!");
    
    FFI->setObjectOffset(FI, LowerBound + FFI->getObjectOffset(FI));
  }
  
  // General register save area starts right below the Floating-point
  // register save area.
  if (HasGPSaveArea || HasG8SaveArea) {
    // Move general register save area spill slots down, taking into account
    // the size of the Floating-point register save area.
    for (unsigned i = 0, e = GPRegs.size(); i != e; ++i) {
      int FI = GPRegs[i].getFrameIdx();
      
      FFI->setObjectOffset(FI, LowerBound + FFI->getObjectOffset(FI));
    }
    
    // Move general register save area spill slots down, taking into account
    // the size of the Floating-point register save area.
    for (unsigned i = 0, e = G8Regs.size(); i != e; ++i) {
      int FI = G8Regs[i].getFrameIdx();

      FFI->setObjectOffset(FI, LowerBound + FFI->getObjectOffset(FI));
    }

    unsigned MinReg = std::min<unsigned>(getRegisterNumbering(MinGPR),
                                         getRegisterNumbering(MinG8R));

    if (Subtarget.isNew64()) {
      LowerBound -= (31 - MinReg + 1) * 8;
    } else {
      LowerBound -= (31 - MinReg + 1) * 4;
    }
  }
  
  // The CR save area is below the general register save area.
  if (HasCRSaveArea) {
    // FIXME SVR4: Is it actually possible to have multiple elements in CSI
    //             which have the CR/CRBIT register class?
    // Adjust the frame index of the CR spill slot.
    for (unsigned i = 0, e = CSI.size(); i != e; ++i) {
      unsigned Reg = CSI[i].getReg();
    
      if (New::CRBITRCRegisterClass->contains(Reg) ||
          New::CRRCRegisterClass->contains(Reg)) {
        int FI = CSI[i].getFrameIdx();

        FFI->setObjectOffset(FI, LowerBound + FFI->getObjectOffset(FI));
      }
    }
    
    LowerBound -= 4; // The CR save area is always 4 bytes long.
  }
  
  if (HasVRSAVESaveArea) {
    // FIXME SVR4: Is it actually possible to have multiple elements in CSI
    //             which have the VRSAVE register class?
    // Adjust the frame index of the VRSAVE spill slot.
    for (unsigned i = 0, e = CSI.size(); i != e; ++i) {
      unsigned Reg = CSI[i].getReg();
    
      if (New::VRSAVERCRegisterClass->contains(Reg)) {
        int FI = CSI[i].getFrameIdx();

        FFI->setObjectOffset(FI, LowerBound + FFI->getObjectOffset(FI));
      }
    }
    
    LowerBound -= 4; // The VRSAVE save area is always 4 bytes long.
  }
  
  if (HasVRSaveArea) {
    // Insert alignment padding, we need 16-byte alignment.
    LowerBound = (LowerBound - 15) & ~(15);
    
    for (unsigned i = 0, e = VRegs.size(); i != e; ++i) {
      int FI = VRegs[i].getFrameIdx();
      
      FFI->setObjectOffset(FI, LowerBound + FFI->getObjectOffset(FI));
    }
  }
}

void
NewRegisterInfo::emitPrologue(MachineFunction &MF) const {
  MachineBasicBlock &MBB = MF.front();   // Prolog goes in entry BB
  MachineBasicBlock::iterator MBBI = MBB.begin();
  MachineFrameInfo *MFI = MF.getFrameInfo();
  MachineModuleInfo &MMI = MF.getMMI();
  DebugLoc dl;
  bool needsFrameMoves = MMI.hasDebugInfo() ||
       !MF.getFunction()->doesNotThrow() ||
       UnwindTablesMandatory;
  
  // Prepare for frame info.
  MCSymbol *FrameLabel = 0;

  // Scan the prolog, looking for an UPDATE_VRSAVE instruction.  If we find it,
  // process it.
  for (unsigned i = 0; MBBI != MBB.end(); ++i, ++MBBI) {
    if (MBBI->getOpcode() == New::UPDATE_VRSAVE) {
      HandleVRSaveUpdate(MBBI, TII);
      break;
    }
  }
  
  // Move MBBI back to the beginning of the function.
  MBBI = MBB.begin();

  // Work out frame sizes.
  determineFrameLayout(MF);
  unsigned FrameSize = MFI->getStackSize();
  
  int NegFrameSize = -FrameSize;
  
  // Get processor type.
  bool isNew64 = Subtarget.isNew64();
  // Get operating system
  bool isDarwinABI = Subtarget.isDarwinABI();
  // Check if the link register (LR) must be saved.
  NewFunctionInfo *FI = MF.getInfo<NewFunctionInfo>();
  bool MustSaveLR = FI->mustSaveLR();
  // Do we have a frame pointer for this function?
  bool HasFP = hasFP(MF) && FrameSize;
  
  int LROffset = NewFrameInfo::getReturnSaveOffset(isNew64, isDarwinABI);

  int FPOffset = 0;
  if (HasFP) {
    if (Subtarget.isSVR4ABI()) {
      MachineFrameInfo *FFI = MF.getFrameInfo();
      int FPIndex = FI->getFramePointerSaveIndex();
      assert(FPIndex && "No Frame Pointer Save Slot!");
      FPOffset = FFI->getObjectOffset(FPIndex);
    } else {
      FPOffset = NewFrameInfo::getFramePointerSaveOffset(isNew64, isDarwinABI);
    }
  }

  if (isNew64) {
    if (MustSaveLR)
      BuildMI(MBB, MBBI, dl, TII.get(New::MFLR8), New::X0);
      
    if (HasFP)
      BuildMI(MBB, MBBI, dl, TII.get(New::STD))
        .addReg(New::X31)
        .addImm(FPOffset/4)
        .addReg(New::X1);
    
    if (MustSaveLR)
      BuildMI(MBB, MBBI, dl, TII.get(New::STD))
        .addReg(New::X0)
        .addImm(LROffset / 4)
        .addReg(New::X1);
  } else {
    if (MustSaveLR)
      BuildMI(MBB, MBBI, dl, TII.get(New::MFLR), New::R0);
      
    if (HasFP)
      BuildMI(MBB, MBBI, dl, TII.get(New::STW))
        .addReg(New::R31)
        .addImm(FPOffset)
        .addReg(New::R1);

    if (MustSaveLR)
      BuildMI(MBB, MBBI, dl, TII.get(New::STW))
        .addReg(New::R0)
        .addImm(LROffset)
        .addReg(New::R1);
  }
  
  // Skip if a leaf routine.
  if (!FrameSize) return;
  
  // Get stack alignments.
  unsigned TargetAlign = MF.getTarget().getFrameInfo()->getStackAlignment();
  unsigned MaxAlign = MFI->getMaxAlignment();

  // Adjust stack pointer: r1 += NegFrameSize.
  // If there is a preferred stack alignment, align R1 now
  if (!isNew64) {
    // New32.
    if (ALIGN_STACK && MaxAlign > TargetAlign) {
      assert(isPowerOf2_32(MaxAlign) && isInt<16>(MaxAlign) &&
             "Invalid alignment!");
      assert(isInt<16>(NegFrameSize) && "Unhandled stack size and alignment!");

      BuildMI(MBB, MBBI, dl, TII.get(New::RLWINM), New::R0)
        .addReg(New::R1)
        .addImm(0)
        .addImm(32 - Log2_32(MaxAlign))
        .addImm(31);
      BuildMI(MBB, MBBI, dl, TII.get(New::SUBFIC) ,New::R0)
        .addReg(New::R0, RegState::Kill)
        .addImm(NegFrameSize);
      BuildMI(MBB, MBBI, dl, TII.get(New::STWUX))
        .addReg(New::R1)
        .addReg(New::R1)
        .addReg(New::R0);
    } else if (isInt<16>(NegFrameSize)) {
      BuildMI(MBB, MBBI, dl, TII.get(New::STWU), New::R1)
        .addReg(New::R1)
        .addImm(NegFrameSize)
        .addReg(New::R1);
    } else {
      BuildMI(MBB, MBBI, dl, TII.get(New::LIS), New::R0)
        .addImm(NegFrameSize >> 16);
      BuildMI(MBB, MBBI, dl, TII.get(New::ORI8), New::R0)
	//EXCESS4
        .addReg(New::R0, RegState::Kill)
        .addImm(NegFrameSize & 0xFFFF);
      BuildMI(MBB, MBBI, dl, TII.get(New::STWUX))
        .addReg(New::R1)
        .addReg(New::R1)
        .addReg(New::R0);
    }
  } else {    // New64.
    if (ALIGN_STACK && MaxAlign > TargetAlign) {
      assert(isPowerOf2_32(MaxAlign) && isInt<16>(MaxAlign) &&
             "Invalid alignment!");
      assert(isInt<16>(NegFrameSize) && "Unhandled stack size and alignment!");

      BuildMI(MBB, MBBI, dl, TII.get(New::RLDICL), New::X0)
        .addReg(New::X1)
        .addImm(0)
        .addImm(64 - Log2_32(MaxAlign));
      BuildMI(MBB, MBBI, dl, TII.get(New::SUBFIC8), New::X0)
        .addReg(New::X0)
        .addImm(NegFrameSize);
      BuildMI(MBB, MBBI, dl, TII.get(New::STDUX))
        .addReg(New::X1)
        .addReg(New::X1)
        .addReg(New::X0);
    } else if (isInt<16>(NegFrameSize)) {
      BuildMI(MBB, MBBI, dl, TII.get(New::STDU), New::X1)
        .addReg(New::X1)
        .addImm(NegFrameSize / 4)
        .addReg(New::X1);
    } else {
      BuildMI(MBB, MBBI, dl, TII.get(New::LIS8), New::X0)
        .addImm(NegFrameSize >> 16);
      BuildMI(MBB, MBBI, dl, TII.get(New::ORI8), New::X0)
        .addReg(New::X0, RegState::Kill)
        .addImm(NegFrameSize & 0xFFFF);
      BuildMI(MBB, MBBI, dl, TII.get(New::STDUX))
        .addReg(New::X1)
        .addReg(New::X1)
        .addReg(New::X0);
    }
  }

  std::vector<MachineMove> &Moves = MMI.getFrameMoves();
  
  // Add the "machine moves" for the instructions we generated above, but in
  // reverse order.
  if (needsFrameMoves) {
    // Mark effective beginning of when frame pointer becomes valid.
    FrameLabel = MMI.getContext().CreateTempSymbol();
    BuildMI(MBB, MBBI, dl, TII.get(New::PROLOG_LABEL)).addSym(FrameLabel);
  
    // Show update of SP.
    if (NegFrameSize) {
      MachineLocation SPDst(MachineLocation::VirtualFP);
      MachineLocation SPSrc(MachineLocation::VirtualFP, NegFrameSize);
      Moves.push_back(MachineMove(FrameLabel, SPDst, SPSrc));
    } else {
      MachineLocation SP(isNew64 ? New::X31 : New::R31);
      Moves.push_back(MachineMove(FrameLabel, SP, SP));
    }
    
    if (HasFP) {
      MachineLocation FPDst(MachineLocation::VirtualFP, FPOffset);
      MachineLocation FPSrc(isNew64 ? New::X31 : New::R31);
      Moves.push_back(MachineMove(FrameLabel, FPDst, FPSrc));
    }

    if (MustSaveLR) {
      MachineLocation LRDst(MachineLocation::VirtualFP, LROffset);
      MachineLocation LRSrc(isNew64 ? New::LR8 : New::LR);
      Moves.push_back(MachineMove(FrameLabel, LRDst, LRSrc));
    }
  }

  MCSymbol *ReadyLabel = 0;

  // If there is a frame pointer, copy R1 into R31
  if (HasFP) {
    if (!isNew64) {
      BuildMI(MBB, MBBI, dl, TII.get(New::OR), New::R31)
        .addReg(New::R1)
        .addReg(New::R1);
    } else {
      BuildMI(MBB, MBBI, dl, TII.get(New::OR8), New::X31)
        .addReg(New::X1)
        .addReg(New::X1);
    }

    if (needsFrameMoves) {
      ReadyLabel = MMI.getContext().CreateTempSymbol();

      // Mark effective beginning of when frame pointer is ready.
      BuildMI(MBB, MBBI, dl, TII.get(New::PROLOG_LABEL)).addSym(ReadyLabel);

      MachineLocation FPDst(HasFP ? (isNew64 ? New::X31 : New::R31) :
                                    (isNew64 ? New::X1 : New::R1));
      MachineLocation FPSrc(MachineLocation::VirtualFP);
      Moves.push_back(MachineMove(ReadyLabel, FPDst, FPSrc));
    }
  }

  if (needsFrameMoves) {
    MCSymbol *Label = HasFP ? ReadyLabel : FrameLabel;

    // Add callee saved registers to move list.
    const std::vector<CalleeSavedInfo> &CSI = MFI->getCalleeSavedInfo();
    for (unsigned I = 0, E = CSI.size(); I != E; ++I) {
      int Offset = MFI->getObjectOffset(CSI[I].getFrameIdx());
      unsigned Reg = CSI[I].getReg();
      if (Reg == New::LR || Reg == New::LR8 || Reg == New::RM) continue;
      MachineLocation CSDst(MachineLocation::VirtualFP, Offset);
      MachineLocation CSSrc(Reg);
      Moves.push_back(MachineMove(Label, CSDst, CSSrc));
    }
  }
}

void NewRegisterInfo::emitEpilogue(MachineFunction &MF,
                                   MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = prior(MBB.end());
  unsigned RetOpcode = MBBI->getOpcode();
  DebugLoc dl;

  assert( (RetOpcode == New::BLR ||
           RetOpcode == New::TCRETURNri ||
           RetOpcode == New::TCRETURNdi ||
           RetOpcode == New::TCRETURNai ||
           RetOpcode == New::TCRETURNri8 ||
           RetOpcode == New::TCRETURNdi8 ||
           RetOpcode == New::TCRETURNai8) &&
         "Can only insert epilog into returning blocks");

  // Get alignment info so we know how to restore r1
  const MachineFrameInfo *MFI = MF.getFrameInfo();
  unsigned TargetAlign = MF.getTarget().getFrameInfo()->getStackAlignment();
  unsigned MaxAlign = MFI->getMaxAlignment();

  // Get the number of bytes allocated from the FrameInfo.
  int FrameSize = MFI->getStackSize();

  // Get processor type.
  bool isNew64 = Subtarget.isNew64();
  // Get operating system
  bool isDarwinABI = Subtarget.isDarwinABI();
  // Check if the link register (LR) has been saved.
  NewFunctionInfo *FI = MF.getInfo<NewFunctionInfo>();
  bool MustSaveLR = FI->mustSaveLR();
  // Do we have a frame pointer for this function?
  bool HasFP = hasFP(MF) && FrameSize;
  
  int LROffset = NewFrameInfo::getReturnSaveOffset(isNew64, isDarwinABI);

  int FPOffset = 0;
  if (HasFP) {
    if (Subtarget.isSVR4ABI()) {
      MachineFrameInfo *FFI = MF.getFrameInfo();
      int FPIndex = FI->getFramePointerSaveIndex();
      assert(FPIndex && "No Frame Pointer Save Slot!");
      FPOffset = FFI->getObjectOffset(FPIndex);
    } else {
      FPOffset = NewFrameInfo::getFramePointerSaveOffset(isNew64, isDarwinABI);
    }
  }
  
  bool UsesTCRet =  RetOpcode == New::TCRETURNri ||
    RetOpcode == New::TCRETURNdi ||
    RetOpcode == New::TCRETURNai ||
    RetOpcode == New::TCRETURNri8 ||
    RetOpcode == New::TCRETURNdi8 ||
    RetOpcode == New::TCRETURNai8;

  if (UsesTCRet) {
    int MaxTCRetDelta = FI->getTailCallSPDelta();
    MachineOperand &StackAdjust = MBBI->getOperand(1);
    assert(StackAdjust.isImm() && "Expecting immediate value.");
    // Adjust stack pointer.
    int StackAdj = StackAdjust.getImm();
    int Delta = StackAdj - MaxTCRetDelta;
    assert((Delta >= 0) && "Delta must be positive");
    if (MaxTCRetDelta>0)
      FrameSize += (StackAdj +Delta);
    else
      FrameSize += StackAdj;
  }

  if (FrameSize) {
    // The loaded (or persistent) stack pointer value is offset by the 'stwu'
    // on entry to the function.  Add this offset back now.
    if (!isNew64) {
      // If this function contained a fastcc call and GuaranteedTailCallOpt is
      // enabled (=> hasFastCall()==true) the fastcc call might contain a tail
      // call which invalidates the stack pointer value in SP(0). So we use the
      // value of R31 in this case.
      if (FI->hasFastCall() && isInt<16>(FrameSize)) {
        assert(hasFP(MF) && "Expecting a valid the frame pointer.");
        BuildMI(MBB, MBBI, dl, TII.get(New::ADDI), New::R1)
          .addReg(New::R31).addImm(FrameSize);
      } else if(FI->hasFastCall()) {
        BuildMI(MBB, MBBI, dl, TII.get(New::LIS), New::R0)
          .addImm(FrameSize >> 16);
        BuildMI(MBB, MBBI, dl, TII.get(New::ORI8), New::R0)
	  //EXCESS4
          .addReg(New::R0, RegState::Kill)
          .addImm(FrameSize & 0xFFFF);
        BuildMI(MBB, MBBI, dl, TII.get(New::ADD4))
          .addReg(New::R1)
          .addReg(New::R31)
          .addReg(New::R0);
      } else if (isInt<16>(FrameSize) &&
                 (!ALIGN_STACK || TargetAlign >= MaxAlign) &&
                 !MFI->hasVarSizedObjects()) {
        BuildMI(MBB, MBBI, dl, TII.get(New::ADDI), New::R1)
          .addReg(New::R1).addImm(FrameSize);
      } else {
        BuildMI(MBB, MBBI, dl, TII.get(New::LWZ),New::R1)
          .addImm(0).addReg(New::R1);
      }
    } else {
      if (FI->hasFastCall() && isInt<16>(FrameSize)) {
        assert(hasFP(MF) && "Expecting a valid the frame pointer.");
        BuildMI(MBB, MBBI, dl, TII.get(New::ADDI8), New::X1)
          .addReg(New::X31).addImm(FrameSize);
      } else if(FI->hasFastCall()) {
        BuildMI(MBB, MBBI, dl, TII.get(New::LIS8), New::X0)
          .addImm(FrameSize >> 16);
        BuildMI(MBB, MBBI, dl, TII.get(New::ORI8), New::X0)
          .addReg(New::X0, RegState::Kill)
          .addImm(FrameSize & 0xFFFF);
        BuildMI(MBB, MBBI, dl, TII.get(New::ADD8))
          .addReg(New::X1)
          .addReg(New::X31)
          .addReg(New::X0);
      } else if (isInt<16>(FrameSize) && TargetAlign >= MaxAlign &&
            !MFI->hasVarSizedObjects()) {
        BuildMI(MBB, MBBI, dl, TII.get(New::ADDI8), New::X1)
           .addReg(New::X1).addImm(FrameSize);
      } else {
        BuildMI(MBB, MBBI, dl, TII.get(New::LD), New::X1)
           .addImm(0).addReg(New::X1);
      }
    }
  }

  if (isNew64) {
    if (MustSaveLR)
      BuildMI(MBB, MBBI, dl, TII.get(New::LD), New::X0)
        .addImm(LROffset/4).addReg(New::X1);
        
    if (HasFP)
      BuildMI(MBB, MBBI, dl, TII.get(New::LD), New::X31)
        .addImm(FPOffset/4).addReg(New::X1);
        
    if (MustSaveLR)
      BuildMI(MBB, MBBI, dl, TII.get(New::MTLR8)).addReg(New::X0);
  } else {
    if (MustSaveLR)
      BuildMI(MBB, MBBI, dl, TII.get(New::LWZ), New::R0)
          .addImm(LROffset).addReg(New::R1);
        
    if (HasFP)
      BuildMI(MBB, MBBI, dl, TII.get(New::LWZ), New::R31)
          .addImm(FPOffset).addReg(New::R1);
          
    if (MustSaveLR)
      BuildMI(MBB, MBBI, dl, TII.get(New::MTLR)).addReg(New::R0);
  }

  // Callee pop calling convention. Pop parameter/linkage area. Used for tail
  // call optimization
  if (GuaranteedTailCallOpt && RetOpcode == New::BLR &&
      MF.getFunction()->getCallingConv() == CallingConv::Fast) {
     NewFunctionInfo *FI = MF.getInfo<NewFunctionInfo>();
     unsigned CallerAllocatedAmt = FI->getMinReservedArea();
     unsigned StackReg = isNew64 ? New::X1 : New::R1;
     unsigned FPReg = isNew64 ? New::X31 : New::R31;
     unsigned TmpReg = isNew64 ? New::X0 : New::R0;
     unsigned ADDIInstr = isNew64 ? New::ADDI8 : New::ADDI;
     unsigned ADDInstr = isNew64 ? New::ADD8 : New::ADD4;
     unsigned LISInstr = isNew64 ? New::LIS8 : New::LIS;
     //unsigned ORIInstr = isNew64 ? New::ORI8 : New::ORI;
     unsigned ORIInstr = New::ORI8;
     //EXCESS4

     if (CallerAllocatedAmt && isInt<16>(CallerAllocatedAmt)) {
       BuildMI(MBB, MBBI, dl, TII.get(ADDIInstr), StackReg)
         .addReg(StackReg).addImm(CallerAllocatedAmt);
     } else {
       BuildMI(MBB, MBBI, dl, TII.get(LISInstr), TmpReg)
          .addImm(CallerAllocatedAmt >> 16);
       BuildMI(MBB, MBBI, dl, TII.get(ORIInstr), TmpReg)
          .addReg(TmpReg, RegState::Kill)
          .addImm(CallerAllocatedAmt & 0xFFFF);
       BuildMI(MBB, MBBI, dl, TII.get(ADDInstr))
          .addReg(StackReg)
          .addReg(FPReg)
          .addReg(TmpReg);
     }
  } else if (RetOpcode == New::TCRETURNdi) {
    MBBI = prior(MBB.end());
    MachineOperand &JumpTarget = MBBI->getOperand(0);
    BuildMI(MBB, MBBI, dl, TII.get(New::TAILB)).
      addGlobalAddress(JumpTarget.getGlobal(), JumpTarget.getOffset());
  } else if (RetOpcode == New::TCRETURNri) {
    MBBI = prior(MBB.end());
    assert(MBBI->getOperand(0).isReg() && "Expecting register operand.");
    BuildMI(MBB, MBBI, dl, TII.get(New::TAILBCTR));
  } else if (RetOpcode == New::TCRETURNai) {
    MBBI = prior(MBB.end());
    MachineOperand &JumpTarget = MBBI->getOperand(0);
    BuildMI(MBB, MBBI, dl, TII.get(New::TAILBA)).addImm(JumpTarget.getImm());
  } else if (RetOpcode == New::TCRETURNdi8) {
    MBBI = prior(MBB.end());
    MachineOperand &JumpTarget = MBBI->getOperand(0);
    BuildMI(MBB, MBBI, dl, TII.get(New::TAILB8)).
      addGlobalAddress(JumpTarget.getGlobal(), JumpTarget.getOffset());
  } else if (RetOpcode == New::TCRETURNri8) {
    MBBI = prior(MBB.end());
    assert(MBBI->getOperand(0).isReg() && "Expecting register operand.");
    BuildMI(MBB, MBBI, dl, TII.get(New::TAILBCTR8));
  } else if (RetOpcode == New::TCRETURNai8) {
    MBBI = prior(MBB.end());
    MachineOperand &JumpTarget = MBBI->getOperand(0);
    BuildMI(MBB, MBBI, dl, TII.get(New::TAILBA8)).addImm(JumpTarget.getImm());
  }
}

unsigned NewRegisterInfo::getRARegister() const {
  return !Subtarget.isNew64() ? New::LR : New::LR8;
}

unsigned NewRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  if (!Subtarget.isNew64())
    return hasFP(MF) ? New::R31 : New::R1;
  else
    return hasFP(MF) ? New::X31 : New::X1;
}

void NewRegisterInfo::getInitialFrameState(std::vector<MachineMove> &Moves)
                                                                         const {
  // Initial state of the frame pointer is R1.
  MachineLocation Dst(MachineLocation::VirtualFP);
  MachineLocation Src(New::R1, 0);
  Moves.push_back(MachineMove(0, Dst, Src));
}

unsigned NewRegisterInfo::getEHExceptionRegister() const {
  //dondum change R3 to R5 20110626
  //  return !Subtarget.isNew64() ? New::R3 : New::X3;
  return !Subtarget.isNew64() ? New::R5 : New::X5;
}

unsigned NewRegisterInfo::getEHHandlerRegister() const {
  return !Subtarget.isNew64() ? New::R4 : New::X4;
}

int NewRegisterInfo::getDwarfRegNum(unsigned RegNum, bool isEH) const {
  // FIXME: Most probably dwarf numbers differs for Linux and Darwin
  return NewGenRegisterInfo::getDwarfRegNumFull(RegNum, 0);
}

#include "NewGenRegisterInfo.inc"
