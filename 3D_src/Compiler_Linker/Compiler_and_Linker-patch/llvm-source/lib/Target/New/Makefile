##===- lib/Target/New/Makefile -------------------------*- Makefile -*-===##
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
##===----------------------------------------------------------------------===##

LEVEL = ../../..
LIBRARYNAME = LLVMNewCodeGen
TARGET = New

# Make sure that tblgen is run, first thing.
BUILT_SOURCES = NewGenInstrNames.inc NewGenRegisterNames.inc \
                NewGenAsmWriter.inc  NewGenCodeEmitter.inc \
                NewGenRegisterInfo.h.inc NewGenRegisterInfo.inc \
                NewGenInstrInfo.inc NewGenDAGISel.inc \
                NewGenSubtarget.inc NewGenCallingConv.inc

DIRS = AsmPrinter TargetInfo

include $(LEVEL)/Makefile.common
