#ifndef GPU_H
#define GPU_H


struct vec4
{
    float x,y,z,w; 
};


struct V_info
{
    struct vec4 attrib[4];
};


struct V_FIFO
{
    int           full,empty;
	int           R_ptr,W_ptr;
	unsigned int  counter;
	struct V_info *list;
};


struct V_FIFO_CTRL
{
    struct V_FIFO *fifo;
	//-----------------------
	struct V_info Vinfo;
	unsigned int  attrib_num;
	int           state;
};


struct specail_list
{
 	float a0,a1,b0,b1,c0,c1,d0,d1;
	float Y_rcp,Y_rsq,Y_pow2,Y_log2;
	int   exp_rcp,exp_rsq,exp_pow2,exp_log2;    
};


struct MAD_list
{
	float MAD_input[12];
	//-------------------
	int   MAD_EXP[4]; //Emax
	float MAD_SRC[12];//X,Y,Z(4 scalar pipeline);
};


struct scalar_pipeline
{
    int   pipe_EXP;
	float pipe_X,pipe_Y,pipe_Z;
};


struct DP_stg_EX34
{
    float DP_12_plus,DP_34_plus;
    int   Emax;	
};


struct IF_ID_if
{
    unsigned int thread_ID,thread_type;
	unsigned int PC;
    //---------------------------------
	unsigned int IM_data_L,IM_data_R;
};


struct ID_READ_if
{
    unsigned int thread_ID,thread_type;
	//---------------------------------
	int          RAW_dep;
	//---------------------------------
	unsigned int PC;
	unsigned int opcode,dst_addr;		
	unsigned int src_addr[3];
	int          swizzle[3],neg[3];
	int          SV,FXL,W_mask;	
	int          RL,sa;
	unsigned int imm_value;
	int          branch_addr;
	int          GPU_IF_sig,FRAG_mode;
};


struct READ_out
{
    // -> [ID]
    unsigned int Tid_ori,Tid_wb_convt;
	int          Wen,out_sig;
	unsigned int dst_addr;
    int          stall_READ;/**** (1)TEX stall (2) output stall ****/   
	int          flush_ID;  /**** (1)data dep(2)stall occur(3)branch ****/			  
    //---------------------------------
	// -> [IF]
	unsigned int branch_Tid;
	int          branch_hit;
	unsigned int branch_target;
};


struct READ_EX1_if
{
    unsigned int thread_ID,thread_type;
	//---------------------------------
	unsigned int opcode,dst_addr;
	int          out_sig,wen,W_mask;
	int          RL,sa,FXL;
	unsigned int imm_value;
	struct vec4  TEX_data;
    float        CMEM_Rdata;
	int          is_FRAG_start,FRAG_mode;
	//---------------------------------
	struct vec4  SRC0_data,SRC1_data,SRC2_data;
};


struct EX1234_out
{
    unsigned int Tid_EXE[4];
	int          wen_EXE[4],out_sig_EXE[4];
	unsigned int dst_addr_EXE[4];
};


struct EX1_EX2_if
{
    unsigned int thread_ID_pipe,thread_type_pipe; 
    //---------------------------------
	unsigned int opcode_pipe,dst_addr_pipe;
	int          out_sig_pipe,wen_pipe,W_mask_pipe;
	int          FXL_pipe;
	unsigned int imm_value_pipe;
    struct vec4  TEX_data_pipe;
	float        CMEM_Rdata_pipe;
	int          is_FRAG_start_pipe,FRAG_mode_pipe;
	//---------------------------------
	struct vec4  SRC0_data_pipe,SRC1_data_pipe,SRC2_data_pipe;
	//---------------------------------
	struct vec4  MOV_pipe,MVI_pipe;
	struct vec4  MAX_pipe,MIN_pipe;
	struct vec4  AND_pipe,OR_pipe,XOR_pipe,INV_pipe;
	struct vec4  SHRL_pipe;
	//---------------------------------
	struct specail_list S_list;
};


struct EX2_EX3_if
{
    unsigned int thread_ID_pipe,thread_type_pipe; 
    //---------------------------------
	unsigned int opcode_pipe,dst_addr_pipe;
	int          out_sig_pipe,wen_pipe,W_mask_pipe;
	int          FXL_pipe;
	unsigned int imm_value_pipe;
    struct vec4  TEX_data_pipe;
	float        CMEM_Rdata_pipe;
	struct vec4  old_DST_data_pipe;
	int          is_FRAG_start_pipe,FRAG_mode_pipe;	
	//---------------------------------
	struct vec4  MOV_pipe,MVI_pipe;
	struct vec4  MAX_pipe,MIN_pipe;
	struct vec4  AND_pipe,OR_pipe,XOR_pipe,INV_pipe;
	struct vec4  SHRL_pipe;
	//---------------------------------
    float        pipe_MUL[4],pipe_ADD[4];
    int          pipe_EXP[4];
};


struct EX3_EX4_if
{
    unsigned int thread_ID_pipe,thread_type_pipe;
    //---------------------------------
	unsigned int opcode_pipe,dst_addr_pipe;
	int          out_sig_pipe,wen_pipe,W_mask_pipe;
	int          FXL_pipe;
	unsigned int imm_value_pipe;
    struct vec4  TEX_data_pipe;
	float        CMEM_Rdata_pipe;
	struct vec4  old_DST_data_pipe;
	int          is_FRAG_start_pipe,FRAG_mode_pipe;	
	//---------------------------------
	struct vec4  MOV_pipe,MVI_pipe;
	struct vec4  MAX_pipe,MIN_pipe;
	struct vec4  AND_pipe,OR_pipe,XOR_pipe,INV_pipe;
	struct vec4  SHRL_pipe;
    //---------------------------------
	float        pipe_MAD[4];
	int          pipe_EXP[4];
	struct DP_stg_EX34 DP_stg34;
};


struct EX4_EX5_if
{
    unsigned int thread_ID_pipe,thread_type_pipe;
    //---------------------------------
	unsigned int opcode_pipe,dst_addr_pipe;
	int          out_sig_pipe,wen_pipe,W_mask_pipe;
	int          FXL_pipe;
	unsigned int imm_value_pipe;
    struct vec4  TEX_data_pipe;
	float        CMEM_Rdata_pipe;
	struct vec4  old_DST_data_pipe;
	int          is_FRAG_start_pipe,FRAG_mode_pipe;	
	//---------------------------------
	struct vec4  MOV_pipe,MVI_pipe;
	struct vec4  MAX_pipe,MIN_pipe;
	struct vec4  AND_pipe,OR_pipe,XOR_pipe,INV_pipe;
	struct vec4  SHRL_pipe;
    //---------------------------------
	float        pipe_out[4];
	float        DP_fraction;
	int          DP_EXP;
};


struct EX5_WB_if
{    
	unsigned int thread_ID,thread_type;
    //---------------------------------
	unsigned int opcode,dst_addr;      
	struct vec4  DST_data,old_DST_data;
	int          out_sig,wen,W_mask;
    unsigned int imm_value;
	//---------------------------------
	int          is_FRAG_start,FRAG_mode;	
};


struct WB_out
{
    unsigned int thread_ID,thread_type;
	//---------------------------------
	unsigned int dst_addr;
	int          out_sig,wen;
	struct vec4  Wdata;
	//---------------------------------
	int          CMEM_Wen;
	unsigned int CMEM_Waddr;
	float        CMEM_Wdata;
	//---------------------------------
	int          is_FRAG_start,FRAG_mode;
};


#endif
