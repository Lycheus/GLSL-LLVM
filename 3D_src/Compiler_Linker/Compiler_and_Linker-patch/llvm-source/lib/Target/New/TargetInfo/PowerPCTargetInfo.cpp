//===-- NewTargetInfo.cpp - New Target Implementation -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "New.h"
#include "llvm/Module.h"
#include "llvm/Target/TargetRegistry.h"
using namespace llvm;

Target llvm::TheNew32Target, llvm::TheNew64Target;

extern "C" void LLVMInitializeNewTargetInfo() { 
  RegisterTarget<Triple::ppc, /*HasJIT=*/true>
    X(TheNew32Target, "newppc32", "New 32");

  RegisterTarget<Triple::ppc64, /*HasJIT=*/true>
    Y(TheNew64Target, "newppc64", "New 64");
}
