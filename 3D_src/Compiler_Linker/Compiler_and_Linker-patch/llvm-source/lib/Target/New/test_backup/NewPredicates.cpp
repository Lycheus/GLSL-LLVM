//===-- NewPredicates.cpp - New Branch Predicate Information --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the PowerPC branch predicates.
//
//===----------------------------------------------------------------------===//

#include "NewPredicates.h"
#include "llvm/Support/ErrorHandling.h"
#include <cassert>
using namespace llvm;

New::Predicate New::InvertPredicate(New::Predicate Opcode) {
  switch (Opcode) {
  default: llvm_unreachable("Unknown New branch opcode!");
  case New::PRED_EQ: return New::PRED_NE;
  case New::PRED_NE: return New::PRED_EQ;
  case New::PRED_LT: return New::PRED_GE;
  case New::PRED_GE: return New::PRED_LT;
  case New::PRED_GT: return New::PRED_LE;
  case New::PRED_LE: return New::PRED_GT;
  case New::PRED_NU: return New::PRED_UN;
  case New::PRED_UN: return New::PRED_NU;
  }
}
