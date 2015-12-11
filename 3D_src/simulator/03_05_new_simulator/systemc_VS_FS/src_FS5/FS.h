//#define QEMU_SC
/*
 * FS.h
 *
 *  Created on: 2009/12/1
 *      Author: Jason
 */
#include <stdio.h>
#include <iostream>
#include <systemc.h>
#include "handshake.h"
#include "FS_ARGU.h"

#ifdef QEMU_SC
extern "C"{
    #include "../sysbus.h"
}
extern bool SC_EN;
extern qemu_irq FS_IRQ;
#endif

#ifndef FS_H
#define FS_H

/*
//SIZE 
//#define SLAVE_LENGTH  1074 //[IM] + [CMEM] + [VERTEX_FIFO] + [control registers]
#define SLAVE_LENGTH  1080 //CYLin: add start FS register
#define IM_LENGTH     256  
#define GPR_LENGTH    64   
#define CMEM_LENGTH   512  
#define V_FIFO_LENGTH 12   //3 vertex
#define TEX_LENGTH    1048579
#define TEX_WIDTH     64
#define LCD_LENGTH    76800//320*240
#define LCD_WIDTH     320
#define LCD_HEIGHT    240

#define IM_BASE          0
#define CMEM_BASE        512 
#define VERTEX_FIFO_BASE 1024
#define CONTROL_REG_BASE 1072
*/

//setup
struct tri_param_x
{
  //1: (x, y, z, w)
  //2: (u, v, uu, vv)
  //3: (r, g, b, a)
  //4: (va1, va2, va3, va4)
  //5: (v5_1, v5_2, v5_3, v5_4)
    float x1,x2,x3;
    float y1,y2,y3;
    float dx1,dx2,dx3;
    float dy1,dy2,dy3;
    float z1,z2,z3;
    float r1,r2,r3;
    float g1,g2,g3;
    float b1,b2,b3;
    float a1,a2,a3;
    float u1,u2,u3;
    float v1,v2,v3;
    float w1,w2,w3;

  //added for varying by sheng-chih
    float uu1,uu2,uu3;
    float vv1,vv2,vv3;
    float va11,va12,va13;
    float va21,va22,va23;
    float va31,va32,va33;
    float va41,va42,va43;
  //end added

  //added for varying by sheng-chang
  float v5_11,v5_12,v5_13;
  float v5_21,v5_22,v5_23;
  float v5_31,v5_32,v5_33;
  float v5_41,v5_42,v5_43;
  //end added

    float dz_x, dz_y;
    float dw_x, dw_y;
    float dr_x, dr_y;
    float dg_x, dg_y;
    float db_x, db_y;
    float da_x, da_y;
    float du_x, du_y;
    float dv_x, dv_y;
 
  //added by sheng-chih
    float duu_x, duu_y;
    float dvv_x, dvv_y;
    float dva1_x, dva1_y;
    float dva2_x, dva2_y;
    float dva3_x, dva3_y;
    float dva4_x, dva4_y;
  //end added    

  //added by sheng-chang
  float dv5_1_x, dv5_1_y;
  float dv5_2_x, dv5_2_y;
  float dv5_3_x, dv5_3_y;
  float dv5_4_x, dv5_4_y;
  //end added
    float x_min;
    float x_max;
    float y_min;
    float y_max;
};

//scan conversion
struct render_param
{
    float E1,E2,E3;
    float x,y;
    float z;
    float a,r,g,b,u,v,w;
    float fz;
    float fa,fr,fg,fb,fu,fv,fw;    

  //added by sheng-chih
  float uu,vv,va1,va2,va3,va4;
  float fuu,fvv,fva1,fva2,fva3,fva4;
  //end added

  //added by sheng-chang
  float v5_1,v5_2,v5_3,v5_4;
  float fv5_1,fv5_2,fv5_3,fv5_4;
  //end added
};


//[IF]
SC_MODULE(FETCH)
{
 public:
 	  sc_fifo_in<bool>            f_FETCH_start_in;  //from [SC]
 	  sc_fifo_in<bool>            f_branch_hit_in;   //from [ID]
 	  sc_fifo_in<bool>            f_finish_in;       //from [WB]
 	  sc_fifo_in<unsigned int>    f_branch_target_in;//from [ID]
 	  //--------------------------------------
 	  sc_fifo_out<unsigned int>   f_PC_out_ID;       //to [ID]
 	  sc_fifo_out< sc_uint<64> >  f_IM_data_out;     //to [ID] 	  

 	  friend class                FS;

 	  SC_CTOR(FETCH): PC(0),is_FETCH_full(0)
 	  {
 	      SC_THREAD(behavior);
 	  }

 private: 	  
 	  unsigned int PC; 
 	  bool is_FETCH_full; 	 	 
 	  sc_uint<64> *IM_IF;
 	  void behavior(void);
};


//[ID]
SC_MODULE(DECODE)
{
 public:
 	  sc_fifo_in<bool>            d_DECODE_start_in;  //from [SC] //CYLin
 	  /**** from [IF] ****/
 	  sc_fifo_in< sc_uint<64> >   d_IM_data_in;
 	  sc_fifo_in<unsigned int>    d_PC_in;
 	  //--------------------------------------
 	  /**** to [IF] ****/
 	  sc_fifo_out<bool>           d_branch_hit_out;
 	  sc_fifo_out<unsigned int>   d_branch_target_out;
 	  /**** to [EX] ****/
 	  sc_fifo_out< sc_uint<5> >   d_opcode_out;
 	  sc_fifo_out< sc_uint<6> >   d_dst_out;
 	  sc_fifo_out< sc_bv<4> >     d_w_mask_out; 	  
	  sc_fifo_out<bool>           d_RL_out; 	  
	  sc_fifo_out<bool>           d_FXL_out;
 	  sc_fifo_out< sc_uint<5> >   d_sa_out;
 	  sc_fifo_out<unsigned int>   d_imm_value_out;
 	  sc_fifo_out< sc_bv<128> >   d_old_DST_out;
 	  sc_fifo_out< sc_bv<32> >    d_X0_out,d_Y0_out,d_Z0_out,d_W0_out;
 	  sc_fifo_out< sc_bv<32> >    d_X1_out,d_Y1_out,d_Z1_out,d_W1_out;
 	  sc_fifo_out< sc_bv<32> >    d_X2_out,d_Y2_out,d_Z2_out,d_W2_out;
 	  sc_fifo_out<unsigned int>   d_TEX_offset_out;//@@
 	  
 	  //void ID_reset(void);

    friend class                FS;    

 	  SC_CTOR(DECODE): is_DECODE_full(0)
 	  {
 	      SC_THREAD(behavior);
 	  }

 private:
 	  bool        is_DECODE_full;
 	  //--------------------------------------
 	  sc_uint<5>  opcode;
 	  sc_uint<6>  dst,src0,src1,src2; 	   
 	  sc_bv<8>    swizzle[3];	  
	  bool        neg[3];
 	  sc_bv<4>    w_mask; 	  
	  bool        RL,FXL;
 	  sc_uint<2>  SV;
 	  sc_uint<32> imm_value;
 	  sc_uint<5>  sa;
 	  sc_int<16>  branch_addr;
 	  //--------------------------------------
 	  sc_bv<32>   X0,Y0,Z0,W0;
 	  sc_bv<32>   X1,Y1,Z1,W1;
 	  sc_bv<32>   X2,Y2,Z2,W2;
 	  //--------------------------------------
 	  sc_bv<128>  *GPR_ID;
 	  //--------------------------------------
 	  void IR_ctrl(sc_uint<64> IM_data);
 	  void SWZ(sc_bv<128> *SWZ_data);
 	  void IR_output_ctrl(sc_bv<128> R_data0,sc_bv<128> R_data1,sc_bv<128> R_data2);
 	  bool BGT(void);
 	  bool BLT(void);
	  bool BEQ(void);
	  bool BNEQ(void);
 	  void behavior(void);
};



//[EX]
SC_MODULE(EXE)
{
 public:
 	  /**** from [ID] ****/
 	  sc_fifo_in< sc_uint<5> >   e_opcode_in;
 	  sc_fifo_in< sc_uint<6> >   e_dst_in;
 	  sc_fifo_in< sc_bv<4> >     e_w_mask_in; 	  
	  sc_fifo_in<bool>           e_RL_in; 	  
	  sc_fifo_in<bool>           e_FXL_in;
 	  sc_fifo_in< sc_uint<5> >   e_sa_in;
 	  sc_fifo_in<unsigned int>   e_imm_value_in;
 	  sc_fifo_in< sc_bv<128> >   e_old_DST_in;
 	  sc_fifo_in< sc_bv<32> >    e_X0_in,e_Y0_in,e_Z0_in,e_W0_in;
 	  sc_fifo_in< sc_bv<32> >    e_X1_in,e_Y1_in,e_Z1_in,e_W1_in;
 	  sc_fifo_in< sc_bv<32> >    e_X2_in,e_Y2_in,e_Z2_in,e_W2_in;
    sc_fifo_in<unsigned int>   e_TEX_offset_in;//@@
 	  //--------------------------------------
 	  /**** to [WB] ****/
 	  sc_fifo_out< sc_bv<32> >   e_RLT_X_out,e_RLT_Y_out,e_RLT_Z_out,e_RLT_W_out;
 	  sc_fifo_out< sc_uint<5> >  e_opcode_out;
 	  sc_fifo_out< sc_uint<6> >  e_dst_out;
 	  sc_fifo_out< sc_bv<4> >    e_w_mask_out;
 	  sc_fifo_out< sc_bv<128> >  e_old_DST_out;

 	  friend class               FS; 	  

 	  SC_CTOR(EXE)
 	  {
 	      SC_THREAD(behavior);
 	  }

 private:
	  sc_bv<4>	w_mask;//gk add
 	  sc_uint<5>   opcode; 	  
	  bool         RL,FXL;
 	  sc_uint<5>   sa;
 	  sc_uint<32>  imm_value;
 	  sc_bv<32>    X0,Y0,Z0,W0;
 	  sc_bv<32>    X1,Y1,Z1,W1;
 	  sc_bv<32>    X2,Y2,Z2,W2;
 	  sc_bv<32>    RLT_X,RLT_Y,RLT_Z,RLT_W;
 	  float        TEX[4];
 	  sc_bv<128>    CMEM_value;
 	  //--------------------------------------
 	  //special funct
 	  float        a0,a1,b0,b1;
	  float        c0,c1,d0,d1;
	  float        Y_rcp,Y_rsq,Y_pow2,Y_log2;
	  int          exp_rcp,exp_rsq,exp_pow2,exp_log2;
	  //--------------------------------------
	  sc_bv<32>    MAD_input[12];
	  //--------------------------------------
	  //ADD , MUL , MAD
	  int          MAD_EXP[4]; //save Emax
	  float        MAD_SRC[12];//X,Y,Z(1~4) ;
	  //--------------------------------------
	  //pipe1~pipe4
	  int   pipe1_EXP,pipe2_EXP,pipe3_EXP,pipe4_EXP;
	  float pipe1_X,pipe1_Y,pipe1_Z;
    float pipe2_X,pipe2_Y,pipe2_Z;
    float pipe3_X,pipe3_Y,pipe3_Z;
    float pipe4_X,pipe4_Y,pipe4_Z;
    float pipe1_out,pipe2_out,pipe3_out,pipe4_out;
    //--------------------------------------
    unsigned int *TEX_MEM_EX;
    sc_bv<128>    *CMEM_EX;

    sc_bv<32> float2bv(float input);
 	  void MOV(void);
 	  void MAXx(void);
 	  void MINx(void);
 	  void AND(void);
 	  void OR(void);
 	  void XOR(void);
 	  void SHRL(void);
 	  void MVI(void);
 	  int RCP_factor(void);
 	  int RSQ_factor(void);
 	  int POW2_factor(void);
 	  int LOG2_factor(void);
 	  void MAD_factor(void);
 	  void pipe_SRC_select(void);
 	  void DP_op(float P1,float P2,float P3,float P4);
 	  void out_select(void);
 	  void behavior(void);
};



//WB
SC_MODULE(WB)
{
 public:
 	sc_fifo_in<bool>          b_WB_start_in;  //from [SC] //CYLin
 	/**** from [EX] ****/
 	sc_fifo_in< sc_bv<32> >   b_RLT_X_in,b_RLT_Y_in,b_RLT_Z_in,b_RLT_W_in;
 	sc_fifo_in< sc_uint<5> >  b_opcode_in;
 	sc_fifo_in< sc_uint<6> >  b_dst_in;
 	sc_fifo_in< sc_bv<4> >    b_w_mask_in;
 	sc_fifo_in< sc_bv<128> >  b_old_DST_in;
 	//--------------------------------------
 	/**** to [IF] ****/
 	sc_fifo_out<bool>         b_finish_out;
 	/**** to [GPU_FRAG_IF] ****/
 	sc_fifo_out<bool>         b_shader_end_out;
 	
 	//void WB_reset(void);

 	friend class              FS;  
  
 	SC_CTOR(WB): is_WB_full(0),is_FIN(0)
 	{
 	    SC_THREAD(behavior);
 	}

 private:
 	  bool is_WB_full;
 	  bool is_FIN;
 	  //--------------------------------------
 	  sc_bv<128> *GPR_WB;
    void behavior(void);
};



//GPU_FRAG_IF
SC_MODULE(GPU_FRAG_IF)
{
 public:
 	  /**** from [WB] ****/
 	  sc_fifo_in<bool>            i_shader_end_in;
 	  //--------------------------------------
 	  /**** to [UNIFIED_FGAG] ****/
 	  sc_fifo_out<bool>           i_shader_finish_out;
 	  sc_fifo_out<unsigned int>   i_X_out,i_Y_out,i_Z_out; 	 
 	  sc_fifo_out< sc_uint<32> >  i_color_out;

 	  friend class                FS;    

 	  SC_CTOR(GPU_FRAG_IF)
 	  {
 	      SC_THREAD(behavior);
 	  }
 private:
 	  sc_bv<128> *GPR_IF; 	
 	  void behavior(void);
};



//UNIFIED_FRAG
SC_MODULE(UNIFIED_FRAG)
{
 public:
    //Initiator port
	sc_fifo_out<Req>  mp_transfer;
	sc_fifo_in<bool>  mp_response;
	unsigned int      databuff;
    /**** from [GPU_FRAG_IF] ****/
 	  sc_fifo_in<bool>          u_shader_finish_in;
 	  sc_fifo_in<unsigned int>  u_X_in,u_Y_in,u_Z_in;
 	  sc_fifo_in< sc_uint<32> > u_color_in;
 	  //--------------------------------------
 	  //<outputs>
 	  sc_fifo_out<bool>         u_FRAG_finish_out;
 	  int info_num;//CYL::debug
 	  //ofstream framebuff;

 	  friend class              FS;

 	  SC_CTOR(UNIFIED_FRAG)
 	  { 
 	      SC_THREAD(behavior);
 	      //framebuff.open("./file/framebuff.txt");
 	  }
 	  //~UNIFIED_FRAG()
 	  //{framebuff.close();}
 private:
 	  Req info_mp;
 	  /**** z compare parameters ****/
 	  bool                      zcompare_enable;
 	  sc_uint<3>                zcompare_func;
 	  /**** stencil test parameters ****/
 	  bool                      stencil_enable;
    sc_uint<3>                stencil_func;
    sc_uint<8>                stencil_ref,stencil_mask;
    sc_uint<3>                fail,zfail,zpass;
 	  /**** blending parameters ****/
 	  bool                      blending_enable;
 	  sc_uint<4>                blending_srcfunc,blending_dstfunc;
 	  //-------------------------------	 	  
 	  unsigned int *ZS_MEM_FRAG;
 	  unsigned int *F_MEM_FRAG;
 	  //--------------------------------------
 	  bool test_circuit(sc_uint<3> func,bool sign,bool eq);
 	  sc_uint<8> stencil_update(sc_uint<3> func,sc_uint<8> S_ref,sc_uint<8> old_S);
 	  sc_uint<32> factor_selector(bool SRC_DST,sc_uint<4> func,sc_uint<32> SRC_color,sc_uint<32> DST_color);
 	  void behavior(void);
};



//FS(Top module)
SC_MODULE(FS)
{
 public:
 	sc_int<32>	FST[21];
	//I/O port //CYLin
	sc_fifo_in<float> sp_VOB;
	sc_in<bool>       sp_VS_END; 
	//Target port
	sc_fifo_in<Req>   sp_transfer;
	sc_fifo_out<bool> sp_response;
	//Initiator port
	sc_fifo_out<Req>  mp_transfer;
	sc_fifo_in<bool>  mp_response;
	//unsigned int      databuff[16];
	bool              access_IO;
	UNIFIED_FRAG *FRAG;     //[UNIFIED_FGAG]
	
	//(sub module)
	//-----------------------
	sc_bv<32>    *SLAVE_MEM;//[SLAVE_MEM]        
	//(storage unit)
	//-----------------------    
	unsigned int *TEX_MEM; //[TEX_MEM]
	unsigned int *ZS_MEM;  //[ZS_MEM]
	unsigned int *F_MEM;   //[F_MEM]
	
	
	SC_CTOR(FS):top_FETCH_start(1),top_DECODE_start(1),top_WB_start(1),top_FRAG_finish(1),//CYLin
	  	          top_branch_hit(1),top_branch_target(1),top_FD_IM_data(1),
	  	          top_FD_PC(1),top_finish(1),top_opcode_DX(1),top_dst_DX(1),
	  	          top_w_mask_DX(1),top_RL(1),top_FXL(1),top_sa(1),top_imm_value(1),
	  	          top_old_DST_DX(1),top_X0(1),top_Y0(1),top_Z0(1),top_W0(1),top_X1(1),
	  	          top_Y1(1),top_Z1(1),top_W1(1),top_X2(1),top_Y2(1),top_Z2(1),top_W2(1),
	  	          top_TEX_offset(1),top_RLT_X(1),top_RLT_Y(1),top_RLT_Z(1),top_RLT_W(1),
	  	          top_opcode_XB(1),top_dst_XB(1),top_w_mask_XB(1),top_old_DST_XB(1),
	  	          top_shader_end(1),top_shader_finish(1),top_X(1),top_Y(1),top_Z(1),
	  	          top_color(1) 	  	  	           	
	{	   	  	
		/**** [SET-UP & SC] ****/
		
		//SC_THREAD(bus_wrap);
		SC_THREAD(render_triangle);
		SC_THREAD(bus_wrap);
		
		//-----------------------
		//open log file
		mylog.open("./output/log.FS.txt");
	    	mylog<<std::hex;
	    					
		//-----------------------
		/**** [IF] ****/
		I_F = new FETCH("I_F");
		// <- [SC]
		I_F->f_FETCH_start_in(top_FETCH_start);
		// <-> [ID]
		I_F->f_branch_hit_in(top_branch_hit);
		I_F->f_branch_target_in(top_branch_target);
		I_F->f_PC_out_ID(top_FD_PC);
		I_F->f_IM_data_out(top_FD_IM_data);
		// <- [WB]
		I_F->f_finish_in(top_finish); 	    
		//-----------------------
		/**** [ID] ****/
		I_D = new DECODE("I_D");
		// <- [SC]
		I_D->d_DECODE_start_in(top_DECODE_start);//CYLin
		// <-> [IF]
		I_D->d_IM_data_in(top_FD_IM_data);
		I_D->d_PC_in(top_FD_PC);
		I_D->d_branch_hit_out(top_branch_hit);
		I_D->d_branch_target_out(top_branch_target);
		// -> [EX]
		I_D->d_opcode_out(top_opcode_DX);
		I_D->d_dst_out(top_dst_DX);
		I_D->d_w_mask_out(top_w_mask_DX);
		I_D->d_RL_out(top_RL);
		I_D->d_FXL_out(top_FXL);
		I_D->d_sa_out(top_sa);
		I_D->d_imm_value_out(top_imm_value);
		I_D->d_old_DST_out(top_old_DST_DX);
		I_D->d_X0_out(top_X0);
		I_D->d_Y0_out(top_Y0);
		I_D->d_Z0_out(top_Z0);
		I_D->d_W0_out(top_W0);
		I_D->d_X1_out(top_X1);
		I_D->d_Y1_out(top_Y1);
		I_D->d_Z1_out(top_Z1);
		I_D->d_W1_out(top_W1);
		I_D->d_X2_out(top_X2);
		I_D->d_Y2_out(top_Y2);
		I_D->d_Z2_out(top_Z2);
		I_D->d_W2_out(top_W2);
		I_D->d_TEX_offset_out(top_TEX_offset);//@@
		//-----------------------
		/**** [EX] ****/
		I_X = new EXE("I_X");
		// <- [ID]
		I_X->e_opcode_in(top_opcode_DX);
		I_X->e_dst_in(top_dst_DX);
		I_X->e_w_mask_in(top_w_mask_DX);
		I_X->e_RL_in(top_RL);
		I_X->e_FXL_in(top_FXL);
		I_X->e_sa_in(top_sa);
		I_X->e_imm_value_in(top_imm_value);
		I_X->e_old_DST_in(top_old_DST_DX);
		I_X->e_X0_in(top_X0);
		I_X->e_Y0_in(top_Y0);
		I_X->e_Z0_in(top_Z0);
		I_X->e_W0_in(top_W0);
		I_X->e_X1_in(top_X1);
		I_X->e_Y1_in(top_Y1);
		I_X->e_Z1_in(top_Z1);
		I_X->e_W1_in(top_W1);
		I_X->e_X2_in(top_X2);
		I_X->e_Y2_in(top_Y2);
		I_X->e_Z2_in(top_Z2);
		I_X->e_W2_in(top_W2);
		I_X->e_TEX_offset_in(top_TEX_offset);//@@
		// -> [WB]
		I_X->e_RLT_X_out(top_RLT_X);
		I_X->e_RLT_Y_out(top_RLT_Y);
		I_X->e_RLT_Z_out(top_RLT_Z);
		I_X->e_RLT_W_out(top_RLT_W);
		I_X->e_opcode_out(top_opcode_XB);
		I_X->e_dst_out(top_dst_XB);
		I_X->e_w_mask_out(top_w_mask_XB);
		I_X->e_old_DST_out(top_old_DST_XB);
		//-----------------------
		/**** [WB] ****/
		I_W = new WB("I_W");
		// <- [SC]
		I_W->b_WB_start_in(top_WB_start);//CYLin
		// <- [EX]
		I_W->b_RLT_X_in(top_RLT_X);
		I_W->b_RLT_Y_in(top_RLT_Y);
		I_W->b_RLT_Z_in(top_RLT_Z);
		I_W->b_RLT_W_in(top_RLT_W);
		I_W->b_opcode_in(top_opcode_XB);
		I_W->b_dst_in(top_dst_XB);
		I_W->b_w_mask_in(top_w_mask_XB);
		I_W->b_old_DST_in(top_old_DST_XB);
		// -> [IF]
		I_W->b_finish_out(top_finish);
		// -> [GPU_FRAG_IF]
		I_W->b_shader_end_out(top_shader_end);
		//-----------------------
		/**** [GPR_FRAG_IF] ****/
		G_F_if = new GPU_FRAG_IF("G_F_if");
		// <- [WB]
		G_F_if->i_shader_end_in(top_shader_end);
		// -> [UNIFIED_FGAG]
		G_F_if->i_shader_finish_out(top_shader_finish);
		G_F_if->i_X_out(top_X);
		G_F_if->i_Y_out(top_Y);
		G_F_if->i_Z_out(top_Z);
		G_F_if->i_color_out(top_color);
		//-----------------------
		/**** [UNIFIED_FRAG] ****/
		FRAG = new UNIFIED_FRAG("FRAG");
		FRAG->mp_transfer(mp_transfer);
		FRAG->mp_response(mp_response);
		// <- [GPU_FRAG_IF]
		FRAG->u_shader_finish_in(top_shader_finish);
		FRAG->u_X_in(top_X);
		FRAG->u_Y_in(top_Y);
		FRAG->u_Z_in(top_Z);
		FRAG->u_color_in(top_color);
		// -> [SC]
		FRAG->u_FRAG_finish_out(top_FRAG_finish);
		//-----------------------
		/**** [IM] [GPR] [CMEM] [TEX_MEM] [ZS_MEM] [F_MEM] ****/   
		IM = new sc_uint<64>[IM_LENGTH];
		GPR = new sc_bv<128>[GPR_LENGTH];
		CMEM = new sc_bv<128>[CMEM_LENGTH];
		SLAVE_MEM = new sc_bv<32>[SLAVE_LENGTH];
		TEX_MEM = new unsigned int[TEX_LENGTH];
		ZS_MEM = new unsigned int[LCD_LENGTH];
		F_MEM = new unsigned int[LCD_LENGTH];  
		I_F->IM_IF = IM;
		I_D->GPR_ID = GPR;
		I_X->TEX_MEM_EX = TEX_MEM;
		I_X->CMEM_EX = CMEM;
		I_W->GPR_WB = GPR;
		G_F_if->GPR_IF = GPR;
		FRAG->ZS_MEM_FRAG = ZS_MEM;
		FRAG->F_MEM_FRAG = F_MEM;      
		
	}//end constructor
	~FS()//CYLin
	{
	    mylog.close();	
	}
 private: 	
	//======CYLin variable======
	//SC event
	sc_event flag_FS;
	//log file
	ofstream mylog;
	//for initiator port
	//Req info_mp;
	//for Target port
	Req info_sp;
	unsigned int *DataAddr;
	unsigned int Addr;
	bool Type;
	int Burst;
	float VOB;//CYLin

	//(channel)
	//-----------------------	
	//[SC] <-> [IF][DECODE][WB]
	sc_fifo<bool>          top_FETCH_start;
	sc_fifo<bool>          top_DECODE_start;//CYLin
	sc_fifo<bool>          top_WB_start;//CYLin
	//[SC] <-> [UNIFIED_FRAG] 	
	sc_fifo<bool>          top_FRAG_finish; 	 	
	//[IF] <-> [ID]
	sc_fifo<bool>          top_branch_hit;
	sc_fifo<unsigned int>  top_branch_target;
	sc_fifo< sc_uint<64> > top_FD_IM_data;
	sc_fifo<unsigned int>  top_FD_PC;
	//[IF] <-> [WB]
	sc_fifo<bool>          top_finish;
	//[ID] <-> [EX]
	sc_fifo< sc_uint<5> >  top_opcode_DX;
	sc_fifo< sc_uint<6> >  top_dst_DX;
	sc_fifo< sc_bv<4> >    top_w_mask_DX; 	 	  
	sc_fifo<bool>          top_RL; 	 	  
	sc_fifo<bool>          top_FXL;
	sc_fifo< sc_uint<5> >  top_sa;
	sc_fifo<unsigned int>  top_imm_value;
	sc_fifo< sc_bv<128> >  top_old_DST_DX;
	sc_fifo< sc_bv<32> >   top_X0,top_Y0,top_Z0,top_W0;
	sc_fifo< sc_bv<32> >   top_X1,top_Y1,top_Z1,top_W1;
	sc_fifo< sc_bv<32> >   top_X2,top_Y2,top_Z2,top_W2;
	sc_fifo<unsigned int>  top_TEX_offset;
	//[EX] <-> [WB]
	sc_fifo< sc_bv<32> >   top_RLT_X,top_RLT_Y,top_RLT_Z,top_RLT_W;
	sc_fifo< sc_uint<5> >  top_opcode_XB;
	sc_fifo< sc_uint<6> >  top_dst_XB;
	sc_fifo< sc_bv<4> >    top_w_mask_XB;
	sc_fifo< sc_bv<128> >  top_old_DST_XB;
	//[WB] <-> [GPU_FRAG_IF]
	sc_fifo<bool>          top_shader_end;
	//[GPU_FRAG_IF] <-> [UNIFIED_FGAG]
	sc_fifo<bool>          top_shader_finish;
	sc_fifo<unsigned int>  top_X,top_Y,top_Z;
	sc_fifo< sc_uint<32> > top_color; 	 	
		 		
	//(members of [SET-UP] & [SC])
	//----------------------- 	
	struct tri_param_x process_triangle;   
 	  
	void shift_param(struct render_param*, struct render_param*, int, int);
	sc_bv<32> float2bv(float);
	void fragment(struct render_param);
	int pixel_test(struct render_param, int, int);
	void scan_conversion(struct render_param);
	void render_triangle(void);
	void bus_wrap(void);
		
	//(sub module)
	//-----------------------
	FETCH * I_F;            //[IF]
	DECODE * I_D;           //[ID]
	EXE * I_X;              //[EX]
	WB * I_W;               //[WB]    
	GPU_FRAG_IF *G_F_if;    //[GPU_FRAG_IF] 
	//UNIFIED_FRAG *FRAG;     //[UNIFIED_FGAG]	  
	sc_uint<64>  *IM;       //[IM]
	sc_bv<128>   *GPR;      //[GPR]
	sc_bv<128>    *CMEM;     //[CMEM]    
};


#endif
