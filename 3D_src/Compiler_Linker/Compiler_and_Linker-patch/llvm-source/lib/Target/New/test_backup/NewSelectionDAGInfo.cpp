//===-- NewSelectionDAGInfo.cpp - PowerPC SelectionDAG Info ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the NewSelectionDAGInfo class.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "powerpc-selectiondag-info"
#include "NewTargetMachine.h"
using namespace llvm;

NewSelectionDAGInfo::NewSelectionDAGInfo(const NewTargetMachine &TM)
  : TargetSelectionDAGInfo(TM) {
}

NewSelectionDAGInfo::~NewSelectionDAGInfo() {
}
