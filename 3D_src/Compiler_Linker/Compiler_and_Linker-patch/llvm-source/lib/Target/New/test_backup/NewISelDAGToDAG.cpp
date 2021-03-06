//===-- PPCISelDAGToDAG.cpp - PPC --pattern matching inst selector --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines a pattern matching instruction selector for PowerPC,
// converting from a legalized dag to a New dag.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "ppc-codegen"
#include "New.h"
#include "NewPredicates.h"
#include "NewTargetMachine.h"
#include "NewHazardRecognizers.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionAnalysis.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Constants.h"
#include "llvm/Function.h"
#include "llvm/GlobalValue.h"
#include "llvm/Intrinsics.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Support/CommandLine.h" //Julian, added by dondum

using namespace llvm;

#include <iostream>
using std::cout;


namespace {
  //===--------------------------------------------------------------------===//
  /// NewDAGToDAGISel - New specific code to select New machine
  /// instructions for SelectionDAG operations.
  ///
  class NewDAGToDAGISel : public SelectionDAGISel {
    const NewTargetMachine &TM;
    const NewTargetLowering &NewLowering;
    const NewSubtarget &NewSubTarget;
    unsigned GlobalBaseReg;
  public:
    explicit NewDAGToDAGISel(NewTargetMachine &tm)
      : SelectionDAGISel(tm), TM(tm),
        NewLowering(*TM.getTargetLowering()),
        NewSubTarget(*TM.getSubtargetImpl()) {}
    
    virtual bool runOnMachineFunction(MachineFunction &MF) {
      // Make sure we re-emit a set of the global base reg if necessary
      GlobalBaseReg = 0;

      SelectionDAGISel::runOnMachineFunction(MF);
      
      InsertVRSaveCode(MF);
      return true;
    }
   
    /// getI32Imm - Return a target constant with the specified value, of type
    /// i32.
    inline SDValue getI32Imm(unsigned Imm) {
      return CurDAG->getTargetConstant(Imm, MVT::i32);
    }

    /// getI64Imm - Return a target constant with the specified value, of type
    /// i64.
    inline SDValue getI64Imm(uint64_t Imm) {
      return CurDAG->getTargetConstant(Imm, MVT::i64);
    }
    
    /// getSmallIPtrImm - Return a target constant of pointer type.
    inline SDValue getSmallIPtrImm(unsigned Imm) {
      return CurDAG->getTargetConstant(Imm, NewLowering.getPointerTy());
    }
    
    /// isRunOfOnes - Returns true iff Val consists of one contiguous run of 1s 
    /// with any number of 0s on either side.  The 1s are allowed to wrap from
    /// LSB to MSB, so 0x000FFF0, 0x0000FFFF, and 0xFF0000FF are all runs.
    /// 0x0F0F0000 is not, since all 1s are not contiguous.
    static bool isRunOfOnes(unsigned Val, unsigned &MB, unsigned &ME);


    /// isRotateAndMask - Returns true if Mask and Shift can be folded into a
    /// rotate and mask opcode and mask operation.
    static bool isRotateAndMask(SDNode *N, unsigned Mask, bool isShiftMask,
                                unsigned &SH, unsigned &MB, unsigned &ME);
    
    /// getGlobalBaseReg - insert code into the entry mbb to materialize the PIC
    /// base register.  Return the virtual register that holds this value.
    SDNode *getGlobalBaseReg();
    
    // Select - Convert the specified operand from a target-independent to a
    // target-specific node if it hasn't already been changed.
    SDNode *Select(SDNode *N);
    
    SDNode *SelectBitfieldInsert(SDNode *N);

    /// SelectCC - Select a comparison of the specified values with the
    /// specified condition code, returning the CR# of the expression.
    SDValue SelectCC(SDValue LHS, SDValue RHS, ISD::CondCode CC, DebugLoc dl);

    /// SelectAddrImm - Returns true if the address N can be represented by
    /// a base register plus a signed 16-bit displacement [r+imm].
    bool SelectAddrImm(SDNode *Op, SDValue N, SDValue &Disp,
                       SDValue &Base) {
      return NewLowering.SelectAddressRegImm(N, Disp, Base, *CurDAG);
    }
    
    /// SelectAddrImmOffs - Return true if the operand is valid for a preinc
    /// immediate field.  Because preinc imms have already been validated, just
    /// accept it.
    bool SelectAddrImmOffs(SDNode *Op, SDValue N, SDValue &Out) const {
      Out = N;
      return true;
    }
      
    /// SelectAddrIdx - Given the specified addressed, check to see if it can be
    /// represented as an indexed [r+r] operation.  Returns false if it can
    /// be represented by [r+imm], which are preferred.
    bool SelectAddrIdx(SDNode *Op, SDValue N, SDValue &Base,
                       SDValue &Index) {
      return NewLowering.SelectAddressRegReg(N, Base, Index, *CurDAG);
    }
    
    /// SelectAddrIdxOnly - Given the specified addressed, force it to be
    /// represented as an indexed [r+r] operation.
    bool SelectAddrIdxOnly(SDNode *Op, SDValue N, SDValue &Base,
                           SDValue &Index) {
      return NewLowering.SelectAddressRegRegOnly(N, Base, Index, *CurDAG);
    }

    /// SelectAddrImmShift - Returns true if the address N can be represented by
    /// a base register plus a signed 14-bit displacement [r+imm*4].  Suitable
    /// for use by STD and friends.
    bool SelectAddrImmShift(SDNode *Op, SDValue N, SDValue &Disp,
                            SDValue &Base) {
      return NewLowering.SelectAddressRegImmShift(N, Disp, Base, *CurDAG);
    }
      
    /// SelectInlineAsmMemoryOperand - Implement addressing mode selection for
    /// inline asm expressions.  It is always correct to compute the value into
    /// a register.  The case of adding a (possibly relocatable) constant to a
    /// register can be improved, but it is wrong to substitute Reg+Reg for
    /// Reg in an asm, because the load or store opcode would have to change.
   virtual bool SelectInlineAsmMemoryOperand(const SDValue &Op,
                                              char ConstraintCode,
                                              std::vector<SDValue> &OutOps) {
      OutOps.push_back(Op);
      return false;
    }
    
    SDValue BuildSDIVSequence(SDNode *N);
    SDValue BuildUDIVSequence(SDNode *N);
    
    void InsertVRSaveCode(MachineFunction &MF);

    virtual const char *getPassName() const {
      return "PowerPC DAG->DAG Pattern Instruction Selection";
    } 
    
    /// CreateTargetHazardRecognizer - Return the hazard recognizer to use for
    /// this target when scheduling the DAG.
    virtual ScheduleHazardRecognizer *CreateTargetHazardRecognizer() {
      // Should use subtarget info to pick the right hazard recognizer.  For
      // now, always return a New970 recognizer.
      const TargetInstrInfo *II = TM.getInstrInfo();
      assert(II && "No InstrInfo?");
      return new NewHazardRecognizer970(*II); 
    }

// Include the pieces autogenerated from the target description.
#include "NewGenDAGISel.inc"
    
private:
    SDNode *SelectSETCC(SDNode *N);
  };
}

/// InsertVRSaveCode - Once the entire function has been instruction selected,
/// all virtual registers are created and all machine instructions are built,
/// check to see if we need to save/restore VRSAVE.  If so, do it.
void NewDAGToDAGISel::InsertVRSaveCode(MachineFunction &Fn) {
  // Check to see if this function uses vector registers, which means we have to
  // save and restore the VRSAVE register and update it with the regs we use.  
  //
  // In this case, there will be virtual registers of vector type created
  // by the scheduler.  Detect them now.
  bool HasVectorVReg = false;
  for (unsigned i = TargetRegisterInfo::FirstVirtualRegister, 
       e = RegInfo->getLastVirtReg()+1; i != e; ++i)
    if (RegInfo->getRegClass(i) == &New::VRRCRegClass) {
      HasVectorVReg = true;
      break;
    }
  if (!HasVectorVReg) return;  // nothing to do.
      
  // If we have a vector register, we want to emit code into the entry and exit
  // blocks to save and restore the VRSAVE register.  We do this here (instead
  // of marking all vector instructions as clobbering VRSAVE) for two reasons:
  //
  // 1. This (trivially) reduces the load on the register allocator, by not
  //    having to represent the live range of the VRSAVE register.
  // 2. This (more significantly) allows us to create a temporary virtual
  //    register to hold the saved VRSAVE value, allowing this temporary to be
  //    register allocated, instead of forcing it to be spilled to the stack.

  // Create two vregs - one to hold the VRSAVE register that is live-in to the
  // function and one for the value after having bits or'd into it.
  unsigned InVRSAVE = RegInfo->createVirtualRegister(&New::GPRCRegClass);
  unsigned UpdatedVRSAVE = RegInfo->createVirtualRegister(&New::GPRCRegClass);
  
  const TargetInstrInfo &TII = *TM.getInstrInfo();
  MachineBasicBlock &EntryBB = *Fn.begin();
  DebugLoc dl;
  // Emit the following code into the entry block:
  // InVRSAVE = MFVRSAVE
  // UpdatedVRSAVE = UPDATE_VRSAVE InVRSAVE
  // MTVRSAVE UpdatedVRSAVE
  MachineBasicBlock::iterator IP = EntryBB.begin();  // Insert Point
  BuildMI(EntryBB, IP, dl, TII.get(New::MFVRSAVE), InVRSAVE);
  BuildMI(EntryBB, IP, dl, TII.get(New::UPDATE_VRSAVE),
          UpdatedVRSAVE).addReg(InVRSAVE);
  BuildMI(EntryBB, IP, dl, TII.get(New::MTVRSAVE)).addReg(UpdatedVRSAVE);
  
  // Find all return blocks, outputting a restore in each epilog.
  for (MachineFunction::iterator BB = Fn.begin(), E = Fn.end(); BB != E; ++BB) {
    if (!BB->empty() && BB->back().getDesc().isReturn()) {
      IP = BB->end(); --IP;
      
      // Skip over all terminator instructions, which are part of the return
      // sequence.
      MachineBasicBlock::iterator I2 = IP;
      while (I2 != BB->begin() && (--I2)->getDesc().isTerminator())
        IP = I2;
      
      // Emit: MTVRSAVE InVRSave
      BuildMI(*BB, IP, dl, TII.get(New::MTVRSAVE)).addReg(InVRSAVE);
    }        
  }
}


/// getGlobalBaseReg - Output the instructions required to put the
/// base address to use for accessing globals into a register.
///
SDNode *NewDAGToDAGISel::getGlobalBaseReg() {
  if (!GlobalBaseReg) {
    const TargetInstrInfo &TII = *TM.getInstrInfo();
    // Insert the set of GlobalBaseReg into the first MBB of the function
    MachineBasicBlock &FirstMBB = MF->front();
    MachineBasicBlock::iterator MBBI = FirstMBB.begin();
    DebugLoc dl;

    if (NewLowering.getPointerTy() == MVT::i32) {
      GlobalBaseReg = RegInfo->createVirtualRegister(New::GPRCRegisterClass);
      BuildMI(FirstMBB, MBBI, dl, TII.get(New::MovePCtoLR), New::LR);
      BuildMI(FirstMBB, MBBI, dl, TII.get(New::MFLR), GlobalBaseReg);
    } else {
      GlobalBaseReg = RegInfo->createVirtualRegister(New::G8RCRegisterClass);
      BuildMI(FirstMBB, MBBI, dl, TII.get(New::MovePCtoLR8), New::LR8);
      BuildMI(FirstMBB, MBBI, dl, TII.get(New::MFLR8), GlobalBaseReg);
    }
  }
  return CurDAG->getRegister(GlobalBaseReg,
                             NewLowering.getPointerTy()).getNode();
}

/// isIntS16Immediate - This method tests to see if the node is either a 32-bit
/// or 64-bit immediate, and if the value can be accurately represented as a
/// sign extension from a 16-bit value.  If so, this returns true and the
/// immediate.
static bool isIntS16Immediate(SDNode *N, short &Imm) {
  if (N->getOpcode() != ISD::Constant)
    return false;

  Imm = (short)cast<ConstantSDNode>(N)->getZExtValue();
  if (N->getValueType(0) == MVT::i32)
    return Imm == (int32_t)cast<ConstantSDNode>(N)->getZExtValue();
  else
    return Imm == (int64_t)cast<ConstantSDNode>(N)->getZExtValue();
}

static bool isIntS16Immediate(SDValue Op, short &Imm) {
  return isIntS16Immediate(Op.getNode(), Imm);
}


/// isInt32Immediate - This method tests to see if the node is a 32-bit constant
/// operand. If so Imm will receive the 32-bit value.
static bool isInt32Immediate(SDNode *N, unsigned &Imm) {
  if (N->getOpcode() == ISD::Constant && N->getValueType(0) == MVT::i32) {
    Imm = cast<ConstantSDNode>(N)->getZExtValue();
    return true;
  }
  return false;
}

/// isInt64Immediate - This method tests to see if the node is a 64-bit constant
/// operand.  If so Imm will receive the 64-bit value.
static bool isInt64Immediate(SDNode *N, uint64_t &Imm) {
  if (N->getOpcode() == ISD::Constant && N->getValueType(0) == MVT::i64) {
    Imm = cast<ConstantSDNode>(N)->getZExtValue();
    return true;
  }
  return false;
}

// isInt32Immediate - This method tests to see if a constant operand.
// If so Imm will receive the 32 bit value.
static bool isInt32Immediate(SDValue N, unsigned &Imm) {
  return isInt32Immediate(N.getNode(), Imm);
}


// isOpcWithIntImmediate - This method tests to see if the node is a specific
// opcode and that it has a immediate integer right operand.
// If so Imm will receive the 32 bit value.
static bool isOpcWithIntImmediate(SDNode *N, unsigned Opc, unsigned& Imm) {
  return N->getOpcode() == Opc
         && isInt32Immediate(N->getOperand(1).getNode(), Imm);
}

bool NewDAGToDAGISel::isRunOfOnes(unsigned Val, unsigned &MB, unsigned &ME) {
  if (isShiftedMask_32(Val)) {
    // look for the first non-zero bit
    MB = CountLeadingZeros_32(Val);
    // look for the first zero bit after the run of ones
    ME = CountLeadingZeros_32((Val - 1) ^ Val);
    return true;
  } else {
    Val = ~Val; // invert mask
    if (isShiftedMask_32(Val)) {
      // effectively look for the first zero bit
      ME = CountLeadingZeros_32(Val) - 1;
      // effectively look for the first one bit after the run of zeros
      MB = CountLeadingZeros_32((Val - 1) ^ Val) + 1;
      return true;
    }
  }
  // no run present
  return false;
}

bool NewDAGToDAGISel::isRotateAndMask(SDNode *N, unsigned Mask, 
                                      bool isShiftMask, unsigned &SH, 
                                      unsigned &MB, unsigned &ME) {
  // Don't even go down this path for i64, since different logic will be
  // necessary for rldicl/rldicr/rldimi.
  if (N->getValueType(0) != MVT::i32)
    return false;

  unsigned Shift  = 32;
  unsigned Indeterminant = ~0;  // bit mask marking indeterminant results
  unsigned Opcode = N->getOpcode();
  if (N->getNumOperands() != 2 ||
      !isInt32Immediate(N->getOperand(1).getNode(), Shift) || (Shift > 31))
    return false;
  
  if (Opcode == ISD::SHL) {
    // apply shift left to mask if it comes first
    if (isShiftMask) Mask = Mask << Shift;
    // determine which bits are made indeterminant by shift
    Indeterminant = ~(0xFFFFFFFFu << Shift);
  } else if (Opcode == ISD::SRL) { 
    // apply shift right to mask if it comes first
    if (isShiftMask) Mask = Mask >> Shift;
    // determine which bits are made indeterminant by shift
    Indeterminant = ~(0xFFFFFFFFu >> Shift);
    // adjust for the left rotate
    Shift = 32 - Shift;
  } else if (Opcode == ISD::ROTL) {
    Indeterminant = 0;
  } else {
    return false;
  }
  
  // if the mask doesn't intersect any Indeterminant bits
  if (Mask && !(Mask & Indeterminant)) {
    SH = Shift & 31;
    // make sure the mask is still a mask (wrap arounds may not be)
    return isRunOfOnes(Mask, MB, ME);
  }
  return false;
}

/// SelectBitfieldInsert - turn an or of two masked values into
/// the rotate left word immediate then mask insert (rlwimi) instruction.
SDNode *NewDAGToDAGISel::SelectBitfieldInsert(SDNode *N) {
  /*
  SDValue Op0 = N->getOperand(0);
  SDValue Op1 = N->getOperand(1);
  DebugLoc dl = N->getDebugLoc();
  
  APInt LKZ, LKO, RKZ, RKO;
  CurDAG->ComputeMaskedBits(Op0, APInt::getAllOnesValue(32), LKZ, LKO);
  CurDAG->ComputeMaskedBits(Op1, APInt::getAllOnesValue(32), RKZ, RKO);
  
  unsigned TargetMask = LKZ.getZExtValue();
  unsigned InsertMask = RKZ.getZExtValue();
  
  if ((TargetMask | InsertMask) == 0xFFFFFFFF) {
    unsigned Op0Opc = Op0.getOpcode();
    unsigned Op1Opc = Op1.getOpcode();
    unsigned Value, SH = 0;
    TargetMask = ~TargetMask;
    InsertMask = ~InsertMask;

    // If the LHS has a foldable shift and the RHS does not, then swap it to the
    // RHS so that we can fold the shift into the insert.
    if (Op0Opc == ISD::AND && Op1Opc == ISD::AND) {
      if (Op0.getOperand(0).getOpcode() == ISD::SHL ||
          Op0.getOperand(0).getOpcode() == ISD::SRL) {
        if (Op1.getOperand(0).getOpcode() != ISD::SHL &&
            Op1.getOperand(0).getOpcode() != ISD::SRL) {
          std::swap(Op0, Op1);
          std::swap(Op0Opc, Op1Opc);
          std::swap(TargetMask, InsertMask);
        }
      }
    } else if (Op0Opc == ISD::SHL || Op0Opc == ISD::SRL) {
      if (Op1Opc == ISD::AND && Op1.getOperand(0).getOpcode() != ISD::SHL &&
          Op1.getOperand(0).getOpcode() != ISD::SRL) {
        std::swap(Op0, Op1);
        std::swap(Op0Opc, Op1Opc);
        std::swap(TargetMask, InsertMask);
      }
    }
    
    unsigned MB, ME;
    if (InsertMask && isRunOfOnes(InsertMask, MB, ME)) {
      SDValue Tmp1, Tmp2;

      if ((Op1Opc == ISD::SHL || Op1Opc == ISD::SRL) &&
          isInt32Immediate(Op1.getOperand(1), Value)) {
        Op1 = Op1.getOperand(0);
        SH  = (Op1Opc == ISD::SHL) ? Value : 32 - Value;
      }
      if (Op1Opc == ISD::AND) {
        unsigned SHOpc = Op1.getOperand(0).getOpcode();
        if ((SHOpc == ISD::SHL || SHOpc == ISD::SRL) &&
            isInt32Immediate(Op1.getOperand(0).getOperand(1), Value)) {
          Op1 = Op1.getOperand(0).getOperand(0);
          SH  = (SHOpc == ISD::SHL) ? Value : 32 - Value;
        } else {
          Op1 = Op1.getOperand(0);
        }
      }

      SH &= 31;
      SDValue Ops[] = { Op0, Op1, getI32Imm(SH), getI32Imm(MB),
                          getI32Imm(ME) };
      return CurDAG->getMachineNode(New::RLWIMI, dl, MVT::i32, Ops, 5);
    }
    }
  */
  //Julian & Excess4
  return 0;
}

/// SelectCC - Select a comparison of the specified values with the specified
/// condition code, returning the CR# of the expression.
SDValue NewDAGToDAGISel::SelectCC(SDValue LHS, SDValue RHS,
                                    ISD::CondCode CC, DebugLoc dl) {
  // Always select the LHS.
  unsigned Opc;
  
  if (LHS.getValueType() == MVT::i32) {
    unsigned Imm;
    if (CC == ISD::SETEQ || CC == ISD::SETNE) {
      if (isInt32Immediate(RHS, Imm)) {
        // SETEQ/SETNE comparison with 16-bit immediate, fold it.
        if (isUInt<16>(Imm))
          return SDValue(CurDAG->getMachineNode(New::CMPLWI, dl, MVT::i32, LHS,
                                                getI32Imm(Imm & 0xFFFF)), 0);
        // If this is a 16-bit signed immediate, fold it.
	/*
        if (isInt<16>((int)Imm))
          return SDValue(CurDAG->getMachineNode(New::CMPWI, dl, MVT::i32, LHS,
                                                getI32Imm(Imm & 0xFFFF)), 0);
	*///Avoid using CMPWI //Excess4

        // For non-equality comparisons, the default code would materialize the
        // constant, then compare against it, like this:
        //   lis r2, 4660
        //   ori r2, r2, 22136 
        //   cmpw cr0, r3, r2
        // Since we are just comparing for equality, we can emit this instead:
        //   xoris r0,r3,0x1234
        //   cmplwi cr0,r0,0x5678
        //   beq cr0,L6
        SDValue Xor(CurDAG->getMachineNode(New::XORIS8, dl, MVT::i32, LHS,
                                           getI32Imm(Imm >> 16)), 0);//Excess4
        return SDValue(CurDAG->getMachineNode(New::CMPLWI, dl, MVT::i32, Xor,
                                              getI32Imm(Imm & 0xFFFF)), 0);
      }
      Opc = New::CMPLW;
    } else if (ISD::isUnsignedIntSetCC(CC)) {
      if (isInt32Immediate(RHS, Imm) && isUInt<16>(Imm))
        return SDValue(CurDAG->getMachineNode(New::CMPLWI, dl, MVT::i32, LHS,
                                              getI32Imm(Imm & 0xFFFF)), 0);
      Opc = New::CMPLW;
    } else {
      /*
      short SImm;
      if (isIntS16Immediate(RHS, SImm))
        return SDValue(CurDAG->getMachineNode(New::CMPWI, dl, MVT::i32, LHS,
                                              getI32Imm((int)SImm & 0xFFFF)),
					      0);*///Excess4
      Opc = New::CMPW;
    }
  } else if (LHS.getValueType() == MVT::i64) {
    uint64_t Imm;
    if (CC == ISD::SETEQ || CC == ISD::SETNE) {
      if (isInt64Immediate(RHS.getNode(), Imm)) {
        // SETEQ/SETNE comparison with 16-bit immediate, fold it.
        if (isUInt<16>(Imm))
          return SDValue(CurDAG->getMachineNode(New::CMPLDI, dl, MVT::i64, LHS,
                                                getI32Imm(Imm & 0xFFFF)), 0);
        // If this is a 16-bit signed immediate, fold it.
        if (isInt<16>(Imm))
          return SDValue(CurDAG->getMachineNode(New::CMPDI, dl, MVT::i64, LHS,
                                                getI32Imm(Imm & 0xFFFF)), 0);
        
        // For non-equality comparisons, the default code would materialize the
        // constant, then compare against it, like this:
        //   lis r2, 4660
        //   ori r2, r2, 22136 
        //   cmpd cr0, r3, r2
        // Since we are just comparing for equality, we can emit this instead:
        //   xoris r0,r3,0x1234
        //   cmpldi cr0,r0,0x5678
        //   beq cr0,L6
        if (isUInt<32>(Imm)) {
          SDValue Xor(CurDAG->getMachineNode(New::XORIS8, dl, MVT::i64, LHS,
                                             getI64Imm(Imm >> 16)), 0);
          return SDValue(CurDAG->getMachineNode(New::CMPLDI, dl, MVT::i64, Xor,
                                                getI64Imm(Imm & 0xFFFF)), 0);
        }
      }
      Opc = New::CMPLD;
    } else if (ISD::isUnsignedIntSetCC(CC)) {
      if (isInt64Immediate(RHS.getNode(), Imm) && isUInt<16>(Imm))
        return SDValue(CurDAG->getMachineNode(New::CMPLDI, dl, MVT::i64, LHS,
                                              getI64Imm(Imm & 0xFFFF)), 0);
      Opc = New::CMPLD;
    } else {
      short SImm;
      if (isIntS16Immediate(RHS, SImm))
        return SDValue(CurDAG->getMachineNode(New::CMPDI, dl, MVT::i64, LHS,
                                              getI64Imm(SImm & 0xFFFF)),
                         0);
      Opc = New::CMPD;
    }
  } else if (LHS.getValueType() == MVT::f32) {
    Opc = New::FCMPUS;
  } else {
    assert(LHS.getValueType() == MVT::f64 && "Unknown vt!");
    Opc = New::FCMPUD;
  }
  return SDValue(CurDAG->getMachineNode(Opc, dl, MVT::i32, LHS, RHS), 0);
}

static New::Predicate getPredicateForSetCC(ISD::CondCode CC) {
  switch (CC) {
  case ISD::SETUEQ:
  case ISD::SETONE:
  case ISD::SETOLE:
  case ISD::SETOGE:
    llvm_unreachable("Should be lowered by legalize!");
  default: llvm_unreachable("Unknown condition!");
  case ISD::SETOEQ:
  case ISD::SETEQ:  return New::PRED_EQ;
  case ISD::SETUNE:
  case ISD::SETNE:  return New::PRED_NE;
  case ISD::SETOLT:
  case ISD::SETLT:  return New::PRED_LT;
  case ISD::SETULE:
  case ISD::SETLE:  return New::PRED_LE;
  case ISD::SETOGT:
  case ISD::SETGT:  return New::PRED_GT;
  case ISD::SETUGE:
  case ISD::SETGE:  return New::PRED_GE;
  case ISD::SETO:   return New::PRED_NU;
  case ISD::SETUO:  return New::PRED_UN;
    // These two are invalid for floating point.  Assume we have int.
  case ISD::SETULT: return New::PRED_LT;
  case ISD::SETUGT: return New::PRED_GT;
  }
}

/// getCRIdxForSetCC - Return the index of the condition register field
/// associated with the SetCC condition, and whether or not the field is
/// treated as inverted.  That is, lt = 0; ge = 0 inverted.
///
/// If this returns with Other != -1, then the returned comparison is an or of
/// two simpler comparisons.  In this case, Invert is guaranteed to be false.
static unsigned getCRIdxForSetCC(ISD::CondCode CC, bool &Invert, int &Other) {
  Invert = false;
  Other = -1;
  switch (CC) {
  default: llvm_unreachable("Unknown condition!");
  case ISD::SETOLT:
  case ISD::SETLT:  return 0;                  // Bit #0 = SETOLT
  case ISD::SETOGT:
  case ISD::SETGT:  return 1;                  // Bit #1 = SETOGT
  case ISD::SETOEQ:
  case ISD::SETEQ:  return 2;                  // Bit #2 = SETOEQ
  case ISD::SETUO:  return 3;                  // Bit #3 = SETUO
  case ISD::SETUGE:
  case ISD::SETGE:  Invert = true; return 0;   // !Bit #0 = SETUGE
  case ISD::SETULE:
  case ISD::SETLE:  Invert = true; return 1;   // !Bit #1 = SETULE
  case ISD::SETUNE:
  case ISD::SETNE:  Invert = true; return 2;   // !Bit #2 = SETUNE
  case ISD::SETO:   Invert = true; return 3;   // !Bit #3 = SETO
  case ISD::SETUEQ: 
  case ISD::SETOGE: 
  case ISD::SETOLE: 
  case ISD::SETONE:
    llvm_unreachable("Invalid branch code: should be expanded by legalize");
  // These are invalid for floating point.  Assume integer.
  case ISD::SETULT: return 0;
  case ISD::SETUGT: return 1;
  }
  return 0;
}

//Define UseGPUBranches
cl::opt<bool> UseGPUBranches("use-gpu-branches",
			     cl::init(false),
			     cl::desc("Use GPU-supported banches instead of PPC CR-based ones"),
			     cl::Hidden);
//Excess4

SDNode *NewDAGToDAGISel::SelectSETCC(SDNode *N) {
  DebugLoc dl = N->getDebugLoc();
  unsigned Imm;
  ISD::CondCode CC = cast<CondCodeSDNode>(N->getOperand(2))->get();
  if (isInt32Immediate(N->getOperand(1), Imm)) {
    // We can codegen setcc op, imm very efficiently compared to a brcond.
    // Check for those cases here.
    // setcc op, 0
    if (Imm == 0) {
      SDValue Op = N->getOperand(0);
      switch (CC) {
      default: break;
	/* case ISD::SETEQ: {
	   Op = SDValue(CurDAG->getMachineNode(New::CNTLZW, dl, MVT::i32, Op), 0);
	   SDValue Ops[] = { Op, getI32Imm(27), getI32Imm(5), getI32Imm(31) };
	   return CurDAG->SelectNodeTo(N, New::RLWINM, MVT::i32, Ops, 4);
	   }*/
      case ISD::SETNE: {
        SDValue AD =
          SDValue(CurDAG->getMachineNode(New::ADDIC, dl, MVT::i32, MVT::Flag,
                                         Op, getI32Imm(~0U)), 0);
        return CurDAG->SelectNodeTo(N, New::SUBFE, MVT::i32, AD, Op, 
                                    AD.getValue(1));
      }
	/*case ISD::SETLT: {
        SDValue Ops[] = { Op, getI32Imm(1), getI32Imm(31), getI32Imm(31) };
        return CurDAG->SelectNodeTo(N, New::RLWINM, MVT::i32, Ops, 4);
      }*/
	/*case ISD::SETGT: {
        SDValue T =
          SDValue(CurDAG->getMachineNode(New::NEG, dl, MVT::i32, Op), 0);
        T = SDValue(CurDAG->getMachineNode(New::ANDC, dl, MVT::i32, T, Op), 0);
        SDValue Ops[] = { T, getI32Imm(1), getI32Imm(31), getI32Imm(31) };
        return CurDAG->SelectNodeTo(N, New::RLWINM, MVT::i32, Ops, 4);
      }*/
      }
    } else if (Imm == ~0U) {        // setcc op, -1
      SDValue Op = N->getOperand(0);
      switch (CC) {
      default: break;
      case ISD::SETEQ:
        Op = SDValue(CurDAG->getMachineNode(New::ADDIC, dl, MVT::i32, MVT::Flag,
                                            Op, getI32Imm(1)), 0);
        return CurDAG->SelectNodeTo(N, New::ADDZE, MVT::i32, 
                              SDValue(CurDAG->getMachineNode(New::LI, dl, 
                                                             MVT::i32,
                                                             getI32Imm(0)), 0),
                                      Op.getValue(1));
	/*      case ISD::SETNE: {
        Op = SDValue(CurDAG->getMachineNode(New::NOR, dl, MVT::i32, Op, Op), 0);
        SDNode *AD = CurDAG->getMachineNode(New::ADDIC, dl, MVT::i32, MVT::Flag,
                                            Op, getI32Imm(~0U));
        return CurDAG->SelectNodeTo(N, New::SUBFE, MVT::i32, SDValue(AD, 0),
                                    Op, SDValue(AD, 1));
      }
      case ISD::SETLT: {
        SDValue AD = SDValue(CurDAG->getMachineNode(New::ADDI, dl, MVT::i32, Op,
                                                    getI32Imm(1)), 0);
        SDValue AN = SDValue(CurDAG->getMachineNode(New::AND, dl, MVT::i32, AD,
                                                    Op), 0);
        SDValue Ops[] = { AN, getI32Imm(1), getI32Imm(31), getI32Imm(31) };
        return CurDAG->SelectNodeTo(N, New::RLWINM, MVT::i32, Ops, 4);
      }
      case ISD::SETGT: {
        SDValue Ops[] = { Op, getI32Imm(1), getI32Imm(31), getI32Imm(31) };
        Op = SDValue(CurDAG->getMachineNode(New::RLWINM, dl, MVT::i32, Ops, 4), 
                     0);
        return CurDAG->SelectNodeTo(N, New::XORI, MVT::i32, Op, 
                                    getI32Imm(1));
				    }*/
      }
    }
  }

  // If we use GPU branches instead of condition registers, we have to use a               
  // special GPU-style SELECT_CC instruction to select the value into a                    
  // new register.  -- Julian   
  ///* Begining of Edit By Julian
  if (UseGPUBranches)
  {
    SDValue Li0Ops[] = { getI32Imm(0) };
    SDValue LoadedZero = SDValue(CurDAG->getMachineNode(New::LI, dl, MVT::i32, Li0Ops, 1),\
 0);
    SDValue Li1Ops[] = { getI32Imm(1) };
    SDValue LoadedOne = SDValue(CurDAG->getMachineNode(New::LI, dl, MVT::i32, Li1Ops, 1), \
0);

    SDValue Ops[] = {
      N->getOperand(0), // $A                                                              
      N->getOperand(1), // $B                                                              
      LoadedOne, // $T                                                                     
      LoadedZero, // $F                                                                    
      getI32Imm(getPredicateForSetCC(CC))  // comparison code (i.e. what instruction to use)    
    };
    if (N->getOperand(0).getValueType() == MVT::i32)
      return CurDAG->SelectNodeTo(N, New::GPU_SELECT_CC_I4, MVT::i32, Ops, 5);
    else if (N->getOperand(0).getValueType() == MVT::i64)
      return CurDAG->SelectNodeTo(N, New::GPU_SELECT_CC_I8, MVT::i32, Ops, 5);
    else if (N->getOperand(0).getValueType() == MVT::f32)
      return CurDAG->SelectNodeTo(N, New::GPU_SELECT_CC_F4, MVT::i32, Ops, 5);
    else if (N->getOperand(0).getValueType() == MVT::f64)
      return CurDAG->SelectNodeTo(N, New::GPU_SELECT_CC_F8, MVT::i32, Ops, 5);
    else
      assert(0 && "Unknown operand type");
  }
  //End of Edit by Julian*/  
  bool Inv;
  int OtherCondIdx;
  unsigned Idx = getCRIdxForSetCC(CC, Inv, OtherCondIdx);
  SDValue CCReg = SelectCC(N->getOperand(0), N->getOperand(1), CC, dl);
  SDValue IntCR;
  
  // Force the ccreg into CR7.
  SDValue CR7Reg = CurDAG->getRegister(New::CR7, MVT::i32);
  
  SDValue InFlag(0, 0);  // Null incoming flag value.
  CCReg = CurDAG->getCopyToReg(CurDAG->getEntryNode(), dl, CR7Reg, CCReg, 
                               InFlag).getValue(1);
  
//diff with Julian(Begin)
IntCR = SDValue(CurDAG->getMachineNode(New::MFCR, dl, MVT::i32, CCReg), 0);


//if (NewSubTarget.isGigaProcessor() && OtherCondIdx == -1)
//    IntCR = SDValue(CurDAG->getMachineNode(New::MFOCRF, dl, MVT::i32, CR7Reg,
//                                           CCReg), 0);
// else
//    IntCR = SDValue(CurDAG->getMachineNode(New::MFCRpseud, dl, MVT::i32,
//                                           CR7Reg, CCReg), 0);
  

  SDValue LiOps[] = { getI32Imm(3 - Idx) };
  SDValue LoadedConstant = SDValue(CurDAG->getMachineNode(New::LI, dl, MVT::i32, LiOps, 1), 0);
  
  SDValue SrwOps[] = { IntCR, LoadedConstant };
  SDValue ShiftedValue = SDValue(CurDAG->getMachineNode(New::SRW, dl, MVT::i32, SrwOps, 2), 0);
  
  SDValue Li1Ops[] = { getI32Imm(1) };
  SDValue LoadedOne = SDValue(CurDAG->getMachineNode(New::LI, dl, MVT::i32, Li1Ops, 1), 0);
  
  SDValue AndOps[] = { ShiftedValue, LoadedOne };


//  SDValue Ops[] = { IntCR, getI32Imm((32-(3-Idx)) & 31),
//                      getI32Imm(31), getI32Imm(31) };
  if (OtherCondIdx == -1 && !Inv)
    //  return CurDAG->SelectNodeTo(N, New::RLWINM, MVT::i32, Ops, 4);
    return CurDAG->SelectNodeTo(N, New::AND, MVT::i32, AndOps, 2);
//End of Julian


  // Get the specified bit.
  SDValue Tmp =
    //    SDValue(CurDAG->getMachineNode(New::RLWINM, dl, MVT::i32, Ops, 4), 0);
    SDValue(CurDAG->getMachineNode(New::AND, dl, MVT::i32, AndOps, 2), 0);
   //Excess4

/*  if (Inv) {
    assert(OtherCondIdx == -1 && "Can't have split plus negation");
    return CurDAG->SelectNodeTo(N, New::XORI, MVT::i32, Tmp, getI32Imm(1));
    }*///EXCESS4

//diff with Julian(Begin)
  // Otherwise, we have to turn an operation like SETONE -> SETOLT | SETOGT.
  // We already got the bit for the first part of the comparison (e.g. SETULE).
  // Get the other bit of the comparison.
  SDValue LiOps2[] = { getI32Imm(3 - OtherCondIdx) };
  SDValue LoadedConstant2 = SDValue(CurDAG->getMachineNode(New::LI, dl, MVT::i32, LiOps2, 1), 0);
  SDValue AndOps2[] = { ShiftedValue, LoadedConstant2 };
  SDValue OtherCond(CurDAG->getMachineNode(New::AND, dl, MVT::i32, AndOps2, 4), 0);
//End of Julian
/* diff with Julian
  // Get the other bit of the comparison.
  Ops[1] = getI32Imm((32-(3-OtherCondIdx)) & 31);
  SDValue OtherCond = 
    SDValue(CurDAG->getMachineNode(New::RLWINM, dl, MVT::i32, Ops, 4), 0);
*/
  return CurDAG->SelectNodeTo(N, New::OR, MVT::i32, Tmp, OtherCond);
}


// Select - Convert the specified operand from a target-independent to a
// target-specific node if it hasn't already been changed.
SDNode *NewDAGToDAGISel::Select(SDNode *N) {
  DebugLoc dl = N->getDebugLoc();
  if (N->isMachineOpcode())
    return NULL;   // Already selected.

  switch (N->getOpcode()) {
  default: break;
  
  case ISD::Constant: {
    if (N->getValueType(0) == MVT::i64) {
      // Get 64 bit value.
      int64_t Imm = cast<ConstantSDNode>(N)->getZExtValue();
      // Assume no remaining bits.
      unsigned Remainder = 0;
      // Assume no shift required.
      unsigned Shift = 0;
      
      // If it can't be represented as a 32 bit value.
      if (!isInt<32>(Imm)) {
        Shift = CountTrailingZeros_64(Imm);
        int64_t ImmSh = static_cast<uint64_t>(Imm) >> Shift;
        
        // If the shifted value fits 32 bits.
        if (isInt<32>(ImmSh)) {
          // Go with the shifted value.
          Imm = ImmSh;
        } else {
          // Still stuck with a 64 bit value.
          Remainder = Imm;
          Shift = 32;
          Imm >>= 32;
        }
      }
      
      // Intermediate operand.
      SDNode *Result;

      // Handle first 32 bits.
      unsigned Lo = Imm & 0xFFFF;
      unsigned Hi = (Imm >> 16) & 0xFFFF;
      
      // Simple value.
      if (isInt<16>(Imm)) {
       // Just the Lo bits.
        Result = CurDAG->getMachineNode(New::LI8, dl, MVT::i64, getI32Imm(Lo));
      } else if (Lo) {
        // Handle the Hi bits.
        unsigned OpC = Hi ? New::LIS8 : New::LI8;
        Result = CurDAG->getMachineNode(OpC, dl, MVT::i64, getI32Imm(Hi));
        // And Lo bits.
        Result = CurDAG->getMachineNode(New::ORI8, dl, MVT::i64,
                                        SDValue(Result, 0), getI32Imm(Lo));
      } else {
       // Just the Hi bits.
        Result = CurDAG->getMachineNode(New::LIS8, dl, MVT::i64, getI32Imm(Hi));
      }
      
      // If no shift, we're done.
      if (!Shift) return Result;

      // Shift for next step if the upper 32-bits were not zero.
      if (Imm) {
        Result = CurDAG->getMachineNode(New::RLDICR, dl, MVT::i64,
                                        SDValue(Result, 0),
                                        getI32Imm(Shift),
                                        getI32Imm(63 - Shift));
      }

      // Add in the last bits as required.
      if ((Hi = (Remainder >> 16) & 0xFFFF)) {
        Result = CurDAG->getMachineNode(New::ORIS8, dl, MVT::i64,
                                        SDValue(Result, 0), getI32Imm(Hi));
      } 
      if ((Lo = Remainder & 0xFFFF)) {
        Result = CurDAG->getMachineNode(New::ORI8, dl, MVT::i64,
                                        SDValue(Result, 0), getI32Imm(Lo));
      }
      
      return Result;
    }
    break;
  }
  
  case ISD::SETCC:
    return SelectSETCC(N);
  case NewISD::GlobalBaseReg:
    return getGlobalBaseReg();
    
  case ISD::FrameIndex: {
    int FI = cast<FrameIndexSDNode>(N)->getIndex();
    SDValue TFI = CurDAG->getTargetFrameIndex(FI, N->getValueType(0));
    unsigned Opc = N->getValueType(0) == MVT::i32 ? New::ADDI : New::ADDI8;
    if (N->hasOneUse())
      return CurDAG->SelectNodeTo(N, Opc, N->getValueType(0), TFI,
                                  getSmallIPtrImm(0));
    return CurDAG->getMachineNode(Opc, dl, N->getValueType(0), TFI,
                                  getSmallIPtrImm(0));
  }

  case NewISD::MFCR: {
    SDValue InFlag = N->getOperand(1);
    // Use MFOCRF if supported.
    if (NewSubTarget.isGigaProcessor())
      return CurDAG->getMachineNode(New::MFOCRF, dl, MVT::i32,
                                    N->getOperand(0), InFlag);
    else
      //      return CurDAG->getMachineNode(New::MFCRpseud, dl, MVT::i32,
      //                              N->getOperand(0), InFlag);
      return CurDAG->getMachineNode(New::MFCR, dl, MVT::i32, InFlag);
    //Dondum
  }
    
  case ISD::SDIV: {
    // FIXME: since this depends on the setting of the carry flag from the srawi
    //        we should really be making notes about that for the scheduler.
    // FIXME: It sure would be nice if we could cheaply recognize the 
    //        srl/add/sra pattern the dag combiner will generate for this as
    //        sra/addze rather than having to handle sdiv ourselves.  oh well.
   /*yakola-This case is map a division by an immedate value.       
    unsigned Imm;
    if (isInt32Immediate(N->getOperand(1), Imm)) {
      SDValue N0 = N->getOperand(0);
      if ((signed)Imm > 0 && isPowerOf2_32(Imm)) {
        SDNode *Op =
          CurDAG->getMachineNode(New::SRAWI, dl, MVT::i32, MVT::Flag,
                                 N0, getI32Imm(Log2_32(Imm)));
        return CurDAG->SelectNodeTo(N, New::ADDZE, MVT::i32, 
                                    SDValue(Op, 0), SDValue(Op, 1));
      } else if ((signed)Imm < 0 && isPowerOf2_32(-Imm)) {
        SDNode *Op =
          CurDAG->getMachineNode(New::SRAWI, dl, MVT::i32, MVT::Flag,
                                 N0, getI32Imm(Log2_32(-Imm)));
        SDValue PT =
          SDValue(CurDAG->getMachineNode(New::ADDZE, dl, MVT::i32,
                                         SDValue(Op, 0), SDValue(Op, 1)),
                    0);
        return CurDAG->SelectNodeTo(N, New::NEG, MVT::i32, PT);
      }
    }
   */
    // Other cases are autogenerated.
    break;
  }
    
  case ISD::LOAD: {
    // Handle preincrement loads.
    LoadSDNode *LD = cast<LoadSDNode>(N);
    EVT LoadedVT = LD->getMemoryVT();
    
    // Normal loads are handled by code generated from the .td file.
    if (LD->getAddressingMode() != ISD::PRE_INC)
      break;
    
    SDValue Offset = LD->getOffset();
    if (isa<ConstantSDNode>(Offset) ||
        Offset.getOpcode() == ISD::TargetGlobalAddress) {
      
      unsigned Opcode;
      bool isSExt = LD->getExtensionType() == ISD::SEXTLOAD;
      if (LD->getValueType(0) != MVT::i64) {
        // Handle New32 integer and normal FP loads.
        assert((!isSExt || LoadedVT == MVT::i16) && "Invalid sext update load");
        switch (LoadedVT.getSimpleVT().SimpleTy) {
          default: llvm_unreachable("Invalid New load type!");
          case MVT::f64: Opcode = New::LFDU; break;
          case MVT::f32: Opcode = New::LFSU; break;
          case MVT::i32: Opcode = New::LWZU; break;
          case MVT::i16: Opcode = isSExt ? New::LHAU : New::LHZU; break;
          case MVT::i1:
          case MVT::i8:  Opcode = New::LBZU; break;
        }
      } else {
        assert(LD->getValueType(0) == MVT::i64 && "Unknown load result type!");
        assert((!isSExt || LoadedVT == MVT::i16) && "Invalid sext update load");
        switch (LoadedVT.getSimpleVT().SimpleTy) {
          default: llvm_unreachable("Invalid New load type!");
          case MVT::i64: Opcode = New::LDU; break;
          case MVT::i32: Opcode = New::LWZU8; break;
          case MVT::i16: Opcode = isSExt ? New::LHAU8 : New::LHZU8; break;
          case MVT::i1:
          case MVT::i8:  Opcode = New::LBZU8; break;
        }
      }
      
      SDValue Chain = LD->getChain();
      SDValue Base = LD->getBasePtr();
      SDValue Ops[] = { Offset, Base, Chain };
      // FIXME: New64
      return CurDAG->getMachineNode(Opcode, dl, LD->getValueType(0),
                                    NewLowering.getPointerTy(),
                                    MVT::Other, Ops, 3);
    } else {
      llvm_unreachable("R+R preindex loads not supported yet!");
    }
  }
    
  case ISD::AND: {
    /* unsigned Imm, Imm2, SH, MB, ME;

    // If this is an and of a value rotated between 0 and 31 bits and then and'd
    // with a mask, emit rlwinm
    if (isInt32Immediate(N->getOperand(1), Imm) &&
        isRotateAndMask(N->getOperand(0).getNode(), Imm, false, SH, MB, ME)) {
      SDValue Val = N->getOperand(0).getOperand(0);
      SDValue Ops[] = { Val, getI32Imm(SH), getI32Imm(MB), getI32Imm(ME) };
      return CurDAG->SelectNodeTo(N, New::RLWINM, MVT::i32, Ops, 4);
    }
    // If this is just a masked value where the input is not handled above, and
    // is not a rotate-left (handled by a pattern in the .td file), emit rlwinm
    if (isInt32Immediate(N->getOperand(1), Imm) &&
        isRunOfOnes(Imm, MB, ME) && 
        N->getOperand(0).getOpcode() != ISD::ROTL) {
      SDValue Val = N->getOperand(0);
      SDValue Ops[] = { Val, getI32Imm(0), getI32Imm(MB), getI32Imm(ME) };
      return CurDAG->SelectNodeTo(N, New::RLWINM, MVT::i32, Ops, 4);
    }
    // AND X, 0 -> 0, not "rlwinm 32".
    if (isInt32Immediate(N->getOperand(1), Imm) && (Imm == 0)) {
      ReplaceUses(SDValue(N, 0), N->getOperand(1));
      return NULL;
    }
    // ISD::OR doesn't get all the bitfield insertion fun.
    // (and (or x, c1), c2) where isRunOfOnes(~(c1^c2)) is a bitfield insert
    if (isInt32Immediate(N->getOperand(1), Imm) && 
        N->getOperand(0).getOpcode() == ISD::OR &&
        isInt32Immediate(N->getOperand(0).getOperand(1), Imm2)) {
      unsigned MB, ME;
      Imm = ~(Imm^Imm2);
      if (isRunOfOnes(Imm, MB, ME)) {
        SDValue Ops[] = { N->getOperand(0).getOperand(0),
                            N->getOperand(0).getOperand(1),
                            getI32Imm(0), getI32Imm(MB),getI32Imm(ME) };
        return CurDAG->getMachineNode(New::RLWIMI, dl, MVT::i32, Ops, 5);
      }
    }*///Julian & Excess4
    
    // Other cases are autogenerated.
    break;
  }
  case ISD::OR:
    if (N->getValueType(0) == MVT::i32)
      if (SDNode *I = SelectBitfieldInsert(N))
        return I;
      
    // Other cases are autogenerated.
    break;
  case ISD::SHL: {
    /*    unsigned Imm, SH, MB, ME;
    if (isOpcWithIntImmediate(N->getOperand(0).getNode(), ISD::AND, Imm) &&
        isRotateAndMask(N, Imm, true, SH, MB, ME)) {
      SDValue Ops[] = { N->getOperand(0).getOperand(0),
                          getI32Imm(SH), getI32Imm(MB), getI32Imm(ME) };
      return CurDAG->SelectNodeTo(N, New::RLWINM, MVT::i32, Ops, 4);
    }
    */ //Excess4 & Julian
    // Other cases are autogenerated.
    break;
  }
  case ISD::SRL: {
    /*    unsigned Imm, SH, MB, ME;
    if (isOpcWithIntImmediate(N->getOperand(0).getNode(), ISD::AND, Imm) &&
        isRotateAndMask(N, Imm, true, SH, MB, ME)) { 
      SDValue Ops[] = { N->getOperand(0).getOperand(0),
                          getI32Imm(SH), getI32Imm(MB), getI32Imm(ME) };
      return CurDAG->SelectNodeTo(N, New::RLWINM, MVT::i32, Ops, 4);
    }
    */ //Excess4 & Julian
    // Other cases are autogenerated.
    break;
  }
  case ISD::SELECT_CC: {
    ISD::CondCode CC = cast<CondCodeSDNode>(N->getOperand(4))->get();
    
    // Handle the setcc cases here.  select_cc lhs, 0, 1, 0, cc
    if (ConstantSDNode *N1C = dyn_cast<ConstantSDNode>(N->getOperand(1)))
      if (ConstantSDNode *N2C = dyn_cast<ConstantSDNode>(N->getOperand(2)))
        if (ConstantSDNode *N3C = dyn_cast<ConstantSDNode>(N->getOperand(3)))
          if (N1C->isNullValue() && N3C->isNullValue() &&
              N2C->getZExtValue() == 1ULL && CC == ISD::SETNE &&
              // FIXME: Implement this optzn for New64.
              N->getValueType(0) == MVT::i32) {
            SDNode *Tmp =
              CurDAG->getMachineNode(New::ADDIC, dl, MVT::i32, MVT::Flag,
                                     N->getOperand(0), getI32Imm(~0U));
            return CurDAG->SelectNodeTo(N, New::SUBFE, MVT::i32,
                                        SDValue(Tmp, 0), N->getOperand(0),
                                        SDValue(Tmp, 1));
          }

    SDValue CCReg = SelectCC(N->getOperand(0), N->getOperand(1), CC, dl);
    unsigned BROpc = getPredicateForSetCC(CC);

    unsigned SelectCCOp;
    if (N->getValueType(0) == MVT::i32)
      SelectCCOp = New::SELECT_CC_I4;
    else if (N->getValueType(0) == MVT::i64)
      SelectCCOp = New::SELECT_CC_I8;
    else if (N->getValueType(0) == MVT::f32)
      SelectCCOp = New::SELECT_CC_F4;
    else if (N->getValueType(0) == MVT::f64)
      SelectCCOp = New::SELECT_CC_F8;
    else
      SelectCCOp = New::SELECT_CC_VRRC;

    SDValue Ops[] = { CCReg, N->getOperand(2), N->getOperand(3),
                        getI32Imm(BROpc) };
    return CurDAG->SelectNodeTo(N, SelectCCOp, N->getValueType(0), Ops, 4);
  }
  case NewISD::COND_BRANCH: {
    // Op #0 is the Chain.
    // Op #1 is the New::PRED_* number.
    // Op #2 is the CR#
    // Op #3 is the Dest MBB
    // Op #4 is the Flag.
    // Prevent New::PRED_* from being selected into LI.
    SDValue Pred =
      getI32Imm(cast<ConstantSDNode>(N->getOperand(1))->getZExtValue());
    SDValue Ops[] = { Pred, N->getOperand(2), N->getOperand(3),
      N->getOperand(0), N->getOperand(4) };
    return CurDAG->SelectNodeTo(N, New::BCC, MVT::Other, Ops, 5);
  }
  case ISD::BR_CC: {//Julian & Dondum
    //printf("BR_CC_______\n");

    if (UseGPUBranches)
      {
	//printf("USE_GPU_BRANCHES\n");
	unsigned Opc;
	if (N->getOperand(2).getValueType() == MVT::i32)
	  Opc = New::GPU_BCC;
	else if (N->getOperand(2).getValueType() == MVT::i64)
	  Opc = New::GPU_BCC_I8;
	else if (N->getOperand(2).getValueType() == MVT::f32)
	  Opc = New::GPU_BCC_F4;
	else if (N->getOperand(2).getValueType() == MVT::f64)
	  Opc = New::GPU_BCC_F8;
	else
	  assert(0 && "Unknown operand type");

	ISD::CondCode CC = cast<CondCodeSDNode>(N->getOperand(1))->get();

	SDValue Ops[] = { N->getOperand(2),
			  N->getOperand(3),
			  getI32Imm(getPredicateForSetCC(CC)),
			  N->getOperand(4), N->getOperand(0) };
	return CurDAG->SelectNodeTo(N, Opc, MVT::Other, Ops, 5);
      }

    else{
      //printf("NO_USE_GPU_BRANCHES\n");
      ISD::CondCode CC = cast<CondCodeSDNode>(N->getOperand(1))->get();
      SDValue CondCode = SelectCC(N->getOperand(2), N->getOperand(3), CC, dl);
      SDValue Ops[] = { getI32Imm(getPredicateForSetCC(CC)), CondCode, 
                        N->getOperand(4), N->getOperand(0) };
      return CurDAG->SelectNodeTo(N, New::BCC, MVT::Other, Ops, 4);
    }
  }
  case ISD::BRIND: {
    // FIXME: Should custom lower this.
    SDValue Chain = N->getOperand(0);
    SDValue Target = N->getOperand(1);
    unsigned Opc = Target.getValueType() == MVT::i32 ? New::MTCTR : New::MTCTR8;
    Chain = SDValue(CurDAG->getMachineNode(Opc, dl, MVT::Other, Target,
                                           Chain), 0);
    return CurDAG->SelectNodeTo(N, New::BCTR, MVT::Other, Chain);
  }
  }
  
  return SelectCode(N);
}



/// createNewISelDag - This pass converts a legalized DAG into a 
/// PowerPC-specific DAG, ready for instruction scheduling.
///
FunctionPass *llvm::createNewISelDag(NewTargetMachine &TM) {
  return new NewDAGToDAGISel(TM);
}

