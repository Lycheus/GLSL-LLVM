//=====-- NewMCAsmInfo.h - New asm properties -----------------*- C++ -*--====//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the MCAsmInfoDarwin class.
//
//===----------------------------------------------------------------------===//

#ifndef NewTARGETASMINFO_H
#define NewTARGETASMINFO_H

#include "llvm/MC/MCAsmInfoDarwin.h"

namespace llvm {

  struct NewMCAsmInfoDarwin : public MCAsmInfoDarwin {
    explicit NewMCAsmInfoDarwin(bool is64Bit);
  };

  struct NewLinuxMCAsmInfo : public MCAsmInfo {
    explicit NewLinuxMCAsmInfo(bool is64Bit);
  };

} // namespace llvm

#endif
