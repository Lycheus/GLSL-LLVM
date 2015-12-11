let isBranch = 1, isTerminator = 1, hasCtrlDep = 1 in {
//fixed by sheng-chih
//For our target GPU, vertex conditional instructions has no opposite property.
//So we follow the original conditional branch.
//Ex:
//           gpu_beq  R4, R5, BB2
//      BB1:
//           ...
//      BB2:
//           ...
//  ==>
//           gpu_bne  R4, R5, 2    (If true, jump 2 instructions)
//           B BB2
//      BB1:
//           ...
//      BB2:
//           ...
//This part do:
//           gpu_bne  R4, R5, BB2
//  ==>            
//           gpu_bne  R4, R5, 2
//           B BB2
//

//  def GPU_BCC : BForm<16, 0, 0, (outs), (ins GPRC:$A, GPRC:$B, gpu_pred:$code, target:$dst),
//                  "gpu_b${code:cc} $A, $B, $dst, <1>">;
//  def GPU_BCC_I8 : BForm<16, 0, 0, (outs), (ins G8RC:$A, G8RC:$B, gpu_pred:$code, target:$dst),
//                   "gpu_b${code:cc} $A, $B, $dst, <1>">;
//  def GPU_BCC_F4 : BForm<16, 0, 0, (outs), (ins F4RC:$A, F4RC:$B, gpu_pred:$code, target:$dst),
//                   "gpu_b${code:cc}f $A, $B, $dst, <1>">;
//  def GPU_BCC_F8 : BForm<16, 0, 0, (outs), (ins F8RC:$A, F8RC:$B, gpu_pred:$code, target:$dst),
//                 "gpu_b${code:cc}f_64 $A, $B, $dst, <1>">;

  def GPU_BCC : BForm<16, 0, 0, (outs), (ins GPRC:$A, GPRC:$B, gpu_pred:$code, target:$dst),
                  "gpu_b${code:cc} $A, $B, 2, <1>\n\tB $dst">;

  def GPU_BCC_I8 : BForm<16, 0, 0, (outs), (ins G8RC:$A, G8RC:$B, gpu_pred:$code, target:$dst),
                   "gpu_b${code:cc} $A, $B, 2, <1>\n\tB $dst">;

  def GPU_BCC_F4 : BForm<16, 0, 0, (outs), (ins F4RC:$A, F4RC:$B, gpu_pred:$code, target:$dst),
                   "gpu_b${code:cc}f $A, $B, 2, <1>\n\tB $dst">;
		   
  def GPU_BCC_F8 : BForm<16, 0, 0, (outs), (ins F8RC:$A, F8RC:$B, gpu_pred:$code, target:$dst),
                   "gpu_b${code:cc}f_64 $A, $B, 2, <1>\n\tB $dst">;

//end fixed
                   
}

//let usesCustomDAGSchedInserter = 1,    // Expanded by the scheduler.
let usesCustomInserter = 1,    // Expanded by the scheduler.
    New970_Single = 1 in {
  def GPU_SELECT_CC_I4 : Pseudo<(outs GPRC:$dst), (ins GPRC:$A, GPRC:$B, GPRC:$T, GPRC:$F,
                              i32imm:$BROPC), "${:comment} GPU_SELECT_CC PSEUDO!",
                              []>;
  def GPU_SELECT_CC_I8 : Pseudo<(outs GPRC:$dst), (ins G8RC:$A, G8RC:$B, GPRC:$T, GPRC:$F,
                             i32imm:$BROPC), "${:comment} SELECT_CC PSEUDO!",
                             []>;
  def GPU_SELECT_CC_F4  : Pseudo<(outs GPRC:$dst), (ins F4RC:$A, F4RC:$B, GPRC:$T, GPRC:$F,
                             i32imm:$BROPC), "${:comment} SELECT_CC PSEUDO!",
                             []>;
  def GPU_SELECT_CC_F8  : Pseudo<(outs GPRC:$dst), (ins F8RC:$A, F8RC:$B, GPRC:$T, GPRC:$F,
                             i32imm:$BROPC), "${:comment} SELECT_CC PSEUDO!",
                             []>;
  // def SELECT_CC_VRRC: Pseudo<(outs VRRC:$dst), (ins CRRC:$cond, VRRC:$T, VRRC:$F,
  //                             i32imm:$BROPC), "${:comment} SELECT_CC PSEUDO!",
  //                             []>;
}

