#include "systemc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "handshake.h"

#ifndef VS_H
#define VS_H

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

class SC_VS : public sc_module
{
  public:
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
    fp_Result      = fopen("./file/VS_Register.txt" , "w");
    V_O            = fopen("./file/VS_VOB.txt","w");
    log_slave      = fopen("./file/log.VS_port.txt","w");//for Debug

    
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
  int branch[4];
  int total_v; //vertex number
  int total_a; //attribute number
  int v_num;       //vertex number 
  int v_entry_num; //entry number
  int v_word_num;  //offset number
  //-------------------memories and register-------------------------------------
  float Ex_MEM[256][4][4];//[v_num][entry 0~3][component xyzw]
  float VAB[8][4], VOB[8][4];
  float GPR[32][4];
  char  IM[256][65];
  float CMEM[50][4];
  int   VST[15];
  
  int Imm; // variable for LD/ST/MVI displacement  
  int Imm_float_sign; // variable for MVI displacement
  //------------------temp-------------------------------------------------------
  float Rs_swiz_temp[4], Rt_swiz_temp[4], Rd_swiz_temp[4];
  float dsq_temp, div_temp;
  int   LD_addr;
  int   ST_addr;
  
  float mvi_temp_FL[4];
  int   mvi_temp_FX[4];
  
  float min_temp_FL[4];
  int   min_temp_FX[4];
  
  float max_temp_FL[4];
  int   max_temp_FX[4];
  
  float add_temp_FL[4];
  int   add_temp_FX[4];
  
  float mulh_temp[4];
  int   mull_temp[4];
  long int mull_tmp64b[4];
  
  float mad_temp_FL[4];
  int   mad_temp_FX[4];
  
  float dp_temp_FL;
  int   dp_temp_FX;
  
  int   inv_temp[4];
  int   and_temp[4];
  float    or_temp[4];
  int   xor_temp[4];
  
  float R1_temp_FL[4], R2_temp_FL[4], R3_temp_FL[4], R4_temp_FL[4];
  int   R1_temp_FX[4], R2_temp_FX[4], R3_temp_FX[4], R4_temp_FX[4];
  
  int  shrl_temp[4];
  //-----------------------------------------------------------------------
  struct decoder
  {
    int op;
    int NOP_stall_cycle;
    int Rd;
    int Rs;
    int Rt;
    int R1;
    int R2;
    int Addr;
    int R3;
    int R4;
    int Sw_d;
    int Sw_d_x;
    int Sw_d_y;
    int Sw_d_z;
    int Sw_d_w;
    int Sw_s_x;
    int Sw_s_y;
    int Sw_s_z;
    int Sw_s_w;
    int Sw_t_x;
    int Sw_t_y;
    int Sw_t_z;
    int Sw_t_w;
    int Neg_Rs;
    int Neg_Rt;
    int Neg_Rd;
    int R_L;
    int WM_x;
    int WM_y;
    int WM_z;
    int WM_w;
    int SCC_x;//single component compare 
    int SCC_y;//single component compare
    int SCC_z;//single component compare
    int SCC_w;//single component compare
    int FXL;
    int S_V;
  		
  }IF;
};

#endif
