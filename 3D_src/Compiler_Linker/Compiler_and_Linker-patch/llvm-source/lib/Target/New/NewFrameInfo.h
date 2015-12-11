//===-- NewFrameInfo.h - Define TargetFrameInfo for PowerPC -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#ifndef POWERPC_FRAMEINFO_H
#define POWERPC_FRAMEINFO_H

#include "New.h"
#include "NewSubtarget.h"
#include "llvm/Target/TargetFrameInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/ADT/STLExtras.h"

namespace llvm {

class NewFrameInfo: public TargetFrameInfo {
  const TargetMachine &TM;

public:
  NewFrameInfo(const TargetMachine &tm, bool LP64)
    : TargetFrameInfo(TargetFrameInfo::StackGrowsDown, 16, 0), TM(tm) {
  }

  /// getReturnSaveOffset - Return the previous frame offset to save the
  /// return address.
  static unsigned getReturnSaveOffset(bool isNew64, bool isDarwinABI) {
    if (isDarwinABI)
      return isNew64 ? 16 : 8;
    // SVR4 ABI:
    return isNew64 ? 16 : 4;
  }

  /// getFramePointerSaveOffset - Return the previous frame offset to save the
  /// frame pointer.
  static unsigned getFramePointerSaveOffset(bool isNew64, bool isDarwinABI) {
    // For the Darwin ABI:
    // We cannot use the TOC save slot (offset +20) in the PowerPC linkage area
    // for saving the frame pointer (if needed.)  While the published ABI has
    // not used this slot since at least MacOSX 10.2, there is older code
    // around that does use it, and that needs to continue to work.
    if (isDarwinABI)
      return isNew64 ? -8U : -4U;
    
    // SVR4 ABI: First slot in the general register save area.
    return isNew64 ? -8U : -4U;
  }
  
  /// getLinkageSize - Return the size of the PowerPC ABI linkage area.
  ///
  static unsigned getLinkageSize(bool isNew64, bool isDarwinABI) {
    if (isDarwinABI || isNew64)
      return 6 * (isNew64 ? 8 : 4);
    
    // SVR4 ABI:
    return 8;
  }

  /// getMinCallArgumentsSize - Return the size of the minium PowerPC ABI
  /// argument area.
  static unsigned getMinCallArgumentsSize(bool isNew64, bool isDarwinABI) {
    // For the Darwin ABI / 64-bit SVR4 ABI:
    // The prolog code of the callee may store up to 8 GPR argument registers to
    // the stack, allowing va_start to index over them in memory if its varargs.
    // Because we cannot tell if this is needed on the caller side, we have to
    // conservatively assume that it is needed.  As such, make sure we have at
    // least enough stack space for the caller to store the 8 GPRs.
    if (isDarwinABI || isNew64)
      return 8 * (isNew64 ? 8 : 4);
    
    // 32-bit SVR4 ABI:
    // There is no default stack allocated for the 8 first GPR arguments.
    return 0;
  }

  /// getMinCallFrameSize - Return the minimum size a call frame can be using
  /// the PowerPC ABI.
  static unsigned getMinCallFrameSize(bool isNew64, bool isDarwinABI) {
    // The call frame needs to be at least big enough for linkage and 8 args.
    return getLinkageSize(isNew64, isDarwinABI) +
           getMinCallArgumentsSize(isNew64, isDarwinABI);
  }

  // With the SVR4 ABI, callee-saved registers have fixed offsets on the stack.
  const SpillSlot *
  getCalleeSavedSpillSlots(unsigned &NumEntries) const {
    if (TM.getSubtarget<NewSubtarget>().isDarwinABI()) {
      NumEntries = 1;
      if (TM.getSubtarget<NewSubtarget>().isNew64()) {
        static const SpillSlot darwin64Offsets = {New::X31, -8};
        return &darwin64Offsets;
      } else {
        static const SpillSlot darwinOffsets = {New::R31, -4};
        return &darwinOffsets;
      }
    }

    // Early exit if not using the SVR4 ABI.
    if (!TM.getSubtarget<NewSubtarget>().isSVR4ABI()) {
      NumEntries = 0;
      return 0;
    }

    static const SpillSlot Offsets[] = {
      // Floating-point register save area offsets.
      /*{New::F31, -8},
      {New::F30, -16},
      {New::F29, -24},
      {New::F28, -32},
      {New::F27, -40},
      {New::F26, -48},
      {New::F25, -56},
      {New::F24, -64},*/
      {New::F23, -72},
      {New::F22, -80},
      {New::F21, -88},
      {New::F20, -96},
      {New::F19, -104},
      {New::F18, -112},
      {New::F17, -120},
      {New::F16, -128},
      {New::F15, -136},
      {New::F14, -144},

      // General register save area offsets.
      /*{New::R31, -4},
      {New::R30, -8},
      {New::R29, -12},
      {New::R28, -16},
      {New::R27, -20},
      {New::R26, -24},
      {New::R25, -28},*/

      /*      {New::R24, -32},
      {New::R23, -36},
      {New::R22, -40},
      {New::R21, -44},
      {New::R20, -48},
      {New::R19, -52},
      {New::R18, -56},
      {New::R17, -60},
      {New::R16, -64},
      {New::R15, -68},*/
      //      {New::R14, -72},
      {New::R14, -32},

      // CR save area offset.
      // FIXME SVR4: Disable CR save area for now.
//      {New::CR2, -4},
//      {New::CR3, -4},
//      {New::CR4, -4},
//      {New::CR2LT, -4},
//      {New::CR2GT, -4},
//      {New::CR2EQ, -4},
//      {New::CR2UN, -4},
//      {New::CR3LT, -4},
//      {New::CR3GT, -4},
//      {New::CR3EQ, -4},
//      {New::CR3UN, -4},
//      {New::CR4LT, -4},
//      {New::CR4GT, -4},
//      {New::CR4EQ, -4},
//      {New::CR4UN, -4},

      // VRSAVE save area offset.
      {New::VRSAVE, -4},

      // Vector register save area
      /*{New::V31, -16},
      {New::V30, -32},
      {New::V29, -48},
      {New::V28, -64},
      {New::V27, -80},
      {New::V26, -96},
      {New::V25, -112},
      {New::V24, -128},*/
      {New::V23, -144},
      {New::V22, -160},
      {New::V21, -176},
      {New::V20, -192}
    };

    static const SpillSlot Offsets64[] = {
      // Floating-point register save area offsets.
      /*{New::F31, -8},
      {New::F30, -16},
      {New::F29, -24},
      {New::F28, -32},
      {New::F27, -40},
      {New::F26, -48},
      {New::F25, -56},
      {New::F24, -64},*/
      {New::F23, -72},
      {New::F22, -80},
      {New::F21, -88},
      {New::F20, -96},
      {New::F19, -104},
      {New::F18, -112},
      {New::F17, -120},
      {New::F16, -128},
      {New::F15, -136},
      {New::F14, -144},

      // General register save area offsets.
      // FIXME 64-bit SVR4: Are 32-bit registers actually allocated in 64-bit
      //                    mode?
      /*{New::R31, -4},
      {New::R30, -12},
      {New::R29, -20},
      {New::R28, -28},
      {New::R27, -36},
      {New::R26, -44},
      {New::R25, -52},*/

      /*      {New::R24, -60},
      {New::R23, -68},
      {New::R22, -76},
      {New::R21, -84},
      {New::R20, -92},
      {New::R19, -100},
      {New::R18, -108},
      {New::R17, -116},
      {New::R16, -124},
      {New::R15, -132},*/
      //{New::R14, -140},
      {New::R14, -60},

      /*{New::X31, -8},
      {New::X30, -16},
      {New::X29, -24},
      {New::X28, -32},
      {New::X27, -40},
      {New::X26, -48},
      {New::X25, -56},*/

      /*      {New::X24, -64},
      {New::X23, -72},
      {New::X22, -80},
      {New::X21, -88},
      {New::X20, -96},
      {New::X19, -104},
      {New::X18, -112},
      {New::X17, -120},
      {New::X16, -128},
      {New::X15, -136},*/
      //      {New::X14, -144},
      {New::X14, -60},

      // CR save area offset.
      // FIXME SVR4: Disable CR save area for now.
//      {New::CR2, -4},
//      {New::CR3, -4},
//      {New::CR4, -4},
//      {New::CR2LT, -4},
//      {New::CR2GT, -4},
//      {New::CR2EQ, -4},
//      {New::CR2UN, -4},
//      {New::CR3LT, -4},
//      {New::CR3GT, -4},
//      {New::CR3EQ, -4},
//      {New::CR3UN, -4},
//      {New::CR4LT, -4},
//      {New::CR4GT, -4},
//      {New::CR4EQ, -4},
//      {New::CR4UN, -4},

      // VRSAVE save area offset.
      {New::VRSAVE, -4},

      // Vector register save area
      /*{New::V31, -16},
      {New::V30, -32},
      {New::V29, -48},
      {New::V28, -64},
      {New::V27, -80},
      {New::V26, -96},
      {New::V25, -112},
      {New::V24, -128},*/
      {New::V23, -144},
      {New::V22, -160},
      {New::V21, -176},
      {New::V20, -192}
    };

    if (TM.getSubtarget<NewSubtarget>().isNew64()) {
      NumEntries = array_lengthof(Offsets64);

      return Offsets64;
    } else {
      NumEntries = array_lengthof(Offsets);

      return Offsets;
    }
  }
};

} // End llvm namespace

#endif
