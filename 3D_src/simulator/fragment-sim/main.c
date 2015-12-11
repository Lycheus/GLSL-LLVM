#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "opcode.h"
#include "gpu.h"
#include "tex_cache.h"
#include "fragment.h"
#include "thread.h"
#include "varying_attrib.h"


#define EXTERNAL_MEM_DELAY 1
#define DEBUG 0
#define DRAW 1


/**** Frame buffer ****/
#define LCD_WIDTH 320
#define LCD_HEIGHT 240

/**** TEX cache ****/
#define TEX_WIDTH 64
#define TEX_HEIGHT 64

/**** IM(SC & VS & FS) ****/
#define IM_LENGTH 256

/**** CMEM ****/
#define CMEM_LENGTH 512

/**** GPR ****/
#define GPR_LENGTH 896//14(1+6+7)

unsigned int ZS_BUF[LCD_WIDTH * LCD_HEIGHT];
unsigned int F_BUF[LCD_WIDTH * LCD_HEIGHT];
unsigned int TEXTURE[TEX_WIDTH * TEX_HEIGHT];
unsigned int SC_IM_L[IM_LENGTH];
unsigned int SC_IM_R[IM_LENGTH];
unsigned int VS_IM_L[IM_LENGTH];
unsigned int VS_IM_R[IM_LENGTH];
unsigned int FS_IM_L[IM_LENGTH];
unsigned int FS_IM_R[IM_LENGTH];
unsigned int CMEM[CMEM_LENGTH];
struct vec4  GPR[GPR_LENGTH];

/**** thread control ****/
struct THREAD_CONTROL thread_ctrl;

/**** varying FIFO ****/
struct V_FIFO_CTRL V_fifo_ctrl;
struct V_FIFO V_fifo;

/**** texture cache ****/
struct cache  T_cache;
struct T_FIFO T_fifo;

/**** fragment ****/	
struct FRAGMENT FRAG;
struct F_FIFO   F_fifo;

/**** [EXE] interface *****/
struct EX1_EX2_if stg4_5;
struct EX2_EX3_if stg5_6;
struct EX3_EX4_if stg6_7;
struct EX4_EX5_if stg7_8;
struct EX1234_out EXE_out; 


/**** system finish ****/
int SYS_finish;

/**** NOP cycle count ****/
int NOP_count = 0;

/**** debug ****/
FILE *dump;
FILE *f_buf;

//---------------------------------------------
//[WB]
void WB(struct EX5_WB_if,struct WB_out*);
//---------------------------------------------
//[EXE]
void mov(struct vec4,struct vec4*);
void mvi(unsigned int,struct vec4*);
void max(int,struct vec4,struct vec4,struct vec4*);
void min(int,struct vec4,struct vec4,struct vec4*);
void and(struct vec4,struct vec4,struct vec4*);
void or(struct vec4,struct vec4,struct vec4*);
void xor(struct vec4,struct vec4,struct vec4*);
void inv(struct vec4,struct vec4*);
void shrl(int,int,struct vec4,struct vec4*);
int  RCP_factor(float,float*,float*,float*);
int  RSQ_factor(float,float*,float*,float*);
int  POW2_factor(float,float*,float*,float*);
int  LOG2_factor(float,float*,float*,float*);
void MAD_factor(unsigned int,int,struct vec4*,struct MAD_list*);
void pipe_SRC_select(unsigned int,struct specail_list*,struct MAD_list,struct scalar_pipeline*);
void DP_op(float*,int*,struct DP_stg_EX34*);
void out_select(struct EX4_EX5_if,float*);
void EXE(struct READ_EX1_if,struct EX5_WB_if*);
//---------------------------------------------
//[READ]
void SWZ(int*,int*,struct vec4*);
void SRC_vec(int,struct vec4*);
int TEX_cache(int,float,float);
int Tfifo_write(struct T_FIFO*,unsigned int,float,float);
int Tfifo_read(struct T_FIFO*,struct T_info*);
int output_check(unsigned int,unsigned int,int);
int Ffifo_write(struct F_FIFO*,struct F_info*);
int Ffifo_read(struct F_FIFO*,struct F_info*);
int test_circuit(unsigned int,int,int);
unsigned int stencil_update(unsigned int,unsigned int,unsigned int);
unsigned int factor_selector(int,unsigned int,unsigned int,unsigned int);
void fragment(void);
int Vfifo_write(struct V_FIFO*,struct V_info*);
int Vfifo_read(struct V_FIFO*,struct V_info*);
int Vfifo_slot(void);
unsigned int slot_detector(unsigned int,unsigned int);
unsigned int VS_out_map(int,unsigned int);
void WB_convt(struct ID_READ_if,struct READ_EX1_if*);
int beq(int,struct vec4,struct vec4);
int bneq(int,struct vec4,struct vec4);
int blt(int,struct vec4,struct vec4);
int bge(int,struct vec4,struct vec4);
void ctrl_unit(int,struct ID_READ_if,struct vec4*,struct READ_out*);
void READ(struct ID_READ_if,struct WB_out,struct READ_EX1_if*,struct READ_out*);
//---------------------------------------------
//[DECODE]
void IR_ctrl(unsigned int,unsigned int,struct ID_READ_if*);
int classify(unsigned int);
void DECODE(struct IF_ID_if,struct READ_out,struct ID_READ_if*,struct READ_EX1_if*);
//---------------------------------------------
//[FETCH]
int scheduler(void);
void FETCH(struct READ_out,struct IF_ID_if*);
//---------------------------------------------
void TEXTURE_init(void);
void SC_IM_init(void);
void VS_IM_init(void);
void FS_IM_init(void);
int GPU(void);



int main()
{
	int i,j;
	struct vec4   in_data;
	struct V_info V_attrib;

	
	//Varying FIFO_ctrl
	V_fifo_ctrl.fifo = &V_fifo;
	V_fifo_ctrl.fifo->list = (struct V_info*)malloc(36 * sizeof(struct V_info));
	
	//Varying FIFO
	V_fifo_ctrl.attrib_num = 0;
	V_fifo_ctrl.fifo->full = 0;
	V_fifo_ctrl.fifo->empty = 1;
	V_fifo_ctrl.fifo->R_ptr = 0;
	V_fifo_ctrl.fifo->W_ptr = 0;	
	V_fifo_ctrl.fifo->counter = 0;
	
	//TEX cache 
	T_cache.cache = (unsigned int*)malloc(64 * sizeof(unsigned int));
	T_cache.fifo = &T_fifo;
	T_cache.state = 0;
	
	for(i=0;i<4;++i)
	{
	    T_cache.valid[i] = 0;//invalid
		T_cache.tag[i].S_tag = 0;
		T_cache.tag[i].T_tag = 0;
	}
	
	for(i=0;i<14;++i)
	   T_cache.TEX_miss_flag[i] = 0;
	   
	//TEX fifo
	T_cache.fifo->full = 0;
	T_cache.fifo->empty = 1;
	T_cache.fifo->R_ptr = 0;
	T_cache.fifo->W_ptr = 0;
	T_cache.fifo->counter = 0;	
	
	//Fragment
	FRAG.out_pool = (struct F_info*)malloc(14 * sizeof(struct F_info));
	FRAG.fifo = &F_fifo;
	FRAG.state = 0;
	
	//Fragment FIFO
	FRAG.fifo->full = 0;
	FRAG.fifo->empty = 1;
	FRAG.fifo->R_ptr = 0;
	FRAG.fifo->W_ptr = 0;
	FRAG.fifo->counter = 0;
	
	//Fragment parameter
	/**** z compare parameters ****/
	FRAG.zcompare_enable = 1;
	FRAG.zcompare_func = LESS;
	
	/**** stencil test parameters ****/
	FRAG.stencil_enable = 0;
	FRAG.stencil_func = 0;
	FRAG.stencil_ref = 255;//0xff
	FRAG.stencil_mask = 255;//0xff
	FRAG.fail = KEEP;
	FRAG.zfail = KEEP;
	FRAG.zpass = KEEP;
	
	/**** blending parameters ****/
	FRAG.blending_enable = 0;
	FRAG.blending_srcfunc = BLEND_ONE;
	FRAG.blending_dstfunc = BLEND_ZERO;
	
	//thread control
	thread_ctrl.info = (struct THREAD_UNIT*)malloc(14 * sizeof(struct THREAD_UNIT));
	memset(thread_ctrl.info,0,(14 * sizeof(struct THREAD_UNIT)));
	
	for(i=0;i<2;++i)
	   for(j=0;j<3;++j)
	      thread_ctrl.triangle[i][j] = -1;//initial value
    
	for(i=0;i<3;++i)
	   thread_ctrl.VS_done[i] = 0;//initial value   
	
	thread_ctrl.SC_slot = -1;
	thread_ctrl.tri_slot = 0;//FIFO control
	thread_ctrl.VS_slot = -1;//FIFO control
	thread_ctrl.FS_slot = -1;
	thread_ctrl.tri_priority = 0;	
	thread_ctrl.NO_count = 14;
	thread_ctrl.SC_count = 0;
	thread_ctrl.VS_count = 0;
	thread_ctrl.FS_count = 0;
	
	//F_BUF & ZS_BUF init
	for(i=0;i<(LCD_WIDTH * LCD_HEIGHT);++i)
	{
	    F_BUF[i] = 0;
		ZS_BUF[i] = 0xffff00ff;//{Z(16-bits),0(8-bits),S(8-bits)}
	}
	
	//Texture memory init
	TEXTURE_init();
	
	//IM init
	SC_IM_init();
	VS_IM_init();
	FS_IM_init();
	
	//CMEM init
	//Red cube uniform set
	CMEM[0]=66;
	CMEM[1]=55;
	CMEM[2]=44;
	CMEM[3]=33;
	//END Red cube uniform set
	
	//V_FIFO write
	for(i=0;i<36;++i)
	{
        //position
		in_data.x = vertex[i][0];in_data.y = vertex[i][1];
		in_data.z = vertex[i][2];in_data.w = (1/vertex[i][3]);
		mov(in_data,&V_attrib.attrib[0]);
        //normal
        in_data.x = 0;in_data.y = 0;
		in_data.z = 0;in_data.w = 0;		
	   	mov(in_data,&V_attrib.attrib[1]);
		//color
		in_data.x = 255;in_data.y = 0;
		in_data.z = 0;in_data.w = 0;
		mov(in_data,&V_attrib.attrib[2]);
		//texture coordinate
		in_data.x = tex_coord[i][0];in_data.y = tex_coord[i][1];
		in_data.z = tex_coord[i][2];in_data.w = tex_coord[i][3];		
		mov(in_data,&V_attrib.attrib[3]);
		
	    Vfifo_write(V_fifo_ctrl.fifo,&V_attrib);
    }
	
	//GPU
    GPU();
	
	free(V_fifo_ctrl.fifo->list);
	free(T_cache.cache);	
    free(T_cache.fifo);
	free(FRAG.out_pool);
	free(FRAG.fifo);
	free(thread_ctrl.info);
	system("pause");
    return 0;
}


void WB(struct EX5_WB_if input,struct WB_out *output)
{
    #if DEBUG
    if(input.wen == 1 && SYS_finish == 36)
	{
	   fprintf(dump,"[WB]\n");
	   fprintf(dump,"========================\n");	
	}
	#endif
	
	output->thread_ID = input.thread_ID;
	output->thread_type = input.thread_type;//currently no uses
    output->dst_addr = input.dst_addr;
    output->out_sig = input.out_sig;
    output->wen = input.wen;
	output->CMEM_Waddr = input.imm_value&0x1ff;//9-bits
	output->CMEM_Wdata = input.old_DST_data.x;
	output->is_FRAG_start = input.is_FRAG_start;
	output->FRAG_mode = input.FRAG_mode;
    
	//CMEM_Wen
	if(input.opcode == ST)
	   output->CMEM_Wen = 1;
	else
	   output->CMEM_Wen = 0;
	   
	//MASK
	if((input.W_mask&0x8) == 8)
	   output->Wdata.x = input.DST_data.x; 
	else
	   output->Wdata.x = input.old_DST_data.x; 

	if((input.W_mask&0x4) == 4)
	   if(input.opcode == DP4)
	      output->Wdata.y = input.DST_data.x;
       else
          output->Wdata.y = input.DST_data.y;	   
	else
	   output->Wdata.y = input.old_DST_data.y;
	   
	if((input.W_mask&0x2) == 2)
	   if(input.opcode == DP4)
	      output->Wdata.z = input.DST_data.x;
       else
          output->Wdata.z = input.DST_data.z;	   
	else
	   output->Wdata.z = input.old_DST_data.z;

	if((input.W_mask&0x1) == 1)
	   if(input.opcode == DP4)
	      output->Wdata.w = input.DST_data.x;
       else
          output->Wdata.w = input.DST_data.w;	   
	else
	   output->Wdata.w = input.old_DST_data.w;	   
	//----------------------------------------

	#if DEBUG
	if(input.wen == 1 && SYS_finish == 36)
	{
	    fprintf(dump,"opcode = %d\n",input.opcode);
	    fprintf(dump,"Tid = %d\n",input.thread_ID);
	    fprintf(dump,"Wen = %d\n",input.wen);
	    fprintf(dump,"out_sig = %d\n",input.out_sig);

	    fprintf(dump,"dst_addr = %d\n",input.dst_addr);
	    fprintf(dump,"Wdata(X) = %f\n",output->Wdata.x);
	    fprintf(dump,"Wdata(Y) = %f\n",output->Wdata.y);
	    fprintf(dump,"Wdata(Z) = %f\n",output->Wdata.z);
	    fprintf(dump,"Wdata(W) = %f\n",output->Wdata.w);
	}
	#endif
	//----------------------------------------
	
}

void mov(struct vec4 SRC,struct vec4 *DST)
{
    (*DST).x = SRC.x;
	(*DST).y = SRC.y;
	(*DST).z = SRC.z;
	(*DST).w = SRC.w;
}

void mvi(unsigned int imm_value,struct vec4 *DST)
{	
    float temp = *((float*)&imm_value);

    (*DST).x = temp;
	(*DST).y = temp;
	(*DST).z = temp;
	(*DST).w = temp;
}

void max(int FXL,struct vec4 SRC0,struct vec4 SRC1,struct vec4 *DST)
{
    int temp1,temp2;
	
    if(FXL)
	{
	    if(SRC0.x > SRC1.x)
		   (*DST).x = SRC0.x;
		else
		   (*DST).x = SRC1.x;
		   
	    if(SRC0.y > SRC1.y)
		   (*DST).y = SRC0.y;
		else
		   (*DST).y = SRC1.y;
		   
	    if(SRC0.z > SRC1.z)
		   (*DST).z = SRC0.z;
		else
		   (*DST).z = SRC1.z;
		   
	    if(SRC0.w > SRC1.w)
		   (*DST).w = SRC0.w;
		else
		   (*DST).w = SRC1.w;		   
	}
	else
	{
	    temp1 = *((int*)&SRC0.x);
		temp2 = *((int*)&SRC1.x);
		if(temp1 > temp2)
		   (*DST).x = SRC0.x;
		else
		   (*DST).x = SRC1.x;
		   
	    temp1 = *((int*)&SRC0.y);
		temp2 = *((int*)&SRC1.y);
		if(temp1 > temp2)
		   (*DST).y = SRC0.y;
		else
		   (*DST).y = SRC1.y;

	    temp1 = *((int*)&SRC0.z);
		temp2 = *((int*)&SRC1.z);
		if(temp1 > temp2)
		   (*DST).z = SRC0.z;
		else
		   (*DST).z = SRC1.z;

	    temp1 = *((int*)&SRC0.w);
		temp2 = *((int*)&SRC1.w);
		if(temp1 > temp2)
		   (*DST).w = SRC0.w;
		else
		   (*DST).w = SRC1.w;		   
	}
}

void min(int FXL,struct vec4 SRC0,struct vec4 SRC1,struct vec4 *DST)
{
    int temp1,temp2;
	
    if(FXL)
	{
	    if(SRC0.x < SRC1.x)
		   (*DST).x = SRC0.x;
		else
		   (*DST).x = SRC1.x;
		   
	    if(SRC0.y < SRC1.y)
		   (*DST).y = SRC0.y;
		else
		   (*DST).y = SRC1.y;
		   
	    if(SRC0.z < SRC1.z)
		   (*DST).z = SRC0.z;
		else
		   (*DST).z = SRC1.z;
		   
	    if(SRC0.w < SRC1.w)
		   (*DST).w = SRC0.w;
		else
		   (*DST).w = SRC1.w;		   
	}
	else
	{
	    temp1 = *((int*)&SRC0.x);
		temp2 = *((int*)&SRC1.x);
		if(temp1 < temp2)
		   (*DST).x = SRC0.x;
		else
		   (*DST).x = SRC1.x;
		   
	    temp1 = *((int*)&SRC0.y);
		temp2 = *((int*)&SRC1.y);
		if(temp1 < temp2)
		   (*DST).y = SRC0.y;
		else
		   (*DST).y = SRC1.y;

	    temp1 = *((int*)&SRC0.z);
		temp2 = *((int*)&SRC1.z);
		if(temp1 < temp2)
		   (*DST).z = SRC0.z;
		else
		   (*DST).z = SRC1.z;

	    temp1 = *((int*)&SRC0.w);
		temp2 = *((int*)&SRC1.w);
		if(temp1 < temp2)
		   (*DST).w = SRC0.w;
		else
		   (*DST).w = SRC1.w;		   
	}
}

void and(struct vec4 SRC0,struct vec4 SRC1,struct vec4 *DST)
{
	unsigned int temp;
	
	temp = *((unsigned int*)&SRC0.x) & *((unsigned int*)&SRC1.x);
	(*DST).x = *((float*)&temp);
	
	temp = *((unsigned int*)&SRC0.y) & *((unsigned int*)&SRC1.y);
	(*DST).y = *((float*)&temp);
	
	temp = *((unsigned int*)&SRC0.z) & *((unsigned int*)&SRC1.z);
	(*DST).z = *((float*)&temp);
	
	temp = *((unsigned int*)&SRC0.w) & *((unsigned int*)&SRC1.w);
	(*DST).w = *((float*)&temp);	
}

void or(struct vec4 SRC0,struct vec4 SRC1,struct vec4 *DST)
{
	unsigned int temp;
	
	temp = *((unsigned int*)&SRC0.x) | *((unsigned int*)&SRC1.x);
	(*DST).x = *((float*)&temp);
	
	temp = *((unsigned int*)&SRC0.y) | *((unsigned int*)&SRC1.y);
	(*DST).y = *((float*)&temp);
	
	temp = *((unsigned int*)&SRC0.z) | *((unsigned int*)&SRC1.z);
	(*DST).z = *((float*)&temp);
	
	temp = *((unsigned int*)&SRC0.w) | *((unsigned int*)&SRC1.w);
	(*DST).w = *((float*)&temp);	
}

void xor(struct vec4 SRC0,struct vec4 SRC1,struct vec4 *DST)
{
	unsigned int temp;
	
	temp = *((unsigned int*)&SRC0.x) ^ *((unsigned int*)&SRC1.x);
	(*DST).x = *((float*)&temp);
	
	temp = *((unsigned int*)&SRC0.y) ^ *((unsigned int*)&SRC1.y);
	(*DST).y = *((float*)&temp);
	
	temp = *((unsigned int*)&SRC0.z) ^ *((unsigned int*)&SRC1.z);
	(*DST).z = *((float*)&temp);
	
	temp = *((unsigned int*)&SRC0.w) ^ *((unsigned int*)&SRC1.w);
	(*DST).w = *((float*)&temp);
}

void inv(struct vec4 SRC,struct vec4 *DST)
{
    unsigned int temp;
	
	temp = ~(*((unsigned int*)&SRC.x));
    (*DST).x = *((float*)&temp);

	temp = ~(*((unsigned int*)&SRC.y));
    (*DST).y = *((float*)&temp);

	temp = ~(*((unsigned int*)&SRC.z));
    (*DST).z = *((float*)&temp);

	temp = ~(*((unsigned int*)&SRC.w));
    (*DST).w = *((float*)&temp);	
}

void shrl(int RL,int sa,struct vec4 SRC,struct vec4 *DST)
{
    unsigned int temp1,temp2,temp3,temp4;
	
	temp1 = *((unsigned int*)&SRC.x);
	temp2 = *((unsigned int*)&SRC.y);
	temp3 = *((unsigned int*)&SRC.z);
	temp4 = *((unsigned int*)&SRC.w);
	
	if(RL)
	{
	    temp1 = temp1 << sa;
		temp2 = temp2 << sa;
		temp3 = temp3 << sa;
		temp4 = temp4 << sa;
	}
	else
	{
	    temp1 = temp1 >> sa;
		temp2 = temp2 >> sa;
		temp3 = temp3 >> sa;
		temp4 = temp4 >> sa;	
	}
	
	(*DST).x = *((float*)&temp1);
	(*DST).y = *((float*)&temp2);
	(*DST).z = *((float*)&temp3);
	(*DST).w = *((float*)&temp4);	
}

int RCP_factor(float X0,float *a0,float *a1,float *Y_rcp)
{
	int exp;
	double fraction;
    double msb,lsb;
    
	fraction = frexp(X0,&exp);
    fraction *=2;
    exp--;
    lsb = modf(fraction*4096,&msb);
    msb /= 4096;
    lsb /= 4096;
    *a0 = (float)1/msb;
    *a1 = (float)(-1 / pow(msb , 2.0));
    *Y_rcp = (float)lsb;

    return (-exp);
}


int RSQ_factor(float X0,float *b0,float *b1,float *Y_rsq)
{
	int exp;
	double fraction;
    double msb,lsb;

    fraction = frexp(X0,&exp);
    if(exp%2 != 0)
    {
        fraction*=2;
        exp--;
    }
    lsb = modf(fraction*4096,&msb);
    msb /= 4096;
    lsb /= 4096;
    *b0 = (float)(1/sqrt(msb));
    *b1 = (float)(-0.5 * pow(msb , 1.5));
    *Y_rsq = (float)lsb;

    return (-exp/2);
}


int POW2_factor(float X0,float *c0,float *c1,float *Y_pow2)
{
	double exp;
	double fraction;
    double msb,lsb;

    fraction = modf(X0,&exp);//X0 = exp.fraction
    lsb = modf(fraction*4096,&msb);
    msb /= 4096;
    lsb /= 4096;
    *c0 = (float)(pow(2.0,msb));
    *c1 = (float)(log(2) * (*c0));
    *Y_pow2 = (float)lsb;

    return (int)exp;
}


int LOG2_factor(float X0,float *d0,float *d1,float *Y_log2)
{
	int    exp;
	double fraction;
    double msb,lsb;

    fraction = frexp(X0,&exp);
    fraction *=2;
    exp--;
    lsb = modf(fraction*4096,&msb);
    msb /= 4096;
    lsb /= 4096;
    *d0 = (log(msb)/log(2.0)) + exp;//add EXP
    *d1 = (float)(1 / ( log(2.0) * msb));    
    *Y_log2 = (float)lsb;

    return 0;//2^0=1
}


void MAD_factor(unsigned int opcode,int FXL,struct vec4 *TEX_data,struct MAD_list *list)
{
    int i;
	float TEX[4];
	int EXP_diff;
	int EXP1,EXP2,EXP3;
	float SRC1,SRC2,SRC3;
	float fraction1,fraction2,fraction3;

	TEX[0] = TEX_data->x;
    TEX[1] = TEX_data->y;
    TEX[2] = TEX_data->z;
    TEX[3] = TEX_data->w;	  
	  
	  for(i=0;i<4;++i)
	  {
	      switch(opcode)
	      {
	      	  case DP3: if(i<3)
	      	  	        {
	      	  	            SRC1 = list->MAD_input[i];
							SRC2 = list->MAD_input[i+4];
							SRC3 = 0;
	      	  	        }
	      	  	        else
	      	  	        {
	      	  	            SRC1 = 0;
							SRC2 = 0;
							SRC3 = 0;
	      	  	        }
	      	  	        break;
	  	      case TXB: if(i<3)
	  	      	        {
	  	      	            SRC1 = TEX[i];
							SRC2 = 1;
							SRC3 = TEX[3];
	  	      	        }
	  	      	        else
	  	      	        {
	  	      	       	    SRC1 = TEX[i];
							SRC2 = 2;
							SRC3 = TEX[3];
	  	      	        }
	  	      	        break;
	          case FADD: SRC1 = list->MAD_input[i]; 
			             SRC2 = 1; 
						 SRC3 = list->MAD_input[i+4];
						 /*
						 fprintf(dump,"SRC1(%d) = %f\n",i,SRC1);
						 fprintf(dump,"SRC2(%d) = %f\n",i,SRC2);
						 fprintf(dump,"SRC3(%d) = %f\n",i,SRC3);
						 */
	          	         break;
	          case FMUL: SRC1 = list->MAD_input[i];
			             SRC2 = list->MAD_input[i+4];
						 SRC3 = 0;
	          	         break;
	          case FMAD: SRC1 = list->MAD_input[i];
			             SRC2 = list->MAD_input[i+4];
						 SRC3 = list->MAD_input[i+8];
	      	             break;
	      	  case DP4: SRC1 = list->MAD_input[i];
			            SRC2 = list->MAD_input[i+4];
						SRC3 = 0;
	      	  	        break;
	          default: SRC1 = 0;
			           SRC2 = 0;
					   SRC3 = 0;
	      	           break;	      	           	           
	      }
	      if(FXL)//floating point
	      {	   
	          if(SRC1 == 0)
	      	  {
	      	   	  fraction1 = 0;
	      	   	  EXP1 = 0;
	      	  }
	      	  else
	      	  {   	   
                  fraction1 = frexp(SRC1,&EXP1);
                  fraction1 *= 2;
                  EXP1--;
              }  
              if(SRC2 == 0)
	      	  {
	      	   	  fraction2 = 0;
	      	   	  EXP2 = 0;
	      	  }   
	      	  else
	      	  {       	           
                  fraction2 = frexp(SRC2,&EXP2);
                  fraction2 *= 2;
                  EXP2--;                        
              }
              if(SRC3 == 0)
	      	  {
	      	   	  fraction3 = 0;
	      	   	  EXP3 = 0;
	      	  }   
	      	  else
	      	  {          
                  fraction3 = frexp(SRC3,&EXP3);
                  fraction3 *= 2;
                  EXP3--;
              }
              
			  /*
			  fprintf(dump,"fraction = %f , %f , %f\n",fraction1,fraction2,fraction3);
			  fprintf(dump,"EXP = %d , %d , %d\n",EXP1,EXP2,EXP3);
			  */
			  
              EXP_diff = (EXP1 + EXP2) - EXP3;
              if(EXP_diff >= 0)
              {
                  list->MAD_EXP[i] = EXP1 + EXP2;
                  list->MAD_SRC[i*3] = fraction1;
	              list->MAD_SRC[i*3+1] = fraction2;
	              list->MAD_SRC[i*3+2] = fraction3/pow(2.0,(double)EXP_diff);	               
              }
              else
              {
                  list->MAD_EXP[i] = EXP3;
                  list->MAD_SRC[i*3] = fraction1/pow(2.0,ceil(-(double)EXP_diff/2));
	              list->MAD_SRC[i*3+1] = fraction2/pow(2.0,floor(-(double)EXP_diff/2));
	              list->MAD_SRC[i*3+2] = fraction3;	               
              }
			  
	          if(opcode == TXB)
	           	 list->MAD_SRC[i*3+2] = -(list->MAD_SRC[i*3+2]);	        
	      }	
	      else //fixed-point
	      {
	      	  list->MAD_EXP[i] = 0;
			  list->MAD_SRC[i*3] = (float)*((int*)&SRC1);
			  list->MAD_SRC[i*3+1] = (float)*((int*)&SRC2);
			  list->MAD_SRC[i*3+2] = (float)*((int*)&SRC3);
	      }
	  }
	  
	  /*
	  for(i=0;i<4;++i)
	  {
		  fprintf(dump,"MAD_EXP = %d\n",list->MAD_EXP[i]);
		  fprintf(dump,"MAD_SRC1 = %f\n",list->MAD_SRC[i*3]);
		  fprintf(dump,"MAD_SRC2 = %f\n",list->MAD_SRC[i*3+1]);
		  fprintf(dump,"MAD_SRC3 = %f\n",list->MAD_SRC[i*3+2]);	  
	  }
	  */
}


void pipe_SRC_select(unsigned int opcode,struct specail_list *S_list,struct MAD_list list,struct scalar_pipeline *pipe)
{
    int i;


	switch(opcode)
	{
		case RCP: pipe[0].pipe_EXP = S_list->exp_rcp;
		          pipe[0].pipe_X = S_list->a1;
				  pipe[0].pipe_Y = S_list->Y_rcp;
				  pipe[0].pipe_Z = S_list->a0;
				  pipe[1].pipe_EXP = S_list->exp_rcp;
		          pipe[1].pipe_X = S_list->a1;
				  pipe[1].pipe_Y = S_list->Y_rcp;
				  pipe[1].pipe_Z = S_list->a0;
				  pipe[2].pipe_EXP = S_list->exp_rcp;
		          pipe[2].pipe_X = S_list->a1;
				  pipe[2].pipe_Y = S_list->Y_rcp;
				  pipe[2].pipe_Z = S_list->a0;
				  pipe[3].pipe_EXP = S_list->exp_rcp;
		          pipe[3].pipe_X = S_list->a1;
				  pipe[3].pipe_Y = S_list->Y_rcp;
				  pipe[3].pipe_Z = S_list->a0;				  
				  break;
		case RSQ: pipe[0].pipe_EXP = S_list->exp_rsq;
		          pipe[0].pipe_X = S_list->b1;
				  pipe[0].pipe_Y = S_list->Y_rsq;
				  pipe[0].pipe_Z = S_list->b0;
				  pipe[1].pipe_EXP = S_list->exp_rsq;
		          pipe[1].pipe_X = S_list->b1;
				  pipe[1].pipe_Y = S_list->Y_rsq;
				  pipe[1].pipe_Z = S_list->b0;
				  pipe[2].pipe_EXP = S_list->exp_rsq;
		          pipe[2].pipe_X = S_list->b1;
				  pipe[2].pipe_Y = S_list->Y_rsq;
				  pipe[2].pipe_Z = S_list->b0;
				  pipe[3].pipe_EXP = S_list->exp_rsq;
		          pipe[3].pipe_X = S_list->b1;
				  pipe[3].pipe_Y = S_list->Y_rsq;
				  pipe[3].pipe_Z = S_list->b0;				  
				  break;
		case LOG2: pipe[0].pipe_EXP = S_list->exp_log2;
		           pipe[0].pipe_X = S_list->d1;
				   pipe[0].pipe_Y = S_list->Y_log2;
				   pipe[0].pipe_Z = S_list->d0;
				   pipe[1].pipe_EXP = S_list->exp_log2;
		           pipe[1].pipe_X = S_list->d1;
				   pipe[1].pipe_Y = S_list->Y_log2;
				   pipe[1].pipe_Z = S_list->d0;
				   pipe[2].pipe_EXP = S_list->exp_log2;
		           pipe[2].pipe_X = S_list->d1;
				   pipe[2].pipe_Y = S_list->Y_log2;
				   pipe[2].pipe_Z = S_list->d0;
				   pipe[3].pipe_EXP = S_list->exp_log2;
		           pipe[3].pipe_X = S_list->d1;
				   pipe[3].pipe_Y = S_list->Y_log2;
				   pipe[3].pipe_Z = S_list->d0;				   
				   break;
		case POW2: pipe[0].pipe_EXP = S_list->exp_pow2;
		           pipe[0].pipe_X = S_list->c1;
				   pipe[0].pipe_Y = S_list->Y_pow2;
				   pipe[0].pipe_Z = S_list->c0;
				   pipe[1].pipe_EXP = S_list->exp_pow2;
		           pipe[1].pipe_X = S_list->c1;
				   pipe[1].pipe_Y = S_list->Y_pow2;
				   pipe[1].pipe_Z = S_list->c0;
				   pipe[2].pipe_EXP = S_list->exp_pow2;
		           pipe[2].pipe_X = S_list->c1;
				   pipe[2].pipe_Y = S_list->Y_pow2;
				   pipe[2].pipe_Z = S_list->c0;
				   pipe[3].pipe_EXP = S_list->exp_pow2;
		           pipe[3].pipe_X = S_list->c1;
				   pipe[3].pipe_Y = S_list->Y_pow2;
				   pipe[3].pipe_Z = S_list->c0;				   
				   break;					
		default: pipe[0].pipe_EXP = list.MAD_EXP[0];
		         pipe[0].pipe_X = list.MAD_SRC[0];
				 pipe[0].pipe_Y = list.MAD_SRC[1];
				 pipe[0].pipe_Z = list.MAD_SRC[2];
				 pipe[1].pipe_EXP = list.MAD_EXP[1];
		         pipe[1].pipe_X = list.MAD_SRC[3];
				 pipe[1].pipe_Y = list.MAD_SRC[4];
				 pipe[1].pipe_Z = list.MAD_SRC[5];
				 pipe[2].pipe_EXP = list.MAD_EXP[2];
		         pipe[2].pipe_X = list.MAD_SRC[6];
				 pipe[2].pipe_Y = list.MAD_SRC[7];
				 pipe[2].pipe_Z = list.MAD_SRC[8];
				 pipe[3].pipe_EXP = list.MAD_EXP[3];
		         pipe[3].pipe_X = list.MAD_SRC[9];
				 pipe[3].pipe_Y = list.MAD_SRC[10];
				 pipe[3].pipe_Z = list.MAD_SRC[11];				 
				 break;
	}
		
/*		
	for(i=0;i<4;++i)
	{
	   fprintf(dump,"pipe_X[%d] = %f\n",i,pipe[i].pipe_X);
	   fprintf(dump,"pipe_Y[%d] = %f\n",i,pipe[i].pipe_Y);
	   fprintf(dump,"pipe_Z[%d] = %f\n",i,pipe[i].pipe_Z);
	}
	for(i=0;i<4;++i)
	   fprintf(dump,"pipe_EXP[%d] = %d\n",i,pipe[i].pipe_EXP);
*/	   
}


void DP_op(float *fraction_list,int *EXP_list,struct DP_stg_EX34 *DP_if)
{
    int   DP4_EXP;
	int   exp_diff1,exp_diff2,exp_diff3,exp_diff4;
	float shift_1,shift_2,shift_3,shift_4;
	float DP_out;

    if(EXP_list[0] > EXP_list[1])
	   DP4_EXP = EXP_list[0];
	else
	   DP4_EXP = EXP_list[1];

	if(EXP_list[2] > DP4_EXP)
	   DP4_EXP = EXP_list[2];

	if(EXP_list[3] > DP4_EXP)
	   DP4_EXP = EXP_list[3];

	exp_diff1 = DP4_EXP - EXP_list[0];
	exp_diff2 = DP4_EXP - EXP_list[1];
	exp_diff3 = DP4_EXP - EXP_list[2];
	exp_diff4 = DP4_EXP - EXP_list[3];

	shift_1 = fraction_list[0] / pow(2.0,(double)exp_diff1);
	shift_2 = fraction_list[1] / pow(2.0,(double)exp_diff2);
	shift_3 = fraction_list[2] / pow(2.0,(double)exp_diff3);
	shift_4 = fraction_list[3] / pow(2.0,(double)exp_diff4);
	
	DP_if->DP_12_plus = shift_1+shift_2;
	DP_if->DP_34_plus = shift_3+shift_4;
	DP_if->Emax = DP4_EXP;
}


void out_select(struct EX4_EX5_if EX45_if,float *RLT)
{	
    float DP_result = EX45_if.DP_fraction * pow(2.0,(double)EX45_if.DP_EXP);
    
    switch(EX45_if.opcode_pipe)
    {
        case MOV: RLT[0] = EX45_if.MOV_pipe.x;
		          RLT[1] = EX45_if.MOV_pipe.y;
				  RLT[2] = EX45_if.MOV_pipe.z;
				  RLT[3] = EX45_if.MOV_pipe.w;
        	      break;
        case LD: RLT[0] = EX45_if.CMEM_Rdata_pipe;
        	     RLT[1] = EX45_if.CMEM_Rdata_pipe;
        	     RLT[2] = EX45_if.CMEM_Rdata_pipe;
        	     RLT[3] = EX45_if.CMEM_Rdata_pipe;
        	     break;        	      
        case MVI: RLT[0] = EX45_if.MVI_pipe.x;
		          RLT[1] = EX45_if.MVI_pipe.y;
				  RLT[2] = EX45_if.MVI_pipe.z;
				  RLT[3] = EX45_if.MVI_pipe.w;				 
        	      break;
        case MIN: RLT[0] = EX45_if.MIN_pipe.x;
		          RLT[1] = EX45_if.MIN_pipe.y;
				  RLT[2] = EX45_if.MIN_pipe.z;
				  RLT[3] = EX45_if.MIN_pipe.w;
        	      break;
        case MAX: RLT[0] = EX45_if.MAX_pipe.x;
		          RLT[1] = EX45_if.MAX_pipe.y;
				  RLT[2] = EX45_if.MAX_pipe.z;
				  RLT[3] = EX45_if.MAX_pipe.w;
        	      break;
        case TEX2D: RLT[0] = EX45_if.TEX_data_pipe.x;
        	        RLT[1] = EX45_if.TEX_data_pipe.y;
        	        RLT[2] = EX45_if.TEX_data_pipe.z;
        	        RLT[3] = EX45_if.TEX_data_pipe.w;         	                              	       
        	        break;
        case INV: RLT[0] = EX45_if.INV_pipe.x;
        	      RLT[1] = EX45_if.INV_pipe.y;
        	      RLT[2] = EX45_if.INV_pipe.z;
        	      RLT[3] = EX45_if.INV_pipe.w;
        	      break;				 
        case AND: RLT[0] = EX45_if.AND_pipe.x;
        	      RLT[1] = EX45_if.AND_pipe.y;
        	      RLT[2] = EX45_if.AND_pipe.z;
        	      RLT[3] = EX45_if.AND_pipe.w;
        	      break;
        case OR: RLT[0] = EX45_if.OR_pipe.x;
        	     RLT[1] = EX45_if.OR_pipe.y;
        	     RLT[2] = EX45_if.OR_pipe.z;
        	     RLT[3] = EX45_if.OR_pipe.w;
        	     break;
        case XOR: RLT[0] = EX45_if.XOR_pipe.x;
        	      RLT[1] = EX45_if.XOR_pipe.y;
        	      RLT[2] = EX45_if.XOR_pipe.z;
        	      RLT[3] = EX45_if.XOR_pipe.w;
        	      break;				 
        case SHRL: RLT[0] = EX45_if.SHRL_pipe.x;
        	       RLT[1] = EX45_if.SHRL_pipe.y;
        	       RLT[2] = EX45_if.SHRL_pipe.z;
        	       RLT[3] = EX45_if.SHRL_pipe.w;
        	       break;        	       
        default: if(EX45_if.opcode_pipe == DP3 || EX45_if.opcode_pipe == DP4)
		         {
		             RLT[0] = DP_result;
        	         RLT[1] = DP_result;
        	         RLT[2] = DP_result;
        	         RLT[3] = DP_result;				 
				 }
		         else
				 {
		             RLT[0] = EX45_if.pipe_out[0];
        	         RLT[1] = EX45_if.pipe_out[1];
        	         RLT[2] = EX45_if.pipe_out[2];
        	         RLT[3] = EX45_if.pipe_out[3];
        	     }
				 break;
    }
}


void EXE(struct READ_EX1_if input,struct EX5_WB_if *output)
{
    int    i;
	struct MAD_list list;
	struct scalar_pipeline scalar_pipe[4];
	float  DP_fraction[4];
	float  EXE_result[4];

	
	//---------------------------------------------------------
	//[EX5]
    out_select(stg7_8,EXE_result);

    output->thread_ID = stg7_8.thread_ID_pipe;
	output->thread_type = stg7_8.thread_type_pipe;
	output->opcode = stg7_8.opcode_pipe;
	output->dst_addr = stg7_8.dst_addr_pipe;
	output->DST_data.x = EXE_result[0];
	output->DST_data.y = EXE_result[1];
	output->DST_data.z = EXE_result[2];
	output->DST_data.w = EXE_result[3];
	mov(stg7_8.old_DST_data_pipe,&output->old_DST_data);
	output->out_sig = stg7_8.out_sig_pipe;
	output->wen = stg7_8.wen_pipe;
	output->W_mask = stg7_8.W_mask_pipe;
	output->imm_value = stg7_8.imm_value_pipe;
	output->is_FRAG_start = stg7_8.is_FRAG_start_pipe;
	output->FRAG_mode = stg7_8.FRAG_mode_pipe;
	
	//release [RAW] stall
	for(i=0;i<14;++i)
	{
	    if(thread_ctrl.info[i].Ttype != NO_THREAD)
		{
	        if(thread_ctrl.info[i].Tstate == STALL && thread_ctrl.info[i].stall_flag == DATA_DEP)
			{
			    thread_ctrl.info[i].RAW_count--;
				if(thread_ctrl.info[i].RAW_count == 0)
				{
				    //[STALL] -> [READY]
					thread_ctrl.info[i].Tstate = READY;				
				}
			}
		}
	}
/*	
    if(thread_ctrl.info[stg7_8.thread_ID_pipe].Tstate == STALL && thread_ctrl.info[stg7_8.thread_ID_pipe].stall_flag == DATA_DEP)
	{
	    if(stg)
		thread_ctrl.info[stg7_8.thread_ID_pipe].RAW_count--;
		
		if(thread_ctrl.info[stg7_8.thread_ID_pipe].RAW_count == 0)
		{
		    //[STALL] -> [READY]
		    thread_ctrl.info[stg7_8.thread_ID_pipe].Tstate = READY;
		}
	}
*/
    
	#if DEBUG
	if(SYS_finish == 36)
	{
	    fprintf(dump,"[EX5]\n");
	    fprintf(dump,"========================\n");
	
	    fprintf(dump,"Tid = %d\n",stg7_8.thread_ID_pipe);
	    fprintf(dump,"opcode = %d\n",stg7_8.opcode_pipe);
	    fprintf(dump,"dst_addr = %d\n",stg7_8.dst_addr_pipe);
    }
	#endif
	//---------------------------------------------------------
	
	//---------------------------------------------------------
	//[EX4]
	for(i=0;i<4;++i)
	   stg7_8.pipe_out[i] = stg6_7.pipe_MAD[i] * pow(2.0,(double)stg6_7.pipe_EXP[i]);
    
    stg7_8.DP_fraction = stg6_7.DP_stg34.DP_12_plus + stg6_7.DP_stg34.DP_34_plus;
    stg7_8.DP_EXP = stg6_7.DP_stg34.Emax;	
	
	//for [RAW]
	EXE_out.Tid_EXE[3] = stg6_7.thread_ID_pipe;
	EXE_out.wen_EXE[3] = stg6_7.wen_pipe; 
	EXE_out.out_sig_EXE[3] = stg6_7.out_sig_pipe;
	EXE_out.dst_addr_EXE[3] = stg6_7.dst_addr_pipe;
	
	stg7_8.thread_ID_pipe = stg6_7.thread_ID_pipe;
	stg7_8.thread_type_pipe = stg6_7.thread_type_pipe;
	stg7_8.opcode_pipe = stg6_7.opcode_pipe;
	stg7_8.dst_addr_pipe = stg6_7.dst_addr_pipe;
	stg7_8.out_sig_pipe = stg6_7.out_sig_pipe;
	stg7_8.wen_pipe = stg6_7.wen_pipe;
	stg7_8.W_mask_pipe = stg6_7.W_mask_pipe;
	stg7_8.FXL_pipe = stg6_7.FXL_pipe;
	stg7_8.imm_value_pipe = stg6_7.imm_value_pipe;
	stg7_8.TEX_data_pipe = stg6_7.TEX_data_pipe;
	stg7_8.CMEM_Rdata_pipe = stg6_7.CMEM_Rdata_pipe;
	stg7_8.is_FRAG_start_pipe = stg6_7.is_FRAG_start_pipe;
	stg7_8.FRAG_mode_pipe = stg6_7.FRAG_mode_pipe;
	
	mov(stg6_7.old_DST_data_pipe,&stg7_8.old_DST_data_pipe);
	mov(stg6_7.MOV_pipe,&stg7_8.MOV_pipe);
	mov(stg6_7.MVI_pipe,&stg7_8.MVI_pipe);
	mov(stg6_7.MAX_pipe,&stg7_8.MAX_pipe);
	mov(stg6_7.MIN_pipe,&stg7_8.MIN_pipe);
	mov(stg6_7.AND_pipe,&stg7_8.AND_pipe);
	mov(stg6_7.OR_pipe,&stg7_8.OR_pipe);
	mov(stg6_7.XOR_pipe,&stg7_8.XOR_pipe);
	mov(stg6_7.INV_pipe,&stg7_8.INV_pipe);
	mov(stg6_7.SHRL_pipe,&stg7_8.SHRL_pipe);

    #if DEBUG
	if(SYS_finish == 36)
	{
	    fprintf(dump,"[EX4]\n");
	    fprintf(dump,"========================\n");
	
	    fprintf(dump,"Tid = %d\n",stg6_7.thread_ID_pipe);
	    fprintf(dump,"opcode = %d\n",stg6_7.opcode_pipe);
	    fprintf(dump,"dst_addr = %d\n",stg6_7.dst_addr_pipe);
    }
	#endif
	//---------------------------------------------------------
	
	//---------------------------------------------------------
	//[EX3]
	for(i=0;i<4;++i)
	{
	    DP_fraction[i] = stg5_6.pipe_MUL[i] + stg5_6.pipe_ADD[i];
	}
	
	DP_op(DP_fraction,stg5_6.pipe_EXP,&stg6_7.DP_stg34);
	
	for(i=0;i<4;++i)
	{
		stg6_7.pipe_MAD[i] = DP_fraction[i];
		stg6_7.pipe_EXP[i] = stg5_6.pipe_EXP[i];
	}

	//for [RAW]
	EXE_out.Tid_EXE[2] = stg5_6.thread_ID_pipe;
	EXE_out.wen_EXE[2] = stg5_6.wen_pipe; 
	EXE_out.out_sig_EXE[2] = stg5_6.out_sig_pipe;
	EXE_out.dst_addr_EXE[2] = stg5_6.dst_addr_pipe;
	
	stg6_7.thread_ID_pipe = stg5_6.thread_ID_pipe;
	stg6_7.thread_type_pipe = stg5_6.thread_type_pipe;
	stg6_7.opcode_pipe = stg5_6.opcode_pipe;
	stg6_7.dst_addr_pipe = stg5_6.dst_addr_pipe;
	stg6_7.out_sig_pipe = stg5_6.out_sig_pipe;
	stg6_7.wen_pipe = stg5_6.wen_pipe;
	stg6_7.W_mask_pipe = stg5_6.W_mask_pipe;
	stg6_7.FXL_pipe = stg5_6.FXL_pipe;
	stg6_7.imm_value_pipe = stg5_6.imm_value_pipe;
	stg6_7.TEX_data_pipe = stg5_6.TEX_data_pipe;
	stg6_7.CMEM_Rdata_pipe = stg5_6.CMEM_Rdata_pipe;
	stg6_7.is_FRAG_start_pipe = stg5_6.is_FRAG_start_pipe;
	stg6_7.FRAG_mode_pipe = stg5_6.FRAG_mode_pipe;
	
	mov(stg5_6.old_DST_data_pipe,&stg6_7.old_DST_data_pipe);
	mov(stg5_6.MOV_pipe,&stg6_7.MOV_pipe);
	mov(stg5_6.MVI_pipe,&stg6_7.MVI_pipe);
	mov(stg5_6.MAX_pipe,&stg6_7.MAX_pipe);
	mov(stg5_6.MIN_pipe,&stg6_7.MIN_pipe);
	mov(stg5_6.AND_pipe,&stg6_7.AND_pipe);
	mov(stg5_6.OR_pipe,&stg6_7.OR_pipe);
	mov(stg5_6.XOR_pipe,&stg6_7.XOR_pipe);
	mov(stg5_6.INV_pipe,&stg6_7.INV_pipe);
	mov(stg5_6.SHRL_pipe,&stg6_7.SHRL_pipe);

    #if DEBUG
	if(SYS_finish == 36)
	{
	    fprintf(dump,"[EX3]\n");
	    fprintf(dump,"========================\n");
	
	    fprintf(dump,"Tid = %d\n",stg5_6.thread_ID_pipe);
	    fprintf(dump,"opcode = %d\n",stg5_6.opcode_pipe);
	    fprintf(dump,"dst_addr = %d\n",stg5_6.dst_addr_pipe);
	}
    #endif	
	//---------------------------------------------------------
	
	//---------------------------------------------------------
	//[EX2]
	list.MAD_input[0] = stg4_5.SRC0_data_pipe.x;
	list.MAD_input[1] = stg4_5.SRC0_data_pipe.y;
	list.MAD_input[2] = stg4_5.SRC0_data_pipe.z;
	list.MAD_input[3] = stg4_5.SRC0_data_pipe.w;
	list.MAD_input[4] = stg4_5.SRC1_data_pipe.x;
	list.MAD_input[5] = stg4_5.SRC1_data_pipe.y;
	list.MAD_input[6] = stg4_5.SRC1_data_pipe.z;
	list.MAD_input[7] = stg4_5.SRC1_data_pipe.w;	
	list.MAD_input[8] = stg4_5.SRC2_data_pipe.x;
	list.MAD_input[9] = stg4_5.SRC2_data_pipe.y;
	list.MAD_input[10] = stg4_5.SRC2_data_pipe.z;
	list.MAD_input[11] = stg4_5.SRC2_data_pipe.w;
	
	//ADD,MUL,MAD,DP3,DP4 factors
	MAD_factor(stg4_5.opcode_pipe,stg4_5.FXL_pipe,&stg4_5.TEX_data_pipe,&list);     
	

	/*
	  for(i=0;i<4;++i)
	  {
		  fprintf(dump,"MAD_EXP = %d\n",list.MAD_EXP[i]);
		  fprintf(dump,"MAD_SRC1 = %f\n",list.MAD_SRC[i*3]);
		  fprintf(dump,"MAD_SRC2 = %f\n",list.MAD_SRC[i*3+1]);
		  fprintf(dump,"MAD_SRC3 = %f\n",list.MAD_SRC[i*3+2]);	  
	  }	
	*/
	
	//select pipe source
    pipe_SRC_select(stg4_5.opcode_pipe,&stg4_5.S_list,list,scalar_pipe);
	
	//pipe1~4
	for(i=0;i<4;++i)
	{
	    stg5_6.pipe_MUL[i] = scalar_pipe[i].pipe_X * scalar_pipe[i].pipe_Y;
		stg5_6.pipe_ADD[i] = scalar_pipe[i].pipe_Z;
		stg5_6.pipe_EXP[i] = scalar_pipe[i].pipe_EXP;
	}	

	//for [RAW]
	EXE_out.Tid_EXE[1] = stg4_5.thread_ID_pipe;
	EXE_out.wen_EXE[1] = stg4_5.wen_pipe; 
	EXE_out.out_sig_EXE[1] = stg4_5.out_sig_pipe;
	EXE_out.dst_addr_EXE[1] = stg4_5.dst_addr_pipe;
	
	stg5_6.thread_ID_pipe = stg4_5.thread_ID_pipe;
	stg5_6.thread_type_pipe = stg4_5.thread_type_pipe;
	stg5_6.opcode_pipe = stg4_5.opcode_pipe;
	stg5_6.dst_addr_pipe = stg4_5.dst_addr_pipe;
	stg5_6.out_sig_pipe = stg4_5.out_sig_pipe;
	stg5_6.wen_pipe = stg4_5.wen_pipe;
	stg5_6.W_mask_pipe = stg4_5.W_mask_pipe;
	stg5_6.FXL_pipe = stg4_5.FXL_pipe;
	stg5_6.imm_value_pipe = stg4_5.imm_value_pipe;
	stg5_6.TEX_data_pipe = stg4_5.TEX_data_pipe;
	stg5_6.CMEM_Rdata_pipe = stg4_5.CMEM_Rdata_pipe;
	stg5_6.is_FRAG_start_pipe = stg4_5.is_FRAG_start_pipe;
	stg5_6.FRAG_mode_pipe = stg4_5.FRAG_mode_pipe;
	
	mov(stg4_5.SRC2_data_pipe,&stg5_6.old_DST_data_pipe);
	mov(stg4_5.MOV_pipe,&stg5_6.MOV_pipe);
	mov(stg4_5.MVI_pipe,&stg5_6.MVI_pipe);
	mov(stg4_5.MAX_pipe,&stg5_6.MAX_pipe);
	mov(stg4_5.MIN_pipe,&stg5_6.MIN_pipe);
	mov(stg4_5.AND_pipe,&stg5_6.AND_pipe);
	mov(stg4_5.OR_pipe,&stg5_6.OR_pipe);
	mov(stg4_5.XOR_pipe,&stg5_6.XOR_pipe);
	mov(stg4_5.INV_pipe,&stg5_6.INV_pipe);
	mov(stg4_5.SHRL_pipe,&stg5_6.SHRL_pipe);
	
    #if DEBUG
	if(SYS_finish == 36)
    {    
		fprintf(dump,"[EX2]\n");
	    fprintf(dump,"========================\n");
	
	    fprintf(dump,"Tid = %d\n",stg4_5.thread_ID_pipe);
	    fprintf(dump,"opcode = %d\n",stg4_5.opcode_pipe);
	    fprintf(dump,"dst_addr = %d\n",stg4_5.dst_addr_pipe);
	}
	#endif
/*	
	fprintf(dump,"SRC0.x = %f\n",stg4_5.SRC0_data_pipe.x);
	fprintf(dump,"SRC0.y = %f\n",stg4_5.SRC0_data_pipe.y);
	fprintf(dump,"SRC0.z = %f\n",stg4_5.SRC0_data_pipe.z);
	fprintf(dump,"SRC0.w = %f\n",stg4_5.SRC0_data_pipe.w);
	fprintf(dump,"SRC1.x = %f\n",stg4_5.SRC1_data_pipe.x);
	fprintf(dump,"SRC1.y = %f\n",stg4_5.SRC1_data_pipe.y);
	fprintf(dump,"SRC1.z = %f\n",stg4_5.SRC1_data_pipe.z);
	fprintf(dump,"SRC1.w = %f\n",stg4_5.SRC1_data_pipe.w);
*/    	
	//---------------------------------------------------------	
	
	//---------------------------------------------------------
	//[EX1]
	mov(input.SRC0_data,&stg4_5.MOV_pipe);
    mvi(input.imm_value,&stg4_5.MVI_pipe);
	max(input.FXL,input.SRC0_data,input.SRC1_data,&stg4_5.MAX_pipe);
    min(input.FXL,input.SRC0_data,input.SRC1_data,&stg4_5.MIN_pipe);
	and(input.SRC0_data,input.SRC1_data,&stg4_5.AND_pipe);
	or(input.SRC0_data,input.SRC1_data,&stg4_5.OR_pipe);
	xor(input.SRC0_data,input.SRC1_data,&stg4_5.XOR_pipe);
	inv(input.SRC0_data,&stg4_5.INV_pipe);
	shrl(input.RL,input.sa,input.SRC0_data,&stg4_5.SHRL_pipe);
	
    //RCP,RSQ,POW2,LOG2's factors
    stg4_5.S_list.exp_rcp = RCP_factor(input.SRC0_data.x,&stg4_5.S_list.a0,&stg4_5.S_list.a1,&stg4_5.S_list.Y_rcp);
    stg4_5.S_list.exp_rsq = RSQ_factor(input.SRC0_data.x,&stg4_5.S_list.b0,&stg4_5.S_list.b1,&stg4_5.S_list.Y_rsq);
    stg4_5.S_list.exp_pow2 = POW2_factor(input.SRC0_data.x,&stg4_5.S_list.c0,&stg4_5.S_list.c1,&stg4_5.S_list.Y_pow2);
    stg4_5.S_list.exp_log2 = LOG2_factor(input.SRC0_data.x,&stg4_5.S_list.d0,&stg4_5.S_list.d1,&stg4_5.S_list.Y_log2);

	//for [RAW]
	EXE_out.Tid_EXE[0] = input.thread_ID;
	EXE_out.wen_EXE[0] = input.wen; 
	EXE_out.out_sig_EXE[0] = input.out_sig;
	EXE_out.dst_addr_EXE[0] = input.dst_addr;
	
	stg4_5.thread_ID_pipe = input.thread_ID;
	stg4_5.thread_type_pipe = input.thread_type;
	stg4_5.opcode_pipe = input.opcode;
	stg4_5.dst_addr_pipe = input.dst_addr;
	stg4_5.out_sig_pipe = input.out_sig;
	stg4_5.wen_pipe = input.wen;
	stg4_5.W_mask_pipe = input.W_mask;
	stg4_5.FXL_pipe = input.FXL;
	stg4_5.imm_value_pipe = input.imm_value;
	stg4_5.TEX_data_pipe = input.TEX_data;
	stg4_5.CMEM_Rdata_pipe = input.CMEM_Rdata;
	stg4_5.is_FRAG_start_pipe = input.is_FRAG_start;
	stg4_5.FRAG_mode_pipe = input.FRAG_mode;
	stg4_5.SRC0_data_pipe = input.SRC0_data;
	stg4_5.SRC1_data_pipe = input.SRC1_data;
	stg4_5.SRC2_data_pipe = input.SRC2_data;
	//---------------------------------------------------------
    
	#if DEBUG
	if(SYS_finish == 36)
	{
	   fprintf(dump,"[EX1]\n");
	   fprintf(dump,"========================\n");
	
	   fprintf(dump,"Tid = %d\n",input.thread_ID);
	   fprintf(dump,"opcode = %d\n",input.opcode);
	   fprintf(dump,"dst_addr = %d\n",input.dst_addr);
	}
    #endif
/*	
	fprintf(dump,"SRC0.x = %f\n",input.SRC0_data.x);
	fprintf(dump,"SRC0.y = %f\n",input.SRC0_data.y);
	fprintf(dump,"SRC0.z = %f\n",input.SRC0_data.z);
	fprintf(dump,"SRC0.w = %f\n",input.SRC0_data.w);
	fprintf(dump,"SRC1.x = %f\n",input.SRC1_data.x);
	fprintf(dump,"SRC1.y = %f\n",input.SRC1_data.y);
	fprintf(dump,"SRC1.z = %f\n",input.SRC1_data.z);
	fprintf(dump,"SRC1.w = %f\n",input.SRC1_data.w);
*/
}


void SWZ(int *neg,int *swizzle,struct vec4 *SRC)
{ 	
	int   i;
	float x_ori,y_ori,z_ori,w_ori;
	  
    for(i=0;i<3;++i) 
    {
        if(neg[i])
        {
            SRC[i].x = -SRC[i].x;
			SRC[i].y = -SRC[i].y;
			SRC[i].z = -SRC[i].z;
			SRC[i].w = -SRC[i].w;
        }
        
		x_ori = SRC[i].x;
		y_ori = SRC[i].y;
		z_ori = SRC[i].z;
		w_ori = SRC[i].w;
		
        //X swizzle
        switch((swizzle[i] & 0xc0)>>6)
        {
            case 0:	 SRC[i].x = x_ori; break;
            case 1:  SRC[i].x = y_ori; break;
            case 2:  SRC[i].x = z_ori; break;
            default: SRC[i].x = w_ori; break;
        }
        
        //Y swizzle
        switch((swizzle[i] & 0x30)>>4)
        {
            case 0:	 SRC[i].y = x_ori; break;
            case 1:  SRC[i].y = y_ori; break;
            case 2:  SRC[i].y = z_ori; break;
            default: SRC[i].y = w_ori; break;
        }
        
        //Z swizzle
        switch((swizzle[i] & 0x0c)>>2)
        {
            case 0:	 SRC[i].z = x_ori; break;
            case 1:  SRC[i].z = y_ori; break;
            case 2:  SRC[i].z = z_ori; break;
            default: SRC[i].z = w_ori; break;
        }
        
        //W swizzle
        switch(swizzle[i] & 0x03)
        {
            case 0:	 SRC[i].w = x_ori; break;
            case 1:  SRC[i].w = y_ori; break;
            case 2:  SRC[i].w = z_ori; break;
            default: SRC[i].w = w_ori; break;
        }                            
    }    	  
}


void SRC_vec(int SV,struct vec4 *SRC)
{
    int i;
	
	for(i=0;i<3;++i)
	{
	    switch(SV)
		{
		    case 0: SRC[i].y = 0;			
			        SRC[i].z = 0;
					SRC[i].w = 0;
					break;
			case 1: SRC[i].z = 0;			
			        SRC[i].w = 0;
					break;			
			case 2: SRC[i].w = 0;
					break;
		    default: //printf("SRC no change\n");
			         break;
		}
	}
}


int TEX_cache(int request,float u,float v)
{
	static int   block_offset,block_index;
	static int   block_Stag,block_Ttag;
    int          s,t;		
	int          temp;
	int          BUS_S,BUS_T,BUS_offset;

    
	if(request == 0)
	   return 0;
	else
	{
	    if(T_cache.state == 0)
	    {	
	        s = (int)(u * TEX_WIDTH);
	        t = (int)(v * TEX_HEIGHT);	    
	        
			//printf("(S,T) = (%d , %d)\n",s,t);
			
	        //calculate  offset , index , tag
	        block_offset = (s%4) + (t%4) * 4;
	        block_index = (s/4)%2 + ((t/4)%2)*2;
	        block_Stag = s/8;
	        block_Ttag = t/8;
	
	        //valid cmp
            if(T_cache.valid[block_index] == 1)
	        {
		        if((T_cache.tag[block_index].S_tag == block_Stag) && (T_cache.tag[block_index].T_tag == block_Ttag))
		        {
				    T_cache.TEX_hit_data = T_cache.cache[block_index*16 + block_offset];
				    return 0;
			    }
			    else
			    {
	                T_cache.TEX_hit_data = 0;
			        T_cache.state = 1;
			        T_cache.transfer_count = 16;
				    T_cache.penalty_count = 0;				
		            return 1;			
			    }
	        }
	        else
	        {
	            T_cache.TEX_hit_data = 0;
			    T_cache.state = 1;
			    T_cache.transfer_count = 16;
			    T_cache.penalty_count = 0;			
		        return 1;
	        }
        }
        else
        {
            if(T_cache.penalty_count == EXTERNAL_MEM_DELAY)
            {		   
		        temp = 16 - T_cache.transfer_count;
			    BUS_S = (block_Stag * 4) + (temp%4);
			    BUS_T = (block_Ttag * 4) + (temp/4);
                BUS_offset = BUS_S + BUS_T * TEX_WIDTH;
			
			    T_cache.cache[block_index*16 + temp] = TEXTURE[BUS_offset];
			    T_cache.penalty_count = 0;
			    T_cache.transfer_count--;					
            }
            else
               T_cache.penalty_count++;		
		
		    if(T_cache.transfer_count == 0)
		    {
				T_cache.TEX_hit_data = T_cache.cache[block_index*16 + block_offset];
			    T_cache.valid[block_index] = 1;
			    T_cache.tag[block_index].S_tag = block_Stag;
			    T_cache.tag[block_index].T_tag = block_Ttag;
			    T_cache.state = 0;
			    return 0;
		    }
		    else
		       return 1;
        }
    }		
}


int Tfifo_write(struct T_FIFO *Tfifo,unsigned int Tid,float u,float v)
{
    if(Tfifo->full == 0)
	{
	    Tfifo->list[Tfifo->W_ptr].u = u;
	    Tfifo->list[Tfifo->W_ptr].v = v;
	    Tfifo->list[Tfifo->W_ptr].Tid = Tid;
		
		if(Tfifo->W_ptr == 13)
		   Tfifo->W_ptr = 0;
		else
		   Tfifo->W_ptr++;
		   
		Tfifo->counter++;
		
		if(Tfifo->counter == 14)
		   Tfifo->full = 1;
		   
		if(Tfifo->counter > 0)
		   Tfifo->empty = 0;
		
		return 1;//success
    }
	else
	   return 0;//fail   
}


int Tfifo_read(struct T_FIFO *Tfifo,struct T_info *Tinfo)
{
    if(Tfifo->empty == 0)
	{
	    Tinfo->u = Tfifo->list[Tfifo->R_ptr].u;
		Tinfo->v = Tfifo->list[Tfifo->R_ptr].v;
		Tinfo->Tid = Tfifo->list[Tfifo->R_ptr].Tid;
		
		if(Tfifo->R_ptr == 13)
		   Tfifo->R_ptr = 0;
		else
           Tfifo->R_ptr++;
		   
		Tfifo->counter--;
		
		if(Tfifo->counter == 0)
		   Tfifo->empty = 1;
		   
		if(Tfifo->counter < 14)
		   Tfifo->full = 0;
        
        return 1;//success		
	}
	else
	   return 0;//fail	
}


int output_check(unsigned int Ttype,unsigned int Tid,int full)
{
    int i,j;
	int tri_index;
	
	switch(Ttype)
	{
	    case SC_THREAD: if(thread_ctrl.FS_count == 7)
		                {
						    thread_ctrl.info[Tid].Tstate = STALL;//[READY] -> [STALL]
							thread_ctrl.info[Tid].stall_flag = OUT_FULL;
							printf("SC out full\n");
							return 1;
						}
						else						   
						   return 0;
		case VS_THREAD: for(i=0;i<2;++i)
		                   for(j=0;j<3;++j)
						   {
						       if(Tid == thread_ctrl.triangle[i][j])
							   {
							       tri_index = i;
							   }
						   }
		
		                if(thread_ctrl.SC_count == 1 || thread_ctrl.tri_priority != tri_index)
		                {
						    thread_ctrl.info[Tid].Tstate = STALL;//[READY] -> [STALL]
							thread_ctrl.info[Tid].stall_flag = OUT_FULL;
							
							return 1;
						}
						else
						   return 0;  
		case FS_THREAD: if(full)
		                {
						    thread_ctrl.info[Tid].Tstate = STALL;//[READY] -> [STALL]
							thread_ctrl.info[Tid].stall_flag = OUT_FULL;

							return 1;
						}
						else
						   return 0;
		default: return 0; 
	}
}


int Ffifo_write(struct F_FIFO *Ffifo,struct F_info *Finfo)
{
    if(Ffifo->full == 0)
	{
	    Ffifo->list[Ffifo->W_ptr].color = Finfo->color;
		Ffifo->list[Ffifo->W_ptr].x = Finfo->x;
	    Ffifo->list[Ffifo->W_ptr].y = Finfo->y;
		Ffifo->list[Ffifo->W_ptr].z = Finfo->z;		
		
		if(Ffifo->W_ptr == 6)
		   Ffifo->W_ptr = 0;
		else
		   Ffifo->W_ptr++;
		   
		Ffifo->counter++;

		if(Ffifo->counter == 7)
		   Ffifo->full = 1;		
		
		if(Ffifo->counter > 0)
		   Ffifo->empty = 0;
		
		return 1;//success
    }
	else
	   return 0;//fail	
}


int Ffifo_read(struct F_FIFO *Ffifo,struct F_info *Finfo)
{
    if(Ffifo->empty == 0)
	{
	    Finfo->color = Ffifo->list[Ffifo->R_ptr].color;
		Finfo->x = Ffifo->list[Ffifo->R_ptr].x;
		Finfo->y = Ffifo->list[Ffifo->R_ptr].y;
		Finfo->z = Ffifo->list[Ffifo->R_ptr].z;
		
		if(Ffifo->R_ptr == 6)
		   Ffifo->R_ptr = 0;
		else
           Ffifo->R_ptr++;
		   
		Ffifo->counter--;
		
		if(Ffifo->counter == 0)
		   Ffifo->empty = 1;
        
		if(Ffifo->counter < 7)
		   Ffifo->full = 0;
		
        return 1;//success		
	}
	else
	   return 0;//fail
}


int test_circuit(unsigned int func,int sign,int eq)
{
    int SRC1_sig,SRC2_sig;
    int SRC1_inv,SRC2_inv;
    int SRC1,SRC2,OR_SRC1,OR_SRC2;

    switch(func)
    {
        case NEVER:  SRC1_sig = 1; SRC2_sig = 1;
		             SRC1_inv = 0; SRC2_inv = 0;
        	         break;
        case ALWAYS: SRC1_sig = 1; SRC2_sig = 1;
        	         SRC1_inv = 1; SRC2_inv = 1;
        	         break;
        case LESS:   SRC1_sig = 0; SRC2_sig = 1;
        	         SRC1_inv = 0; SRC2_inv = 0;
        	         break;
        case LEQUAL: SRC1_sig = 0; SRC2_sig = 0;
        	         SRC1_inv = 0; SRC2_inv = 0;
        	         break;
        case EQUAL:  SRC1_sig = 1; SRC2_sig = 0;
        	         SRC1_inv = 0; SRC2_inv = 0;
        	         break;
        case GEQUAL: SRC1_sig = 0; SRC2_sig = 0;
        	         SRC1_inv = 1; SRC2_inv = 0;
        	         break;
        case GREATER: SRC1_sig = 0; SRC2_sig = 1;
        	          SRC1_inv = 1; SRC2_inv = 0;
        	          break;
        case NOTEQUAL: SRC1_sig = 1; SRC2_sig = 0;
        	           SRC1_inv = 0; SRC2_inv = 1;
        	           break;
        default:       SRC1_sig = 0; SRC2_sig = 0;
        	           SRC1_inv = 0; SRC2_inv = 0;
        	           break;
    }

    SRC1 = (SRC1_sig)? 0 : sign;
    SRC2 = (SRC2_sig)? 0 : eq;
	OR_SRC1 = (SRC1_inv)? (SRC1+1)%2 : SRC1;
    OR_SRC2 = (SRC2_inv)? (SRC2+1)%2 : SRC2;

	if(OR_SRC1 == 1 || OR_SRC2 == 1)
	   return 1;
	else
	   return 0;
}


unsigned int stencil_update(unsigned int func,unsigned int S_ref,unsigned int old_S)
{
	unsigned int return_data;

    switch(func)
    {
        case KEEP:	  return_data = old_S;
        	          break;
        case ZERO:    return_data = 0;
        	          break;
        case REPLACE: return_data = S_ref;
        	          break;
        case INCR:    if(old_S < 255)
        	             return_data = old_S + 1;
        	          else
        	             return_data = 255;
        	          break;
        case DECR:    if(old_S > 0)
        	             return_data = old_S - 1;
        	          else
        	             return_data = 0;
        	          break;
        case INVERT:  return_data = ~(old_S | 0xffffff00);
        	          break;
        case INCR_WRAP: if(old_S < 255)
        	               return_data = old_S + 1;
        	            else
        	               return_data = 0;
        	            break;
        case DECR_WRAP: if(old_S > 0)
        	               return_data = old_S - 1;
        	            else
        	               return_data = 255;
        	            break;
        default: return_data = old_S;
        	     break;
    }

    return return_data;
}


unsigned int factor_selector(int SRC_DST,unsigned int func,unsigned int SRC_color,unsigned int DST_color)
{
    unsigned int color,return_data;
    unsigned int DST_color_A_com,blend_f;
	unsigned int SRC_A,SRC_R,SRC_G,SRC_B;
	unsigned int DST_A,DST_R,DST_G,DST_B;
    unsigned int factor_A,factor_R,factor_G,factor_B;
    unsigned int A,R,G,B;

	//decompose
	SRC_A = SRC_color >>24;
	SRC_R = (SRC_color & 0x00ff0000) >>16;
	SRC_G = (SRC_color & 0x0000ff00) >>8;
	SRC_B = (SRC_color & 0x000000ff);

	DST_A = DST_color >>24;
	DST_R = (DST_color & 0x00ff0000) >>16;
	DST_G = (DST_color & 0x0000ff00) >>8;
	DST_B = (DST_color & 0x000000ff);	
	
    //blend_f
    DST_color_A_com = 255 - DST_A;
	
    if(SRC_A < DST_color_A_com)
       blend_f = SRC_A;
    else
       blend_f = DST_color_A_com;

    //select factor
    switch(func)
    {
        case BLEND_ZERO: factor_A = 0;
        	             factor_R = 0;
        	             factor_G = 0;
        	             factor_B = 0;
        	             break;
        case BLEND_ONE: factor_A = 255;//8'hff
        	            factor_R = 255;
        	            factor_G = 255;
        	            factor_B = 255;
        	            break;
        case BLEND_SRCCOLOR: factor_A = SRC_A;
        	                 factor_R = SRC_R;
        	                 factor_G = SRC_G;
        	                 factor_B = SRC_B;
        	                 break;
        case BLEND_ONEMINUSSRCCOLOR: factor_A = 255 - SRC_A;
        	                         factor_R = 255 - SRC_R;
        	                         factor_G = 255 - SRC_G;
        	                         factor_B = 255 - SRC_B;
        	                         break;
        case BLEND_DSTCOLOR: factor_A = DST_A;
        	                 factor_R = DST_R;
        	                 factor_G = DST_G;
        	                 factor_B = DST_B;
        	                 break;
        case BLEND_ONEMINUSDSTCOLOR: factor_A = 255 - DST_A;
        	                         factor_R = 255 - DST_R;
        	                         factor_G = 255 - DST_G;
        	                         factor_B = 255 - DST_B;
        	                         break;
        case BLEND_SRCALPHA: factor_A = SRC_A;
        	                 factor_R = SRC_A;
        	                 factor_G = SRC_A;
        	                 factor_B = SRC_A;
        	                 break;
        case BLEND_ONEMINUSSRCALPHA: factor_A = 255 - SRC_A;
        	                         factor_R = 255 - SRC_A;
        	                         factor_G = 255 - SRC_A;
        	                         factor_B = 255 - SRC_A;
        	                         break;
        case BLEND_DSTALPHA: factor_A = DST_A;
        	                 factor_R = DST_A;
        	                 factor_G = DST_A;
        	                 factor_B = DST_A;
        	                 break;
        case BLEND_ONEMINUSDSTALPHA: factor_A = 255 - DST_A;
        	                         factor_R = 255 - DST_A;
        	                         factor_G = 255 - DST_A;
        	                         factor_B = 255 - DST_A;
        	                         break;
        case BLEND_SRCALPHASATURATE: factor_A = 255;
        	                         factor_R = blend_f;
        	                         factor_G = blend_f;
        	                         factor_B = blend_f;
        	                         break;
        default: factor_A = 255;
        	     factor_R = 255;
        	     factor_G = 255;
        	     factor_B = 255;
        	     break;
    }

    //real factor
    if(factor_A != 0)
       ++factor_A;

    if(factor_R != 0)
       ++factor_R;

    if(factor_R != 0)
       ++factor_G;

    if(factor_B != 0)
       ++factor_B;

    //SRC_color or DST_color?
    if(SRC_DST)
       color = DST_color;
    else
       color = SRC_color;           

    A = (factor_A * (color >> 24)) / 256;
    R = (factor_R * ((color&0x00ff0000) >> 16)) / 256;
    G = (factor_G * ((color&0x0000ff00) >> 8)) / 256;
    B = (factor_B * (color&0x000000ff)) / 256;

    return_data = 0 | (A<<24) | (R<<16) | (G<<8) | B;

    return return_data;
}


void fragment(void)
{
    int           i,T_min;
	unsigned int  PC_min;
	struct F_info proc_info;
	unsigned int  offset,old_value;
	unsigned int  S_ref,old_S,update_S;
	unsigned int  old_Z,update_Z;
	int           sign,eq;
	int           S_pass,Z_pass,is_S_pass,is_Z_pass,Z_CMP_pass;
	int           ZS_pass;
	unsigned int  update_func;
	unsigned int  blend_src,blend_dst;
	unsigned int  add_A,add_R,add_G,add_B;
	
	if(FRAG.state == 0)
	{
	    if(Ffifo_read(FRAG.fifo,&proc_info))
		{
		    #if DEBUG
			fprintf(dump,"<Frag unit begin>\n");
			#endif
			
			FRAG.penalty_count = 2;//calculate offset
			
			offset = proc_info.x + (LCD_WIDTH * proc_info.y);
			S_ref = FRAG.stencil_ref & FRAG.stencil_mask;
			
			//initialize ZS_pass
			ZS_pass = 1;
			if(FRAG.zcompare_enable == 1 || FRAG.stencil_enable == 1)
			{
			    //read from [ZS_BUF]
			    old_value = ZS_BUF[offset];
				old_Z = (old_value>>16)%65536;//mod 65536 can be removed
				old_S = (old_value%256) & FRAG.stencil_mask;
				
				/*
				printf("offset = %d\n",offset);
				printf("(old_Z,old_S) = (%x,%x)\n",old_Z,old_S);
				*/
				
				//stencil test
				sign = (S_ref < old_S)? 1 : 0;
				eq = (S_ref == old_S)? 1 : 0;
				S_pass = test_circuit(FRAG.stencil_func,sign,eq);
				
				if(FRAG.stencil_enable == 0 || S_pass == 1)
				   is_S_pass = 1;
				else
				   is_S_pass = 0;
				   
				//depth test
                sign = (proc_info.z < old_Z)? 1 : 0;
                eq = (proc_info.z == old_Z)? 1 : 0;
                Z_pass = test_circuit(FRAG.zcompare_func,sign,eq);
				
				if(FRAG.zcompare_enable == 0 || Z_pass == 1)
				   is_Z_pass = 1;
				else
				   is_Z_pass = 0;
				
				if(FRAG.zcompare_enable == 1 && Z_pass == 1)
				   Z_CMP_pass = 1;
				else
				   Z_CMP_pass = 0;

                //set ZS_pass
				if(is_S_pass == 1 && is_Z_pass == 1)
				   ZS_pass = 1;
				else
                   ZS_pass = 0;
				   
                //select stencil update function
                if(is_S_pass == 0)
                   update_func = FRAG.fail;
                else if(is_Z_pass == 1)
                   update_func = FRAG.zpass;
                else
                   update_func = FRAG.zfail;

                //stencil update op.
                if(FRAG.stencil_enable == 1)
                   update_S = stencil_update(update_func,S_ref,old_S);
                else
                   update_S = old_S;

                //z compare update op.
                if(Z_CMP_pass)
                   update_Z = proc_info.z;
                else
                   update_Z = old_Z;

                //write to [ZS_BUF]?
                if(FRAG.stencil_enable == 1 || Z_CMP_pass == 1)
                   ZS_BUF[offset] = (update_Z << 16) + update_S;				   
				
				//calculate waste cycle
				FRAG.penalty_count += 4;//ZS OPs
				
				if(FRAG.stencil_enable == 1 || Z_CMP_pass == 1)
				   FRAG.penalty_count += (2 * EXTERNAL_MEM_DELAY);
				else
				   FRAG.penalty_count += EXTERNAL_MEM_DELAY;
			}
			
			//printf("ZS pass = %d\n",ZS_pass);
			
            //do blending op.?
            if(FRAG.blending_enable & ZS_pass)
            {
                //read from [F_BUF]
                old_value = F_BUF[offset];
				
                blend_src = factor_selector(0,FRAG.blending_srcfunc,proc_info.color,old_value);
                blend_dst = factor_selector(1,FRAG.blending_dstfunc,proc_info.color,old_value);

                //blend src color & dst color + clamp
                add_A = (blend_src >> 24) + (blend_dst >> 24);
                if(add_A > 255)
                   add_A = 255;
                add_R = ((blend_src & 0x00ff0000)>>16) + ((blend_dst & 0x00ff0000)>>16);
                if(add_R > 255)
                   add_R = 255;
                add_G = ((blend_src & 0x0000ff00)>>8) + ((blend_dst & 0x0000ff00)>>8);
                if(add_G > 255)
                   add_G = 255;
                add_B = (blend_src & 0x000000ff) + (blend_dst & 0x000000ff);
                if(add_B > 255)
                   add_B = 255;
				   
                //update write color
                proc_info.color = 0 | (add_A<<24) | (add_R<<16) | (add_G<<8) | add_B;

				//calculate waste cycle
				FRAG.penalty_count += 6;//B OPs
				FRAG.penalty_count += EXTERNAL_MEM_DELAY;
            }

            //write to [F_BUF]?
            if(ZS_pass == 1)
            {                           	
                F_BUF[offset] = proc_info.color;
				FRAG.penalty_count += EXTERNAL_MEM_DELAY;
                
				#if DRAW
				fprintf(f_buf,"%d\n%d\n",offset,(proc_info.color & 0x00ffffff));				
				#endif
            }
            
            FRAG.state = 1;			
	    }
    }
	else
	{
	    FRAG.penalty_count--;
		
		if(FRAG.penalty_count == 0)
		{
		    FRAG.state = 0;

			#if DEBUG
			//fprintf(dump,"<Frag unit end>\n");
			#endif
			
			T_min = -1;
			PC_min = IM_LENGTH;//over (0 ~ IM_LENGTH-1)
			for(i=0;i<14;++i)
			{
			    //[find FS with smallest PC]
				if(thread_ctrl.info[i].Ttype == FS_THREAD)
				{
				    if(thread_ctrl.info[i].Tstate == STALL && thread_ctrl.info[i].stall_flag == OUT_FULL)
					{
					    if(thread_ctrl.info[i].PC < PC_min)
						{
						    T_min = i;
							PC_min = thread_ctrl.info[i].PC;
						}
					}
				}
			}
			
			if(T_min != -1)
			{
			    //(STALL) -> (READY)
				thread_ctrl.info[T_min].Tstate = READY;
			}
		}
	}
}


int Vfifo_write(struct V_FIFO *Vfifo,struct V_info *Vinfo)
{
    int i;
	
	if(Vfifo->full == 0)
	{
		for(i=0;i<4;++i)
		   mov(Vinfo->attrib[i],&Vfifo->list[Vfifo->W_ptr].attrib[i]);
		
		if(Vfifo->W_ptr == 35)
		   Vfifo->W_ptr = 0;
		else
           Vfifo->W_ptr++;
		   
		Vfifo->counter++;
		
		if(Vfifo->counter == 36)
		   Vfifo->full = 1;
		   
		if(Vfifo->counter > 0)
		   Vfifo->empty = 0;
        
        return 1;//success		
	}
	else
	   return 0;//fail    
}


int Vfifo_read(struct V_FIFO *Vfifo,struct V_info *Vinfo)
{
    int i;
	
	if(Vfifo->empty == 0)
	{
		for(i=0;i<4;++i)
		   mov(Vfifo->list[Vfifo->R_ptr].attrib[i],&Vinfo->attrib[i]);
		
		if(Vfifo->R_ptr == 35)
		   Vfifo->R_ptr = 0;
		else
           Vfifo->R_ptr++;
		   
		Vfifo->counter--;
		
		if(Vfifo->counter == 0)
		   Vfifo->empty = 1;
		
		if(Vfifo->counter < 36)
		   Vfifo->full = 0;
        
        return 1;//success		
	}
	else
	   return 0;//fail
}


int Vfifo_slot(void)
{
    int i;
	int slot[2];
	
	slot[0] = thread_ctrl.SC_slot;
	slot[1] = thread_ctrl.FS_slot;
	
	for(i=0;i<14;++i)
	{
	    if(thread_ctrl.info[i].Ttype == NO_THREAD)
		{
		    if((i != slot[0]) && (i != slot[1]))
			   return i;//100% success
		}
	}
}


unsigned int slot_detector(unsigned int Tid,unsigned int Ttype)
{
    int          i,j;
	int          slot[2];
	
    if(Ttype == SC_THREAD)
	{
	    if(thread_ctrl.FS_slot != -1)
		   return thread_ctrl.FS_slot;
		else
		{
		    slot[0] = thread_ctrl.VS_slot;
			slot[1] = thread_ctrl.SC_slot;
		
		    for(i=0;i<14;++i)
		       if(thread_ctrl.info[i].Ttype == NO_THREAD)
			   {			   
				   if((i != slot[0]) && (i != slot[1]))
				   {
					   thread_ctrl.FS_slot = i;
					   return i;
				   }				   
			   }
			   
			return Tid;//no space
		}
	}
	else if(Ttype == VS_THREAD)
	{			  
		if(thread_ctrl.SC_slot != -1)
		   return thread_ctrl.SC_slot;
		else
		{
		    slot[0] = thread_ctrl.VS_slot;
			slot[1] = thread_ctrl.FS_slot;			
			
			for(i=0;i<14;++i)
		       if(thread_ctrl.info[i].Ttype == NO_THREAD)
		       {
		           if((i != slot[0]) && (i != slot[1]))
			       {
			           thread_ctrl.SC_slot = i;
				       return i;
			       }
		       }
			   
			return Tid;//no space   
	    }
	}
	else//FS
	   return Tid;
}


unsigned int VS_out_map(int tri_order,unsigned int dst_addr)
{
    int index = tri_order * 4 + (dst_addr%4);//built-in varying variable
	
	switch(index)
	{
	    case 0: return 21;
		case 1: return 36;
		case 2: return 42;
		case 3: return 30;
		//-----------------------
	    case 4: return 22;
		case 5: return 37;
		case 6: return 43;
		case 7:	return 31;	
		//-----------------------
	    case 8: return 23;
		case 9: return 38;
		case 10: return 44;
		case 11: return 32;		
		default: return 0;
	}
}


void WB_convt(struct ID_READ_if input,struct READ_EX1_if *output)
{
    int i,j;
    int tri_order;
	

	//thread_ID
	if(input.GPU_IF_sig)
	   output->thread_ID = slot_detector(input.thread_ID,input.thread_type);
	else
	   output->thread_ID = input.thread_ID;
    
	//out_sig
	if(input.thread_type == FS_THREAD && input.GPU_IF_sig == 1)
	   output->out_sig = 1;
	else
	   output->out_sig = 0;
	
	//dst_addr
	if(input.thread_type == VS_THREAD && input.GPU_IF_sig == 1)
	{
	    for(i=0;i<2;++i)
	       for(j=0;j<3;++j)
	          if(input.thread_ID == thread_ctrl.triangle[i][j])
		         tri_order = j;
				 
		output->dst_addr = VS_out_map(tri_order,input.dst_addr);
	}
	else
	   output->dst_addr = input.dst_addr;
}


int beq(int FXL,struct vec4 SRC1,struct vec4 SRC2)
{
    int counter = 0;
	
    if(FXL)
    {
        if(SRC1.x == SRC2.x)
           counter++;
        if(SRC1.y == SRC2.y)
           counter++;
        if(SRC1.z == SRC2.z)
           counter++;
        if(SRC1.w == SRC2.w)
           counter++;
    }
    else
    {
        if(*((int*)(&SRC1.x)) == *((int*)(&SRC2.x)))
           counter++;
        if(*((int*)(&SRC1.y)) == *((int*)(&SRC2.y)))
           counter++;
        if(*((int*)(&SRC1.z)) == *((int*)(&SRC2.z)))
           counter++;
        if(*((int*)(&SRC1.w)) == *((int*)(&SRC2.w)))
           counter++;
    }
	
    if(counter == 4)
       return 1;
    else
       return 0;
}


int bneq(int FXL,struct vec4 SRC1,struct vec4 SRC2)
{
    int counter = 0;
	
    if(FXL)
    {
        if(SRC1.x != SRC2.x)
           counter++;
        if(SRC1.y != SRC2.y)
           counter++;
        if(SRC1.z != SRC2.z)
           counter++;
        if(SRC1.w != SRC2.w)
           counter++;
    }
    else
    {
        if(*((int*)(&SRC1.x)) != *((int*)(&SRC2.x)))
           counter++;
        if(*((int*)(&SRC1.y)) != *((int*)(&SRC2.y)))
           counter++;
        if(*((int*)(&SRC1.z)) != *((int*)(&SRC2.z)))
           counter++;
        if(*((int*)(&SRC1.w)) != *((int*)(&SRC2.w)))
           counter++;
    }
    if(counter > 0)
       return 1;
    else
       return 0;	
	
}


int blt(int FXL,struct vec4 SRC1,struct vec4 SRC2)
{
    int counter = 0;
	
    if(FXL)
    {
        if(SRC1.x < SRC2.x)
           counter++;
        if(SRC1.y < SRC2.y)
           counter++;
        if(SRC1.z < SRC2.z)
           counter++;
        if(SRC1.w < SRC2.w)
           counter++;
    }
    else
    {
        if(*((int*)(&SRC1.x)) < *((int*)(&SRC2.x)))
           counter++;
        if(*((int*)(&SRC1.y)) < *((int*)(&SRC2.y)))
           counter++;
        if(*((int*)(&SRC1.z)) < *((int*)(&SRC2.z)))
           counter++;
        if(*((int*)(&SRC1.w)) < *((int*)(&SRC2.w)))
           counter++;
    }
    if(counter == 4)
       return 1;
    else
       return 0;	
	
}


int bge(int FXL,struct vec4 SRC1,struct vec4 SRC2)
{
    int counter = 0;

    if(FXL)
    {
        if(SRC1.x >= SRC2.x)
           counter++;
        if(SRC1.y >= SRC2.y)
           counter++;
        if(SRC1.z >= SRC2.z)
           counter++;
        if(SRC1.w >= SRC2.w)
           counter++;
    }
    else
    {
        if(*((int*)(&SRC1.x)) >= *((int*)(&SRC2.x)))
           counter++;
        if(*((int*)(&SRC1.y)) >= *((int*)(&SRC2.y)))
           counter++;
        if(*((int*)(&SRC1.z)) >= *((int*)(&SRC2.z)))
           counter++;
        if(*((int*)(&SRC1.w)) >= *((int*)(&SRC2.w)))
           counter++;
    }
    if(counter == 4)
       return 1;
    else
       return 0;
}


void ctrl_unit(int stall_occur,struct ID_READ_if input,struct vec4 *SRC_data,struct READ_out *output)
{
    int          hit;
	unsigned int target;
	
	output->branch_Tid = input.thread_ID;
	
	if(stall_occur)
	{
	    output->flush_ID = 1;		
		output->branch_hit = 1;
	    output->branch_target = input.PC;		
	}
	else
	{
	    switch(input.opcode)
		{
		    case  BEQ: output->branch_hit = beq(input.FXL,SRC_data[0],SRC_data[1]);
			           output->branch_target = input.PC + input.branch_addr;
					   
					   if(output->branch_hit == 1)
					      output->flush_ID = 1;
					   else
					      output->flush_ID = 0;
					   break;
		    case BNEQ: output->branch_hit = bneq(input.FXL,SRC_data[0],SRC_data[1]);
			           output->branch_target = input.PC + input.branch_addr;
					   
					   if(output->branch_hit == 1)
					      output->flush_ID = 1;
					   else
					      output->flush_ID = 0;
					   break;
		    case  BLT: output->branch_hit = blt(input.FXL,SRC_data[0],SRC_data[1]);
			           output->branch_target = input.PC + input.branch_addr;
					   
					   if(output->branch_hit == 1)
					      output->flush_ID = 1;
					   else
					      output->flush_ID = 0;
					   break;
		    case  BGE: output->branch_hit = bge(input.FXL,SRC_data[0],SRC_data[1]);
			           output->branch_target = input.PC + input.branch_addr;
					   
					   if(output->branch_hit == 1)
					      output->flush_ID = 1;
					   else
					      output->flush_ID = 0;
					   break;
		    case  RET: output->flush_ID = 1;
			           output->branch_hit = 1;
			           output->branch_target = (unsigned int)SRC_data[0].x;
					   break;
			case JUMP: output->flush_ID = 1;
			           output->branch_hit = 1;
			           output->branch_target = input.PC + input.branch_addr*8;
					   break;
			default: output->flush_ID = 0;
			         output->branch_hit = 0;
			         output->branch_target = 0;
					 break;
		}
		
		#if DEBUG
		if(output->flush_ID == 1)
		   fprintf(dump,"<flush ID>\n");
		
		if(output->branch_hit == 1)
		   fprintf(dump,"<branch hit>\n");
		#endif
		  
	}
}


void READ(struct ID_READ_if input,struct WB_out WB_output,struct READ_EX1_if *RD_EX_if,struct READ_out *output)
{    
	int           i,j,temp;
	
	struct vec4   SRC_data[3];
	int           Wen = 1;
	int           is_TEX = 0,T_flush = 0;
	int           out_flush = 0;
	int           active_sig = 0;
	unsigned int  TEX_color = 0;
	int           stall_occur;
	float         TEX_u,TEX_v;
	struct T_info TEX_info;	
	struct F_info frag_info;
	int           Vfifo2GPR_wen =0;
	unsigned int  Vfifo2GPR_dst;
	struct vec4   Vfifo2GPR_Wdata;
	unsigned int  int_X,int_Y,int_Z,int_W,int_color;

    #if DEBUG
	if(SYS_finish == 36)
	{
	    fprintf(dump,"[READ]\n");
	    fprintf(dump,"========================\n");
    }
	#endif
   	
	//GPR read
	for(i=0;i<3;++i)
	{
	    mov(GPR[64 * input.thread_ID + input.src_addr[i]],&SRC_data[i]);
		
        #if DEBUG	
		if(SYS_finish == 36)
		{
		   fprintf(dump,"Rdata[%d].x = %f\n",i,SRC_data[i].x);
		   fprintf(dump,"Rdata[%d].y = %f\n",i,SRC_data[i].y);
		   fprintf(dump,"Rdata[%d].z = %f\n",i,SRC_data[i].z);
		   fprintf(dump,"Rdata[%d].w = %f\n",i,SRC_data[i].w);
		}
		#endif
    }
	//generate wen
	if(input.opcode == ST || input.opcode == FIN)
	   Wen = 0;
	if(input.opcode >= 24 && input.opcode != SHRL)//Branch or NOP
	   Wen = 0;
	   
	//forwarding(just from [WB]->[READ])
    if(WB_output.thread_ID == input.thread_ID)
	{
	    if(WB_output.wen == 1 && WB_output.out_sig == 0)
		{
		    for(i=0;i<3;++i)
	           if(WB_output.dst_addr == input.src_addr[i])
			   {
		 	       mov(WB_output.Wdata,&SRC_data[i]);
				   
				   #if DEBUG
				   fprintf(dump,"<forwarding>\n");
				   #endif
               }				   
		}		  
    }
	
	//Swizzle
	SWZ(input.neg,input.swizzle,SRC_data);
	
	//SV
	SRC_vec(input.SV,SRC_data);

/*
	for(i=0;i<3;++i)
	{
		fprintf(dump,"SRC_data[%d].x = %f\n",i,SRC_data[i].x);
		fprintf(dump,"SRC_data[%d].y = %f\n",i,SRC_data[i].y);
		fprintf(dump,"SRC_data[%d].z = %f\n",i,SRC_data[i].z);
		fprintf(dump,"SRC_data[%d].w = %f\n",i,SRC_data[i].w);
	}
*/		

	//output stall check
	if(input.GPU_IF_sig == 1 && Wen == 1)
       out_flush = output_check(input.thread_type,input.thread_ID,FRAG.fifo->full);
	
	//TEX cache access
	if(input.opcode == TXB || input.opcode == TEX2D)
	   is_TEX = 1;
	
	if(T_cache.state)
	{
	    if(is_TEX)
		{
		    if(T_cache.TEX_miss_flag[input.thread_ID])
			{
			    TEX_color = T_cache.TEX_miss_data[input.thread_ID];
				T_cache.TEX_miss_flag[input.thread_ID] = 0;
				
				#if DEBUG
				fprintf(dump,"<Get missed TEX data>\n");
				#endif				
			}
			else
			{			   							    
				TEX_u = SRC_data[0].x;
				TEX_v = SRC_data[0].y;				
				Tfifo_write(T_cache.fifo,input.thread_ID,TEX_u,TEX_v);//add FIFO(100% success)				
				T_cache.TEX_miss_flag[input.thread_ID] = 1;
				thread_ctrl.info[input.thread_ID].Tstate = STALL;//(READY) -> (STALL)
				thread_ctrl.info[input.thread_ID].stall_flag = TEX_MISS;				
				T_flush = 1;
				
				#if DEBUG
				   fprintf(dump,"<TEX cache miss>\n");
				#endif
			}
		}
		
		if(TEX_cache(1,0,0) == 0)
		{
		    T_cache.TEX_miss_data[T_cache.proc_thread] = T_cache.TEX_hit_data;
			thread_ctrl.info[T_cache.proc_thread].Tstate = READY;//(STALL) -> (READY)
			
			#if DEBUG
		    fprintf(dump,"(STALL) -> (READY) = Tid[%d]\n",T_cache.proc_thread);
		    #endif			
		}
	}
	else
	{
	    if(T_cache.fifo->empty)
		{
		    if(is_TEX)//may need to check (TEX_miss_flag)
			{
			    T_cache.proc_thread = input.thread_ID;
				TEX_u = SRC_data[0].x;
				TEX_v = SRC_data[0].y;
				
				if(TEX_cache(1,TEX_u,TEX_v))
				{
				    T_cache.TEX_miss_flag[input.thread_ID] = 1;
					thread_ctrl.info[input.thread_ID].Tstate = STALL;//(READY) -> (STALL)
					thread_ctrl.info[input.thread_ID].stall_flag = TEX_MISS;
					T_flush = 1;
				
				    #if DEBUG
				    fprintf(dump,"<TEX cache miss>\n");
				    #endif					
				}
				else
				{
				    TEX_color = T_cache.TEX_hit_data;

				    #if DEBUG
				    fprintf(dump,"<TEX cache hit>\n");
				    #endif					
				}
			}
		}
		else
		{
		    if(is_TEX)
			{
			    if(T_cache.TEX_miss_flag[input.thread_ID])
				{
				    TEX_color = T_cache.TEX_miss_data[input.thread_ID];
					T_cache.TEX_miss_flag[input.thread_ID] = 0;			
				}
				else
				{
					TEX_u = SRC_data[0].x;
					TEX_v = SRC_data[0].y;
					Tfifo_write(T_cache.fifo,input.thread_ID,TEX_u,TEX_v);//write FIFO(100% success)
					T_cache.TEX_miss_flag[input.thread_ID] = 1;										
					thread_ctrl.info[input.thread_ID].Tstate = STALL;//(READY) -> (STALL)
					thread_ctrl.info[input.thread_ID].stall_flag = TEX_MISS;
					T_flush = 1;
					
				    #if DEBUG
				    fprintf(dump,"<TEX cache miss>\n");
				    #endif					
				}
			}

			Tfifo_read(T_cache.fifo,&TEX_info);//read FIFO(100% success)
			T_cache.proc_thread = TEX_info.Tid;
			TEX_u = TEX_info.u;            
			TEX_v = TEX_info.v;
			
			if(TEX_cache(1,TEX_u,TEX_v) == 0)
			{
			    T_cache.TEX_miss_data[T_cache.proc_thread] = T_cache.TEX_hit_data;
				thread_ctrl.info[T_cache.proc_thread].Tstate = READY;//(STALL) -> (READY)
			}
		}
	}
	
	
	//Fragment read
	fragment();
	
	//Fragment write
	if(WB_output.is_FRAG_start)//from [WB]
	{
	    #if DEBUG
	    fprintf(dump,"<FRAG start>\n");
		fprintf(dump,"<color = %x>\n",FRAG.out_pool[WB_output.thread_ID].color);
		fprintf(dump,"<x = %d>\n",FRAG.out_pool[WB_output.thread_ID].x);
		fprintf(dump,"<y = %d>\n",FRAG.out_pool[WB_output.thread_ID].y);
		fprintf(dump,"<z = %d>\n",FRAG.out_pool[WB_output.thread_ID].z);
		#endif
		
		frag_info.color = FRAG.out_pool[WB_output.thread_ID].color;
		frag_info.x = FRAG.out_pool[WB_output.thread_ID].x;
		frag_info.y = FRAG.out_pool[WB_output.thread_ID].y;
		frag_info.z = FRAG.out_pool[WB_output.thread_ID].z;
	
	    if(Ffifo_write(FRAG.fifo,&frag_info) == 0)//fail
        {
		    //printf("FRAG FIFO is full~~!!!\n");
		}
	}	
	

	//finish : (STALL) -> (READY)
	if(input.opcode == FIN && input.GPU_IF_sig == 0)
	{ 
        switch(input.thread_type)
        {
            case SC_THREAD: thread_ctrl.info[input.thread_ID].Ttype = NO_THREAD;
			                thread_ctrl.SC_count--;
			                thread_ctrl.NO_count++;
				
							//release [VS]
							for(i=0;i<14;++i)
							{
							    if(thread_ctrl.info[i].Ttype == VS_THREAD)
								{
								    if(thread_ctrl.info[i].Tstate == STALL && thread_ctrl.info[i].stall_flag == OUT_FULL)
									   thread_ctrl.info[i].Tstate = READY;
								}
							}
							
							#if DEBUG
							fprintf(dump,"<SC_finish> ->(NO,SC)= (%d,%d)\n",thread_ctrl.NO_count,thread_ctrl.SC_count);
							fprintf(dump,"PC = %d\n",input.PC);
							#endif
														
							SYS_finish++;							
							printf("triangle #[%d] finish~~!!\n",SYS_finish);
							break;
		    case VS_THREAD: /*
			                for(i=0;i<2;++i)
			                for(j=0;j<3;++j)
							   if(input.thread_ID == thread_ctrl.triangle[i][j])
							      thread_ctrl.triangle[i][j] = -1;
			                
			                thread_ctrl.VS_count--;
			                thread_ctrl.NO_count++;
							*/
							thread_ctrl.info[input.thread_ID].Tstate = STALL;
							thread_ctrl.info[input.thread_ID].stall_flag = WAIT_SC;
							
							#if DEBUG
							fprintf(dump,"<VS_finish>\n");
							fprintf(dump,"PC = %d\n",input.PC);
							#endif
							
							break;			
		    case FS_THREAD: thread_ctrl.info[input.thread_ID].Ttype = NO_THREAD;
			                thread_ctrl.FS_count--;
			                thread_ctrl.NO_count++;
							
							//release [SC]
							for(i=0;i<14;++i)
							{
							    if(thread_ctrl.info[i].Ttype == SC_THREAD)
								{
								    if(thread_ctrl.info[i].Tstate == STALL && thread_ctrl.info[i].stall_flag == OUT_FULL)
									   thread_ctrl.info[i].Tstate = READY;
								}
							}
                            
							#if DEBUG
							fprintf(dump,"<FS_finish> ->(NO,FS)= (%d,%d)\n",thread_ctrl.NO_count,thread_ctrl.FS_count);
							fprintf(dump,"PC = %d\n",input.PC);
							#endif
							
							break;
		    default: printf("Impossible\n");
			         break;
        }		   
	}
	
	
	//active_sig
	if(input.opcode == FIN && input.GPU_IF_sig == 1)
	   active_sig = 1;
	   
	//is_FRAG_start(FS)
	if(input.thread_type == FS_THREAD && active_sig == 1)
	   RD_EX_if->is_FRAG_start = 1;
	else
	   RD_EX_if->is_FRAG_start = 0;
	   
	//active_FS(SC)
	if(input.thread_type == SC_THREAD && active_sig == 1)
	{
	    thread_ctrl.info[thread_ctrl.FS_slot].Ttype = FS_THREAD;
		thread_ctrl.info[thread_ctrl.FS_slot].PC = 0;
		thread_ctrl.info[thread_ctrl.FS_slot].Tstate = READY;
	
		/**** initialization(BRICK) ****/
		//R58(64,64,0,0)
		GPR[thread_ctrl.FS_slot*64 + 58].x = 64;
		GPR[thread_ctrl.FS_slot*64 + 58].y = 64;
		GPR[thread_ctrl.FS_slot*64 + 58].z = 0;
		GPR[thread_ctrl.FS_slot*64 + 58].w = 0;
		//R59(4,5,0,0)
		GPR[thread_ctrl.FS_slot*64 + 59].x = 4;
		GPR[thread_ctrl.FS_slot*64 + 59].y = 5;
		GPR[thread_ctrl.FS_slot*64 + 59].z = 0;
		GPR[thread_ctrl.FS_slot*64 + 59].w = 0;
		//R60(8,9,0,0)
		GPR[thread_ctrl.FS_slot*64 + 60].x = 8;
		GPR[thread_ctrl.FS_slot*64 + 60].y = 9;
		GPR[thread_ctrl.FS_slot*64 + 60].z = 0;
		GPR[thread_ctrl.FS_slot*64 + 60].w = 0;
		//R61(1,1,1,1)
		GPR[thread_ctrl.FS_slot*64 + 61].x = 1;
		GPR[thread_ctrl.FS_slot*64 + 61].y = 1;
		GPR[thread_ctrl.FS_slot*64 + 61].z = 1;
		GPR[thread_ctrl.FS_slot*64 + 61].w = 1;
		//R62(RED)
		GPR[thread_ctrl.FS_slot*64 + 62].x = 255;
		GPR[thread_ctrl.FS_slot*64 + 62].y = 0;
		GPR[thread_ctrl.FS_slot*64 + 62].z = 0;
		GPR[thread_ctrl.FS_slot*64 + 62].w = 0;
		//R63(WHITE)
		GPR[thread_ctrl.FS_slot*64 + 63].x = 255;
		GPR[thread_ctrl.FS_slot*64 + 63].y = 255;
		GPR[thread_ctrl.FS_slot*64 + 63].z = 255;
		GPR[thread_ctrl.FS_slot*64 + 63].w = 0;		
	
		thread_ctrl.FS_slot = -1;		
		thread_ctrl.FS_count++;
		thread_ctrl.NO_count--;	
		
		
		#if DEBUG
        fprintf(dump,"<FS_active> ->(NO,FS)= (%d,%d)\n",thread_ctrl.NO_count,thread_ctrl.FS_count);
        fprintf(dump,"PC = %d\n",input.PC);
		#endif
		
	}
	
	
	//assert VS_done
	if(input.thread_type == VS_THREAD && active_sig == 1)
	{
		//find triangle order
		for(i=0;i<2;++i)
		for(j=0;j<3;++j)
		{
			if(thread_ctrl.triangle[i][j] == input.thread_ID)
			{
			    thread_ctrl.VS_done[j] = 1;//vertex done
				
				#if DEBUG
				fprintf(dump,"assert VS_done[%d]\n",j);
				#endif
			}	
		}
    }
	
	//active_SC(VS)
	if(input.thread_type == VS_THREAD && (input.opcode == FIN && input.GPU_IF_sig == 0))
	{
		temp = 0;
		for(i=0;i<3;++i)
		   temp += thread_ctrl.VS_done[i];
		   
		if(temp == 3)
		{
		    thread_ctrl.info[thread_ctrl.SC_slot].Ttype = SC_THREAD;
			thread_ctrl.info[thread_ctrl.SC_slot].PC = 0;
			thread_ctrl.info[thread_ctrl.SC_slot].Tstate = READY;
			
			/**** set [GPR] constant ****/		
			//R0 = (0,0,0,0)
			GPR[64 * thread_ctrl.SC_slot + 0].x = 0;
			GPR[64 * thread_ctrl.SC_slot + 0].y = 0;
			GPR[64 * thread_ctrl.SC_slot + 0].z = 0;
			GPR[64 * thread_ctrl.SC_slot + 0].w = 0;
			//R1 = (1,0,0,0)
			GPR[64 * thread_ctrl.SC_slot + 1].x = 1;
			GPR[64 * thread_ctrl.SC_slot + 1].y = 0;
			GPR[64 * thread_ctrl.SC_slot + 1].z = 0;
			GPR[64 * thread_ctrl.SC_slot + 1].w = 0;			
			//R20 = (320,240,0,0)
			GPR[64 * thread_ctrl.SC_slot + 20].x = 320;
			GPR[64 * thread_ctrl.SC_slot + 20].y = 240;
			GPR[64 * thread_ctrl.SC_slot + 20].z = 0;
			GPR[64 * thread_ctrl.SC_slot + 20].w = 0;			
			//R63 = (0,0,0,0)
			GPR[64 * thread_ctrl.SC_slot + 63].x = 0;
			GPR[64 * thread_ctrl.SC_slot + 63].y = 0;
			GPR[64 * thread_ctrl.SC_slot + 63].z = 0;
			GPR[64 * thread_ctrl.SC_slot + 63].w = 0;
			
		    //reset
			for(i=0;i<3;++i)
			{
		        thread_ctrl.VS_done[i] = 0;
				thread_ctrl.info[thread_ctrl.triangle[thread_ctrl.tri_priority][i]].Ttype = NO_THREAD;
				thread_ctrl.triangle[thread_ctrl.tri_priority][i] = -1;
			}			   
			
			thread_ctrl.SC_slot = -1;			   
			thread_ctrl.tri_priority = (thread_ctrl.tri_priority + 1) % 2;
			thread_ctrl.VS_count-=3;
			thread_ctrl.NO_count+=3;
			
			#if DEBUG
            fprintf(dump,"<SC_active> ->(NO,VS)= (%d,%d)\n",thread_ctrl.NO_count,thread_ctrl.VS_count);			
			#endif			
			
			thread_ctrl.SC_count++;
			thread_ctrl.NO_count--;
			
			#if DEBUG
            fprintf(dump,"            ->(NO,SC)= (%d,%d)\n",thread_ctrl.NO_count,thread_ctrl.SC_count);
            fprintf(dump,"PC = %d\n",input.PC);
			#endif
		}
	}
	
	//active_VS(FIFO control)
	if(WB_output.wen == 0)
	{		
		if(V_fifo_ctrl.state == 0)
		{					
			//have empty thread slot?
			if(thread_ctrl.VS_count < 6)
			{		
				//Vfifo empty?
				if(Vfifo_read(V_fifo_ctrl.fifo,&V_fifo_ctrl.Vinfo) == 1)
			    {
				    //VS_slot
					thread_ctrl.VS_slot = Vfifo_slot();//@@@@
					
					//1st attrib
					mov(V_fifo_ctrl.Vinfo.attrib[V_fifo_ctrl.attrib_num],&Vfifo2GPR_Wdata);
					
				    Vfifo2GPR_wen = 1;
				    Vfifo2GPR_dst = V_fifo_ctrl.attrib_num;
					
				    V_fifo_ctrl.state = 1;
					
					#if DEBUG
					fprintf(dump,"FIFO -> GPR[%d]\n",Vfifo2GPR_dst);
					fprintf(dump,"FIFO -> GPR(X) = %f\n",Vfifo2GPR_Wdata.x);
					fprintf(dump,"FIFO -> GPR(Y) = %f\n",Vfifo2GPR_Wdata.y);
					fprintf(dump,"FIFO -> GPR(Z) = %f\n",Vfifo2GPR_Wdata.z);
					fprintf(dump,"FIFO -> GPR(W) = %f\n",Vfifo2GPR_Wdata.w);
					#endif
				}
			}					
	    }
		else
		{
		    V_fifo_ctrl.attrib_num++;
			
			if(V_fifo_ctrl.attrib_num == 4)//built-in varying attrib num = 4
			{
			    V_fifo_ctrl.attrib_num = 0;
				V_fifo_ctrl.state = 0;
				
				thread_ctrl.info[thread_ctrl.VS_slot].Ttype = VS_THREAD;
				thread_ctrl.info[thread_ctrl.VS_slot].PC = 0;
				thread_ctrl.info[thread_ctrl.VS_slot].Tstate = READY;

				for(i=0;i<3;++i)
				{   
				    if(thread_ctrl.triangle[thread_ctrl.tri_slot][i] == -1)
					{   
					    thread_ctrl.triangle[thread_ctrl.tri_slot][i] = thread_ctrl.VS_slot;
                        
						#if DEBUG
						fprintf(dump,"assign triangle[%d][%d]\n",thread_ctrl.tri_slot,i);
						#endif
						
						if(i == 2)
						   thread_ctrl.tri_slot = (thread_ctrl.tri_slot + 1) % 2;		
						break;						
				    }
				}
				
				thread_ctrl.VS_slot = -1;
				thread_ctrl.VS_count++;
				thread_ctrl.NO_count--;
				
				#if DEBUG
                fprintf(dump,"<VS_active> ->(NO,VS)= (%d,%d)\n",thread_ctrl.NO_count,thread_ctrl.VS_count);
                fprintf(dump,"PC = %d\n",input.PC);
                #endif				
			}
			else
			{
			    mov(V_fifo_ctrl.Vinfo.attrib[V_fifo_ctrl.attrib_num],&Vfifo2GPR_Wdata);
				
				Vfifo2GPR_wen = 1;
				Vfifo2GPR_dst = V_fifo_ctrl.attrib_num;
                
                #if DEBUG			
			    fprintf(dump,"FIFO -> GPR[%d]\n",Vfifo2GPR_dst);
				fprintf(dump,"FIFO -> GPR(X) = %f\n",Vfifo2GPR_Wdata.x);
				fprintf(dump,"FIFO -> GPR(Y) = %f\n",Vfifo2GPR_Wdata.y);
				fprintf(dump,"FIFO -> GPR(Z) = %f\n",Vfifo2GPR_Wdata.z);
				fprintf(dump,"FIFO -> GPR(W) = %f\n",Vfifo2GPR_Wdata.w);
				#endif
			}
		}
		NOP_count++;
	}
	
	//WB convt
	WB_convt(input,RD_EX_if);
	
    //[READ_out]
	output->Tid_ori = input.thread_ID;
	output->Tid_wb_convt = RD_EX_if->thread_ID;
	output->dst_addr = RD_EX_if->dst_addr;
	output->out_sig = RD_EX_if->out_sig;
	output->stall_READ = out_flush + T_flush;

	//flush [RD]|[EX1]'s opcode
	if(input.RAW_dep == 1 || T_flush == 1 || out_flush == 1)
	{
	    RD_EX_if->opcode = NOP;
	    RD_EX_if->wen = 0;
		
		output->Wen = 0;

		#if DEBUG
		if(SYS_finish == 36)
		    fprintf(dump,"flush [RD]|[EX1]~~!!\n");
		#endif
	}
    else
    {
	    RD_EX_if->opcode = input.opcode;
	    RD_EX_if->wen = Wen;
		
		output->Wen = Wen;
    }
	
    //flush [ID]'s opcode
	if(input.RAW_dep == 1 || T_flush == 1 || out_flush == 1)
	   stall_occur = 1;
	else	  
	   stall_occur = 0;

	//[READ] [EXE] interface
	RD_EX_if->thread_type = input.thread_type;
	RD_EX_if->W_mask = input.W_mask;
	RD_EX_if->RL = input.RL;
	RD_EX_if->sa = input.sa;
	RD_EX_if->FXL = input.FXL;
	RD_EX_if->imm_value = input.imm_value;
	RD_EX_if->TEX_data.x = (float)(TEX_color >> 24);
    RD_EX_if->TEX_data.y = (float)((TEX_color & 0x00ff0000) >> 16);
	RD_EX_if->TEX_data.z = (float)((TEX_color & 0x0000ff00) >> 8);
	RD_EX_if->TEX_data.w = (float)(TEX_color & 0x000000ff);
	RD_EX_if->CMEM_Rdata = CMEM[(input.imm_value%512)];//CMEM read
    RD_EX_if->FRAG_mode = input.FRAG_mode;
	
	mov(SRC_data[0],&RD_EX_if->SRC0_data);
	mov(SRC_data[1],&RD_EX_if->SRC1_data);
	mov(SRC_data[2],&RD_EX_if->SRC2_data);
	
    //control unit (need to recover PC)
    ctrl_unit(stall_occur,input,SRC_data,output);
	
	//GPR write & FRAG output pool write
	if(Vfifo2GPR_wen == 1)
	{
	    mov(Vfifo2GPR_Wdata,&GPR[64 * thread_ctrl.VS_slot + Vfifo2GPR_dst]);
		
		//fprintf(dump,"Vfifo2GPR_dst = %d\n",(64 * thread_ctrl.VS_slot + Vfifo2GPR_dst));	    
	}
	else if(WB_output.wen == 1)
	{   
		if(WB_output.out_sig == 0)
		{
		    //GPR
			mov(WB_output.Wdata,&GPR[64 * WB_output.thread_ID + WB_output.dst_addr]);
		}
		else
		{
		    //FRAG out pool
			switch(WB_output.dst_addr)
			{
			    case 0: if(WB_output.Wdata.x > 256)
				           int_X = 255;
						else if(WB_output.Wdata.x < 0)
						   int_X = 0;
						else
				           int_X = (unsigned int)WB_output.Wdata.x;
						   
						if(WB_output.Wdata.y > 256)
				           int_Y = 255;
						else if(WB_output.Wdata.y < 0)
						   int_Y = 0;
						else
				           int_Y = (unsigned int)WB_output.Wdata.y;
						
						if(WB_output.Wdata.z > 256)
				           int_Z = 255;
						else if(WB_output.Wdata.z < 0)
						   int_Z = 0;
						else
				           int_Z = (unsigned int)WB_output.Wdata.z;
						   
						if(WB_output.Wdata.w > 256)
				           int_W = 255;
						else if(WB_output.Wdata.w < 0)
						   int_W = 0;
						else
				           int_W = (unsigned int)WB_output.Wdata.w;
               
                        int_color = 0 | (int_W<<24) | (int_X<<16) | (int_Y<<8) | int_Z;					
				        FRAG.out_pool[WB_output.thread_ID].color = int_color;
				        break;
				case 1: FRAG.out_pool[WB_output.thread_ID].x = (unsigned int)WB_output.Wdata.x;
				        FRAG.out_pool[WB_output.thread_ID].y = (unsigned int)WB_output.Wdata.y;
				        break;
				case 2: FRAG.out_pool[WB_output.thread_ID].z = (unsigned int)WB_output.Wdata.x;
				        break;
				default: //printf("over FRAG out pool size(4)\n");
				         break;
			}
		}
	}
	
	//CMEM write
	if(WB_output.CMEM_Wen == 1)
	{
	    CMEM[WB_output.CMEM_Waddr] = *((unsigned int*)&WB_output.CMEM_Wdata);
	}
	//---------------------------------------------------------
    
	#if DEBUG
	if(SYS_finish == 36)
	{
	   fprintf(dump,"Tid = %d\n",RD_EX_if->thread_ID);	
	   fprintf(dump,"opcode = %d\n",RD_EX_if->opcode);
	   fprintf(dump,"dst_addr = %d\n",RD_EX_if->dst_addr);
    }
	#endif	
}


void IR_ctrl(unsigned int IM_data_L,unsigned int IM_data_R,struct ID_READ_if *output)
{
    unsigned int opcode;
	int          temp;
	
	opcode = IM_data_L >> 27;//[63:59]
	output->opcode = opcode;

    if(opcode >=24 && opcode <= 29)//BEQ,BNEQ,BLT,BGE,RET,JUMP
    {
        output->dst_addr = 0;
        output->src_addr[0] = (IM_data_L & 0x07e00000)>>21;//[58:53]
        output->src_addr[1] = (IM_data_L & 0x001f8000)>>15;//[52:47]
        output->src_addr[2] = 0;
		
		temp = ((IM_data_L & 0x00007fff)<<1) + (IM_data_R>>31);//[46:31]
		
		if((temp & 0x00008000) != 0)//sign = 1
		   output->branch_addr = temp | 0xffff0000;//sign extension
		else
		   output->branch_addr = temp;
        
		output->swizzle[0] = (IM_data_R & 0x7f800000)>>23;//[30:23]
        output->swizzle[1] = (IM_data_R & 0x007f8000)>>15;//[22:15]
        output->swizzle[2] = 27;//00011011
        output->neg[0] = 0;
        output->neg[1] = 0;
        output->neg[2] = 0;
        output->W_mask = 15;//1111
		output->FXL = (IM_data_R & 0x00000200)>>9;//[9]
        output->SV = (IM_data_R & 0x00000180)>>7;//[8:7]
		output->GPU_IF_sig = (IM_data_R & 0x00000040)>>6;//[6]
		output->FRAG_mode = (IM_data_R & 0x00000020)>>5;//[5]
        output->RL = 0;
		output->sa = 0;
        output->imm_value = 0;       
    }
    else if(opcode>=8 && opcode <= 10)//LD,ST,MVI
    {
        if(opcode == ST)
		{
		    output->dst_addr = (IM_data_L & 0x07e00000)>>21;//[58:53]
			output->src_addr[0] = (IM_data_L & 0x001f8000)>>15;//[52:47]		    
		}
		else
		{
		    output->src_addr[0] = (IM_data_L & 0x07e00000)>>21;//[58:53]
			output->dst_addr = (IM_data_L & 0x001f8000)>>15;//[52:47]		
		}
		
        output->imm_value = ((IM_data_L & 0x00007fff)<<17) + ((IM_data_R & 0xffff8000)>>15);//[46:15]
        output->src_addr[1] = 0;
        output->src_addr[2] = output->dst_addr;
        output->swizzle[0] = 27;//00011011
        output->swizzle[1] = 27;//00011011
        output->swizzle[2] = 27;//00011011
        output->neg[0] = 0;
        output->neg[1] = 0;
        output->neg[2] = 0;
        output->W_mask = (IM_data_R & 0x00007800)>>10;//[13:10]
        output->RL = 0;
        output->FXL = (IM_data_R & 0x00000200)>>9;//[9]
        output->SV = (IM_data_R & 0x00000180)>>7;//[8:7]
		output->GPU_IF_sig = (IM_data_R & 0x00000040)>>6;//[6]
		output->FRAG_mode = (IM_data_R & 0x00000020)>>5;//[5]        
        output->sa = 0;
        output->branch_addr = 0;
    }
    else if(opcode == 30)//SHRL
    {
        output->dst_addr = (IM_data_L & 0x07e00000)>>21;//[58:53]
        output->src_addr[0] = (IM_data_L & 0x001f8000)>>15;//[52:47]
        output->src_addr[1] = 0;
        output->src_addr[2] = (IM_data_L & 0x07e00000)>>21;//[58:53]
        output->swizzle[0] = (IM_data_L & 0x000001fe)>>1;//[40:33]
        output->swizzle[1] = ((IM_data_L & 0x00000001)<<7) + ((IM_data_R & 0xfe000000)>>25);//[32:25]
        output->swizzle[2] = (IM_data_R & 0x01fe0000)>>17;//[24:17]
        output->neg[0] = (IM_data_R & 0x00010000)>>16;//[16]
        output->neg[1] = (IM_data_R & 0x00008000)>>15;//[15]
        output->neg[2] = (IM_data_R & 0x00004000)>>14;//[14]
        output->W_mask = (IM_data_R & 0x00003c00)>>10;//[13:10]
        output->RL = (IM_data_L & 0x00000200)>>9;//[41]
        output->FXL = (IM_data_R & 0x00000200)>>9;//[9]
        output->SV = (IM_data_R & 0x00000180)>>7;//[8:7]
		output->GPU_IF_sig = (IM_data_R & 0x00000040)>>6;//[6]
		output->FRAG_mode = (IM_data_R & 0x00000020)>>5;//[5] 		
        output->imm_value = 0;
        output->sa = (IM_data_L & 0x001f8000)>>15;//[46:42]
        output->branch_addr = 0;
    }
    else
    {
        output->dst_addr = (IM_data_L & 0x07e00000)>>21;//[58:53]
        output->src_addr[0] = (IM_data_L & 0x001f8000)>>15;//[52:47]
        output->src_addr[1] = (IM_data_L & 0x00007e00)>>9;//[46:41]
        output->src_addr[2] = (IM_data_L & 0x07e00000)>>21;//[58:53]        
        output->swizzle[0] = (IM_data_L & 0x000001fe)>>1;//[40:33]
        output->swizzle[1] = ((IM_data_L & 0x00000001)<<7) + ((IM_data_R & 0xfe000000)>>25);//[32:25]
        output->swizzle[2] = (IM_data_R & 0x01fe0000)>>17;//[24:17]
        output->neg[0] = (IM_data_R & 0x00010000)>>16;//[16]
        output->neg[1] = (IM_data_R & 0x00008000)>>15;//[15]
        output->neg[2] = (IM_data_R & 0x00004000)>>14;//[14]
        output->W_mask = (IM_data_R & 0x00003c00)>>10;//[13:10]
        output->RL = 0;
        output->FXL = (IM_data_R & 0x00000200)>>9;//[9]
        output->SV = (IM_data_R & 0x00000180)>>7;//[8:7]
		output->GPU_IF_sig = (IM_data_R & 0x00000040)>>6;//[6]
		output->FRAG_mode = (IM_data_R & 0x00000020)>>5;//[5]		
        output->imm_value = 0;
        output->sa = 0;
        output->branch_addr = 0;
    }
}


int classify(unsigned int opcode)
{
    //return source operand num.
    switch(opcode)
	{
	    case RCP: return 1;
		case RSQ: return 1;
		case LOG2: return 1;
		case POW2: return 1;
		case DP3: return 2;
		case MOV: return 1;
		case LD: return 0;
		case ST: return 1;
		case MVI: return 0;
		case MIN: return 2;
		case MAX: return 2;
		case TXB: return 1;
		case FIN: return 0;
		case TEX2D: return 1;
		case FADD: return 2;
		case FMUL: return 2;
		case FMAD: return 3;
		case DP4: return 2;
		case INV: return 2;
		case AND: return 2;
		case OR: return 2;
		case XOR: return 2;
		case BEQ: return 2;
		case BNEQ: return 2;
		case BLT: return 2;
		case BGE: return 2;
		case RET: return 1;
		case JUMP: return 1;
		case SHRL: return 1;
		case NOP: return 0;
		default: return 0;	
	}
}


void DECODE(struct IF_ID_if input,struct READ_out RD_out,struct ID_READ_if *output,struct READ_EX1_if *RD_EX1_if)
{
    int          i;
	unsigned int W_Tid,dst_addr;
	int          wen,out_sig;
	int          src0_dep,src1_dep,src2_dep;
	int          RAW_dep;
	unsigned int RAW_count = 0;
	int          type;

	#if DEBUG
	if(SYS_finish == 36)
	{
	    fprintf(dump,"[DECODE]\n");
	    fprintf(dump,"========================\n");
    }
	#endif
	
	//IR control
    IR_ctrl(input.IM_data_L,input.IM_data_R,output);
	
    #if DEBUG
	if(SYS_finish == 36)
	{
	    fprintf(dump,"IM_L = %x\n",input.IM_data_L);
	    fprintf(dump,"IM_R = %x\n",input.IM_data_R);
    }
	#endif	

	//flush [ID]'s opcode
	if((RD_out.Tid_ori == input.thread_ID)  && (RD_out.flush_ID == 1))
	{
	    output->opcode = NOP;
		
		#if DEBUG
		if(SYS_finish == 36)
		   fprintf(dump,"flush [ID]~~!!\n");
		#endif
	}
	
	//RAW detector(RD,EX1~4)	
	for(i=4;i>-1;--i)
	{
	    if(i==0)//RD
		{
		    W_Tid = RD_out.Tid_wb_convt;
			dst_addr = RD_out.dst_addr;
			wen = RD_out.Wen;
			out_sig = RD_out.out_sig;
		}
		else//EX1~4
		{
		    W_Tid = EXE_out.Tid_EXE[i-1];
			dst_addr = EXE_out.dst_addr_EXE[i-1];
			wen = EXE_out.wen_EXE[i-1];
			out_sig = EXE_out.out_sig_EXE[i-1];		    
		}
		/*
		if(SYS_finish == 1)
		{
		    fprintf(dump,"W_Tid[%d] = %d\n",i,W_Tid);
			fprintf(dump,"dst_addr[%d] = %d\n",i,dst_addr);
			fprintf(dump,"wen[%d] = %d\n",i,wen);
			fprintf(dump,"out_sig[%d] = %d\n",i,out_sig);
		}
        */		
        if(wen == 1 && out_sig == 0)
        {
            if(W_Tid == input.thread_ID)
			{
			    src0_dep = (dst_addr == output->src_addr[0])? 1 : 0;
				src1_dep = (dst_addr == output->src_addr[1])? 1 : 0;
				src2_dep = (dst_addr == output->src_addr[2])? 1 : 0;
				
				type = classify(output->opcode);
				
				switch(type)
				{
				    case 0: //fprintf(dump,"No RAW hazard~~!!\n");
					        break;
					case 1: if(src0_dep == 1)
					           //RAW_count++;
							   RAW_count = 5-i;
							break;
					case 2: if(src0_dep == 1 || src1_dep == 1)
					           //RAW_count++;
							   RAW_count = 5-i;
							break;
					case 3: if(src0_dep == 1 || src1_dep == 1 || src0_dep == 1)
					           //RAW_count++;
							   RAW_count = 5-i;
							break;
					default: printf("impossible\n");
							 break;
				}
			}
        }		
	}
	
	//RAW_dep
	if(RAW_count>0)	   
	   RAW_dep = 1;
	else
	   RAW_dep = 0;
	   
	//(READY) -> (STALL)
	if(RAW_dep == 1 && RD_out.stall_READ == 0)
	{
	    #if DEBUG
		if(SYS_finish == 36)   
		   fprintf(dump,"<RAW dep>\n");
		#endif
		
		thread_ctrl.info[input.thread_ID].Tstate = STALL;
		thread_ctrl.info[input.thread_ID].stall_flag = DATA_DEP;
		thread_ctrl.info[input.thread_ID].RAW_count = RAW_count;
	}
	
	output->RAW_dep = RAW_dep;
	output->thread_ID = input.thread_ID;
	output->thread_type = input.thread_type;
	output->PC = input.PC;
	//---------------------------------------------------------
    
    #if DEBUG
    if(SYS_finish == 36)
    {   	
	    fprintf(dump,"opcode = %d\n",output->opcode);
	    fprintf(dump,"dst_addr = %d\n",output->dst_addr);
	    fprintf(dump,"SRC0 = %d\n",output->src_addr[0]);
	    fprintf(dump,"SRC1 = %d\n",output->src_addr[1]);
	    fprintf(dump,"SRC2 = %d\n",output->src_addr[2]);
	    fprintf(dump,"swizzle0 = %d\n",output->swizzle[0]);
	    fprintf(dump,"swizzle1 = %d\n",output->swizzle[1]);
	    fprintf(dump,"swizzle2 = %d\n",output->swizzle[2]);
	    fprintf(dump,"neg0 = %d\n",output->neg[0]);
	    fprintf(dump,"neg1 = %d\n",output->neg[1]);
	    fprintf(dump,"neg2 = %d\n",output->neg[2]);
	    fprintf(dump,"SV = %d\n",output->SV);
	    fprintf(dump,"FXL = %d\n",output->FXL);
	    fprintf(dump,"W_mask = %d\n",output->W_mask);
	    fprintf(dump,"GPU_IF_sig = %d\n",output->GPU_IF_sig);
	    fprintf(dump,"branch_addr = %d\n",output->branch_addr);
    }
    #endif
}


int scheduler(void)
{
	int          i,j,k;
	unsigned int SC_ready = 0,VS_ready = 0,FS_ready = 0;
	unsigned int SC_record,FS_record[7];
	unsigned int VS_record[6];
	int          tri_index;
	int          Tid = -1;
	int          PC_min = IM_LENGTH;//MAX	
	
	
	//initial
	for(i=0;i<6;++i)
	   VS_record[i] = 0;
	
	//statistic
	for(i=0;i<14;++i)
	{
	    switch(thread_ctrl.info[i].Ttype)
		{
		    case SC_THREAD: if(thread_ctrl.info[i].Tstate == READY)
			                {
							    SC_record = i;
                                SC_ready++;
							}
			                break;
			case VS_THREAD: if(thread_ctrl.info[i].Tstate == READY)
                            {
							    for(j=0;j<2;++j)
								   for(k=0;k<3;++k)
								      if(thread_ctrl.triangle[j][k] == i)
									  {   
										  VS_record[j*3+k] = 1;
                                      }
							    VS_ready++;
							}
							break;
			case FS_THREAD: if(thread_ctrl.info[i].Tstate == READY)
                            {
							    FS_record[FS_ready] = i;
								FS_ready++;
							}
							break;
			default: //printf("Don't mind~~!!!\n");
			         break;
		}
	}
/*	
	fprintf(dump,"VS_ready = %d\n",VS_ready);
*/	
	//issue policy
	if(FS_ready > 0)
	{
	    for(i=0;i<FS_ready;++i)
		{
		    if(thread_ctrl.info[FS_record[i]].PC < PC_min)
			{
			    Tid = FS_record[i];
				PC_min = thread_ctrl.info[FS_record[i]].PC;
			}
		}
	}
	else if(SC_ready > 0)
	{
	    Tid = SC_record;		
	}
	else if(VS_ready > 0)//consider triangle num.
	{	
		tri_index = thread_ctrl.tri_priority;
		
		for(i=0;i<3;++i)
        {			
			if(VS_record[tri_index*3+i] == 1)
			{
			    if(thread_ctrl.info[thread_ctrl.triangle[tri_index][i]].PC < PC_min)
				{
				    Tid = thread_ctrl.triangle[tri_index][i];
					PC_min = thread_ctrl.info[thread_ctrl.triangle[tri_index][i]].PC;				
				}
			}
		}
		
		if(Tid == -1)
		{
		    tri_index = (tri_index + 1) % 2;
			PC_min = IM_LENGTH;
			
			for(i=0;i<3;++i)
			{
			    if(VS_record[tri_index*3+i] == 1)
				{
				    if(thread_ctrl.info[thread_ctrl.triangle[tri_index][i]].PC < PC_min)
					{
					    Tid = thread_ctrl.triangle[tri_index][i];
						PC_min = thread_ctrl.info[thread_ctrl.triangle[tri_index][i]].PC;
					}
				}			
			}
		}
	}

    return Tid;	
}


void FETCH(struct READ_out RD_out,struct IF_ID_if *output)
{
    int Tid;

    #if DEBUG	
	if(SYS_finish == 36)
	{
	    fprintf(dump,"[FETCH]\n");
	    fprintf(dump,"========================\n");
    }
	#endif
	
	//branch update
	if(RD_out.branch_hit == 1)
	{
	    thread_ctrl.info[RD_out.branch_Tid].PC = RD_out.branch_target;
	}
	
	//schedule
	Tid = scheduler();

	//fetch instruction
	if(Tid == -1)
	{
	    output->thread_ID = 0;
		output->thread_type = NO_THREAD;//@@
		output->PC = 0;
		output->IM_data_L = 0xf8000000;//NOP
		output->IM_data_R = 0x00000000;
	}
	else
	{
	    output->thread_ID = Tid;
		output->thread_type = thread_ctrl.info[Tid].Ttype;
		output->PC = thread_ctrl.info[Tid].PC;
		
		switch(thread_ctrl.info[Tid].Ttype)
		{
		    case SC_THREAD: output->IM_data_L = SC_IM_L[thread_ctrl.info[Tid].PC];
			                output->IM_data_R = SC_IM_R[thread_ctrl.info[Tid].PC];
							break;
			case VS_THREAD: output->IM_data_L = VS_IM_L[thread_ctrl.info[Tid].PC];
			                output->IM_data_R = VS_IM_R[thread_ctrl.info[Tid].PC];
							break;
			case FS_THREAD: output->IM_data_L = FS_IM_L[thread_ctrl.info[Tid].PC];
			                output->IM_data_R = FS_IM_R[thread_ctrl.info[Tid].PC];
							break;
			default: printf("impossible\n");
			         break;
		}
	}
    
	
	#if DEBUG
	if(SYS_finish == 36)
	{   
	   fprintf(dump,"FETCH(Tid) = %d\n",Tid);
	   fprintf(dump,"FETCH(Ttype) = %d\n",output->thread_type);
	   fprintf(dump,"PC = %d\n",output->PC);
	   fprintf(dump,"IM_L = %x\n",output->IM_data_L);
	   fprintf(dump,"IM_R = %x\n",output->IM_data_R);		
	}
	#endif
	
	//update PC
	if(Tid != -1)
	   thread_ctrl.info[Tid].PC += 1;
}


//Image input
void TEXTURE_init(void)
{
   int i,j;
   FILE *pFile_img=fopen("wood2.bmp","r");
   char bmp_info[54];
   //Original image created by sub5
   /*
   for(i=0;i<64;++i)
     {   
       for(j=0;j<64;++j)
	 {	  
	   if(((i<16)||(i>31&&i<48))&&((j<16)||(j>31&&j<48)))
	     TEXTURE[j+i*64] = 0x00ff0000;
	   else if(((i>15&&i<32)||(i>48))&&((j>15&&j<32)||(j>48)))
	     TEXTURE[j+i*64] = 0xff000000;
	   else
	     TEXTURE[j+i*64] = 0x0000ff00;
	 }
     }
   */
   //END original image

   //Wood image

   for(i=0;i<54;i++)
     {
       bmp_info[i]=getc(pFile_img);
     }
   for(i=0;i<TEX_HEIGHT;++i)
   {   
     for(j=0;j<TEX_WIDTH;++j)
       {
	 int k;
	 int get_color;
	 for(k=3;k>0;k--)
	   {
	     get_color=(int)getc(pFile_img);
	     TEXTURE[j+i*TEX_HEIGHT] |= get_color;//<<8*k;
	     TEXTURE[j+i*TEX_HEIGHT] = TEXTURE[j+i*TEX_HEIGHT] << 8;
	   }
       }
   }
   //END Wood image

}


void SC_IM_init(void)
{
    SC_IM_L[0] = 0x836b2e36;
    SC_IM_L[1] = 0x838baa36;
    SC_IM_L[2] = 0x83aaac36;
    SC_IM_L[3] = 0x8bcf2a37;
    SC_IM_L[4] = 0x8befac37;
    SC_IM_L[5] = 0x8c102e37;
    SC_IM_L[6] = 0x84f2cc36;
    SC_IM_L[7] = 0x85134836;
    SC_IM_L[8] = 0x85324a36;
    SC_IM_L[9] = 0x8a4daaaa;
    SC_IM_L[10] = 0x3eee8000;
    SC_IM_L[11] = 0x3f0e80aa;
    SC_IM_L[12] = 0x842fc036;
    SC_IM_L[13] = 0x84503c36;
    SC_IM_L[14] = 0x846f3e36;
    SC_IM_L[15] = 0x85b5d836;
    SC_IM_L[16] = 0x85d65436;
    SC_IM_L[17] = 0x85f55636;
    SC_IM_L[18] = 0x8b2dab76;
    SC_IM_L[19] = 0x8b4dab76;
    SC_IM_L[20] = 0x8cb3aa36;
    SC_IM_L[21] = 0x8cd3aa36;
    SC_IM_L[22] = 0x924e2caa;
    SC_IM_L[23] = 0x8bf0aa36;
    SC_IM_L[24] = 0x8c10aa36;
    SC_IM_L[25] = 0x8d76aa36;
    SC_IM_L[26] = 0x8d96aa36;
    SC_IM_L[27] = 0x66caac36;
    SC_IM_L[28] = 0x5e2aac36;
    SC_IM_L[29] = 0x932e2d76;
    SC_IM_L[30] = 0x934e2d76;
    SC_IM_L[31] = 0x924eaeaa;
    SC_IM_L[32] = 0x94b42c36;
    SC_IM_L[33] = 0x94d42c36;
    SC_IM_L[34] = 0x93f12c36;
    SC_IM_L[35] = 0x94112c36;
    SC_IM_L[36] = 0x95772c36;
    SC_IM_L[37] = 0x95972c36;
    SC_IM_L[38] = 0xca400001;
    SC_IM_L[39] = 0xf8000000;
    SC_IM_L[40] = 0x70000000;
    SC_IM_L[41] = 0xf8000000;
    SC_IM_L[42] = 0x0a490036;
    SC_IM_L[43] = 0x66db2e36;
    SC_IM_L[44] = 0x5e38ae36;
    SC_IM_L[45] = 0x932eaf76;
    SC_IM_L[46] = 0x934eaf76;
    SC_IM_L[47] = 0x94b4ae36;
    SC_IM_L[48] = 0x94d4ae36;
    SC_IM_L[49] = 0x93f1ae36;
    SC_IM_L[50] = 0x9411ae36;
    SC_IM_L[51] = 0x9577ae36;
    SC_IM_L[52] = 0x9597ae36;
    SC_IM_L[53] = 0x8278aa36;
    SC_IM_L[54] = 0x8618aa36;
    SC_IM_L[55] = 0x8238aaaa;
    SC_IM_L[56] = 0x5edb2836;
    SC_IM_L[57] = 0x3eed8000;
    SC_IM_L[58] = 0x3f0d80aa;
    SC_IM_L[59] = 0x8f293236;
    SC_IM_L[60] = 0x8b493436;
    SC_IM_L[61] = 0x8f493e36;
    SC_IM_L[62] = 0x8c094036;
    SC_IM_L[63] = 0x8618ac00;
    SC_IM_L[64] = 0x8238acaa;
    SC_IM_L[65] = 0x8f694a36;
    SC_IM_L[66] = 0x8f894c36;
    SC_IM_L[67] = 0x8fa95636;
    SC_IM_L[68] = 0x8fc95836;
    SC_IM_L[69] = 0x3eee0000;
    SC_IM_L[70] = 0x3f0e00aa;
    SC_IM_L[71] = 0x3f2d0022;
    SC_IM_L[72] = 0x3f500022;
    SC_IM_L[73] = 0x8618ae00;
    SC_IM_L[74] = 0x8238aeaa;
    SC_IM_L[75] = 0x8e89f600;
    SC_IM_L[76] = 0x8ea9fa00;
    SC_IM_L[77] = 0xf8000000;
    SC_IM_L[78] = 0x8e49f200;
    SC_IM_L[79] = 0x8e69f400;
    SC_IM_L[80] = 0x8e187036;
    SC_IM_L[81] = 0x8a28ee36;
    SC_IM_L[82] = 0x9689f8aa;
    SC_IM_L[83] = 0x96a9fcaa;
    SC_IM_L[84] = 0xf8000000;
    SC_IM_L[85] = 0x9649f2ab;
    SC_IM_L[86] = 0x9669f4ab;
    SC_IM_L[87] = 0xf8000000;
    SC_IM_L[88] = 0x86182236;
    SC_IM_L[89] = 0x869a4836;
    SC_IM_L[90] = 0x86bad436;
    SC_IM_L[91] = 0xf8000000;
    SC_IM_L[92] = 0x86592a37;
    SC_IM_L[93] = 0x8679bc36;
    SC_IM_L[94] = 0xf8000000;
    SC_IM_L[95] = 0xf8000000;
    SC_IM_L[96] = 0xf8000000;
    SC_IM_L[97] = 0xf8000000;
    SC_IM_L[98] = 0x3bd90036;
    SC_IM_L[99] = 0x3df80036;
    SC_IM_L[100] = 0x3dd98036;
    SC_IM_L[101] = 0x3bf88036;
    SC_IM_L[102] = 0x3bba0036;
    SC_IM_L[103] = 0x3b9a8036;
    SC_IM_L[104] = 0x0b6f00aa;
    SC_IM_L[105] = 0x3d9f8154;
    SC_IM_L[106] = 0x3db88036;
    SC_IM_L[107] = 0xdde0000c;
    SC_IM_L[108] = 0x83cf7236;
    SC_IM_L[109] = 0x83ef8236;
    SC_IM_L[110] = 0x85f7f036;
    SC_IM_L[111] = 0x85d77436;
    SC_IM_L[112] = 0x83aef636;
    SC_IM_L[113] = 0x838e7a36;
    SC_IM_L[114] = 0x0b6f00aa;
    SC_IM_L[115] = 0xdecffffc;
    SC_IM_L[116] = 0x83f88236;
    SC_IM_L[117] = 0x85f86e36;
    SC_IM_L[118] = 0x83d97237;
    SC_IM_L[119] = 0x85d9f437;
    SC_IM_L[120] = 0x83ba7836;
    SC_IM_L[121] = 0x839afc36;
    SC_IM_L[122] = 0x3e2f8036;
    SC_IM_L[123] = 0x3e178036;
    SC_IM_L[124] = 0x0b6f00aa;
    SC_IM_L[125] = 0x3e4f0036;
    SC_IM_L[126] = 0x3e770036;
    SC_IM_L[127] = 0x3e8e8036;
    SC_IM_L[128] = 0x3eae0036;
    SC_IM_L[129] = 0xdecffff5;
    SC_IM_L[130] = 0x70000000;
    SC_IM_L[131] = 0x834fda36;
    SC_IM_L[132] = 0x8b6ddc36;
    SC_IM_L[133] = 0xd8160000;
    SC_IM_L[134] = 0x3d9f8154;
    SC_IM_L[135] = 0x380e0036;
    SC_IM_L[136] = 0x382f8036;
    SC_IM_L[137] = 0x384f0036;
    SC_IM_L[138] = 0x386e8036;
    SC_IM_L[139] = 0x388d8036;
    SC_IM_L[140] = 0xf8000000;
    SC_IM_L[141] = 0xf8000000;
    SC_IM_L[142] = 0xf8000000;
    SC_IM_L[143] = 0xf8000000;
    SC_IM_L[144] = 0xf8000000;
    SC_IM_L[145] = 0x70000000;
    SC_IM_L[146] = 0xd8007fed;
	//------------------------
    SC_IM_R[0] = 0x3636bf80;
    SC_IM_R[1] = 0x3636bf80;
    SC_IM_R[2] = 0x3636bf80;
    SC_IM_R[3] = 0xfe363f80;
    SC_IM_R[4] = 0xfe363f80;
    SC_IM_R[5] = 0xfe363f80;
    SC_IM_R[6] = 0x3636bf80;
    SC_IM_R[7] = 0x3636bf80;
    SC_IM_R[8] = 0x3636bf80;
    SC_IM_R[9] = 0x00373f80;
    SC_IM_R[10] = 0x36372380;
    SC_IM_R[11] = 0x36372380;
    SC_IM_R[12] = 0x3636bf80;
    SC_IM_R[13] = 0x3636bf80;
    SC_IM_R[14] = 0x3636bf80;
    SC_IM_R[15] = 0x3636bf80;
    SC_IM_R[16] = 0x3636bf80;
    SC_IM_R[17] = 0x3636bf80;
    SC_IM_R[18] = 0xaa363f80;
    SC_IM_R[19] = 0x00373f80;
    SC_IM_R[20] = 0xaa363f80;
    SC_IM_R[21] = 0x00373f80;
    SC_IM_R[22] = 0x00373f80;
    SC_IM_R[23] = 0xaa363f80;
    SC_IM_R[24] = 0x00373f80;
    SC_IM_R[25] = 0xaa363f80;
    SC_IM_R[26] = 0x00373f80;
    SC_IM_R[27] = 0x36363f80;
    SC_IM_R[28] = 0x36363f80;
    SC_IM_R[29] = 0xaa363f80;
    SC_IM_R[30] = 0x00373f80;
    SC_IM_R[31] = 0x00373f80;
    SC_IM_R[32] = 0xaa363f80;
    SC_IM_R[33] = 0x00373f80;
    SC_IM_R[34] = 0xaa363f80;
    SC_IM_R[35] = 0x00373f80;
    SC_IM_R[36] = 0xaa363f80;
    SC_IM_R[37] = 0x00373f80;
    SC_IM_R[38] = 0x800d8000;
    SC_IM_R[39] = 0x00000000;
    SC_IM_R[40] = 0x00000000;
    SC_IM_R[41] = 0x00000000;
    SC_IM_R[42] = 0x36363f80;
    SC_IM_R[43] = 0x36363f80;
    SC_IM_R[44] = 0x36363f80;
    SC_IM_R[45] = 0xaa363f80;
    SC_IM_R[46] = 0x00373f80;
    SC_IM_R[47] = 0xaa363f80;
    SC_IM_R[48] = 0x00373f80;
    SC_IM_R[49] = 0xaa363f80;
    SC_IM_R[50] = 0x00373f80;
    SC_IM_R[51] = 0xaa363f80;
    SC_IM_R[52] = 0x00373f80;
    SC_IM_R[53] = 0x3636bf80;
    SC_IM_R[54] = 0x3636bf80;
    SC_IM_R[55] = 0xaa36bf80;
    SC_IM_R[56] = 0x36363f80;
    SC_IM_R[57] = 0x36371380;
    SC_IM_R[58] = 0x36371380;
    SC_IM_R[59] = 0x36363f80;
    SC_IM_R[60] = 0x36363f80;
    SC_IM_R[61] = 0x36363f80;
    SC_IM_R[62] = 0x36363f80;
    SC_IM_R[63] = 0x00369380;
    SC_IM_R[64] = 0xaa369380;
    SC_IM_R[65] = 0x36363f80;
    SC_IM_R[66] = 0x36363f80;
    SC_IM_R[67] = 0x36363f80;
    SC_IM_R[68] = 0x36363f80;
    SC_IM_R[69] = 0x36370b80;
    SC_IM_R[70] = 0x36370b80;
    SC_IM_R[71] = 0x36360f80;
    SC_IM_R[72] = 0x36360f80;
    SC_IM_R[73] = 0x00368b80;
    SC_IM_R[74] = 0xaa368b80;
    SC_IM_R[75] = 0x36363f80;
    SC_IM_R[76] = 0x36363f80;
    SC_IM_R[77] = 0x00000000;
    SC_IM_R[78] = 0x36363f80;
    SC_IM_R[79] = 0x36363f80;
    SC_IM_R[80] = 0x36363f80;
    SC_IM_R[81] = 0x36363f80;
    SC_IM_R[82] = 0x36363f80;
    SC_IM_R[83] = 0x36363f80;
    SC_IM_R[84] = 0x00000000;
    SC_IM_R[85] = 0x76363f80;
    SC_IM_R[86] = 0x76363f80;
    SC_IM_R[87] = 0x00000000;
    SC_IM_R[88] = 0x3636bf80;
    SC_IM_R[89] = 0x36363f80;
    SC_IM_R[90] = 0x36363f80;
    SC_IM_R[91] = 0x00000000;
    SC_IM_R[92] = 0x76363f80;
    SC_IM_R[93] = 0x36363f80;
    SC_IM_R[94] = 0x00000000;
    SC_IM_R[95] = 0x00000000;
    SC_IM_R[96] = 0x00000000;
    SC_IM_R[97] = 0x00000000;
    SC_IM_R[98] = 0x36363f80;
    SC_IM_R[99] = 0x36363f80;
    SC_IM_R[100] = 0x36363f80;
    SC_IM_R[101] = 0x36363f80;
    SC_IM_R[102] = 0x36363f80;
    SC_IM_R[103] = 0x36363f80;
    SC_IM_R[104] = 0x36363f80;
    SC_IM_R[105] = 0x36363f80;
    SC_IM_R[106] = 0x36363f80;
    SC_IM_R[107] = 0x0c0d8380;
    SC_IM_R[108] = 0x22363f80;
    SC_IM_R[109] = 0x36362380;
    SC_IM_R[110] = 0x36363f80;
    SC_IM_R[111] = 0x22363f80;
    SC_IM_R[112] = 0x36363f80;
    SC_IM_R[113] = 0x36363f80;
    SC_IM_R[114] = 0x36363f80;
    SC_IM_R[115] = 0x00000380;
    SC_IM_R[116] = 0x8a363f80;
    SC_IM_R[117] = 0x3636bf80;
    SC_IM_R[118] = 0x76363f80;
    SC_IM_R[119] = 0x76363f80;
    SC_IM_R[120] = 0x36363f80;
    SC_IM_R[121] = 0x36363f80;
    SC_IM_R[122] = 0x36363f80;
    SC_IM_R[123] = 0x36363f80;
    SC_IM_R[124] = 0x36363f80;
    SC_IM_R[125] = 0x36363f80;
    SC_IM_R[126] = 0x36363f80;
    SC_IM_R[127] = 0x36363f80;
    SC_IM_R[128] = 0x36363f80;
    SC_IM_R[129] = 0x2aaa8380;
    SC_IM_R[130] = 0x00000000;
    SC_IM_R[131] = 0x3636bf80;
    SC_IM_R[132] = 0x36363f80;
    SC_IM_R[133] = 0x8d8d8380;
    SC_IM_R[134] = 0x36363f80;
    SC_IM_R[135] = 0x36363fc0;
    SC_IM_R[136] = 0x36363fc0;
    SC_IM_R[137] = 0x36363fc0;
    SC_IM_R[138] = 0x36363fc0;
    SC_IM_R[139] = 0x36363fc0;
    SC_IM_R[140] = 0x00000000;
    SC_IM_R[141] = 0x00000000;
    SC_IM_R[142] = 0x00000000;
    SC_IM_R[143] = 0x00000000;
    SC_IM_R[144] = 0x00000000;
    SC_IM_R[145] = 0x00000040;
    SC_IM_R[146] = 0x0d8d8380;	
}


void VS_IM_init(void)
{
   VS_IM_L[0] = 0x38000036;//MOV @R0 R0
   VS_IM_R[0] = 0x36363fc0;
   
   VS_IM_L[1] = 0x38208036;//MOV @R1 R1
   VS_IM_R[1] = 0x36363fc0;

   VS_IM_L[2] = 0x38410036;//MOV @R2 R2
   VS_IM_R[2] = 0x36363fc0;

   VS_IM_L[3] = 0x38618036;//MOV @R3 R3
   VS_IM_R[3] = 0x36363fc0;

   VS_IM_L[4] = 0x70000000;//FRAG start
   VS_IM_R[4] = 0x00000040;

   VS_IM_L[5] = 0x70000000;//FIN
   VS_IM_R[5] = 0x00000000;

   VS_IM_L[6] = 0xf8000000;//NOP
   VS_IM_R[6] = 0x00000000;   
}


//TEX
void FS_IM_init(void)
{
  
  // Original instruction

  //   FS_IM_L[0] = 0x78020036;//TEX @R0 R4  
  //   FS_IM_R[0] = 0x36363fc0;

  //   FS_IM_L[1] = 0x38208036;//MOV @R1 R1   
  //   FS_IM_R[1] = 0x36363fc0;
   
  //   FS_IM_L[2] = 0x38410036;//MOV @R2 R2   
  //   FS_IM_R[2] = 0x36363fc0;
   
  //   FS_IM_L[3] = 0x70000000;//FRAG start   
  //   FS_IM_R[3] = 0x00000040;

  //   FS_IM_L[4] = 0x70000000;//FIN   
  //   FS_IM_R[4] = 0x00000000;   

  //   FS_IM_L[5] = 0xf8000000;//NOP   
  //   FS_IM_R[5] = 0x00000000;

  // END Original instruction


  //PC: 0   5060000000002000:       LDI     R3.1000, R0, 0
  //PC: 8   5080000000002000:       LDI     R4.1000, R0, 0
  //PC: 16  8041880000002000:       ADD     R2.1000, R3, R4
  //PC: 24  4041000000018400:       LD      R2.0001, R2, 3
  //PC: 32  4041000000010800:       LD      R2.0010, R2, 2
  //PC: 40  4041000000009000:       LD      R2.0100, R2, 1
  //PC: 48  4041000000002000:       LD      R2.1000, R2, 0
  //PC: 56  3c01003636003f80:       MVR     R32.1111, R2.xyzw, <4>   

  FS_IM_L[0] = 0x38208036;//MOV @R1 R1   
  FS_IM_R[0] = 0x36363fc0;

  FS_IM_L[1] = 0x38E10036;//MOV @R7 R2   
  FS_IM_R[1] = 0x36363fc0;

  FS_IM_L[2] = 0x50600000;//LDI     R3.1000, R0, 0
  FS_IM_R[2] = 0x00002000;

  FS_IM_L[3] = 0x50800000;//LDI     R4.1000, R0, 0
  FS_IM_R[3] = 0x00002000;
  
  FS_IM_L[4] = 0x80418800;//ADD     R2.1000, R3, R4
  FS_IM_R[4] = 0x00002000;
  
  FS_IM_L[5] = 0x40410000;//LD      R2.0001, R2, 3
  FS_IM_R[5] = 0x00018400;
  
  FS_IM_L[6] = 0x40410000;//LD      R2.0010, R2, 2
  FS_IM_R[6] = 0x00010800;
  
  FS_IM_L[7] = 0x40410000;//LD      R2.0100, R2, 1
  FS_IM_R[7] = 0x00009000;
  
  FS_IM_L[8] = 0x40410000;//LD      R2.1000, R2, 0
  FS_IM_R[8] = 0x00002000;
  
  FS_IM_L[9] = 0x38010036;//MVR     R0.1111, R2.xyzw, <4>
  FS_IM_R[9] = 0x36003f80;//I change the return color register.
  
  FS_IM_L[10] = 0x38438036;//MOV @R2 R7   
  FS_IM_R[10] = 0x36363fc0;

  FS_IM_L[11] = 0x70000000;//FRAG start   
  FS_IM_R[11] = 0x00000040;
  
  FS_IM_L[12] = 0x70000000;//FIN   
  FS_IM_R[12] = 0x00000000;   
  
  FS_IM_L[13] = 0xf8000000;//NOP   
  FS_IM_R[13] = 0x00000000;


  /*
   FS_IM_L[0] = 0x78020036;//TEX @R0 R4  
   FS_IM_R[0] = 0x36363fc0;

   FS_IM_L[1] = 0x38208036;//MOV @R1 R1   
   FS_IM_R[1] = 0x36363fc0;

//FS_IM_L[2] = 0x38410036;//MOV @R2 R2   
//FS_IM_R[2] = 0x36363fc0;

   FS_IM_L[2] = 0x38E10036;//MOV @R7 R2     
   FS_IM_R[2] = 0x36363fc0;

   FS_IM_L[3] = 0x38438036;//MOV @R2 R7   
   FS_IM_R[3] = 0x36363fc0;
  
   FS_IM_L[4] = 0x70000000;//FRAG start   
   FS_IM_R[4] = 0x00000040;

   FS_IM_L[5] = 0x70000000;//FIN   
   FS_IM_R[5] = 0x00000000;   

   FS_IM_L[6] = 0xf8000000;//NOP   
   FS_IM_R[6] = 0x00000000;
  */
}

/*
//BRICK
void FS_IM_init(void)
{
   //MUL R4  R4.xyxy,R58.xyxy -> R4=(s,t,s,t)
   FS_IM_L[0] = 0x88827422; 
   FS_IM_R[0] = 0x22363f80;

   //MOV R57 R4.yyyy          -> R57=(t,t,t,t)
   FS_IM_L[1] = 0x3f2200aa; 
   FS_IM_R[1] = 0x36363f80;

   //MOV R56 R4.xxxx          -> R56=(s,s,s,s)
   FS_IM_L[2] = 0x3f020000; 
   FS_IM_R[2] = 0x36363f80;

   //BLT R56 R60.yyyy #3      -> while(s >= 8)
   FS_IM_L[3] = 0xd71e0001; 
   FS_IM_R[3] = 0x8daa8380;

   //ADD R56 R56,-R60.yyyy          s-=8;//s=s%8;
   FS_IM_L[4] = 0x871c7836; 
   FS_IM_R[4] = 0xaa36bf80;

   //BGE R61 R61 #-2
   FS_IM_L[5] = 0xdfbeffff; 
   FS_IM_R[5] = 0x0d8d8380;

   //BLT R57 R59.yyyy #3      -> while(t >= 4)
   FS_IM_L[6] = 0xd73d8001; 
   FS_IM_R[6] = 0x8daa8380;

   //ADD R57 R57,-R59.yyyy          t-=4;//t=t%4;
   FS_IM_L[7] = 0x873cf636; 
   FS_IM_R[7] = 0xaa36bf80;

   //BGE R61 R61 #-2          
   FS_IM_L[8] = 0xdfbeffff; 
   FS_IM_R[8] = 0x0d8d8380;

   //BGE R56 R61 #3           -> if(s >= 1) jump;
   FS_IM_L[9] = 0xdf1e8001; 
   FS_IM_R[9] = 0x8d8d8380;

   //MOV R0 R63                  else       R0 = RED;//mortar color
   FS_IM_L[10] = 0x381f8036; 
   FS_IM_R[10] = 0x36363f80;               

   //BGE R61 R61 #11
   FS_IM_L[11] = 0xdfbe8005; 
   FS_IM_R[11] = 0x8d8d8380;

   //BGE R60.xxxx R56 #3      -> if(7 >= s) jump;
   FS_IM_L[12] = 0xdf9c0001; 
   FS_IM_R[12] = 0x800d8380;

   //MOV R0 R63                  else       R0 = RED;//mortar color               
   FS_IM_L[13] = 0x381f8036; 
   FS_IM_R[13] = 0x36363f80;

   //BGE R61 R61 #8
   FS_IM_L[14] = 0xdfbe8004; 
   FS_IM_R[14] = 0x0d8d8380;

   //BGE R57 R61 #3           -> if(t >= 1) jump;
   FS_IM_L[15] = 0xdf3e8001; 
   FS_IM_R[15] = 0x8d8d8380;

   //MOV R0 R63                  else       R0 = RED;//mortar color               
   FS_IM_L[16] = 0x381f8036; 
   FS_IM_R[16] = 0x36363f80;

   //BGE R61 R61 #5
   FS_IM_L[17] = 0xdfbe8002; 
   FS_IM_R[17] = 0x8d8d8380;

   //BGE R59.xxxx R57 #3      -> if(3 >= t) jump;
   FS_IM_L[18] = 0xdf7c8001; 
   FS_IM_R[18] = 0x800d8380;

   //MOV R0 R63                  else       R0 = RED;//mortar color               
   FS_IM_L[19] = 0x381f8036; 
   FS_IM_R[19] = 0x36363f80;

   //BGE R61 R61 #2
   FS_IM_L[20] = 0xdfbe8001; 
   FS_IM_R[20] = 0x0d8d8380;

   //MOV R0 R62               -> R0 = BLUE
   FS_IM_L[21] = 0x381f0036; 
   FS_IM_R[21] = 0x36363f80;

   //MOV @R0 R0
   FS_IM_L[22] = 0x38000036; 
   FS_IM_R[22] = 0x36363fc0;

   //MOV @R1 R1
   FS_IM_L[23] = 0x38208036; 
   FS_IM_R[23] = 0x36363fc0;

   //MOV @R2 R2
   FS_IM_L[24] = 0x38410036; 
   FS_IM_R[24] = 0x36363fc0;

   //FRAG start
   FS_IM_L[25] = 0x70000000; 
   FS_IM_R[25] = 0x00000040;

   //FIN
   FS_IM_L[26] = 0x70000000; 
   FS_IM_R[26] = 0x00000000;

   //NOP
   FS_IM_L[27] = 0xf8000000; 
   FS_IM_R[27] = 0x00000000;
}
 */

int GPU(void)
{
    int i,j;
	
	int cycle_count = 0;
	
    /**** interface ****/
	struct IF_ID_if stg1_2;
	struct ID_READ_if stg2_3;
	struct READ_out stg3_out;
	struct READ_EX1_if stg3_4;
    struct EX5_WB_if stg8_9;
	struct WB_out stg9_out;

	
	#if DEBUG
	dump = fopen("dump.txt","w");
	
    if(!dump)
    {
        printf("File open is error!!!\n");
        return 0;
    }	
	#endif
	
	#if DRAW
	f_buf = fopen("frame_buffer.dat","w");
	
    if(!f_buf)	
    {
        printf("File open is error!!!\n");
        return 0;
    }	

	fprintf(f_buf,"320\n");
	fprintf(f_buf,"240\n");	
 	#endif

	
	/**** initialization of interface ****/
	//[IF] | [ID]
    memset(&stg1_2,0,sizeof(struct IF_ID_if));
	stg1_2.IM_data_L = 0xf8000000;//NOP
	stg1_2.IM_data_R = 0x00000000;
	//[ID] | [RD]
	memset(&stg2_3,0,sizeof(struct ID_READ_if));
	stg2_3.opcode = NOP;
	//[RD]'s output
	memset(&stg3_out,0,sizeof(struct READ_out));
	//[RD] | [EX1]
	memset(&stg3_4,0,sizeof(struct READ_EX1_if));
	stg3_4.opcode = NOP;
	//[EX1] | [EX2]
	memset(&stg4_5,0,sizeof(struct EX1_EX2_if));
	stg4_5.opcode_pipe = NOP;
	//[EX2] | [EX3]
	memset(&stg5_6,0,sizeof(struct EX2_EX3_if));
	stg5_6.opcode_pipe = NOP;
	//[EX3] | [EX4]
	memset(&stg6_7,0,sizeof(struct EX3_EX4_if));
	stg6_7.opcode_pipe = NOP;
	//[EX4] | [EX5]
	memset(&stg7_8,0,sizeof(struct EX4_EX5_if));
	stg7_8.opcode_pipe = NOP;
	//[EX5] | [WB]
	memset(&stg8_9,0,sizeof(struct EX5_WB_if));
	stg8_9.opcode = NOP;
	//[EX]'s output
	memset(&EXE_out,0,sizeof(struct EX1234_out));
	//[WB]'s output
	memset(&stg9_out,0,sizeof(struct WB_out));
	
	
	SYS_finish =0;
	
	while(SYS_finish < 12)
	//while(cycle_count < 10000)
	{
	    WB(stg8_9,&stg9_out);
        EXE(stg3_4,&stg8_9);
        READ(stg2_3,stg9_out,&stg3_4,&stg3_out);
        DECODE(stg1_2,stg3_out,&stg2_3,&stg3_4);
        FETCH(stg3_out,&stg1_2);
		
		cycle_count++;

	    
		#if DEBUG
		for(i=0;i<2;++i)
		   for(j=0;j<3;++j)
		      fprintf(dump,"triangle[%d][%d] = %d\n",i,j,thread_ctrl.triangle[i][j]);
	    
		for(i=0;i<14;++i)
		   if(thread_ctrl.info[i].Ttype == FS_THREAD)
		   {
		       fprintf(dump,"FS = %d\n",i);
			   fprintf(dump,"------------------\n");
           }
        #endif
		
		if(cycle_count%10000 == 0)
		   printf("XXXXXXXX\n");
		
		#if DEBUG
		if(SYS_finish == 36)
		{
      		for(i=0;i<14;++i)
			{
		        fprintf(dump,"Ttype[%d] = %d\n",i,thread_ctrl.info[i].Ttype);
				fprintf(dump,"T_state[%d] = %d\n",i,thread_ctrl.info[i].Tstate);
			}

			fprintf(dump,"VS count = %d\n",thread_ctrl.SC_count);
			fprintf(dump,"VS count = %d\n",thread_ctrl.VS_count);
			fprintf(dump,"FS count = %d\n",thread_ctrl.FS_count);
			fprintf(dump,"--------------------------------------------------\n");
		}
		#endif
	}
	printf("cycle_count = %d\n",cycle_count);
	printf("NOP_count = %d\n",NOP_count);
	printf("finish\n");
	
	#if DEBUG
	fclose(dump);
	#endif
	
	#if DRAW
	fclose(f_buf);
	#endif
}

