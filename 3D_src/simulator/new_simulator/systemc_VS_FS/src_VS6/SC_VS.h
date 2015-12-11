//#define QEMU_SC

#ifdef QEMU_SC
#include "../src_FS/FS.h"
extern FS *FS_QEMU;
#endif

#include "systemc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "handshake.h"


#define IM_offset   0
#define CMEM_offset 9000
#define VST_offset  11000
#define VAB_offset  12000
#define en_offset   43000
//#define en_offset   85000

#ifndef VS_H
#define VS_H

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

class SC_VS : public sc_module
{
  public:
  sc_event	VS_en;
  sc_event	FS_en;
  sc_int<32>	VST[9];    
  //I/O port //CYLin
  sc_fifo_out<float> mp_VOB;
  sc_out<bool>       mp_VS_END;
  //Target port
  sc_fifo_in<Req>   sp_transfer;
  sc_fifo_out<bool> sp_response;
  //Initiator port
  sc_fifo_out<Req>  mp_transfer;
  sc_fifo_in<bool>  mp_response;
    		
  SC_HAS_PROCESS(SC_VS);
  SC_VS(sc_module_name name):sc_module(name)
  {    
    SC_THREAD(access);
    SC_THREAD(VScore);
    /*SC_METHOD(VScore);
      sensitive << flag_VScore;
      dont_initialize();*/
    		   
    //fp_Instruction = fopen("./file/Transformation.txt" , "r");
    //fp_Uniform     = fopen("./file/Uniform.txt" , "r");
    //fp_VST         = fopen("./file/VST.txt" , "r");
    //fp_Vertex      = fopen("./file/Vertex_Data.txt" , "r");
    fp_Result      = fopen("./output/VS_Register.txt" , "w");
    V_O            = fopen("./output/VS_VOB.txt","w");
    log_slave      = fopen("./output/log.VS_port.txt","w");//for Debug

    
    im_count = 0;

        
  };
  ~SC_VS()
  {
    //fclose( fp_Instruction );
    //fclose( fp_Vertex );
    //fclose( fp_Uniform );
    //fclose( fp_VST );
    fclose(fp_Result );
    fclose(V_O);
    fclose(log_slave);
  };
  
  private:
  //SC Process
  void access(); 
  void VScore();


  void binarytohex();
  void bintohex();
  void chartoint();
  void chatoint();
  void decode(int program_counter);
  void DecTofloat(float a);
  void get_fix(int n);
  void get_float(float a);
  void itobs(int n);

  // Instuction Function
  void VS_01_DSQ();  
  void VS_02_RCP();
  void VS_03_RSQ();
  void VS_04_LOG2();
  void VS_05_POW2();
  void VS_06_DIV();
  void VS_07_MVR();
  void VS_08_LDV();
  void VS_09_STV();
  void VS_10_MVI();
  void VS_11_MIN();
  void VS_12_MAX();
  void VS_13_ITOF_FTOI();
  void VS_14_FIN();
  void VS_16_ADD();
  void VS_17_MULF();
  void VS_18_MAD();
  void VS_19_DP();
  void VS_20_INV();
  void VS_21_AND();
  void VS_22_OR();
  void VS_23_XOR();
  void VS_24_BEQ();
  void VS_25_BNEQ();
  void VS_26_BLT();
  void VS_27_BGT();
  void VS_28_MULX();
  void VS_29_CAL();
  void VS_30_SHRL();
  void VS_31_NOP();

  //SC variable
  sc_event flag_VScore,flag_end;
  //for initiator port
  Req info_mp;
  //for Target port
  Req info_sp;
  unsigned int *DataAddr;
  unsigned int Addr;
  bool Type;
  int Burst;

  //---------------------------------------
  //variable declaration
  //---------------------------------------    
  //FILE *fp_Instruction, *fp_Uniform, *fp_VST, *fp_Vertex ;
  FILE *fp_Result,*V_O,*log_slave;
  
  float a;
  char word[32], temp[32];  
  int hex[8];  
  int hx[8];  
  int n;
  char c[8 * sizeof(int)], s[8 * sizeof(int)];
  int im_count;
  int vertex_count; 
  
  int i, j, k;
  int PC;
  unsigned int branch_taken[4];
  int total_v; //vertex number
  int total_a; //attribute number
  int v_num;       //vertex number 
  int v_entry_num; //entry number
  int v_word_num;  //offset number
  //-------------------memories and register-------------------------------------
  //float Ex_MEM[256][4][4];//[v_num][entry 0~3][component xyzw]
  //sheng-chang modifyed
  unsigned int Ex_MEM[2880][4][4];//[v_num][entry 0~3][component xyzw]
  unsigned int VAB[8][4], VOB[8][4];
  unsigned int GPR[32][4];
  //fixed by sheng-chih
  char  IM[8192][65];
  //char  IM[256][65];
  unsigned int CMEM[4500][4];
  //float CMEM[50][4];
  //end fixed
  //int   VST[15];
  
  int Imm_FX; // variable for LD/ST/MVI displacement  
  float Imm_FL; // variable for LD/ST/MVI displacement  
  //int Imm_float_sign; // variable for MVI displacement
  //------------------temp-------------------------------------------------------
  unsigned int Rs_SV_temp[4], Rt_SV_temp[4], Rd_SV_temp[4], Rs_swiz_temp[4], Rt_swiz_temp[4], Rd_swiz_temp[4];
  float Rs_swiz_temp_FL[4], Rt_swiz_temp_FL[4], Rd_swiz_temp_FL[4];
  int Rs_swiz_temp_FX[4], Rt_swiz_temp_FX[4], Rd_swiz_temp_FX[4];

  // Special Function temp
  unsigned int dsq_temp, rcp_temp, rsq_temp, log2_temp, pow2_temp, div_temp;
  float dsq_temp_FL, rcp_temp_FL, rsq_temp_FL, log2_temp_FL, pow2_temp_FL, div_temp_FL;

  // MVR temp
  unsigned int mvr_temp[4];
  float mvr_temp_FL[4];

  // LDV/STV
  int   LD_addr;
  int   ST_addr;
  unsigned int ldv_temp[4];
  unsigned int stv_temp[4];

  // MVI
  unsigned int mvi_temp[4];
  float mvi_temp_FL[4];
  int   mvi_temp_FX[4];
  
  // MIN MAX
  unsigned int min_temp[4];
  float min_temp_FL[4];
  int   min_temp_FX[4];  
  unsigned int max_temp[4];
  float max_temp_FL[4];
  int   max_temp_FX[4];

  // ITOF, FTOI
  unsigned int itof_ftoi_temp[4];
  float itof_temp_FL[4];
  int   ftoi_temp_FX[4];
  
  // ADD
  unsigned int add_temp[4];
  float add_temp_FL[4];
  int   add_temp_FX[4];
  
  // MULF
  unsigned int mulf_temp[4];
  float mulf_temp_FL[4];

  float mulh_temp[4];
  int   mull_temp[4];
  long int mull_tmp64b[4];
  
  // MAD
  unsigned int mad_temp[4];
  float mad_temp_FL[4];
  int   mad_temp_FX[4];
  
  // DP
  unsigned int dp_temp;
  float dp_temp_FL;
  int   dp_temp_FX;
  
  // INV, AND, OR, XOR
  unsigned int inv_temp[4];
  unsigned int and_temp[4];
  unsigned int or_temp[4];
  unsigned int xor_temp[4];
  
  // BEQ, BNEQ, BLT, BGT
  unsigned int R1_SV_temp[4], R2_SV_temp[4], R3_SV_temp[4], R4_SV_temp[4], R1_temp[4], R2_temp[4], R3_temp[4], R4_temp[4];
  float R1_temp_FL[4], R2_temp_FL[4], R3_temp_FL[4], R4_temp_FL[4];
  int   R1_temp_FX[4], R2_temp_FX[4], R3_temp_FX[4], R4_temp_FX[4];

  // MULX
  unsigned int mulx_temp_H[4];
  unsigned int mulx_temp_L[4];
  long mulx64_temp_FX[4];

  // CAL
  unsigned int PC_temp;
  
  //SHRL
  unsigned int shrl_temp[4];
  unsigned int shrl_temp_L[4];
  int  shrl_temp_A[4];
  //-----------------------------------------------------------------------
  struct decoder
  {
    unsigned int op;
    unsigned int NOP_stall_cycle;
    unsigned int Rd;
	unsigned int Rd2;
    unsigned int Rs;
    unsigned int Rt;
    unsigned int R1;
    unsigned int R2;
    unsigned int Addr;
    unsigned int R3;
    unsigned int R4;
    //unsigned int Sw_d;
    unsigned int Sw_d_x;
    unsigned int Sw_d_y;
    unsigned int Sw_d_z;
    unsigned int Sw_d_w;
    unsigned int Sw_s_x;
    unsigned int Sw_s_y;
    unsigned int Sw_s_z;
    unsigned int Sw_s_w;
    unsigned int Sw_t_x;
    unsigned int Sw_t_y;
    unsigned int Sw_t_z;
    unsigned int Sw_t_w;
    unsigned int Neg_Rs;
    unsigned int Neg_Rt;
    unsigned int Neg_Rd;
    unsigned int R_L;
    unsigned int WM_x;
    unsigned int WM_y;
    unsigned int WM_z;
    unsigned int WM_w;
    unsigned int SCC_x;//single component compare 
    unsigned int SCC_y;//single component compare
    unsigned int SCC_z;//single component compare
    unsigned int SCC_w;//single component compare
    unsigned int FXL;
	unsigned int L_A;
    unsigned int Br_m; //Branch MODE
    unsigned int S_V;
	unsigned int Imm;  		
  }IF;
};

#endif
