//===- New.td - Describe the New Target Machine --------*- tablegen -*-===//
// 
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// 
//===----------------------------------------------------------------------===//
//
// This is the top level entry point for the New target.
//
//===----------------------------------------------------------------------===//

// Get the target-independent interfaces which we are implementing.
//
include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// New Subtarget features.
//
 
//===----------------------------------------------------------------------===//
// CPU Directives                                                             //
//===----------------------------------------------------------------------===//

def Directive601 : SubtargetFeature<"", "DarwinDirective", "New::DIR_601", "">;
def Directive602 : SubtargetFeature<"", "DarwinDirective", "New::DIR_602", "">;
def Directive603 : SubtargetFeature<"", "DarwinDirective", "New::DIR_603", "">;
def Directive604 : SubtargetFeature<"", "DarwinDirective", "New::DIR_603", "">;
def Directive620 : SubtargetFeature<"", "DarwinDirective", "New::DIR_603", "">;
def Directive7400: SubtargetFeature<"", "DarwinDirective", "New::DIR_7400", "">;
def Directive750 : SubtargetFeature<"", "DarwinDirective", "New::DIR_750", "">;
def Directive970 : SubtargetFeature<"", "DarwinDirective", "New::DIR_970", "">;
def Directive32  : SubtargetFeature<"", "DarwinDirective", "New::DIR_32", "">;
def Directive64  : SubtargetFeature<"", "DarwinDirective", "New::DIR_64", "">;

def Feature64Bit     : SubtargetFeature<"64bit","Has64BitSupport", "true",
                                        "Enable 64-bit instructions">;
def Feature64BitRegs : SubtargetFeature<"64bitregs","Use64BitRegs", "true",
                              "Enable 64-bit registers usage for ppc32 [beta]">;
def FeatureAltivec   : SubtargetFeature<"altivec","HasAltivec", "true",
                                        "Enable Altivec instructions">;
def FeatureGPUL      : SubtargetFeature<"gpul","IsGigaProcessor", "true",
                                        "Enable GPUL instructions">;
def FeatureFSqrt     : SubtargetFeature<"fsqrt","HasFSQRT", "true",
                                        "Enable the fsqrt instruction">; 
def FeatureSTFIWX    : SubtargetFeature<"stfiwx","HasSTFIWX", "true",
                                        "Enable the stfiwx instruction">; 

//===----------------------------------------------------------------------===//
// Register File Description
//===----------------------------------------------------------------------===//

include "NewRegisterInfo.td"
include "NewSchedule.td"
include "NewInstrInfo.td"



//===----------------------------------------------------------------------===//
// GPU instruction set (parts that are not included in New)
//===----------------------------------------------------------------------===//

include "NewBranches.td"


//===----------------------------------------------------------------------===//
// New processors supported.
//

def : Processor<"generic", G3Itineraries, [Directive32]>;
def : Processor<"601", G3Itineraries, [Directive601]>;
def : Processor<"602", G3Itineraries, [Directive602]>;
def : Processor<"603", G3Itineraries, [Directive603]>;
def : Processor<"603e", G3Itineraries, [Directive603]>;
def : Processor<"603ev", G3Itineraries, [Directive603]>;
def : Processor<"604", G3Itineraries, [Directive604]>;
def : Processor<"604e", G3Itineraries, [Directive604]>;
def : Processor<"620", G3Itineraries, [Directive620]>;
def : Processor<"g3", G3Itineraries, [Directive7400]>;
def : Processor<"7400", G4Itineraries, [Directive7400, FeatureAltivec]>;
def : Processor<"g4", G4Itineraries, [Directive7400, FeatureAltivec]>;
def : Processor<"7450", G4PlusItineraries, [Directive7400, FeatureAltivec]>;
def : Processor<"g4+", G4PlusItineraries, [Directive750, FeatureAltivec]>;
def : Processor<"750", G4Itineraries, [Directive750, FeatureAltivec]>;
def : Processor<"970", G5Itineraries,
                  [Directive970, FeatureAltivec,
                   FeatureGPUL, FeatureFSqrt, FeatureSTFIWX,
                   Feature64Bit /*, Feature64BitRegs */]>;
def : Processor<"g5", G5Itineraries,
                  [Directive970, FeatureAltivec,
                   FeatureGPUL, FeatureFSqrt, FeatureSTFIWX,
                   Feature64Bit /*, Feature64BitRegs */]>;
def : Processor<"ppc", G3Itineraries, [Directive32]>;
def : Processor<"ppc64", G5Itineraries,
                  [Directive64, FeatureAltivec,
                   FeatureGPUL, FeatureFSqrt, FeatureSTFIWX,
                   Feature64Bit /*, Feature64BitRegs */]>;


//===----------------------------------------------------------------------===//
// Calling Conventions
//===----------------------------------------------------------------------===//

include "NewCallingConv.td"

def NewInstrInfo : InstrInfo {
  let isLittleEndianEncoding = 1;
}


def New : Target {
  // Information about the instructions.
  let InstructionSet = NewInstrInfo;
}
