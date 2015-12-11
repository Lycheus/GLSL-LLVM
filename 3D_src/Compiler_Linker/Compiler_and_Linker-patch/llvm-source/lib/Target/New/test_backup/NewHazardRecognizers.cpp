//===-- NewHazardRecognizers.cpp - PowerPC Hazard Recognizer Impls --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements hazard recognizers for scheduling on PowerPC processors.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "pre-RA-sched"
#include "NewHazardRecognizers.h"
#include "New.h"
#include "NewInstrInfo.h"
#include "llvm/CodeGen/ScheduleDAG.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

//===----------------------------------------------------------------------===//
// PowerPC 970 Hazard Recognizer
//
// This models the dispatch group formation of the New970 processor.  Dispatch
// groups are bundles of up to five instructions that can contain various mixes
// of instructions.  The New970 can dispatch a peak of 4 non-branch and one 
// branch instruction per-cycle.
//
// There are a number of restrictions to dispatch group formation: some
// instructions can only be issued in the first slot of a dispatch group, & some
// instructions fill an entire dispatch group.  Additionally, only branches can
// issue in the 5th (last) slot.
//
// Finally, there are a number of "structural" hazards on the New970.  These
// conditions cause large performance penalties due to misprediction, recovery,
// and replay logic that has to happen.  These cases include setting a CTR and
// branching through it in the same dispatch group, and storing to an address,
// then loading from the same address within a dispatch group.  To avoid these
// conditions, we insert no-op instructions when appropriate.
//
// FIXME: This is missing some significant cases:
//   1. Modeling of microcoded instructions.
//   2. Handling of serialized operations.
//   3. Handling of the esoteric cases in "Resource-based Instruction Grouping".
//

NewHazardRecognizer970::NewHazardRecognizer970(const TargetInstrInfo &tii)
  : TII(tii) {
  EndDispatchGroup();
}

void NewHazardRecognizer970::EndDispatchGroup() {
  DEBUG(errs() << "=== Start of dispatch group\n");
  NumIssued = 0;
  
  // Structural hazard info.
  HasCTRSet = false;
  NumStores = 0;
}


NewII::New970_Unit 
NewHazardRecognizer970::GetInstrType(unsigned Opcode,
                                     bool &isFirst, bool &isSingle,
                                     bool &isCracked,
                                     bool &isLoad, bool &isStore) {
  if ((int)Opcode >= 0) {
    isFirst = isSingle = isCracked = isLoad = isStore = false;
    return NewII::New970_Pseudo;
  }
  Opcode = ~Opcode;
  
  const TargetInstrDesc &TID = TII.get(Opcode);
  
  isLoad  = TID.mayLoad();
  isStore = TID.mayStore();
  
  uint64_t TSFlags = TID.TSFlags;
  
  isFirst   = TSFlags & NewII::New970_First;
  isSingle  = TSFlags & NewII::New970_Single;
  isCracked = TSFlags & NewII::New970_Cracked;
  return (NewII::New970_Unit)(TSFlags & NewII::New970_Mask);
}

/// isLoadOfStoredAddress - If we have a load from the previously stored pointer
/// as indicated by StorePtr1/StorePtr2/StoreSize, return true.
bool NewHazardRecognizer970::
isLoadOfStoredAddress(unsigned LoadSize, SDValue Ptr1, SDValue Ptr2) const {
  for (unsigned i = 0, e = NumStores; i != e; ++i) {
    // Handle exact and commuted addresses.
    if (Ptr1 == StorePtr1[i] && Ptr2 == StorePtr2[i])
      return true;
    if (Ptr2 == StorePtr1[i] && Ptr1 == StorePtr2[i])
      return true;
    
    // Okay, we don't have an exact match, if this is an indexed offset, see if
    // we have overlap (which happens during fp->int conversion for example).
    if (StorePtr2[i] == Ptr2) {
      if (ConstantSDNode *StoreOffset = dyn_cast<ConstantSDNode>(StorePtr1[i]))
        if (ConstantSDNode *LoadOffset = dyn_cast<ConstantSDNode>(Ptr1)) {
          // Okay the base pointers match, so we have [c1+r] vs [c2+r].  Check
          // to see if the load and store actually overlap.
          int StoreOffs = StoreOffset->getZExtValue();
          int LoadOffs  = LoadOffset->getZExtValue();
          if (StoreOffs < LoadOffs) {
            if (int(StoreOffs+StoreSize[i]) > LoadOffs) return true;
          } else {
            if (int(LoadOffs+LoadSize) > StoreOffs) return true;
          }
        }
    }
  }
  return false;
}

/// getHazardType - We return hazard for any non-branch instruction that would
/// terminate the dispatch group.  We turn NoopHazard for any
/// instructions that wouldn't terminate the dispatch group that would cause a
/// pipeline flush.
ScheduleHazardRecognizer::HazardType NewHazardRecognizer970::
getHazardType(SUnit *SU) {
  const SDNode *Node = SU->getNode()->getFlaggedMachineNode();
  bool isFirst, isSingle, isCracked, isLoad, isStore;
  NewII::New970_Unit InstrType = 
    GetInstrType(Node->getOpcode(), isFirst, isSingle, isCracked,
                 isLoad, isStore);
  if (InstrType == NewII::New970_Pseudo) return NoHazard;  
  unsigned Opcode = Node->getMachineOpcode();

  // We can only issue a New970_First/New970_Single instruction (such as
  // crand/mtspr/etc) if this is the first cycle of the dispatch group.
  if (NumIssued != 0 && (isFirst || isSingle))
    return Hazard;
  
  // If this instruction is cracked into two ops by the decoder, we know that
  // it is not a branch and that it cannot issue if 3 other instructions are
  // already in the dispatch group.
  if (isCracked && NumIssued > 2)
    return Hazard;
      
  switch (InstrType) {
  default: llvm_unreachable("Unknown instruction type!");
  case NewII::New970_FXU:
  case NewII::New970_LSU:
  case NewII::New970_FPU:
  case NewII::New970_VALU:
  case NewII::New970_VPERM:
    // We can only issue a branch as the last instruction in a group.
    if (NumIssued == 4) return Hazard;
    break;
  case NewII::New970_CRU:
    // We can only issue a CR instruction in the first two slots.
    if (NumIssued >= 2) return Hazard;
    break;
  case NewII::New970_BRU:
    break;
  }
  
  // Do not allow MTCTR and BCTRL to be in the same dispatch group.
  if (HasCTRSet && (Opcode == New::BCTRL_Darwin || Opcode == New::BCTRL_SVR4))
    return NoopHazard;
  
  // If this is a load following a store, make sure it's not to the same or
  // overlapping address.
  if (isLoad && NumStores) {
    unsigned LoadSize;
    switch (Opcode) {
    default: llvm_unreachable("Unknown load!");
    case New::LBZ:   case New::LBZU:
    case New::LBZX:
    case New::LBZ8:  case New::LBZU8:
    case New::LBZX8:
    case New::LVEBX:
      LoadSize = 1;
      break;
    case New::LHA:   case New::LHAU:
    case New::LHAX:
    case New::LHZ:   case New::LHZU:
    case New::LHZX:
    case New::LVEHX:
    case New::LHBRX:
    case New::LHA8:   case New::LHAU8:
    case New::LHAX8:
    case New::LHZ8:   case New::LHZU8:
    case New::LHZX8:
      LoadSize = 2;
      break;
    case New::LFS:    case New::LFSU:
    case New::LFSX:
    case New::LWZ:    case New::LWZU:
    case New::LWZX:
    case New::LWA:
    case New::LWAX:
    case New::LVEWX:
    case New::LWBRX:
    case New::LWZ8:
    case New::LWZX8:
      LoadSize = 4;
      break;
    case New::LFD:    case New::LFDU:
    case New::LFDX:
    case New::LD:     case New::LDU:
    case New::LDX:
      LoadSize = 8;
      break;
    case New::LVX:
    case New::LVXL:
      LoadSize = 16;
      break;
    }
    
    if (isLoadOfStoredAddress(LoadSize, 
                              Node->getOperand(0), Node->getOperand(1)))
      return NoopHazard;
  }
  
  return NoHazard;
}

void NewHazardRecognizer970::EmitInstruction(SUnit *SU) {
  const SDNode *Node = SU->getNode()->getFlaggedMachineNode();
  bool isFirst, isSingle, isCracked, isLoad, isStore;
  NewII::New970_Unit InstrType = 
    GetInstrType(Node->getOpcode(), isFirst, isSingle, isCracked,
                 isLoad, isStore);
  if (InstrType == NewII::New970_Pseudo) return;  
  unsigned Opcode = Node->getMachineOpcode();

  // Update structural hazard information.
  if (Opcode == New::MTCTR) HasCTRSet = true;
  
  // Track the address stored to.
  if (isStore) {
    unsigned ThisStoreSize;
    switch (Opcode) {
    default: llvm_unreachable("Unknown store instruction!");
    case New::STB:    case New::STB8:
    case New::STBU:   case New::STBU8:
    case New::STBX:   case New::STBX8:
    case New::STVEBX:
      ThisStoreSize = 1;
      break;
    case New::STH:    case New::STH8:
    case New::STHU:   case New::STHU8:
    case New::STHX:   case New::STHX8:
    case New::STVEHX:
    case New::STHBRX:
      ThisStoreSize = 2;
      break;
    case New::STFS:
    case New::STFSU:
    case New::STFSX:
    case New::STWX:   case New::STWX8:
    case New::STWUX:
    case New::STW:    case New::STW8:
    case New::STWU:
    case New::STVEWX:
    case New::STFIWX:
    case New::STWBRX:
      ThisStoreSize = 4;
      break;
    case New::STD_32:
    case New::STDX_32:
    case New::STD:
    case New::STDU:
    case New::STFD:
    case New::STFDX:
    case New::STDX:
    case New::STDUX:
      ThisStoreSize = 8;
      break;
    case New::STVX:
    case New::STVXL:
      ThisStoreSize = 16;
      break;
    }
    
    StoreSize[NumStores] = ThisStoreSize;
    StorePtr1[NumStores] = Node->getOperand(1);
    StorePtr2[NumStores] = Node->getOperand(2);
    ++NumStores;
  }
  
  if (InstrType == NewII::New970_BRU || isSingle)
    NumIssued = 4;  // Terminate a d-group.
  ++NumIssued;
  
  // If this instruction is cracked into two ops by the decoder, remember that
  // we issued two pieces.
  if (isCracked)
    ++NumIssued;
  
  if (NumIssued == 5)
    EndDispatchGroup();
}

void NewHazardRecognizer970::AdvanceCycle() {
  assert(NumIssued < 5 && "Illegal dispatch group!");
  ++NumIssued;
  if (NumIssued == 5)
    EndDispatchGroup();
}
