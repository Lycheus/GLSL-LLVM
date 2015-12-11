#ifndef __SC_LINKER_INSTRUCTION_H__
#define __SC_LINKER_INSTRUCTION_H__

#define OPCODE_SIZE 5
#define OPCODE_BIT 59
#define RD_SIZE 6
#define RD_BIT 53
#define RS_SIZE 6
#define RS_BIT 47
#define RT_SIZE 6
#define RT_BIT 41
#define R3_SIZE 6
#define R3_BIT 25
#define R4_SIZE 5
#define R4_BIT 20
#define OFFSET_SIZE 16
#define OFFSET_BIT 31
#define SA_SIZE 6
#define SA_BIT 41
#define WM_SIZE 2
#define WM_BIT 10
#define SCC_BIT 10
#define IMM_SIZE 32
#define IMM_BIT 15
#define F_SIZE 1
#define F_BIT 9
#define SV_SIZE 2
#define SV_BIT 7
#define SW_RD_SIZE 8
#define SW_RD_BIT 33
#define SW_RS_SIZE 8
#define SW_RS_BIT 25
#define SW_RT_SIZE 8
#define SW_RT_BIT 17
#define BR_SW_R1_BIT 23
#define BR_SW_R2_BIT 15
#define NE_SIZE 1
#define NE_BIT 14
#define VECTOR_SCALAR_BIT 14
#define VECTOR_SCALAR_SIZE 1
#define NE_SHIFT_SIZE 2
#define NE_SHIFT_BIT 15
#define RL_SIZE 1
#define RL_BIT 14
#define REGISTER_BIT 6
#define SWIZZLE_BIT 8
#define NOP_BIT 55
#define RED_DEFAULT -1
#define DEFAULT -1
#define BRANCH_WRITE_FRONT_REGISTER 1
#define BRANCH_WRITE_LATER_REGISTER 2
#define NO_MEANING 0
#define FLOAT 1
#define BRANCH_IMM 2

struct all_element
{
  char opcode[8];
  int opcode_value;
  int regi[4];
  int sampler;
  int swizzle[3];
  int neg;
  int wm;
  int scc;
  int vector_scalar;
  int fxfl;
  int b_vs;
  int sv;
  int sa;
  int rl;
  int change_imm;
  unsigned int imm;
  int address;
  int nop_number;
  int interface_register;
};


#endif
