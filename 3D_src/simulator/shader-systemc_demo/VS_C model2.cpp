//=============================================================================
// Title  : Vertex Shader C model
// Date   : 2009/12/5 ~~ now
// Lab    : F5015
// Author : Li-Yao Chen
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "clip.cpp"



FILE *fp_Instruction, *fp_Uniform, *fp_Uniform_check, *fp_VST, *fp_Vertex ;
FILE *fp_Result,*V_O;

//=============================================================================
//                                                                           ||
//                                                                           ||
//                  decimal to IEEE754 float function                        ||
//                                                                           ||
//                                                                           ||
//=============================================================================

float a;
char word[32], temp[32];
int hex[8];

union VS_vertex_type // for OR instruction 2010/09/29  ya
{
    int int_format;
    float float_format;    
};    

VS_vertex_type VS_or_src1 , VS_or_src2 , VS_or_dst ;


void do_or()
{
    VS_or_dst.int_format = VS_or_src1.int_format | VS_or_src2.int_format ;
}   


char DecTofloat(float a)
{
	int y,i;
    //char word[32];
    y=*((int*)((void*)&a));
    for(i=31;i>=0;i--){
        word[i]=(y&1)+48;
        y>>=1;
    }
	/*
    for(i=0;i<32;i++){
        temp[i] = word[i];
        printf("%c",temp[i]);
        //fprintf(ver_out,"%c",word[i]);
    }
    //fprintf(ver_out,"\n");
    printf("\n");*/
    
    //return temp[];
    
}

void chartoint()
{
    int i;
    for(i=0;i<32;i++){
        temp[i] = word[i]-48;
        //printf("%d",temp[i]);

    }
    //printf("\n");
}

void binarytohex()
{
    int i;
    hex[0] = temp[0]*8 + temp[1]*4 + temp[2]*2 + temp[3];
    hex[1] = temp[4]*8 + temp[5]*4 + temp[6]*2 + temp[7];
    hex[2] = temp[8]*8 + temp[9]*4 + temp[10]*2+ temp[11];
    hex[3] = temp[12]*8+ temp[13]*4+ temp[14]*2+ temp[15]; 
    hex[4] = temp[16]*8+ temp[17]*4+ temp[18]*2+ temp[19];
    hex[5] = temp[20]*8+ temp[21]*4+ temp[22]*2+ temp[23];
    hex[6] = temp[24]*8+ temp[25]*4+ temp[26]*2+ temp[27];
    hex[7] = temp[28]*8+ temp[29]*4+ temp[30]*2+ temp[31];
    
}

void get_float(float a)
{
   int i;
   
   DecTofloat(a);
   chartoint();
   binarytohex();
   for(i=0;i<8;i++)
        fprintf(fp_Result,"%x",hex[i]);
}
//=============================================================================

//=============================================================================
//                                                                           ||
//                                                                           ||
//                    decimal to 32-b fixed function                    ||
//                                                                           ||
//                                                                           ||
//=============================================================================

int hx[8];
int n;
char c[8 * sizeof(n)], s[8 * sizeof(n)];



void itobs(int n)
{
   int size = 8 * sizeof(n);
   int i = size-1;
  
   for(i=size-1; i>=0; i--)
   {
      c[i] = (1 & n) + '0';
      n >>= 1;
   }
  
   //ps[size] = '\0';
   //return ps;
}

void chatoint()
{
    int i;
    for(i=0;i<32;i++){
        s[i] = c[i]-48;
        //printf("%d",temp[i]);

    }
    //printf("\n");
}

void bintohex()
{
    hx[0] = s[0]*8 + s[1]*4 + s[2]*2 + s[3];
    hx[1] = s[4]*8 + s[5]*4 + s[6]*2 + s[7];
    hx[2] = s[8]*8 + s[9]*4 + s[10]*2+ s[11];
    hx[3] = s[12]*8+ s[13]*4+ s[14]*2+ s[15]; 
    hx[4] = s[16]*8+ s[17]*4+ s[18]*2+ s[19];
    hx[5] = s[20]*8+ s[21]*4+ s[22]*2+ s[23];
    hx[6] = s[24]*8+ s[25]*4+ s[26]*2+ s[27];
    hx[7] = s[28]*8+ s[29]*4+ s[30]*2+ s[31];
    
}

void get_fix(int n)
{
   int i; 
    
   itobs(n);
   chatoint();
   bintohex();
   for(i=0;i<8;i++)
       fprintf(fp_Result,"%x",hx[i]);
   //for(i=0;i<32;i++)
   //    printf("%c",c[i]);
   //printf("%d = %s\n", n, itobs(n,s));
}
//=============================================================================

//=============================================================================
//                                                                           ||
//                                                                           ||
//                    hex_int to float function code                    ||
//                                                                           ||
//                                                                           ||
//=============================================================================
//FILE *output;

float int_to_float(int a)
{
    int i;
    int mod[32];
    int sign;
    int exp=0;
    float fraction=1;
    float result;
    
    //output = fopen("output.txt", "w");
    
    for(i=0;i<32;i++)
    {
       mod[31-i] = a%2;
       a=a/2;
       //fprintf(output, "a=%d\n   ",a);
       //fprintf(output, "mod[%d]=%d\n",31-i,mod[31-i]);
    }
    /*
    for(i=0;i<32;i++)
    {
       printf("%d", mod[i]);
    }
    printf("\n");
    */
    if(mod[0]==0){
       sign = 1;
       printf("sign = %d\n", 0);
       }
    else{
       sign = -1;
       printf("sign bit= %d\n", 1);
       }
       
    
    for(i=7;i>-1;i--)
       exp = exp + mod[8-i]*int(pow(2,i));
    exp = exp - 127;
    printf("exp = %d\n", exp);
    
    for(i=-1;i>-24;i--)
       fraction = fraction + mod[8-i]*pow(2,i);
    printf("fraction = %f\n", fraction);
    
    
    result = sign*(pow(2,exp))*fraction;
    printf("result = %f\n", result);
    
    return result;
}
//=============================================================================

//=============================================================================
//                                                                           ||
//                                                                           ||
//                            VS C model code                                ||
//                                                                           ||
//                                                                           ||
//=============================================================================
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))



 
int i, j, k;
int PC;
int branch[4];
int total_v = 8; //vertex number
int total_a = 1;  //attribute number

int v_num;       //vertex number 
int v_entry_num; //entry number
int v_word_num;  //offset number


//-------------------memories and register-------------------------------------
float Ex_MEM[36][4][4];//[v_num][entry 0~3][component xyzw]
float VAB[8][4], VOB[8][4];
float GPR[32][4];
char  IM[256][64];
float CMEM[50][4];
int   VST[14];

int Imm; // variable for LD/ST/MVI displacement  
int Imm_float_sign; // variable for MVI displacement

//------------------temp-------------------------------------------------------
float Rs_swiz_temp[4]={0}, Rt_swiz_temp[4]={0}, Rd_swiz_temp[4]={0};
float dsq_temp=0, div_temp=0;
int   LD_addr;
int   ST_addr;

float mvi_temp_FL[4]={0};
int   mvi_temp_FX[4]={0};

float min_temp_FL[4]={0};
int   min_temp_FX[4]={0};

float max_temp_FL[4]={0};
int   max_temp_FX[4]={0};

float add_temp_FL[4]={0};
int   add_temp_FX[4]={0};

float mulh_temp[4]={0};
int   mull_temp[4]={0};
long int mull_tmp64b[4]={0};

float mad_temp_FL[4]={0};
int   mad_temp_FX[4]={0};

float dp_temp_FL=0;
int   dp_temp_FX=0;

int   inv_temp[4]={0};
int   and_temp[4]={0};
float    or_temp[4]={0};
int   xor_temp[4]={0};

float R1_temp_FL[4]={0}, R2_temp_FL[4]={0}, R3_temp_FL[4]={0}, R4_temp_FL[4]={0};
int   R1_temp_FX[4]={0}, R2_temp_FX[4]={0}, R3_temp_FX[4]={0}, R4_temp_FX[4]={0};

int  shrl_temp[4]={0};
//-----------------------------------------------------------------------
struct decoder{
		int     op;
		int     NOP_stall_cycle;
		int     Rd;
		int     Rs;
		int     Rt;
		int     R1;
		int     R2;
		int     Addr;
		int     R3;
		int     R4;
		int     Sw_d;
		int     Sw_d_x;
		int     Sw_d_y;
		int     Sw_d_z;
		int     Sw_d_w;
		int     Sw_s_x;
		int     Sw_s_y;
		int     Sw_s_z;
		int     Sw_s_w;
		int     Sw_t_x;
		int     Sw_t_y;
		int     Sw_t_z;
		int     Sw_t_w;
		int     Neg_Rs;
		int     Neg_Rt;
		int     Neg_Rd;
		int     R_L;
		int     WM_x;
		int     WM_y;
		int     WM_z;
		int     WM_w;
        int     SCC_x;//single component compare 
        int     SCC_y;//single component compare
        int     SCC_z;//single component compare
        int     SCC_w;//single component compare
		int     FXL;
		int     S_V;
		
	}IF;
		


void decode(int program_counter)
{

//---------------------char-to-int for each block for decode---------------	
    //--------Op-code------------  
    IF.op = 0;
    for(i=0;i<=4;i++){
       
       IF.op = IF.op + IM[program_counter][i]-48; // sub48 for char to int
       IF.op <<= 1 ;
      
      }                  
     IF.op >>= 1 ;   
     
                 
    if(IF.op==31)//---------------NOP
    {
       IF.NOP_stall_cycle = 0;
       for(i=5;i<=8;i++){
       
          IF.NOP_stall_cycle = IF.NOP_stall_cycle + IM[program_counter][i]-48;
          IF.NOP_stall_cycle <<= 1 ;
       
       } 
                   
       IF.NOP_stall_cycle >>= 1 ;
    }
    else if((IF.op>=24 && IF.op<=27) || IF.op==29)//-----for branch instructions
    {
       //--------R1------------
       IF.R1 = 0;
       for(i=5;i<=10;i++){
       
          IF.R1 = IF.R1 + IM[program_counter][i]-48;
          IF.R1 <<= 1 ;
       
       } 
                   
       IF.R1 >>= 1 ;

       //--------R2------------     
       IF.R2 = 0;
       for(i=11;i<=16;i++){
       
          IF.R2 = IF.R2 + IM[program_counter][i]-48;
          IF.R2 <<= 1 ;
      
       } 
                   
       IF.R2 >>= 1 ;

       //--------Addr------------
       IF.Addr = 0;
       for(i=17;i<=32;i++){
       
          IF.Addr = IF.Addr + IM[program_counter][i]-48;
          IF.Addr <<= 1 ;
       
       } 
                   
       IF.Addr >>= 1 ;

       //--------R3------------     
       IF.R3 = 0;
       for(i=33;i<=38;i++){
       
          IF.R3 = IF.R3 + IM[program_counter][i]-48;
          IF.R3 <<= 1 ;
      
       } 
                   
       IF.R3 >>= 1 ;
     
       //--------R4------------     
       IF.R4 = 0;
       for(i=39;i<=43;i++){
       
         IF.R4 = IF.R4 + IM[program_counter][i]-48;
         IF.R4 <<= 1 ;
      
       } 
                   
       IF.R4 >>= 1 ; 
       
       if(IF.op!=29){
          //--------SCC------------            
          IF.SCC_x = IM[program_counter][50]-48;
          IF.SCC_y = IM[program_counter][51]-48;
          IF.SCC_z = IM[program_counter][52]-48;
          IF.SCC_w = IM[program_counter][53]-48;
       }    
    }
    else//-----------------------for other instructions
    {
       //--------Rd------------     
       IF.Rd = 0;
       for(i=5;i<=10;i++){
       
          IF.Rd = IF.Rd + IM[program_counter][i]-48;
          IF.Rd <<= 1 ;
      
       } 
                   
       IF.Rd >>= 1 ; 
     
       //--------Rs------------     
       IF.Rs = 0;
       for(i=11;i<=16;i++){
       
          IF.Rs = IF.Rs + IM[program_counter][i]-48;
          IF.Rs <<= 1 ;
      
       } 
                   
       IF.Rs >>= 1 ; 
    
       //--------Imm------------ 
               //-------FXL-----------
               IF.FXL = IM[program_counter][54]-48; 
       Imm = 0;
       Imm_float_sign = IM[program_counter][17]-48;
       //fprintf(fp_Result,"Imm_float_sign = %d\n", Imm_float_sign);
       if(IF.FXL==1)
       {
          for(i=17;i<=48;i++){
             
             if(i==17){
                Imm = Imm + 0;
                Imm <<= 1 ;
             }
             else if(i==48){
                Imm = Imm + IM[program_counter][i]-48;
             }        
             else{   
                Imm = Imm + IM[program_counter][i]-48;
                Imm <<= 1 ;

             }    
      
          } 
       }
       else
       {
          for(i=17;i<=48;i++){
          
             Imm = Imm + IM[program_counter][i]-48;
             Imm <<= 1 ;
          }
          
          Imm >>= 1 ;     
       } 
                       
       
       //--------Rt------------    
       IF.Rt = 0;
       for(i=17;i<=22;i++){
       
          IF.Rt = IF.Rt + IM[program_counter][i]-48;
          IF.Rt <<= 1 ;
      
       } 
                   
       IF.Rt >>= 1 ; 
       
       //--------Sw_d------------       
       IF.Sw_d_x = 0;
       for(i=23;i<=24;i++){
       
          IF.Sw_d_x = IF.Sw_d_x + IM[program_counter][i]-48;
          IF.Sw_d_x <<= 1 ;
       
       } 
                   
       IF.Sw_d_x >>= 1 ;
     
       IF.Sw_d_y = 0;
       for(i=25;i<=26;i++){
       
          IF.Sw_d_y = IF.Sw_d_y + IM[program_counter][i]-48;
          IF.Sw_d_y <<= 1 ;
       
       } 
                   
       IF.Sw_d_y >>= 1 ;
       
       IF.Sw_d_z = 0;
       for(i=27;i<=28;i++){
       
          IF.Sw_d_z = IF.Sw_d_z + IM[program_counter][i]-48;
          IF.Sw_d_z <<= 1 ;
       
       } 
                   
       IF.Sw_d_z >>= 1 ;
       
       IF.Sw_d_w = 0;
       for(i=29;i<=30;i++){
       
          IF.Sw_d_w = IF.Sw_d_w + IM[program_counter][i]-48;
          IF.Sw_d_w <<= 1 ;
      
       } 
                   
       IF.Sw_d_w >>= 1 ; 
       //--------Sw_s------------    
       IF.Sw_s_x = 0;
       for(i=31;i<=32;i++){
       
          IF.Sw_s_x = IF.Sw_s_x + IM[program_counter][i]-48;
          IF.Sw_s_x <<= 1 ;
      
       }             
       IF.Sw_s_x >>= 1 ; 
     
       IF.Sw_s_y = 0;
       for(i=33;i<=34;i++){
       
          IF.Sw_s_y = IF.Sw_s_y + IM[program_counter][i]-48;
          IF.Sw_s_y <<= 1 ;
      
       }             
       IF.Sw_s_y >>= 1 ; 
    
       IF.Sw_s_z = 0;
       for(i=35;i<=36;i++){
       
          IF.Sw_s_z = IF.Sw_s_z + IM[program_counter][i]-48;
          IF.Sw_s_z <<= 1 ;
      
       }             
       IF.Sw_s_z >>= 1 ; 
     
       IF.Sw_s_w = 0;
       for(i=37;i<=38;i++){
       
          IF.Sw_s_w = IF.Sw_s_w + IM[program_counter][i]-48;
          IF.Sw_s_w <<= 1 ;
       
       }             
       IF.Sw_s_w >>= 1 ;  
     
       //--------Sw_t------------         
       IF.Sw_t_x = 0;
       for(i=39;i<=40;i++){
       
          IF.Sw_t_x = IF.Sw_t_x + IM[program_counter][i]-48;
          IF.Sw_t_x <<= 1 ;
      
       }             
       IF.Sw_t_x >>= 1 ; 
     
       IF.Sw_t_y = 0;
       for(i=41;i<=42;i++){
       
          IF.Sw_t_y = IF.Sw_t_y + IM[program_counter][i]-48;
          IF.Sw_t_y <<= 1 ;
      
       }             
       IF.Sw_t_y >>= 1 ; 
    
       IF.Sw_t_z = 0;
       for(i=43;i<=44;i++){
       
          IF.Sw_t_z = IF.Sw_t_z + IM[program_counter][i]-48;
          IF.Sw_t_z <<= 1 ;
      
       }             
       IF.Sw_t_z >>= 1 ; 
     
       IF.Sw_t_w = 0;
       for(i=45;i<=46;i++){
       
          IF.Sw_t_w = IF.Sw_t_w + IM[program_counter][i]-48;
          IF.Sw_t_w <<= 1 ;
       
       }             
       IF.Sw_t_w >>= 1 ;   
     
       //--------Neg------------    
       IF.Neg_Rs = IM[program_counter][47]-48; 
       IF.Neg_Rt = IM[program_counter][48]-48;
       if(IF.op!=30) 
          IF.Neg_Rd = IM[program_counter][49]-48; //just for MAD = =|||
       else
          IF.R_L = IM[program_counter][49]-48; //for SHRL
    
       //--------WM------------    
       IF.WM_x = IM[program_counter][50]-48;
       IF.WM_y = IM[program_counter][51]-48;
       IF.WM_z = IM[program_counter][52]-48;
       IF.WM_w = IM[program_counter][53]-48;
       //printf("IF.WM:%d%d%d%d\n",IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
    }  
    //-------FXL-----------
    IF.FXL = IM[program_counter][54]-48;
    //-------S/V-----------
    IF.S_V = 0;
    for(i=55;i<=56;i++){
       
       IF.S_V = IF.S_V + IM[program_counter][i]-48;
       IF.S_V <<= 1 ;
      
    }             
    IF.S_V >>= 1 ;    
//------------------------------------------------------------------------------      
        
      
//-------------printf for debug------------------------------------------------- 
    
    fprintf(fp_Result,"PC = %d\n", PC);
    
    switch(IF.op)
    {
       case 1 : fprintf(fp_Result,"Op:%d = DSQ\n", IF.op)            ; break;
       case 2 : fprintf(fp_Result,"Op:%d = RCP\n", IF.op)            ; break;
       case 3 : fprintf(fp_Result,"Op:%d = RSQ\n", IF.op)            ; break;
       case 4 : fprintf(fp_Result,"Op:%d = LOG2\n", IF.op)           ; break;
       case 5 : fprintf(fp_Result,"Op:%d = POW2\n", IF.op)           ; break;
       case 6 : fprintf(fp_Result,"Op:%d = DIV\n", IF.op)            ; break;
       case 7 : fprintf(fp_Result,"Op:%d = MVR\n", IF.op)            ; break;
       case 8 : fprintf(fp_Result,"Op:%d = LD\n", IF.op)             ; break;
       case 9 : fprintf(fp_Result,"Op:%d = ST\n", IF.op)             ; break;
       case 10: fprintf(fp_Result,"Op:%d = MVI\n", IF.op)            ; break;
       case 11: fprintf(fp_Result,"Op:%d = MIN\n", IF.op)            ; break;
       case 12: fprintf(fp_Result,"Op:%d = MAX\n", IF.op)            ; break;
       case 13: fprintf(fp_Result,"Op:%d = LDV\n", IF.op)            ; break;
       case 14: fprintf(fp_Result,"Op:%d = STV\n", IF.op)            ; break;
       case 16: fprintf(fp_Result,"Op:%d = ADD\n", IF.op)            ; break;
       case 17: fprintf(fp_Result,"Op:%d = MULH\n", IF.op)           ; break;
       case 18: fprintf(fp_Result,"Op:%d = MAD\n", IF.op)            ; break;
       case 19: fprintf(fp_Result,"Op:%d = DP%d\n", IF.op, IF.S_V+1) ; break;
       case 20: fprintf(fp_Result,"Op:%d = INV\n", IF.op)            ; break;
       case 21: fprintf(fp_Result,"Op:%d = AND\n", IF.op)            ; break;
       case 22: fprintf(fp_Result,"Op:%d = OR\n", IF.op)             ; break;
       case 23: fprintf(fp_Result,"Op:%d = XOR\n", IF.op)            ; break;
       case 24: fprintf(fp_Result,"Op:%d = BEQ\n", IF.op)            ; break;
       case 25: fprintf(fp_Result,"Op:%d = BNEQ\n", IF.op)           ; break;
       case 26: fprintf(fp_Result,"Op:%d = BLT\n", IF.op)            ; break;
       case 27: fprintf(fp_Result,"Op:%d = BGE\n", IF.op)            ; break;
       case 28: fprintf(fp_Result,"Op:%d = MULL\n", IF.op)           ; break;
       case 29: fprintf(fp_Result,"Op:%d = CAL\n", IF.op)            ; break;
       case 30: fprintf(fp_Result,"Op:%d = SHRL\n", IF.op)           ; break;
       case 31: fprintf(fp_Result,"Op:%d = NOP\n", IF.op)            ; break;
    }
if(IF.op==31)
{
   fprintf(fp_Result,"stall:[%d] cycles\n", IF.NOP_stall_cycle);
}
else
{   
    if((IF.op<24 || IF.op>27) && IF.op!=29)
    {
       if(IF.Rd>=32)
          fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);    
       else
          fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
    } 
    if((IF.op<24 || IF.op>27) && IF.op!=29)
    {
       if(IF.Rs>=32)
          fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
       else
          fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
    }      
          
    
    if(IF.op==8 || IF.op==9 || IF.op==10 || IF.op==13 || IF.op==14)
    {
       if(IF.FXL==1)
       {
          if(Imm_float_sign==1)
             fprintf(fp_Result,"Imm:%f\n",0-int_to_float(Imm));
          else
             fprintf(fp_Result,"Imm:%f\n",int_to_float(Imm));
       }    
       else
          fprintf(fp_Result,"Imm:%d\n",Imm);
    }    
    if(IF.op!=8 && IF.op!=9 && IF.op!=10 && IF.op!=13 && IF.op!=14)
    {
       if((IF.op>=24 && IF.op<=27) || IF.op==29)
       {
          if(IF.op==29)
          {
             fprintf(fp_Result,"R1:GPR[%d]\n", IF.R1);
             if(IF.R2>=32)
                fprintf(fp_Result,"R2:VAB_entry[%d]\n", IF.R2-32);
             else
                fprintf(fp_Result,"R2:GPR[%d]\n", IF.R2);
             fprintf(fp_Result,"Addr Offset:%d\n", IF.Addr);
          }
          else
          {
             if(IF.R1>=32)
                fprintf(fp_Result,"R1:VAB_entry[%d]\n", IF.R1-32);
             else
                fprintf(fp_Result,"R1:GPR[%d]\n", IF.R1);
             
             if(IF.R2>=32)
                fprintf(fp_Result,"R2:VAB_entry[%d]\n", IF.R2-32);
             else
                fprintf(fp_Result,"R2:GPR[%d]\n", IF.R2);
             
             fprintf(fp_Result,"Addr Offset:%d\n", IF.Addr);   
          
             if(IF.R3>=32)
                fprintf(fp_Result,"R3:VAB_entry[%d]\n", IF.R3-32);
             else
                fprintf(fp_Result,"R3:GPR[%d]\n", IF.R3);
             
             fprintf(fp_Result,"R4:GPR[%d]\n", IF.R4);
             fprintf(fp_Result,"SCC:%d%d%d%d\n", IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
          }    
       }    
       else  
       {    
          if(IF.Rt>=32)
          {
             if(IF.op!=7)
             {
                if(IF.op==30)
                   fprintf(fp_Result,"Rt.x:VAB_entry[%d].x\n", IF.Rt-32);
                else
                   fprintf(fp_Result,"Rt:VAB_entry[%d]\n", IF.Rt-32);
             }
          }    
          else
          {
             if(IF.op<=7 || IF.op==20)
                fprintf(fp_Result,"Rt:Dont care\n");
             else
             {
                if(IF.op==30)
                   fprintf(fp_Result,"Rt.x:GPR[%d].x\n", IF.Rt);
                else
                   fprintf(fp_Result,"Rt:GPR[%d]\n", IF.Rt);
             }
          }
       
          if(IF.op==18)//MAD needs Sw_d
             fprintf(fp_Result,"Sw_d:%d%d%d%d\n", IF.Sw_d_x, IF.Sw_d_y, IF.Sw_d_z, IF.Sw_d_w); 
          if(IF.op>=6)
          {
             if(IF.op==7 || IF.op==20)
             {
                fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w);
                fprintf(fp_Result,"Neg:%d_Dont care_Dont care\n", IF.Neg_Rs);
             }
             else if(IF.op==30)
             {
                fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w);
                fprintf(fp_Result,"R/L:%d\n",IF.R_L);
                
             }
             else
             {
                fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w);   
                fprintf(fp_Result,"Sw_t:%d%d%d%d\n", IF.Sw_t_x, IF.Sw_t_y, IF.Sw_t_z, IF.Sw_t_w);
                fprintf(fp_Result,"Neg:%d_%d_%d\n",IF.Neg_Rs,IF.Neg_Rt,0);
             }    
          }
          else
          { 
             fprintf(fp_Result,"Sw_s:Dont care\n");
             fprintf(fp_Result,"Sw_t:Dont care\n");
             fprintf(fp_Result,"Neg:Dont care\n");
          }
       }    
    }	
	if((IF.op<24 || IF.op>27) && IF.op!=29)
	   fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
}
//------------------------------------------------------------------------------	

	
    switch(IF.op){
       
       // DSQ     
       case 1:
            //------------determine the source----------------------------------
            Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;
            Rd_swiz_temp[0] = (IF.Rd>=32) ? VAB[IF.Rd-32][0] : GPR[IF.Rd][0] ;
            
            //------------do DSQ function---------------------------------------
            dsq_temp = Rd_swiz_temp[0]/sqrt(Rs_swiz_temp[0]) ;
            //------------ WB --------------------------------------------------
            if(IF.S_V!=0 || IF.FXL!=1)
               fprintf(fp_Result,"S/V must be 00 and FXL must be 1.");
            else
            {      
               if(IF.WM_x==1)
                  GPR[IF.Rd][0] = dsq_temp;
               
               if(IF.WM_y==1)
                  GPR[IF.Rd][1] = dsq_temp;
               
               if(IF.WM_z==1)
                  GPR[IF.Rd][2] = dsq_temp;
               
               if(IF.WM_w==1)
                  GPR[IF.Rd][3] = dsq_temp;
            }
            //---------------------------------printf for debug--------------------------------------------------------------
            //-------- Source
            fprintf(fp_Result,"Rs_GPR[%d]:%.20f_%f_%f_%f\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
            fprintf(fp_Result,"Rd_GPR[%d]:%f_%f_%f_%f\n", IF.Rd, Rd_swiz_temp[0], Rd_swiz_temp[1], Rd_swiz_temp[2], Rd_swiz_temp[3]);                  
            //-------- out  
            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            fprintf(fp_Result,"GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result,"GPR[%d] = ", IF.Rd);
            get_float(GPR[IF.Rd][0]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][1]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][2]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][3]);
            fprintf(fp_Result,"\n");    
            break;
       // RCP     
       case 2:
            //
            //------------determine the source----------------------------------
            Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;
            //------------do RCP function and WB---------------------------------------
            if(IF.S_V!=0 || IF.FXL!=1)
               fprintf(fp_Result,"S/V must be 00 and FXL must be 1.");
            else
            {        
               if(IF.WM_x==1)
                  GPR[IF.Rd][0] = 1/Rs_swiz_temp[0];
               
               if(IF.WM_y==1)
                  GPR[IF.Rd][1] = 1/Rs_swiz_temp[0];
               
               if(IF.WM_z==1)
                  GPR[IF.Rd][2] = 1/Rs_swiz_temp[0];
               
               if(IF.WM_w==1)
                  GPR[IF.Rd][3] = 1/Rs_swiz_temp[0];
            }       
            //---------------------------------printf for debug--------------------------------------------------------------
            //-------- Source
            fprintf(fp_Result,"Rs_GPR[%d]:%f_%f_%f_%f\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
               
            //-------- out 
            fprintf(fp_Result,"-----------------------------------------------------------------\n"); 
            fprintf(fp_Result,"GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result,"GPR[%d] = ", IF.Rd);
            get_float(GPR[IF.Rd][0]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][1]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][2]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][3]);
            fprintf(fp_Result,"\n");
            break;
       // RSQ  
       case 3:
            //
            //------------determine the source----------------------------------
            Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;
            //------------do RSQ function and WB---------------------------------------
            if(IF.S_V!=0 || IF.FXL!=1)
              fprintf(fp_Result,"S/V must be 00 and FXL must be 1.");
            else
            {
               if(IF.WM_x==1)
                  GPR[IF.Rd][0] = 1/sqrt(Rs_swiz_temp[0]);
              
               if(IF.WM_y==1)
                  GPR[IF.Rd][1] = 1/sqrt(Rs_swiz_temp[0]);
               
               if(IF.WM_z==1)
                  GPR[IF.Rd][2] = 1/sqrt(Rs_swiz_temp[0]);
               
               if(IF.WM_w==1)
                  GPR[IF.Rd][3] = 1/sqrt(Rs_swiz_temp[0]);
            }       
            //---------------------------------printf for debug--------------------------------------------------------------
            //-------- Source
            fprintf(fp_Result, "Rs_GPR[%d]:%f_%f_%f_%f\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
               
            //-------- out 
            fprintf(fp_Result, "-----------------------------------------------------------------\n"); 
            fprintf(fp_Result, "GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result, "GPR[%d] = ", IF.Rd);
            get_float(GPR[IF.Rd][0]);
            fprintf(fp_Result, "_");
            get_float(GPR[IF.Rd][1]);
            fprintf(fp_Result, "_");
            get_float(GPR[IF.Rd][2]);
            fprintf(fp_Result, "_");
            get_float(GPR[IF.Rd][3]);
            fprintf(fp_Result, "\n");
            break;
       // LOG2  
       case 4:
            //
            //------------determine the source----------------------------------
            Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;
            //------------do LOG2 function and WB---------------------------------------
            if(IF.S_V!=0 || IF.FXL!=1)
              fprintf(fp_Result, "S/V must be 00 and FXL must be 1.");
            else
            {
               if(IF.WM_x==1)
                  GPR[IF.Rd][0] = log(Rs_swiz_temp[0])/log(2);
              
               if(IF.WM_y==1)
                  GPR[IF.Rd][1] = log(Rs_swiz_temp[0])/log(2);
               
               if(IF.WM_z==1)
                  GPR[IF.Rd][2] = log(Rs_swiz_temp[0])/log(2);
               
               if(IF.WM_w==1)
                  GPR[IF.Rd][3] = log(Rs_swiz_temp[0])/log(2);
            }    
            //---------------------------------printf for debug--------------------------------------------------------------
            //-------- Source
            fprintf(fp_Result, "Rs_GPR[%d]:%f_%f_%f_%f\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
               
            //-------- out 
            fprintf(fp_Result, "-----------------------------------------------------------------\n"); 
            fprintf(fp_Result, "GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result, "GPR[%d] = ", IF.Rd);
            get_float(GPR[IF.Rd][0]);
            fprintf(fp_Result, "_");
            get_float(GPR[IF.Rd][1]);
            fprintf(fp_Result, "_");
            get_float(GPR[IF.Rd][2]);
            fprintf(fp_Result, "_");
            get_float(GPR[IF.Rd][3]);
            fprintf(fp_Result, "\n");
            break;
       // POW2  
       case 5:
            //
            //------------determine the source----------------------------------
            Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;
            //------------do POW2 function and WB---------------------------------------
            if(IF.S_V!=0 || IF.FXL!=1)
              fprintf(fp_Result, "S/V must be 00 and FXL must be 1.");
            else
            {
               if(IF.WM_x==1)
                  GPR[IF.Rd][0] = pow(2, GPR[IF.Rs][0]);
              
               if(IF.WM_y==1)
                  GPR[IF.Rd][1] = pow(2, GPR[IF.Rs][0]);
               
               if(IF.WM_z==1)
                  GPR[IF.Rd][2] = pow(2, GPR[IF.Rs][0]);
               
               if(IF.WM_w==1)
                  GPR[IF.Rd][3] = pow(2, GPR[IF.Rs][0]);
            }    
            //---------------------------------printf for debug--------------------------------------------------------------
            //-------- Source
            fprintf(fp_Result, "Rs_GPR[%d]:%f_%f_%f_%f\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
               
            //-------- out 
            fprintf(fp_Result, "-----------------------------------------------------------------\n"); 
            fprintf(fp_Result, "GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result, "GPR[%d] = ", IF.Rd);
            get_float(GPR[IF.Rd][0]);
            fprintf(fp_Result, "_");
            get_float(GPR[IF.Rd][1]);
            fprintf(fp_Result, "_");
            get_float(GPR[IF.Rd][2]);
            fprintf(fp_Result, "_");
            get_float(GPR[IF.Rd][3]);
            fprintf(fp_Result, "\n");
            break;
       // DIV  
       case 6:
            //
            //------------determine the source----------------------------------
            Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;
            Rd_swiz_temp[0] = (IF.Rd>=32) ? VAB[IF.Rd-32][0] : GPR[IF.Rd][0] ;
            
            //------------do DIV function---------------------------------------
            div_temp = Rd_swiz_temp[0]/Rs_swiz_temp[0] ;
            //------------ WB --------------------------------------------------
            if(IF.S_V!=0 || IF.FXL!=1)
              fprintf(fp_Result, "S/V must be 00 and FXL must be 1.");
            else
            {
               if(IF.WM_x==1)
                  GPR[IF.Rd][0] = div_temp;
              
               if(IF.WM_y==1)
                  GPR[IF.Rd][1] = div_temp;
               
               if(IF.WM_z==1)
                  GPR[IF.Rd][2] = div_temp;
               
               if(IF.WM_w==1)
                  GPR[IF.Rd][3] = div_temp;
            }    
            //---------------------------------printf for debug--------------------------------------------------------------
            //-------- Source
            fprintf(fp_Result, "Rs_GPR[%d]:%f_%f_%f_%f\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
            fprintf(fp_Result, "Rd_GPR[%d]:%f_%f_%f_%f\n", IF.Rd, Rd_swiz_temp[0], Rd_swiz_temp[1], Rd_swiz_temp[2], Rd_swiz_temp[3]);                  
            //-------- out  
            fprintf(fp_Result, "-----------------------------------------------------------------\n");
            fprintf(fp_Result, "GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result, "GPR[%d] = ", IF.Rd);
            get_float(GPR[IF.Rd][0]);
            fprintf(fp_Result, "_");
            get_float(GPR[IF.Rd][1]);
            fprintf(fp_Result, "_");
            get_float(GPR[IF.Rd][2]);
            fprintf(fp_Result, "_");
            get_float(GPR[IF.Rd][3]);
            fprintf(fp_Result, "\n");
            break;
       // MVR     
       case 7:
            //
            switch(IF.Sw_d_x){
                    
                case 0:
                     Rd_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     Rd_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rd_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rd_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break;
                }    
            switch(IF.Sw_d_y){
                    
                case 0:
                     Rd_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;
                     break;         
                case 1:
                     Rd_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rd_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rd_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break; 
                } 
            switch(IF.Sw_d_z){
                    
                case 0:
                     Rd_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;  
                     break;         
                case 1:
                     Rd_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rd_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rd_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break; 
                }   
            switch(IF.Sw_d_w){
                    
                case 0:
                     Rd_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ; 
                     break;         
                case 1:
                     Rd_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ; 
                     break; 
                case 2:
                     Rd_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ; 
                     break; 
                case 3:
                     Rd_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ; 
                     break; 
                }
            //----------determine Rs Neg--------------------------
            if(IF.Neg_Rs==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  Rd_swiz_temp[i] = -Rd_swiz_temp[i];
            }
            //---------------------WB-------------------------
            if(IF.WM_x==1)
            {
               if(IF.Rd>=32)
                  VOB[IF.Rd-32][0]= Rd_swiz_temp[0];
               else
                  GPR[IF.Rd][0]= Rd_swiz_temp[0];
            }       
            if(IF.WM_y==1)
            {
               if(IF.Rd>=32)
                  VOB[IF.Rd-32][1]= Rd_swiz_temp[1];
               else
                  GPR[IF.Rd][1]= Rd_swiz_temp[1];
            }  
            if(IF.WM_z==1)
            {
               if(IF.Rd>=32)
                  VOB[IF.Rd-32][2]= Rd_swiz_temp[2];
               else
                  GPR[IF.Rd][2]= Rd_swiz_temp[2];
            }   
            if(IF.WM_w==1)
            {
               if(IF.Rd>=32)
                  VOB[IF.Rd-32][3]= Rd_swiz_temp[3];
               else
                  GPR[IF.Rd][3]= Rd_swiz_temp[3];
            }
               
            //---------------------------------printf for debug--------------------------------------------------------------
            //-------- Source
            if(IF.Rs>=32)
               fprintf(fp_Result, "Rs_VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
            else
               fprintf(fp_Result, "Rs_GPR[%d]:%f_%f_%f_%f\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
            //-------- out 
            printf("-----------------------------------------------------------------\n"); 
            if(IF.Rd>=32) 
               fprintf(fp_Result, "Rd_VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
            else
               fprintf(fp_Result, "Rd_GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);   
            //--------get hex floating point result------------------------------------------------------------
            if(IF.Rd>=32)
            {
               fprintf(fp_Result, "Rd_VOB_entry[%d] = ", IF.Rd-32);
               get_float(VOB[IF.Rd-32][0]);
               fprintf(fp_Result, "_");
               get_float(VOB[IF.Rd-32][1]);
               fprintf(fp_Result, "_");
               get_float(VOB[IF.Rd-32][2]);
               fprintf(fp_Result, "_");
               get_float(VOB[IF.Rd-32][3]);
               fprintf(fp_Result, "\n");
            }    
            else
            {
               fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
               get_float(GPR[IF.Rd][0]);
               fprintf(fp_Result,"_");
               get_float(GPR[IF.Rd][1]);
               fprintf(fp_Result,"_");
               get_float(GPR[IF.Rd][2]);
               fprintf(fp_Result,"_");
               get_float(GPR[IF.Rd][3]);
               fprintf(fp_Result,"\n"); 
            }
            break;
       // LD           
       case 8:	
            //
            
            LD_addr = int(GPR[IF.Rs][0]) + Imm;
            if(IF.WM_x==1)
                  GPR[IF.Rd][0] = CMEM[LD_addr][IF.S_V] ;
                  
            if(IF.WM_y==1)
                  GPR[IF.Rd][1] = CMEM[LD_addr][IF.S_V] ;
                  
            if(IF.WM_z==1)
                  GPR[IF.Rd][2] = CMEM[LD_addr][IF.S_V] ;
                  
            if(IF.WM_w==1)
                  GPR[IF.Rd][3] = CMEM[LD_addr][IF.S_V] ;
            
            

            fprintf(fp_Result,"CMEM[%d][%d] = %f = 0x", LD_addr, IF.S_V, CMEM[LD_addr][IF.S_V] );
            get_float(CMEM[LD_addr][IF.S_V] );
            fprintf(fp_Result,"\n");
            fprintf(fp_Result,"GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result,"GPR[%d] = ", IF.Rd);
            get_float(GPR[IF.Rd][0]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][1]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][2]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][3]);
            fprintf(fp_Result,"\n");             
            break;
            
       //ST
       case 9:
            //
            ST_addr = int(GPR[IF.Rd][0]) + Imm;
            CMEM[ST_addr][IF.S_V] = GPR[IF.Rs][0] ; //S/V =00 => write x, S/V=01 => write y, S/V=10 => write z, S/V=11 => write w
       
            fprintf(fp_Result,"GPR[%d][%d] = %f\n", IF.Rs, 0, GPR[IF.Rs][0]);  
            fprintf(fp_Result,"CMEM[%d][%d] = %f\n",ST_addr, IF.S_V, CMEM[ST_addr][IF.S_V]);            
            break;
       // MVI
       case 10:
            //------------determine the source----------------------------------
            for(i=0; i<=IF.S_V; i++)
               Rs_swiz_temp[i] = (IF.Rs>=32) ? VAB[IF.Rs-32][i] : GPR[IF.Rs][i] ;
            //------------add Imm-----------------------------------------------
            if(IF.FXL==1)//flp format
            {
               if(Imm_float_sign==1){
                  for(i=0; i<=IF.S_V; i++)//S/V to select how many Src component needed
                     mvi_temp_FL[i] = Rs_swiz_temp[i] + (0-int_to_float(Imm));
                  }
               else{
                  for(i=0; i<=IF.S_V; i++)//S/V to select how many Src component needed
                     mvi_temp_FL[i] = Rs_swiz_temp[i] + int_to_float(Imm);
                  }                 
            }    
            else//fxd format
            {
               for(i=0; i<=IF.S_V; i++)//S/V to select how many Src component needed
                  mvi_temp_FX[i] = (int)(Rs_swiz_temp[i]) + Imm;
            }            
            //----------- WB ---------------------------------------------------
            if(IF.WM_x==1)
            {
               if(IF.Rd>=32)
               {
                  if(IF.FXL==1)
                     VOB[IF.Rd-32][0] = mvi_temp_FL[0];
                  else
                     (VOB[IF.Rd-32][0]) = mvi_temp_FX[0];
               }
               else
               {
                  if(IF.FXL==1)
                     GPR[IF.Rd][0] = mvi_temp_FL[0];
                  else
                     (GPR[IF.Rd][0]) = mvi_temp_FX[0];
               }    
            }
                  
            if(IF.WM_y==1)
            {
               if(IF.Rd>=32)
               {
                  if(IF.FXL==1)
                     VOB[IF.Rd-32][1] = mvi_temp_FL[1];
                  else
                     (VOB[IF.Rd-32][1]) = mvi_temp_FX[1];
               }
               else
               {
                  if(IF.FXL==1)
                     GPR[IF.Rd][1] = mvi_temp_FL[1];
                  else
                     (GPR[IF.Rd][1]) = mvi_temp_FX[1];
               }
            }    
                  
            if(IF.WM_z==1)
            {
               if(IF.Rd>=32)
               {
                  if(IF.FXL==1)
                     VOB[IF.Rd-32][2] = mvi_temp_FL[2];
                  else
                     (VOB[IF.Rd-32][2]) = mvi_temp_FX[2];
               }
               else
               {
                  if(IF.FXL==1)
                     GPR[IF.Rd][2] = mvi_temp_FL[2];
                  else
                     (GPR[IF.Rd][2]) = mvi_temp_FX[2];
               }
            }
                  
            if(IF.WM_w==1)
            {
               if(IF.Rd>=32)
               {
                  if(IF.FXL==1)
                     VOB[IF.Rd-32][3] = mvi_temp_FL[3];
                  else
                     (VOB[IF.Rd-32][3]) = mvi_temp_FX[3];
               }
               else
               {
                  if(IF.FXL==1)
                     GPR[IF.Rd][3] = mvi_temp_FL[3];
                  else
                     (GPR[IF.Rd][3]) = mvi_temp_FX[3];
               }
            }
  
            
            //---------------------------------printf for debug--------------------------------------------------------------
            //-------- Source
            if(IF.Rs>=32)
               fprintf(fp_Result,"Rs_VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
            else
               fprintf(fp_Result,"Rs_GPR[%d]:%f_%f_%f_%f\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
            
            if(IF.FXL==1)
            {   
                if(Imm_float_sign==1)
                   fprintf(fp_Result,"Imm:%f\n",0-int_to_float(Imm));
                else
                   fprintf(fp_Result,"Imm = %f\n", int_to_float(Imm));
            }    
            else   
               fprintf(fp_Result,"Imm = %d\n", Imm);
            //-------- out 
            fprintf(fp_Result,"-----------------------------------------------------------------\n");   
            //--------get hex floating point result------------------------------------------------------------
            if(IF.FXL==1)
            {
               if(IF.Rd>=32) 
                  fprintf(fp_Result,"Rd_VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
               else
                  fprintf(fp_Result,"Rd_GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
               //--------get hex floating point result------------------------------------------------------------
               if(IF.Rd>=32)
               {
                  fprintf(fp_Result,"Rd_VOB_entry[%d] = ", IF.Rd-32);
                  get_float(VOB[IF.Rd-32][0]);
                  fprintf(fp_Result,"_");
                  get_float(VOB[IF.Rd-32][1]);
                  fprintf(fp_Result,"_");
                  get_float(VOB[IF.Rd-32][2]);
                  fprintf(fp_Result,"_");
                  get_float(VOB[IF.Rd-32][3]);
                  fprintf(fp_Result,"\n");
               }    
               else
               {
                  fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
                  get_float(GPR[IF.Rd][0]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][1]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][2]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][3]);
                  fprintf(fp_Result,"\n"); 
               }
            }    
            else
            {
               if(IF.Rd>=32)
                  fprintf(fp_Result,"Rd_VOB_entry[%d] = %d_%d_%d_%d\n", IF.Rd-32, (VOB[IF.Rd-32][0]), (VOB[IF.Rd-32][1]), (VOB[IF.Rd-32][2]), (VOB[IF.Rd-32][3]));
               else
                  fprintf(fp_Result,"Rd_GPR[%d] = %d_%d_%d_%d\n", IF.Rd, (GPR[IF.Rd][0]), (GPR[IF.Rd][1]), (GPR[IF.Rd][2]), (GPR[IF.Rd][3]));
               //--------get hex fixed point result------------------------------------------------------------
               if(IF.Rd>=32)
               {
                  fprintf(fp_Result,"Rd_VOB_entry[%d] = ", IF.Rd-32);
                  get_fix(int(VOB[IF.Rd-32][0]));
                  fprintf(fp_Result,"_");
                  get_fix(int(VOB[IF.Rd-32][1]));
                  fprintf(fp_Result,"_");
                  get_fix(int(VOB[IF.Rd-32][2]));
                  fprintf(fp_Result,"_");
                  get_fix(int(VOB[IF.Rd-32][3]));
                  fprintf(fp_Result,"\n");
               }    
               else
               {
                  fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
                  get_fix(int(GPR[IF.Rd][0]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][1]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][2]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][3]));
                  fprintf(fp_Result,"\n");
               }    
            }
            break; 
       // MIN
       case 11:
            //Rs do swizzle
            switch(IF.Sw_s_x){
                    
                case 0:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break;
                }    
            switch(IF.Sw_s_y){
                    
                case 0:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;
                     break;         
                case 1:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break; 
                } 
            switch(IF.Sw_s_z){
                    
                case 0:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;  
                     break;         
                case 1:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break; 
                }   
            switch(IF.Sw_s_w){
                    
                case 0:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ; 
                     break;         
                case 1:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ; 
                     break; 
                case 2:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ; 
                     break; 
                case 3:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ; 
                     break; 
                }
            // Rt do swizzle----------------------------------------------------------
            switch(IF.Sw_t_x){
                    
                case 0:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break;
                }    
            switch(IF.Sw_t_y){
                    
                case 0:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break; 
                } 
            switch(IF.Sw_t_z){
                    
                case 0:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break;
                }   
            switch(IF.Sw_t_w){
                    
                case 0:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break; 
                }
            //do MIN function------------------------------------------------doesnt support Neg
            if(IF.FXL==1)//flp format
            {
               for(i=0; i<=IF.S_V; i++)//S/V to select how many Src component needed
                  min_temp_FL[i] = MIN(Rs_swiz_temp[i], Rt_swiz_temp[i]);               
            }    
            else//fxd format
            {
               for(i=0; i<=IF.S_V; i++)//S/V to select how many Src component needed
                  min_temp_FX[i] = MIN(int(Rs_swiz_temp[i]), int(Rt_swiz_temp[i]));
            }         
            //--------- WB -------------------------------------------------
            //WB
            if(IF.WM_x==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][0] = min_temp_FL[0];
               else
                  (GPR[IF.Rd][0]) = min_temp_FX[0];
            }
                  
            if(IF.WM_y==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][1] = min_temp_FL[1];
               else
                  (GPR[IF.Rd][1]) = min_temp_FX[1];
            }    
                  
            if(IF.WM_z==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][2] = min_temp_FL[2];
               else
                  (GPR[IF.Rd][2]) = min_temp_FX[2];
            }
                  
            if(IF.WM_w==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][3] = min_temp_FL[3];
               else
                  (GPR[IF.Rd][3]) = min_temp_FX[3];
            }
            
            //---------------------------------printf for debug--------------------------------------------------------------
            if(IF.Rs>=32)
               fprintf(fp_Result,"Rs_VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
            else
               fprintf(fp_Result,"Rs_GPR[%d]:%f_%f_%f_%f\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
               
            if(IF.Rt>=32)   
               fprintf(fp_Result,"Rt_VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
            else
               fprintf(fp_Result,"Rt_GPR[%d]:%f_%f_%f_%f\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            //-------- out -----------------------------------------------------------------------------------------
            if(IF.FXL==1)
            {
               fprintf(fp_Result,"Rd_GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
               //--------get hex floating point result------------------------------------------------------------
               fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
                  get_float(GPR[IF.Rd][0]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][1]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][2]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][3]);
                  fprintf(fp_Result,"\n");
            }    
            else
            {
               fprintf(fp_Result,"Rd_GPR[%d] = %d_%d_%d_%d\n", IF.Rd, (GPR[IF.Rd][0]), (GPR[IF.Rd][1]), (GPR[IF.Rd][2]), (GPR[IF.Rd][3]));
               //--------get hex fixed point result------------------------------------------------------------
               fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
                  get_fix(int(GPR[IF.Rd][0]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][1]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][2]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][3]));
                  fprintf(fp_Result,"\n");    
            }
            break;   
       // MAX
       case 12:
            //Rs do swizzle
            switch(IF.Sw_s_x){
                    
                case 0:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break;
                }    
            switch(IF.Sw_s_y){
                    
                case 0:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;
                     break;         
                case 1:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break; 
                } 
            switch(IF.Sw_s_z){
                    
                case 0:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;  
                     break;         
                case 1:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break; 
                }   
            switch(IF.Sw_s_w){
                    
                case 0:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ; 
                     break;         
                case 1:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ; 
                     break; 
                case 2:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ; 
                     break; 
                case 3:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ; 
                     break; 
                }
            // Rt do swizzle----------------------------------------------------------
            switch(IF.Sw_t_x){
                    
                case 0:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break;
                }    
            switch(IF.Sw_t_y){
                    
                case 0:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break; 
                } 
            switch(IF.Sw_t_z){
                    
                case 0:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break;
                }   
            switch(IF.Sw_t_w){
                    
                case 0:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break; 
                }
            //do MAX function------------------------------------------------doesnt support Neg
            if(IF.FXL==1)//flp format
            {
               for(i=0; i<=IF.S_V; i++)//S/V to select how many Src component needed
                  max_temp_FL[i] = MAX(Rs_swiz_temp[i], Rt_swiz_temp[i]);               
            }    
            else//fxd format
            {
               for(i=0; i<=IF.S_V; i++)//S/V to select how many Src component needed
                  max_temp_FX[i] = MAX(int(Rs_swiz_temp[i]), int(Rt_swiz_temp[i]));
            }         
            //--------WB----------------------------------------------------------
            if(IF.WM_x==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][0] = max_temp_FL[0];
               else
                  (GPR[IF.Rd][0]) = max_temp_FX[0];
            }
                  
            if(IF.WM_y==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][1] = max_temp_FL[1];
               else
                  (GPR[IF.Rd][1]) = max_temp_FX[1];
            }    
                  
            if(IF.WM_z==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][2] = max_temp_FL[2];
               else
                  (GPR[IF.Rd][2]) = max_temp_FX[2];
            }
                  
            if(IF.WM_w==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][3] = max_temp_FL[3];
               else
                  (GPR[IF.Rd][3]) = max_temp_FX[3];
            }
            
            //---------------------------------printf for debug--------------------------------------------------------------
            if(IF.Rs>=32)
               fprintf(fp_Result,"Rs_VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
            else
               fprintf(fp_Result,"Rs_GPR[%d]:%f_%f_%f_%f\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
               
            if(IF.Rt>=32)   
               fprintf(fp_Result,"Rt_VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
            else
               fprintf(fp_Result,"Rt_GPR[%d]:%f_%f_%f_%f\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
            printf("-----------------------------------------------------------------\n");
            //-------- out -----------------------------------------------------------------------------------------
            if(IF.FXL==1)
            {
               fprintf(fp_Result,"Rd_GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
               //--------get hex floating point result------------------------------------------------------------
               fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
                  get_float(GPR[IF.Rd][0]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][1]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][2]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][3]);
                  fprintf(fp_Result,"\n");
            }    
            else
            {
               fprintf(fp_Result,"Rd_GPR[%d] = %d_%d_%d_%d\n", IF.Rd, (GPR[IF.Rd][0]), (GPR[IF.Rd][1]), (GPR[IF.Rd][2]), (GPR[IF.Rd][3]));
               //--------get hex fixed point result------------------------------------------------------------
               fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
                  get_fix(int(GPR[IF.Rd][0]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][1]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][2]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][3]));
                  fprintf(fp_Result,"\n");    
            }
            break;
       //LDV
       case 13:	
            //
            
            LD_addr = int(GPR[IF.Rs][0]) + Imm;
            if(IF.S_V==0)
                  GPR[IF.Rd][0] = CMEM[LD_addr][0] ;
                  
            if(IF.S_V==1)
            {
                  GPR[IF.Rd][0] = CMEM[LD_addr][0] ;
                  GPR[IF.Rd][1] = CMEM[LD_addr][1] ;
            }    
                  
            if(IF.S_V==2)
            {
                  GPR[IF.Rd][0] = CMEM[LD_addr][0] ;
                  GPR[IF.Rd][1] = CMEM[LD_addr][1] ;
                  GPR[IF.Rd][2] = CMEM[LD_addr][2] ;
            }
                  
            if(IF.S_V==3)
            {
                  GPR[IF.Rd][0] = CMEM[LD_addr][0] ;
                  GPR[IF.Rd][1] = CMEM[LD_addr][1] ;
                  GPR[IF.Rd][2] = CMEM[LD_addr][2] ;
                  GPR[IF.Rd][3] = CMEM[LD_addr][3] ;
            }
            
            

            fprintf(fp_Result,"CMEM[%d] = %f_%f_%f_%f\n", LD_addr, CMEM[LD_addr][0], CMEM[LD_addr][1], CMEM[LD_addr][2], CMEM[LD_addr][3] );
            fprintf(fp_Result,"CMEM[%d] = ", LD_addr);
            get_float(CMEM[LD_addr][0] );
            fprintf(fp_Result,"_");
            get_float(CMEM[LD_addr][1] );
            fprintf(fp_Result,"_");
            get_float(CMEM[LD_addr][2] );
            fprintf(fp_Result,"_");
            get_float(CMEM[LD_addr][3] );
            fprintf(fp_Result,"\n");
            fprintf(fp_Result,"GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result,"GPR[%d] = ", IF.Rd);
            get_float(GPR[IF.Rd][0]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][1]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][2]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][3]);
            fprintf(fp_Result,"\n");             
            break;
       //STV
       case 14:
            //
            ST_addr = int(GPR[IF.Rd][0]) + Imm;
            if(IF.S_V==0)
                  CMEM[ST_addr][0] = GPR[IF.Rs][0] ;
                  
            if(IF.S_V==1)
            {
                  CMEM[ST_addr][0] = GPR[IF.Rs][0] ;
                  CMEM[ST_addr][1] = GPR[IF.Rs][1] ;
            }    
                  
            if(IF.S_V==2)
            {
                  CMEM[ST_addr][0] = GPR[IF.Rs][0] ;
                  CMEM[ST_addr][1] = GPR[IF.Rs][1] ;
                  CMEM[ST_addr][2] = GPR[IF.Rs][2] ;
            }
                  
            if(IF.S_V==3)
            {
                  CMEM[ST_addr][0] = GPR[IF.Rs][0] ;
                  CMEM[ST_addr][1] = GPR[IF.Rs][1] ;
                  CMEM[ST_addr][2] = GPR[IF.Rs][2] ;
                  CMEM[ST_addr][3] = GPR[IF.Rs][3] ;
            } //S/V =00 => write x, S/V=01 => write y, S/V=10 => write z, S/V=11 => write w
       
 
            fprintf(fp_Result,"GPR[%d] = %f_%f_%f_%f\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
            fprintf(fp_Result,"CMEM[%d] = %f_%f_%f_%f\n",ST_addr, CMEM[ST_addr][0], CMEM[ST_addr][1], CMEM[ST_addr][2], CMEM[ST_addr][3]);            
            break;
       // ADD
       case 16:
            //
            //Rs do swizzle
            switch(IF.Sw_s_x){
                    
                case 0:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break;
                }    
            switch(IF.Sw_s_y){
                    
                case 0:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;
                     break;         
                case 1:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break; 
                } 
            switch(IF.Sw_s_z){
                    
                case 0:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;  
                     break;         
                case 1:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break; 
                }   
            switch(IF.Sw_s_w){
                    
                case 0:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ; 
                     break;         
                case 1:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ; 
                     break; 
                case 2:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ; 
                     break; 
                case 3:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ; 
                     break; 
                }
            // Rt do swizzle----------------------------------------------------------
            switch(IF.Sw_t_x){
                    
                case 0:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break;
                }    
            switch(IF.Sw_t_y){
                    
                case 0:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break; 
                } 
            switch(IF.Sw_t_z){
                    
                case 0:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break;
                }   
            switch(IF.Sw_t_w){
                    
                case 0:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break; 
                }
            //----------determine Rs Neg--------------------------
            if(IF.Neg_Rs==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  Rs_swiz_temp[i] = -Rs_swiz_temp[i];
            }
            //----------determine Rt Neg--------------------------
            if(IF.Neg_Rt==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  Rt_swiz_temp[i] = -Rt_swiz_temp[i];
            }
            //---------ADD function------------------------
            if(IF.FXL==1)//flp format
            {
               for(i=0; i<=IF.S_V; i++)//S/V to select how many Src component needed
                  add_temp_FL[i] = Rs_swiz_temp[i] + Rt_swiz_temp[i];               
            }    
            else//fxd format
            {
               for(i=0; i<=IF.S_V; i++)//S/V to select how many Src component needed
                  add_temp_FX[i] = int(Rs_swiz_temp[i]) + int(Rt_swiz_temp[i]);
            }

            //--------- WB -------------------------------------------------
            if(IF.WM_x==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][0] = add_temp_FL[0];
               else
                  (GPR[IF.Rd][0]) = add_temp_FX[0];
            }
                  
            if(IF.WM_y==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][1] = add_temp_FL[1];
               else
                  (GPR[IF.Rd][1]) = add_temp_FX[1];
            }    
                  
            if(IF.WM_z==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][2] = add_temp_FL[2];
               else
                  (GPR[IF.Rd][2]) = add_temp_FX[2];
            }
                  
            if(IF.WM_w==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][3] = add_temp_FL[3];
               else
                  (GPR[IF.Rd][3]) = add_temp_FX[3];
            }
            
            //---------------------------------printf for debug--------------------------------------------------------------
            if(IF.Rs>=32)
               fprintf(fp_Result,"Rs_VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
            else
               fprintf(fp_Result,"Rs_GPR[%d]:%f_%f_%f_%f\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
               
            if(IF.Rt>=32)   
               fprintf(fp_Result,"Rt_VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
            else
               fprintf(fp_Result,"Rt_GPR[%d]:%f_%f_%f_%f\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            //-------- out -----------------------------------------------------------------------------------------
            if(IF.FXL==1)
            {
               fprintf(fp_Result,"Rd_GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
               //--------get hex floating point result------------------------------------------------------------
               fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
                  get_float(GPR[IF.Rd][0]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][1]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][2]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][3]);
                  fprintf(fp_Result,"\n");
            }    
            else
            {
               fprintf(fp_Result,"Rd_GPR[%d] = %d_%d_%d_%d\n", IF.Rd, (GPR[IF.Rd][0]), (GPR[IF.Rd][1]), (GPR[IF.Rd][2]), (GPR[IF.Rd][3]));
               //--------get hex fixed point result------------------------------------------------------------
               fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
                  get_fix(int(GPR[IF.Rd][0]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][1]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][2]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][3]));
                  fprintf(fp_Result,"\n");    
            }
            break;
       // MULH
       case 17:
            //Rs do swizzle
            switch(IF.Sw_s_x){
                    
                case 0:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break;
                }    
            switch(IF.Sw_s_y){
                    
                case 0:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;
                     break;         
                case 1:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break; 
                } 
            switch(IF.Sw_s_z){
                    
                case 0:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;  
                     break;         
                case 1:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break; 
                }   
            switch(IF.Sw_s_w){
                    
                case 0:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ; 
                     break;         
                case 1:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ; 
                     break; 
                case 2:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ; 
                     break; 
                case 3:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ; 
                     break; 
                }
            // Rt do swizzle----------------------------------------------------------
            switch(IF.Sw_t_x){
                    
                case 0:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break;
                }    
            switch(IF.Sw_t_y){
                    
                case 0:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break; 
                } 
            switch(IF.Sw_t_z){
                    
                case 0:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break;
                }   
            switch(IF.Sw_t_w){
                    
                case 0:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break; 
                }
            //----------Rs do Neg--------------------------
            if(IF.Neg_Rs==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  Rs_swiz_temp[i] = -Rs_swiz_temp[i];
            }
            //----------Rt do Neg--------------------------
            if(IF.Neg_Rt==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  Rt_swiz_temp[i] = -Rt_swiz_temp[i];
            }
            //---------MULH function------------------------
            if(IF.FXL==0)//flp format
               printf("MULH doesnt support fixed point.");
            else
            {
               for(i=0; i<=IF.S_V; i++)//S/V to select how many Src component needed
                  GPR[IF.Rd][i] = Rs_swiz_temp[i] * Rt_swiz_temp[i];               
            }
            /*    
            // -------------WB----------------------------------
            if(IF.WM_x==1)
               GPR[IF.Rd][0] = mulh_temp[0];
                  
            if(IF.WM_y==1)
               GPR[IF.Rd][1] = mulh_temp[1];
                  
            if(IF.WM_z==1)
               GPR[IF.Rd][2] = mulh_temp[2];
                  
            if(IF.WM_w==1)
               GPR[IF.Rd][3] = mulh_temp[3];
            */
            
            //----------print for debug------------------------------------------------------------------------
            if(IF.Rs>=32)
               fprintf(fp_Result,"VAB[%d]:%f_%f_%f_%f\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
            else
               fprintf(fp_Result,"Rs_GPR[%d]:%f_%f_%f_%f\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
            
            if(IF.Rt>=32)
               fprintf(fp_Result,"VAB[%d]:%f_%f_%f_%f\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
            else
               fprintf(fp_Result,"Rt_GPR[%d]:%f_%f_%f_%f\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            //------------ out -----------------
            fprintf(fp_Result,"GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result,"GPR[%d] = ", IF.Rd);
            get_float(GPR[IF.Rd][0]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][1]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][2]);
            fprintf(fp_Result,"_");
            get_float(GPR[IF.Rd][3]);
            fprintf(fp_Result,"\n");
            break;
       // MAD
       case 18:
            //Rs do swizzle
            switch(IF.Sw_s_x){
                    
                case 0:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break;
                }    
            switch(IF.Sw_s_y){
                    
                case 0:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;
                     break;         
                case 1:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break; 
                } 
            switch(IF.Sw_s_z){
                    
                case 0:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;  
                     break;         
                case 1:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break; 
                }   
            switch(IF.Sw_s_w){
                    
                case 0:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ; 
                     break;         
                case 1:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ; 
                     break; 
                case 2:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ; 
                     break; 
                case 3:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ; 
                     break; 
                }
            // Rt do swizzle----------------------------------------------------------
            switch(IF.Sw_t_x){
                    
                case 0:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break;
                }    
            switch(IF.Sw_t_y){
                    
                case 0:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break; 
                } 
            switch(IF.Sw_t_z){
                    
                case 0:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break;
                }   
            switch(IF.Sw_t_w){
                    
                case 0:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break; 
                }
            // Rd do swizzle----------------------------------------------------------
            switch(IF.Sw_d_x){
                    
                case 0:
                     Rd_swiz_temp[0] = (IF.Rd>=32) ? VAB[IF.Rd-32][0] : GPR[IF.Rd][0] ;   
                     break;         
                case 1:
                     Rd_swiz_temp[0] = (IF.Rd>=32) ? VAB[IF.Rd-32][1] : GPR[IF.Rd][1] ;
                     break; 
                case 2:
                     Rd_swiz_temp[0] = (IF.Rd>=32) ? VAB[IF.Rd-32][2] : GPR[IF.Rd][2] ;
                     break; 
                case 3:
                     Rd_swiz_temp[0] = (IF.Rd>=32) ? VAB[IF.Rd-32][3] : GPR[IF.Rd][3] ;
                     break;
                }    
            switch(IF.Sw_d_y){
                    
                case 0:
                     Rd_swiz_temp[1] = (IF.Rd>=32) ? VAB[IF.Rd-32][0] : GPR[IF.Rd][0] ;   
                     break;         
                case 1:
                     Rd_swiz_temp[1] = (IF.Rd>=32) ? VAB[IF.Rd-32][1] : GPR[IF.Rd][1] ;
                     break; 
                case 2:
                     Rd_swiz_temp[1] = (IF.Rd>=32) ? VAB[IF.Rd-32][2] : GPR[IF.Rd][2] ;
                     break; 
                case 3:
                     Rd_swiz_temp[1] = (IF.Rd>=32) ? VAB[IF.Rd-32][3] : GPR[IF.Rd][3] ;
                     break; 
                } 
            switch(IF.Sw_d_z){
                    
                case 0:
                     Rd_swiz_temp[2] = (IF.Rd>=32) ? VAB[IF.Rd-32][0] : GPR[IF.Rd][0] ;   
                     break;         
                case 1:
                     Rd_swiz_temp[2] = (IF.Rd>=32) ? VAB[IF.Rd-32][1] : GPR[IF.Rd][1] ;
                     break; 
                case 2:
                     Rd_swiz_temp[2] = (IF.Rd>=32) ? VAB[IF.Rd-32][2] : GPR[IF.Rd][2] ;
                     break; 
                case 3:
                     Rd_swiz_temp[2] = (IF.Rd>=32) ? VAB[IF.Rd-32][3] : GPR[IF.Rd][3] ;
                     break;
                }   
            switch(IF.Sw_d_w){
                    
                case 0:
                     Rd_swiz_temp[3] = (IF.Rd>=32) ? VAB[IF.Rd-32][0] : GPR[IF.Rd][0] ;   
                     break;         
                case 1:
                     Rd_swiz_temp[3] = (IF.Rd>=32) ? VAB[IF.Rd-32][1] : GPR[IF.Rd][1] ;
                     break; 
                case 2:
                     Rd_swiz_temp[3] = (IF.Rd>=32) ? VAB[IF.Rd-32][2] : GPR[IF.Rd][2] ;
                     break; 
                case 3:
                     Rd_swiz_temp[3] = (IF.Rd>=32) ? VAB[IF.Rd-32][3] : GPR[IF.Rd][3] ;
                     break; 
                }
            //----------Rs do Neg--------------------------
            if(IF.Neg_Rs==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  Rs_swiz_temp[i] = -Rs_swiz_temp[i];
            }
            //----------Rt do Neg--------------------------
            if(IF.Neg_Rt==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  Rt_swiz_temp[i] = -Rt_swiz_temp[i];
            }
            //----------Rd do Neg--------------------------
            if(IF.Neg_Rd==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  Rd_swiz_temp[i] = -Rd_swiz_temp[i];
            }
            //---------MAD function------------------------
            if(IF.FXL==1)//flp format
            {
               for(i=0; i<=IF.S_V; i++)//S/V to select how many Src component needed
                  mad_temp_FL[i] = (Rs_swiz_temp[i] * Rt_swiz_temp[i]) + Rd_swiz_temp[i];               
            }    
            else//fxd format
            {
               for(i=0; i<=IF.S_V; i++)//S/V to select how many Src component needed
                  mad_temp_FX[i] = (int(Rs_swiz_temp[i]) * int(Rt_swiz_temp[i])) + int(Rd_swiz_temp[i]);
            }

            //--------- WB -------------------------------------------------
            if(IF.WM_x==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][0] = mad_temp_FL[0];
               else
                  (GPR[IF.Rd][0]) = mad_temp_FX[0];
            }
                  
            if(IF.WM_y==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][1] = mad_temp_FL[1];
               else
                  (GPR[IF.Rd][1]) = mad_temp_FX[1];
            }    
                  
            if(IF.WM_z==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][2] = mad_temp_FL[2];
               else
                  (GPR[IF.Rd][2]) = mad_temp_FX[2];
            }
                  
            if(IF.WM_w==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][3] = mad_temp_FL[3];
               else
                  (GPR[IF.Rd][3]) = mad_temp_FX[3];
            }
            //---------------------------------printf for debug--------------------------------------------------------------
            if(IF.Rs>=32)
               fprintf(fp_Result,"Rs_VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
            else
               fprintf(fp_Result,"Rs_GPR[%d]:%f_%f_%f_%f\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
               
            if(IF.Rt>=32)   
               fprintf(fp_Result,"Rt_VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
            else
               fprintf(fp_Result,"Rt_GPR[%d]:%f_%f_%f_%f\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
               
            fprintf(fp_Result,"Rd_GPR[%d]:%f_%f_%f_%f\n", IF.Rd, Rd_swiz_temp[0], Rd_swiz_temp[1], Rd_swiz_temp[2], Rd_swiz_temp[3]);
            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            //-------- out ------------------------------------------------------------
            if(IF.FXL==1)
            {
               fprintf(fp_Result,"Rd_GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
               //--------get hex floating point result------------------------------------------------------------
               fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
                  get_float(GPR[IF.Rd][0]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][1]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][2]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][3]);
                  fprintf(fp_Result,"\n");
            }    
            else
            {
               fprintf(fp_Result,"Rd_GPR[%d] = %d_%d_%d_%d\n", IF.Rd, (GPR[IF.Rd][0]), (GPR[IF.Rd][1]), (GPR[IF.Rd][2]), (GPR[IF.Rd][3]));
               //--------get hex fixed point result------------------------------------------------------------
               fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
                  get_fix(int(GPR[IF.Rd][0]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][1]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][2]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][3]));
                  fprintf(fp_Result,"\n");    
            }
            break;
       // DP     
       case 19:
            //Rs do swizzle
            switch(IF.Sw_s_x){
                    
                case 0:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break;
                }    
            switch(IF.Sw_s_y){
                    
                case 0:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;
                     break;         
                case 1:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break; 
                } 
            switch(IF.Sw_s_z){
                    
                case 0:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ;  
                     break;         
                case 1:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ;
                     break; 
                case 2:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ;
                     break; 
                case 3:
                     Rs_swiz_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ;
                     break; 
                }   
            switch(IF.Sw_s_w){
                    
                case 0:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0] ; 
                     break;         
                case 1:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1] ; 
                     break; 
                case 2:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2] ; 
                     break; 
                case 3:
                     Rs_swiz_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3] ; 
                     break; 
                }
            // Rt do swizzle----------------------------------------------------------
            switch(IF.Sw_t_x){
                    
                case 0:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break;
                }    
            switch(IF.Sw_t_y){
                    
                case 0:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break; 
                } 
            switch(IF.Sw_t_z){
                    
                case 0:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break;
                }   
            switch(IF.Sw_t_w){
                    
                case 0:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0] ;   
                     break;         
                case 1:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1] ;
                     break; 
                case 2:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2] ;
                     break; 
                case 3:
                     Rt_swiz_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3] ;
                     break; 
                }
            //----------determine Neg of Rs-------------------------------------
            if(IF.Neg_Rs==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  Rs_swiz_temp[i] = -Rs_swiz_temp[i];
            }
            //----------determine Neg of Rt-------------------------------------
            if(IF.Neg_Rt==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  Rt_swiz_temp[i] = -Rt_swiz_temp[i];
            } 
            //---------DP function----------------------------------------------
            //IF.S_V = 3(DP4), IF.S_V = 2(DP3) IF.S_V = 1(DP2), IF.S_V = 0(DP1)
            dp_temp_FL = 0;
            dp_temp_FX = 0;
            if(IF.FXL==1)//flp format
            {
               for(i=0; i<=IF.S_V; i++)//S/V to select how many Src component needed
                  dp_temp_FL = dp_temp_FL + (Rs_swiz_temp[i] * Rt_swiz_temp[i]);               
            }    
            else//fxd format
            {
               for(i=0; i<=IF.S_V; i++)//S/V to select how many Src component needed
                  dp_temp_FX = dp_temp_FX + (int(Rs_swiz_temp[i]) * int(Rt_swiz_temp[i]));
            }              
       
            //--------- WB -------------------------------------------------
            if(IF.WM_x==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][0] = dp_temp_FL;
               else
                  (GPR[IF.Rd][0]) = dp_temp_FX;
            }
                  
            if(IF.WM_y==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][1] = dp_temp_FL;
               else
                  (GPR[IF.Rd][1]) = dp_temp_FX;
            }    
                  
            if(IF.WM_z==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][2] = dp_temp_FL;
               else
                  (GPR[IF.Rd][2]) = dp_temp_FX;
            }
                  
            if(IF.WM_w==1)
            {
               if(IF.FXL==1)
                  GPR[IF.Rd][3] = dp_temp_FL;
               else
                  (GPR[IF.Rd][3]) = dp_temp_FX;
            }
            //---------------------------------printf for debug--------------------------------------------------------------
            if(IF.Rs>=32)
               fprintf(fp_Result,"Rs_VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
            else
               fprintf(fp_Result,"Rs_GPR[%d]:%f_%f_%f_%f\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
               
            if(IF.Rt>=32)   
               fprintf(fp_Result,"Rt_VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
            else
               fprintf(fp_Result,"Rt_GPR[%d]:%f_%f_%f_%f\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            //-------- out ------------------------------------------------------------
            if(IF.FXL==1)
            {
               fprintf(fp_Result,"Rd_GPR[%d] = %f_%f_%f_%f\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
               //--------get hex floating point result------------------------------------------------------------
               fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
                  get_float(GPR[IF.Rd][0]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][1]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][2]);
                  fprintf(fp_Result,"_");
                  get_float(GPR[IF.Rd][3]);
                  fprintf(fp_Result,"\n");
            }    
            else
            {
               fprintf(fp_Result,"Rd_GPR[%d] = %d_%d_%d_%d\n", IF.Rd, (GPR[IF.Rd][0]), (GPR[IF.Rd][1]), (GPR[IF.Rd][2]), (GPR[IF.Rd][3]));
               //--------get hex fixed point result------------------------------------------------------------
               fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
                  get_fix(int(GPR[IF.Rd][0]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][1]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][2]));
                  fprintf(fp_Result,"_");
                  get_fix(int(GPR[IF.Rd][3]));
                  fprintf(fp_Result,"\n");    
            }
            break;
       // INV  
       case 20:
            //---------------determine Rs swizzle--------------------------------
            switch(IF.Sw_s_x){
                    
                case 0:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break;
                }    
            switch(IF.Sw_s_y){
                    
                case 0:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break;
                } 
            switch(IF.Sw_s_z){
                    
                case 0:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break; 
                }   
            switch(IF.Sw_s_w){
                    
                case 0:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break; 
                }
            //----------determine Rs Neg--------------------------
            if(IF.Neg_Rs==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  (Rs_swiz_temp[i]) = -(Rs_swiz_temp[i]);
            }

            //--------------do INV-----------------------------------------------------
            for(i=0; i<=IF.S_V; i++)
               inv_temp[i] = -int(Rs_swiz_temp[i]);
            
            //-------- WB -------------------------------------------------------------
            if(IF.FXL!=0)
              printf("Instruction ERROR!! INV is bit-wise, FXL must be 0.");
            else
            {
               if(IF.WM_x==1)
                  (GPR[IF.Rd][0]) = inv_temp[0];
              
               if(IF.WM_y==1)
                  (GPR[IF.Rd][1]) = inv_temp[1];
               
               if(IF.WM_z==1)
                  (GPR[IF.Rd][2]) = inv_temp[2];
               
               if(IF.WM_w==1)
                  (GPR[IF.Rd][3]) = inv_temp[3];
            }
            //----------print for debug------------------------------------------------------------------------
            
            fprintf(fp_Result,"Rs_GPR[%d]:%d_%d_%d_%d\n", IF.Rs, int(Rs_swiz_temp[0]), int(Rs_swiz_temp[1]), int(Rs_swiz_temp[2]), int(Rs_swiz_temp[3]));

            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            //------------ out -----------------
            fprintf(fp_Result,"GPR[%d] = %d_%d_%d_%d\n", IF.Rd, int(GPR[IF.Rd][0]), int(GPR[IF.Rd][1]), int(GPR[IF.Rd][2]), int(GPR[IF.Rd][3]));
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
            get_fix(int(GPR[IF.Rd][0]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][1]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][2]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][3]));
            fprintf(fp_Result,"\n"); 
            break;
       // AND
       case 21:
            //---------------determine Rs swizzle--------------------------------
            switch(IF.Sw_s_x){
                    
                case 0:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break;
                }    
            switch(IF.Sw_s_y){
                    
                case 0:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break;
                } 
            switch(IF.Sw_s_z){
                    
                case 0:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break; 
                }   
            switch(IF.Sw_s_w){
                    
                case 0:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break; 
                } 
            //---------------determine Rt swizzle--------------------------------
            switch(IF.Sw_t_x){
                    
                case 0:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break;
                }    
            switch(IF.Sw_t_y){
                    
                case 0:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break;
                } 
            switch(IF.Sw_t_z){
                    
                case 0:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break;
                }   
            switch(IF.Sw_t_w){
                    
                case 0:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break; 
                }
            //----------determine Neg of Rs-------------------------------------
            if(IF.Neg_Rs==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  (Rs_swiz_temp[i]) = -(Rs_swiz_temp[i]);
            }
            //----------determine Neg of Rt-------------------------------------
            if(IF.Neg_Rt==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  (Rt_swiz_temp[i]) = -(Rt_swiz_temp[i]);
            }
            //--------------do AND-----------------------------------------------------
            for(i=0; i<=IF.S_V; i++)
               and_temp[i] = int(Rs_swiz_temp[i]) & int(Rt_swiz_temp[i]);
            
            //-------- WB -------------------------------------------------------------
            if(IF.FXL!=0)
              printf("Instruction ERROR!! AND is bit-wise, FXL must be 0.");
            else
            {
               if(IF.WM_x==1)
                  (GPR[IF.Rd][0]) = and_temp[0];
              
               if(IF.WM_y==1)
                  (GPR[IF.Rd][1]) = and_temp[1];
               
               if(IF.WM_z==1)
                  (GPR[IF.Rd][2]) = and_temp[2];
               
               if(IF.WM_w==1)
                  (GPR[IF.Rd][3]) = and_temp[3];
            }
            //----------print for debug------------------------------------------------------------------------
            
            fprintf(fp_Result,"Rs_GPR[%d]:%d_%d_%d_%d\n", IF.Rs, int(Rs_swiz_temp[0]), int(Rs_swiz_temp[1]), int(Rs_swiz_temp[2]), int(Rs_swiz_temp[3]));
            fprintf(fp_Result,"Rt_GPR[%d]:%d_%d_%d_%d\n", IF.Rt, int(Rt_swiz_temp[0]), int(Rt_swiz_temp[1]), int(Rt_swiz_temp[2]), int(Rt_swiz_temp[3]));
            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            //------------ out -----------------
            fprintf(fp_Result,"GPR[%d] = %d_%d_%d_%d\n", IF.Rd, int(GPR[IF.Rd][0]), int(GPR[IF.Rd][1]), int(GPR[IF.Rd][2]), int(GPR[IF.Rd][3]));
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
            get_fix(int(GPR[IF.Rd][0]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][1]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][2]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][3]));
            fprintf(fp_Result,"\n");
            break;
       // OR
       case 22:
            //---------------determine Rs swizzle--------------------------------
            switch(IF.Sw_s_x){
                    
                case 0:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break;
                }    
            switch(IF.Sw_s_y){
                    
                case 0:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break;
                } 
            switch(IF.Sw_s_z){
                    
                case 0:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break; 
                }   
            switch(IF.Sw_s_w){
                    
                case 0:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break; 
                } 
            //---------------determine Rt swizzle--------------------------------
            switch(IF.Sw_t_x){
                    
                case 0:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break;
                }    
            switch(IF.Sw_t_y){
                    
                case 0:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break;
                } 
            switch(IF.Sw_t_z){
                    
                case 0:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break;
                }   
            switch(IF.Sw_t_w){
                    
                case 0:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break; 
                }
            //----------determine Neg of Rs-------------------------------------
            if(IF.Neg_Rs==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  (Rs_swiz_temp[i]) = -(Rs_swiz_temp[i]);
            }
            //----------determine Neg of Rt-------------------------------------
            if(IF.Neg_Rt==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  (Rt_swiz_temp[i]) = -(Rt_swiz_temp[i]);
            }
            //--------------do OR-----------------------------------------------------
            for(i=0; i<=IF.S_V; i++)
            {
                VS_or_src1.float_format = Rs_swiz_temp[i];
                VS_or_src2.float_format = Rt_swiz_temp[i];
                do_or();
                or_temp[i] = VS_or_dst.float_format;   
            }     //2010/09/29 ya
             //or_temp[i] = (int &)Rs_swiz_temp[i] | (int &)Rt_swiz_temp[i] ; //int(Rs_swiz_temp[i]) | int(Rt_swiz_temp[i]);
             //change by ya 2010/09/29
             //or_temp[i] = (int &)(float &(Rs_swiz_temp[i])) | Rt_swiz_temp[i] ; 
            
            //-------- WB -------------------------------------------------------------
            if(IF.FXL!=0)
              fprintf(fp_Result,"Instruction ERROR!! OR is bit-wise, FXL must be 0.");
            else
            {
               if(IF.WM_x==1)
                  (GPR[IF.Rd][0]) = or_temp[0];
              
               if(IF.WM_y==1)
                  (GPR[IF.Rd][1]) = or_temp[1];
               
               if(IF.WM_z==1)
                  (GPR[IF.Rd][2]) = or_temp[2];
               
               if(IF.WM_w==1)
                  (GPR[IF.Rd][3]) = or_temp[3];
            }
            //----------print for debug------------------------------------------------------------------------
            
            fprintf(fp_Result,"Rs_GPR[%d]:%d_%d_%d_%d\n", IF.Rs, int(Rs_swiz_temp[0]), int(Rs_swiz_temp[1]), int(Rs_swiz_temp[2]), int(Rs_swiz_temp[3]));
            fprintf(fp_Result,"Rt_GPR[%d]:%d_%d_%d_%d\n", IF.Rt, int(Rt_swiz_temp[0]), int(Rt_swiz_temp[1]), int(Rt_swiz_temp[2]), int(Rt_swiz_temp[3]));
            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            //------------ out -----------------
            fprintf(fp_Result,"GPR[%d] = %d_%d_%d_%d\n", IF.Rd, int(GPR[IF.Rd][0]), int(GPR[IF.Rd][1]), int(GPR[IF.Rd][2]), int(GPR[IF.Rd][3]));
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
            get_fix(int(GPR[IF.Rd][0]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][1]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][2]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][3]));
            fprintf(fp_Result,"\n");
            break;
        // XOR
       case 23:
            //---------------determine Rs swizzle--------------------------------
            switch(IF.Sw_s_x){
                    
                case 0:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break;
                }    
            switch(IF.Sw_s_y){
                    
                case 0:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break;
                } 
            switch(IF.Sw_s_z){
                    
                case 0:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break; 
                }   
            switch(IF.Sw_s_w){
                    
                case 0:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break; 
                } 
            //---------------determine Rt swizzle--------------------------------
            switch(IF.Sw_t_x){
                    
                case 0:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break;
                }    
            switch(IF.Sw_t_y){
                    
                case 0:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break;
                } 
            switch(IF.Sw_t_z){
                    
                case 0:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break;
                }   
            switch(IF.Sw_t_w){
                    
                case 0:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break; 
                }
            //----------determine Neg of Rs-------------------------------------
            if(IF.Neg_Rs==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  (Rs_swiz_temp[i]) = -(Rs_swiz_temp[i]);
            }
            //----------determine Neg of Rt-------------------------------------
            if(IF.Neg_Rt==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  (Rt_swiz_temp[i]) = -(Rt_swiz_temp[i]);
            }
            //--------------do AND-----------------------------------------------------
            for(i=0; i<=IF.S_V; i++)
               xor_temp[i] = int(Rs_swiz_temp[i]) ^ int(Rt_swiz_temp[i]);
            
            //-------- WB -------------------------------------------------------------
            if(IF.FXL!=0)
              fprintf(fp_Result,"Instruction ERROR!! XOR is bit-wise, FXL must be 0.");
            else
            {
               if(IF.WM_x==1)
                  (GPR[IF.Rd][0]) = xor_temp[0];
              
               if(IF.WM_y==1)
                  (GPR[IF.Rd][1]) = xor_temp[1];
               
               if(IF.WM_z==1)
                  (GPR[IF.Rd][2]) = xor_temp[2];
               
               if(IF.WM_w==1)
                  (GPR[IF.Rd][3]) = xor_temp[3];
            }
            //----------print for debug------------------------------------------------------------------------
            
            fprintf(fp_Result,"Rs_GPR[%d]:%d_%d_%d_%d\n", IF.Rs, int(Rs_swiz_temp[0]), int(Rs_swiz_temp[1]), int(Rs_swiz_temp[2]), int(Rs_swiz_temp[3]));
            fprintf(fp_Result,"Rt_GPR[%d]:%d_%d_%d_%d\n", IF.Rt, int(Rt_swiz_temp[0]), int(Rt_swiz_temp[1]), int(Rt_swiz_temp[2]), int(Rt_swiz_temp[3]));
            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            //------------ out -----------------
            fprintf(fp_Result,"GPR[%d] = %d_%d_%d_%d\n", IF.Rd, int(GPR[IF.Rd][0]), int(GPR[IF.Rd][1]), int(GPR[IF.Rd][2]), int(GPR[IF.Rd][3]));
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
            get_fix(int(GPR[IF.Rd][0]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][1]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][2]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][3]));
            fprintf(fp_Result,"\n");
            break;
       // BEQ
       case 24:
            //
            //------------determine the source----------------------------------
            for(i=0; i<=IF.S_V; i++)
            {
               if(IF.FXL==1)
               {
                  R1_temp_FL[i] = (IF.R1>=32) ? VAB[IF.R1-32][i] : GPR[IF.R1][i] ;
                  R2_temp_FL[i] = (IF.R2>=32) ? VAB[IF.R2-32][i] : GPR[IF.R2][i] ;
                  R3_temp_FL[i] = (IF.R3>=32) ? VAB[IF.R3-32][i] : GPR[IF.R3][i] ;
                  R4_temp_FL[i] = GPR[IF.R4][i] ;//---R4 only support read from GPR
               }
               else
               {
                  R1_temp_FX[i] = (IF.R1>=32) ? int(VAB[IF.R1-32][i]) : int(GPR[IF.R1][i]) ;
                  R2_temp_FX[i] = (IF.R2>=32) ? int(VAB[IF.R2-32][i]) : int(GPR[IF.R2][i]) ;
                  R3_temp_FX[i] = (IF.R3>=32) ? int(VAB[IF.R3-32][i]) : int(GPR[IF.R3][i]) ;
                  R4_temp_FX[i] = int(GPR[IF.R4][i]) ;//---R4 only support read from GPR
               }
            }        
            //-----------compare component x---------------------------------------
            if(IF.SCC_x==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[0]==R2_temp_FL[0]) && (R3_temp_FL[0]==R4_temp_FL[0]))
                     branch[0] = 1;
               }
               else
               {
                  if((R1_temp_FX[0]==R2_temp_FX[0]) && (R3_temp_FX[0]==R4_temp_FX[0]))
                     branch[0] = 1;
               }
            }    
            
            //-----------compare component y---------------------------------------
            if(IF.SCC_y==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[1]==R2_temp_FL[1]) && (R3_temp_FL[1]==R4_temp_FL[1]))
                     branch[1] = 1;
               }
               else
               {
                  if((R1_temp_FX[1]==R2_temp_FX[1]) && (R3_temp_FX[1]==R4_temp_FX[1]))
                     branch[1] = 1;
               }
            }    
            
            //-----------compare component z---------------------------------------
            if(IF.SCC_z==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[2]==R2_temp_FL[2]) && (R3_temp_FL[2]==R4_temp_FL[2]))
                     branch[2] = 1;
               }
               else
               {
                  if((R1_temp_FX[2]==R2_temp_FX[2]) && (R3_temp_FX[2]==R4_temp_FX[2]))
                     branch[2] = 1;
               
               }
            }    
            
            //-----------compare component w---------------------------------------
            if(IF.SCC_w==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[3]==R2_temp_FL[3]) && (R3_temp_FL[3]==R4_temp_FL[3]))
                     branch[3] = 1;
               }
               else
               {
                  if((R1_temp_FX[3]==R2_temp_FX[3]) && (R3_temp_FX[3]==R4_temp_FX[3]))
                     branch[3] = 1;
               }
            }      
            //-----------determein branch or not---------------------------------------
            if(branch[0]==IF.SCC_x && branch[1]==IF.SCC_y && branch[2]==IF.SCC_z && branch[3]==IF.SCC_w)
               PC = PC+IF.Addr;
            
            //----------print for debug------------------------------------------------------------------------
            if(IF.FXL==1)
            {
               fprintf(fp_Result,"R1_GPR[%d]:%f_%f_%f_%f\n", IF.R1, R1_temp_FL[0], R1_temp_FL[1], R1_temp_FL[2], R1_temp_FL[3]);
               fprintf(fp_Result,"R2_GPR[%d]:%f_%f_%f_%f\n", IF.R2, R2_temp_FL[0], R2_temp_FL[1], R2_temp_FL[2], R2_temp_FL[3]);
               fprintf(fp_Result,"R3_GPR[%d]:%f_%f_%f_%f\n", IF.R3, R3_temp_FL[0], R3_temp_FL[1], R3_temp_FL[2], R3_temp_FL[3]);
               fprintf(fp_Result,"R4_GPR[%d]:%f_%f_%f_%f\n", IF.R4, R4_temp_FL[0], R4_temp_FL[1], R4_temp_FL[2], R4_temp_FL[3]);
            }
            else
            {
               fprintf(fp_Result,"R1_GPR[%d]:%d_%d_%d_%d\n", IF.R1, R1_temp_FX[0], R1_temp_FX[1], R1_temp_FX[2], R1_temp_FX[3]);
               fprintf(fp_Result,"R2_GPR[%d]:%d_%d_%d_%d\n", IF.R2, R2_temp_FX[0], R2_temp_FX[1], R2_temp_FX[2], R2_temp_FX[3]);
               fprintf(fp_Result,"R3_GPR[%d]:%d_%d_%d_%d\n", IF.R3, R3_temp_FX[0], R3_temp_FX[1], R3_temp_FX[2], R3_temp_FX[3]);
               fprintf(fp_Result,"R4_GPR[%d]:%d_%d_%d_%d\n", IF.R4, R4_temp_FX[0], R4_temp_FX[1], R4_temp_FX[2], R4_temp_FX[3]);
            }
            printf("-----------------------------------------------------------------\n");
            //------------ out -----------------
            
            if(branch[0]==IF.SCC_x && branch[1]==IF.SCC_y && branch[2]==IF.SCC_z && branch[3]==IF.SCC_w)
            {
               fprintf(fp_Result,"branch:%d%d%d%d = SCC:%d%d%d%d\n", branch[0], branch[1], branch[2], branch[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
               fprintf(fp_Result,"New PC = %d\n", PC);
            }     
            else
            {
               fprintf(fp_Result,"branch:%d%d%d%d != SCC:%d%d%d%d\n", branch[0], branch[1], branch[2], branch[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
               fprintf(fp_Result,"branch not taken.\n");
            }           
            break;
       // BNEQ
       case 25:
            //
            //------------determine the source----------------------------------
            for(i=0; i<=IF.S_V; i++)
            {
               if(IF.FXL==1)
               {
                  R1_temp_FL[i] = (IF.R1>=32) ? VAB[IF.R1-32][i] : GPR[IF.R1][i] ;
                  R2_temp_FL[i] = (IF.R2>=32) ? VAB[IF.R2-32][i] : GPR[IF.R2][i] ;
                  R3_temp_FL[i] = (IF.R3>=32) ? VAB[IF.R3-32][i] : GPR[IF.R3][i] ;
                  R4_temp_FL[i] = GPR[IF.R4][i] ;//---R4 only support read from GPR
               }
               else
               {
                  R1_temp_FX[i] = (IF.R1>=32) ? int(VAB[IF.R1-32][i]) : int(GPR[IF.R1][i]) ;
                  R2_temp_FX[i] = (IF.R2>=32) ? int(VAB[IF.R2-32][i]) : int(GPR[IF.R2][i]) ;
                  R3_temp_FX[i] = (IF.R3>=32) ? int(VAB[IF.R3-32][i]) : int(GPR[IF.R3][i]) ;
                  R4_temp_FX[i] = int(GPR[IF.R4][i]) ;//---R4 only support read from GPR
               }
            }        
            //-----------compare component x---------------------------------------
            if(IF.SCC_x==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[0]!=R2_temp_FL[0]) && (R3_temp_FL[0]!=R4_temp_FL[0]))
                     branch[0] = 1;
               }
               else
               {
                  if((R1_temp_FX[0]!=R2_temp_FX[0]) && (R3_temp_FX[0]!=R4_temp_FX[0]))
                     branch[0] = 1;
               }
            }    
            
            //-----------compare component y---------------------------------------
            if(IF.SCC_y==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[1]!=R2_temp_FL[1]) && (R3_temp_FL[1]!=R4_temp_FL[1]))
                     branch[1] = 1;
               }
               else
               {
                  if((R1_temp_FX[1]!=R2_temp_FX[1]) && (R3_temp_FX[1]!=R4_temp_FX[1]))
                     branch[1] = 1;
               }
            }    
            
            //-----------compare component z---------------------------------------
            if(IF.SCC_z==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[2]!=R2_temp_FL[2]) && (R3_temp_FL[2]!=R4_temp_FL[2]))
                     branch[2] = 1;
               }
               else
               {
                  if((R1_temp_FX[2]!=R2_temp_FX[2]) && (R3_temp_FX[2]!=R4_temp_FX[2]))
                     branch[2] = 1;
               
               }
            }    
            
            //-----------compare component w---------------------------------------
            if(IF.SCC_w==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[3]!=R2_temp_FL[3]) && (R3_temp_FL[3]!=R4_temp_FL[3]))
                     branch[3] = 1;
               }
               else
               {
                  if((R1_temp_FX[3]!=R2_temp_FX[3]) && (R3_temp_FX[3]!=R4_temp_FX[3]))
                     branch[3] = 1;
               }
            }
            //-----------determein branch or not---------------------------------------
            if(branch[0]==IF.SCC_x && branch[1]==IF.SCC_y && branch[2]==IF.SCC_z && branch[3]==IF.SCC_w)
               PC = PC+IF.Addr;
            
            //----------print for debug------------------------------------------------------------------------
            if(IF.FXL==1)
            {
               fprintf(fp_Result,"R1_GPR[%d]:%f_%f_%f_%f\n", IF.R1, R1_temp_FL[0], R1_temp_FL[1], R1_temp_FL[2], R1_temp_FL[3]);
               fprintf(fp_Result,"R2_GPR[%d]:%f_%f_%f_%f\n", IF.R2, R2_temp_FL[0], R2_temp_FL[1], R2_temp_FL[2], R2_temp_FL[3]);
               fprintf(fp_Result,"R3_GPR[%d]:%f_%f_%f_%f\n", IF.R3, R3_temp_FL[0], R3_temp_FL[1], R3_temp_FL[2], R3_temp_FL[3]);
               fprintf(fp_Result,"R4_GPR[%d]:%f_%f_%f_%f\n", IF.R4, R4_temp_FL[0], R4_temp_FL[1], R4_temp_FL[2], R4_temp_FL[3]);
            }
            else
            {
               fprintf(fp_Result,"R1_GPR[%d]:%d_%d_%d_%d\n", IF.R1, R1_temp_FX[0], R1_temp_FX[1], R1_temp_FX[2], R1_temp_FX[3]);
               fprintf(fp_Result,"R2_GPR[%d]:%d_%d_%d_%d\n", IF.R2, R2_temp_FX[0], R2_temp_FX[1], R2_temp_FX[2], R2_temp_FX[3]);
               fprintf(fp_Result,"R3_GPR[%d]:%d_%d_%d_%d\n", IF.R3, R3_temp_FX[0], R3_temp_FX[1], R3_temp_FX[2], R3_temp_FX[3]);
               fprintf(fp_Result,"R4_GPR[%d]:%d_%d_%d_%d\n", IF.R4, R4_temp_FX[0], R4_temp_FX[1], R4_temp_FX[2], R4_temp_FX[3]);
            }
            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            //------------ out -----------------
            if(branch[0]==IF.SCC_x && branch[1]==IF.SCC_y && branch[2]==IF.SCC_z && branch[3]==IF.SCC_w)
            {
               fprintf(fp_Result,"branch:%d%d%d%d = SCC:%d%d%d%d\n", branch[0], branch[1], branch[2], branch[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
               fprintf(fp_Result,"New PC = %d\n", PC);
            }     
            else
            {
               fprintf(fp_Result,"branch:%d%d%d%d != SCC:%d%d%d%d\n", branch[0], branch[1], branch[2], branch[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
               fprintf(fp_Result,"branch not taken.\n");
            }
                       
            break;
       // BLT
       case 26:
            //
            //------------determine the source----------------------------------
            for(i=0; i<=IF.S_V; i++)
            {
               if(IF.FXL==1)
               {
                  R1_temp_FL[i] = (IF.R1>=32) ? VAB[IF.R1-32][i] : GPR[IF.R1][i] ;
                  R2_temp_FL[i] = (IF.R2>=32) ? VAB[IF.R2-32][i] : GPR[IF.R2][i] ;
                  R3_temp_FL[i] = (IF.R3>=32) ? VAB[IF.R3-32][i] : GPR[IF.R3][i] ;
                  R4_temp_FL[i] = GPR[IF.R4][i] ;//---R4 only support read from GPR
               }
               else
               {
                  R1_temp_FX[i] = (IF.R1>=32) ? int(VAB[IF.R1-32][i]) : int(GPR[IF.R1][i]) ;
                  R2_temp_FX[i] = (IF.R2>=32) ? int(VAB[IF.R2-32][i]) : int(GPR[IF.R2][i]) ;
                  R3_temp_FX[i] = (IF.R3>=32) ? int(VAB[IF.R3-32][i]) : int(GPR[IF.R3][i]) ;
                  R4_temp_FX[i] = int(GPR[IF.R4][i]) ;//---R4 only support read from GPR
               }
            }        
            
            //-----------compare component x---------------------------------------
            if(IF.SCC_x==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[0]<R2_temp_FL[0]) && (R3_temp_FL[0]<R4_temp_FL[0]))
                     branch[0] = 1;
               }
               else
               {
                  if((R1_temp_FX[0]<R2_temp_FX[0]) && (R3_temp_FX[0]<R4_temp_FX[0]))
                     branch[0] = 1;
               }
            }    
            
            //-----------compare component y---------------------------------------
            if(IF.SCC_y==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[1]<R2_temp_FL[1]) && (R3_temp_FL[1]<R4_temp_FL[1]))
                     branch[1] = 1;
               }
               else
               {
                  if((R1_temp_FX[1]<R2_temp_FX[1]) && (R3_temp_FX[1]<R4_temp_FX[1]))
                     branch[1] = 1;
               }
            }    
            
            //-----------compare component z---------------------------------------
            if(IF.SCC_z==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[2]<R2_temp_FL[2]) && (R3_temp_FL[2]<R4_temp_FL[2]))
                     branch[2] = 1;
               }
               else
               {
                  if((R1_temp_FX[2]<R2_temp_FX[2]) && (R3_temp_FX[2]<R4_temp_FX[2]))
                     branch[2] = 1;
               
               }
            }    
            
            //-----------compare component w---------------------------------------
            if(IF.SCC_w==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[3]<R2_temp_FL[3]) && (R3_temp_FL[3]<R4_temp_FL[3]))
                     branch[3] = 1;
               }
               else
               {
                  if((R1_temp_FX[3]<R2_temp_FX[3]) && (R3_temp_FX[3]<R4_temp_FX[3]))
                     branch[3] = 1;
               }
            }  
            //-----------determein branch or not---------------------------------------
            if(branch[0]==IF.SCC_x && branch[1]==IF.SCC_y && branch[2]==IF.SCC_z && branch[3]==IF.SCC_w)
               PC = PC+IF.Addr;
            
            //----------print for debug------------------------------------------------------------------------
            if(IF.FXL==1)
            {
               fprintf(fp_Result,"R1_GPR[%d]:%f_%f_%f_%f\n", IF.R1, R1_temp_FL[0], R1_temp_FL[1], R1_temp_FL[2], R1_temp_FL[3]);
               fprintf(fp_Result,"R2_GPR[%d]:%f_%f_%f_%f\n", IF.R2, R2_temp_FL[0], R2_temp_FL[1], R2_temp_FL[2], R2_temp_FL[3]);
               fprintf(fp_Result,"R3_GPR[%d]:%f_%f_%f_%f\n", IF.R3, R3_temp_FL[0], R3_temp_FL[1], R3_temp_FL[2], R3_temp_FL[3]);
               fprintf(fp_Result,"R4_GPR[%d]:%f_%f_%f_%f\n", IF.R4, R4_temp_FL[0], R4_temp_FL[1], R4_temp_FL[2], R4_temp_FL[3]);
            }
            else
            {
               fprintf(fp_Result,"R1_GPR[%d]:%d_%d_%d_%d\n", IF.R1, R1_temp_FX[0], R1_temp_FX[1], R1_temp_FX[2], R1_temp_FX[3]);
               fprintf(fp_Result,"R2_GPR[%d]:%d_%d_%d_%d\n", IF.R2, R2_temp_FX[0], R2_temp_FX[1], R2_temp_FX[2], R2_temp_FX[3]);
               fprintf(fp_Result,"R3_GPR[%d]:%d_%d_%d_%d\n", IF.R3, R3_temp_FX[0], R3_temp_FX[1], R3_temp_FX[2], R3_temp_FX[3]);
               fprintf(fp_Result,"R4_GPR[%d]:%d_%d_%d_%d\n", IF.R4, R4_temp_FX[0], R4_temp_FX[1], R4_temp_FX[2], R4_temp_FX[3]);
            }
            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            //------------ out -----------------
            
            if(branch[0]==IF.SCC_x && branch[1]==IF.SCC_y && branch[2]==IF.SCC_z && branch[3]==IF.SCC_w)
            {
               fprintf(fp_Result,"branch:%d%d%d%d = SCC:%d%d%d%d\n", branch[0], branch[1], branch[2], branch[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
               fprintf(fp_Result,"New PC = %d\n", PC);
            }     
            else
            {
               fprintf(fp_Result,"branch:%d%d%d%d != SCC:%d%d%d%d\n", branch[0], branch[1], branch[2], branch[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
               fprintf(fp_Result,"branch not taken.\n");
            }           
            break;
       // BGE
       case 27:
            //
            //------------determine how many components are used ----------------------------------
            for(i=0; i<=IF.S_V; i++)
            {
               if(IF.FXL==1)
               {
                  R1_temp_FL[i] = (IF.R1>=32) ? VAB[IF.R1-32][i] : GPR[IF.R1][i] ;
                  R2_temp_FL[i] = (IF.R2>=32) ? VAB[IF.R2-32][i] : GPR[IF.R2][i] ;
                  R3_temp_FL[i] = (IF.R3>=32) ? VAB[IF.R3-32][i] : GPR[IF.R3][i] ;
                  R4_temp_FL[i] = GPR[IF.R4][i] ;//---R4 only support read from GPR
               }
               else
               {
                  R1_temp_FX[i] = (IF.R1>=32) ? int(VAB[IF.R1-32][i]) : int(GPR[IF.R1][i]) ;
                  R2_temp_FX[i] = (IF.R2>=32) ? int(VAB[IF.R2-32][i]) : int(GPR[IF.R2][i]) ;
                  R3_temp_FX[i] = (IF.R3>=32) ? int(VAB[IF.R3-32][i]) : int(GPR[IF.R3][i]) ;
                  R4_temp_FX[i] = int(GPR[IF.R4][i]) ;//---R4 only support read from GPR
               }
            }
            //------------determine which components are going to compare----------------------------------   
 
            
            //-----------compare component x---------------------------------------
            if(IF.SCC_x==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[0]>R2_temp_FL[0]) && (R3_temp_FL[0]>R4_temp_FL[0]))
                     branch[0] = 1;
               }
               else
               {
                  if((R1_temp_FX[0]>R2_temp_FX[0]) && (R3_temp_FX[0]>R4_temp_FX[0]))
                     branch[0] = 1;
               }
            }    
            
            //-----------compare component y---------------------------------------
            if(IF.SCC_y==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[1]>R2_temp_FL[1]) && (R3_temp_FL[1]>R4_temp_FL[1]))
                     branch[1] = 1;
               }
               else
               {
                  if((R1_temp_FX[1]>R2_temp_FX[1]) && (R3_temp_FX[1]>R4_temp_FX[1]))
                     branch[1] = 1;
               }
            }    
            
            //-----------compare component z---------------------------------------
            if(IF.SCC_z==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[2]>R2_temp_FL[2]) && (R3_temp_FL[2]>R4_temp_FL[2]))
                     branch[2] = 1;
               }
               else
               {
                  if((R1_temp_FX[2]>R2_temp_FX[2]) && (R3_temp_FX[2]>R4_temp_FX[2]))
                     branch[2] = 1;
               
               }
            }    
            
            //-----------compare component w---------------------------------------
            if(IF.SCC_w==1)
            {
               if(IF.FXL==1)
               {
                  if((R1_temp_FL[3]>R2_temp_FL[3]) && (R3_temp_FL[3]>R4_temp_FL[3]))
                     branch[3] = 1;
               }
               else
               {
                  if((R1_temp_FX[3]>R2_temp_FX[3]) && (R3_temp_FX[3]>R4_temp_FX[3]))
                     branch[3] = 1;
               }
            }  
            //-----------determein branch or not---------------------------------------
            if(branch[0]==IF.SCC_x && branch[1]==IF.SCC_y && branch[2]==IF.SCC_z && branch[3]==IF.SCC_w)
               PC = PC+IF.Addr;
            
            //----------print for debug------------------------------------------------------------------------
            if(IF.FXL==1)
            {
               fprintf(fp_Result,"R1_GPR[%d]:%f_%f_%f_%f\n", IF.R1, R1_temp_FL[0], R1_temp_FL[1], R1_temp_FL[2], R1_temp_FL[3]);
               fprintf(fp_Result,"R2_GPR[%d]:%f_%f_%f_%f\n", IF.R2, R2_temp_FL[0], R2_temp_FL[1], R2_temp_FL[2], R2_temp_FL[3]);
               fprintf(fp_Result,"R3_GPR[%d]:%f_%f_%f_%f\n", IF.R3, R3_temp_FL[0], R3_temp_FL[1], R3_temp_FL[2], R3_temp_FL[3]);
               fprintf(fp_Result,"R4_GPR[%d]:%f_%f_%f_%f\n", IF.R4, R4_temp_FL[0], R4_temp_FL[1], R4_temp_FL[2], R4_temp_FL[3]);
            }
            else
            {
               fprintf(fp_Result,"R1_GPR[%d]:%d_%d_%d_%d\n", IF.R1, R1_temp_FX[0], R1_temp_FX[1], R1_temp_FX[2], R1_temp_FX[3]);
               fprintf(fp_Result,"R2_GPR[%d]:%d_%d_%d_%d\n", IF.R2, R2_temp_FX[0], R2_temp_FX[1], R2_temp_FX[2], R2_temp_FX[3]);
               fprintf(fp_Result,"R3_GPR[%d]:%d_%d_%d_%d\n", IF.R3, R3_temp_FX[0], R3_temp_FX[1], R3_temp_FX[2], R3_temp_FX[3]);
               fprintf(fp_Result,"R4_GPR[%d]:%d_%d_%d_%d\n", IF.R4, R4_temp_FX[0], R4_temp_FX[1], R4_temp_FX[2], R4_temp_FX[3]);
            }
            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            //------------ out -----------------
            
            if(branch[0]==IF.SCC_x && branch[1]==IF.SCC_y && branch[2]==IF.SCC_z && branch[3]==IF.SCC_w)
            {
               fprintf(fp_Result,"branch:%d%d%d%d = SCC:%d%d%d%d\n", branch[0], branch[1], branch[2], branch[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
               fprintf(fp_Result,"New PC = %d\n", PC);
            }     
            else
            {
               fprintf(fp_Result,"branch:%d%d%d%d != SCC:%d%d%d%d\n", branch[0], branch[1], branch[2], branch[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
               fprintf(fp_Result,"branch not taken.\n");
            }           
            break;
       // MULL
       case 28:
            //---------------determine Rs swizzle--------------------------------
            switch(IF.Sw_s_x){
                    
                case 0:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break;
                }    
            switch(IF.Sw_s_y){
                    
                case 0:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break;
                } 
            switch(IF.Sw_s_z){
                    
                case 0:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break; 
                }   
            switch(IF.Sw_s_w){
                    
                case 0:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break; 
                } 
            //---------------determine Rt swizzle--------------------------------
            switch(IF.Sw_t_x){
                    
                case 0:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break;
                }    
            switch(IF.Sw_t_y){
                    
                case 0:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[1]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break;
                } 
            switch(IF.Sw_t_z){
                    
                case 0:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[2]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break;
                }   
            switch(IF.Sw_t_w){
                    
                case 0:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][1]) : (GPR[IF.Rt][1]) ;
                     break; 
                case 2:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][2]) : (GPR[IF.Rt][2]) ;
                     break; 
                case 3:
                     (Rt_swiz_temp[3]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][3]) : (GPR[IF.Rt][3]) ;
                     break; 
                }
            //----------determine Neg of Rs-------------------------------------
            if(IF.Neg_Rs==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  (Rs_swiz_temp[i]) = -(Rs_swiz_temp[i]);
            }
            //----------determine Neg of Rt-------------------------------------
            if(IF.Neg_Rt==1)
            {
               for(i=0; i<=IF.S_V; i++)
                  (Rt_swiz_temp[i]) = -(Rt_swiz_temp[i]);
            }
            //--------------do MULL-----------------------------------------------------
            for(i=0; i<=IF.S_V; i++)
               mull_tmp64b[i] = int(Rs_swiz_temp[i]) * int(Rt_swiz_temp[i]);
            for(i=0; i<=IF.S_V; i++)
               mull_temp[i] = (mull_tmp64b[i]);
            //-------- WB -------------------------------------------------------------
            if(IF.FXL!=0)
              fprintf(fp_Result,"Instruction ERROR!! MULL is fixed, FXL must be 0.");
            else
            {
               if(IF.WM_x==1)
                  (GPR[IF.Rd][0]) = mull_temp[0];
              
               if(IF.WM_y==1)
                  (GPR[IF.Rd][1]) = mull_temp[1];
               
               if(IF.WM_z==1)
                  (GPR[IF.Rd][2]) = mull_temp[2];
               
               if(IF.WM_w==1)
                  (GPR[IF.Rd][3]) = mull_temp[3];
            }
            //----------print for debug------------------------------------------------------------------------
            
            fprintf(fp_Result,"Rs_GPR[%d]:%d_%d_%d_%d\n", IF.Rs, (Rs_swiz_temp[0]), (Rs_swiz_temp[1]), (Rs_swiz_temp[2]), (Rs_swiz_temp[3]));
            fprintf(fp_Result,"Rt_GPR[%d]:%d_%d_%d_%d\n", IF.Rt, (Rt_swiz_temp[0]), (Rt_swiz_temp[1]), (Rt_swiz_temp[2]), (Rt_swiz_temp[3]));
            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            //------------ out -----------------
            fprintf(fp_Result,"GPR[%d] = %d_%d_%d_%d\n", IF.Rd, (GPR[IF.Rd][0]), (GPR[IF.Rd][1]), (GPR[IF.Rd][2]), (GPR[IF.Rd][3]));
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
            get_fix(int(GPR[IF.Rd][0]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][1]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][2]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][3]));
            fprintf(fp_Result,"\n");
            break;
       // CAL
       case 29:
            if(IF.R1!=0)
               (GPR[IF.R1][0]) = PC;
      
            PC = int(GPR[IF.R2][0]) + IF.Addr;

            
            //----------print for debug------------------------------------------------------------------------
            if(IF.R1!=0)
               fprintf(fp_Result,"R1_GPR[%d].x:%d\n", IF.R1, int(GPR[IF.R1][0]));
            else
               fprintf(fp_Result,"GPR0 cant be written!!\n");
               
            if(PC==0)
               fprintf(fp_Result,"PC returns to 0\n");
            else
               fprintf(fp_Result,"PC will jump to:%d\n", PC);

            break;
       // SHRL
       case 30:
            //----- determine Source and swizzle for Rs -------------------------------------
            switch(IF.Sw_s_x){
                    
                case 0:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[0]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break;
                }    
            switch(IF.Sw_s_y){
                    
                case 0:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[1]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break;
                } 
            switch(IF.Sw_s_z){
                    
                case 0:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[2]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break; 
                }   
            switch(IF.Sw_s_w){
                    
                case 0:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][0]) : (GPR[IF.Rs][0]) ;//determine the source....from GPR or VAB  
                     break;         
                case 1:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][1]) : (GPR[IF.Rs][1]) ;
                     break; 
                case 2:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][2]) : (GPR[IF.Rs][2]) ;
                     break; 
                case 3:
                     (Rs_swiz_temp[3]) = (IF.Rs>=32) ? (VAB[IF.Rs-32][3]) : (GPR[IF.Rs][3]) ;
                     break; 
                }
            //---------- determine source of Rt --------------------------------
            (Rt_swiz_temp[0]) = (IF.Rt>=32) ? (VAB[IF.Rt-32][0]) : (GPR[IF.Rt][0]) ;
            if((Rt_swiz_temp[0]) < 0)//make sure Rt.x is positive
               (Rt_swiz_temp[0]) = -(Rt_swiz_temp[0]);
            
            //--------- do SHRL -----------------------------------------------
            for(i=0; i<=IF.S_V; i++)
            {
               if(IF.R_L==1)
                  shrl_temp[i] = int(Rs_swiz_temp[i]) << int(Rt_swiz_temp[0]);
               else
                  shrl_temp[i] = int(Rs_swiz_temp[i]) >> int(Rt_swiz_temp[0]);
            }
            
            //------------WB---------------------------------------------------
            
            if(IF.FXL!=0)
              fprintf(fp_Result,"Instruction ERROR!! SHRL only supports fixed, FXL must be 0.");
            else
            {
               if(IF.WM_x==1)
                  (GPR[IF.Rd][0]) = shrl_temp[0];
              
               if(IF.WM_y==1)
                  (GPR[IF.Rd][1]) = shrl_temp[1];
               
               if(IF.WM_z==1)
                  (GPR[IF.Rd][2]) = shrl_temp[2];
               
               if(IF.WM_w==1)
                  (GPR[IF.Rd][3]) = shrl_temp[3];
            }
            //----------print for debug------------------------------------------------------------------------
            
            fprintf(fp_Result,"Rs_GPR[%d]:%d_%d_%d_%d\n", IF.Rs, int(Rs_swiz_temp[0]), int(Rs_swiz_temp[1]), int(Rs_swiz_temp[2]), int(Rs_swiz_temp[3]));
            fprintf(fp_Result,"Rt_GPR[%d].x:%d\n", IF.Rt, int(Rt_swiz_temp[0]));
            fprintf(fp_Result,"-----------------------------------------------------------------\n");
            //------------ out -----------------
            fprintf(fp_Result,"GPR[%d] = %d_%d_%d_%d\n", IF.Rd, int(GPR[IF.Rd][0]), int(GPR[IF.Rd][1]), int(GPR[IF.Rd][2]), int(GPR[IF.Rd][3]));
            //--------get hex floating point result------------------------------------------------------------
            fprintf(fp_Result,"Rd_GPR[%d] = ", IF.Rd);
            get_fix(int(GPR[IF.Rd][0]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][1]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][2]));
            fprintf(fp_Result,"_");
            get_fix(int(GPR[IF.Rd][3]));
            fprintf(fp_Result,"\n");
            break;            
       // Nop
       case 31:
            //
            
            fprintf(fp_Result,"For RAW situation\n");  
            fprintf(fp_Result,"==============================================\n"); 
    }
     
}


int main(){
 
 GPR[0][0] = 0;
 GPR[0][1] = 0;
 GPR[0][2] = 0;
 GPR[0][3] = 0;  
 
 
  
   int vertex_count=0;
//-----------------initialize GPR[0]--------------------------------------------
	int m;
	//GPR[0]=0 , special location 
	for(m = 0 ; m<=3 ; m++)
       GPR[0][m]=0;
    
//------------------------------------------------------------------------------       

	
    //----- Open File ---------------------------------- 
    //fp_Instruction = fopen("morphing_ball/the ball/ball_instruction.txt" , "r");
    //fp_Instruction = fopen("Lighting.txt" , "r");
    //fp_Instruction = fopen("Transformation.txt" , "r");
    fp_Instruction = fopen("test_ins.txt" , "r");
    
    //fp_Uniform          = fopen("morphing_ball/the ball/ball_uniform.txt" , "r");
    //fp_Uniform         = fopen("Uniform.txt" , "r");
    fp_Uniform         = fopen("test_constant.txt" , "r");
    
    fp_VST         = fopen("VST.txt" , "r");
    fp_Vertex      = fopen("Vertex_Data.txt" , "r");
    //fp_Vertex      = fopen("morphing_ball/the ball/ball_vertex.txt" , "r");
	fp_Result      = fopen("Vertex_Out.txt" , "w");
    V_O          = fopen("V_O.txt","w");
    //----------- read Uniform data ------------------------------------------------
    
    int CMEM_entry=0;
    int CMEM_component = 0;
    while ( !feof(fp_Uniform) )
    { 
               
        fscanf (fp_Uniform ,"%f\n", &CMEM[CMEM_entry][CMEM_component]);
        //printf("CMEM[%d][%d] = %f\n", CMEM_entry, CMEM_component, CMEM[CMEM_entry][CMEM_component]);    
        if(CMEM_component<3)
           CMEM_component++;
        else
        {
           CMEM_component = 0;
           CMEM_entry++;
        }
        //system ("pause") ; 
    }
    
    //----------- read VST data ------------------------------------------------
    
    int vst_count=0;
    while ( !feof(fp_VST) )
    { 
        //system ("pause") ;        
        fscanf (fp_VST ,"%f\n", &VST[vst_count]);
        //printf("CMEM[%d] = %.16f\n", ct_count, CMEM[ct_count]);        
        vst_count++;
    }
    //----------- read vertex data --------------------------------------------
    

    for(v_num=0; v_num < total_v; v_num++)
        {
           //printf("Vertex %d\n", v_num+1);
           for(v_entry_num=0; v_entry_num < total_a; v_entry_num++)
           {
              for(v_word_num=0; v_word_num<=3; v_word_num++)
              {
                 fscanf(fp_Vertex ,"%f\n", &Ex_MEM[v_num][v_entry_num][v_word_num]);        
                 //printf("Ex_MEM[%d][%d][%d] = %f\n", v_num, v_entry_num, v_word_num, Ex_MEM[v_num][v_entry_num][v_word_num]);
              }    
           }
           //system ("pause") ;    
    }    


       
    //----------- read instruction --------------------------------------------
    int im_count=0;   
    while ( !feof(fp_Instruction) )
    { 
        
        fscanf (fp_Instruction ,"%s\n", &IM[im_count]);        
        im_count++;
    }
    
    //--------- VS start -------------------------------------------------------
    
    
    for(v_num=0; v_num < total_v; v_num++)
    {
       //vertex fetch---------------------
       for(v_entry_num=0; v_entry_num < total_a; v_entry_num++)
       {
          for(v_word_num=0; v_word_num<=3; v_word_num++)
             VAB[v_entry_num][v_word_num] = Ex_MEM[v_num][v_entry_num][v_word_num];
       }    
       //---------------------------------    
       PC=0;
       while(PC<im_count && PC!=1024)
       {
          //system ("pause") ;
          fprintf(fp_Result,"\nVertex %d\n", v_num+1);
          //-----------initial register---
          for(i=0 ; i<4 ; i++)
          {
             Rs_swiz_temp[i] = 0;
             Rt_swiz_temp[i] = 0;
             Rd_swiz_temp[i] = 0;
             branch[i] = 0;
             
          }     
          
          //------------------------------
          decode(PC);
          if(IF.op!=29)
          {
             if(IF.op<24 || IF.op>27)
                PC++;
             else
             {
                if(branch[0]!=IF.SCC_x || branch[1]!=IF.SCC_y || branch[2]!=IF.SCC_z || branch[3]!=IF.SCC_w)
                   PC++;
             }
          }
          else
          {
             if(PC==0)
                PC = 1024;
          } 
       }
       
        //-------------------------------------clip c model start---------------------------------------
          if(VST[0])
          {
          
          clip clip_model;//clip initail
          clip_model.x_max=VST[1];
          clip_model.x_min=VST[2];
          clip_model.y_max=VST[3];
          clip_model.y_min=VST[4];
          
          //printf("clip_model.x_max=%f\n",clip_model.x_max);
          //printf("clip_model.y_max=%f\n",clip_model.y_max);
          //printf("clip_model.x_min=%f\n",clip_model.x_min);
          //printf("clip_model.y_min=%f\n",clip_model.y_min);
          //vertex input
          for(i=0;i<4;i++)
           {
              clip_model.Vertex[v_num%3].entry1[i]=VOB[0][i];
              clip_model.Vertex[v_num%3].entry2[i]=VOB[1][i];
              clip_model.Vertex[v_num%3].entry3[i]=VOB[2][i];
              clip_model.Vertex[v_num%3].entry4[i]=VOB[3][i];
           }
           
           //printf("vertex1: x=%f y=%f z=%f \n",clip_model.Vertex[0].entry1[0],clip_model.Vertex[0].entry1[1],clip_model.Vertex[0].entry1[2]);
           //printf("vertex2: x=%f y=%f z=%f \n",clip_model.Vertex[1].entry1[0],clip_model.Vertex[1].entry1[1],clip_model.Vertex[1].entry1[2]);
           //printf("vertex3: x=%f y=%f z=%f \n",clip_model.Vertex[2].entry1[0],clip_model.Vertex[2].entry1[1],clip_model.Vertex[2].entry1[2]);
           
       if(v_num%3==2)// triangle in
       {
         clip_model.clip_module();
       
       printf("\nnumber=%d\n",clip_model.vertex_number_out);
       for(i=0;i<clip_model.vertex_number_out;i++)
       {
           //printf("x=%f,y=%f,z=%f\n",clip_model.vertex_reg_out[i].entry1[0],clip_model.vertex_reg_out[i].entry1[1],clip_model.vertex_reg_out[i].entry1[2]);
           fprintf(V_O,"\nvertex %d out\n",++vertex_count);
           for(j=0;j<4;j++)
           {
              VOB[0][j]=clip_model.vertex_reg_out[i].entry1[j];
              VOB[1][j]=clip_model.vertex_reg_out[i].entry2[j];
              VOB[2][j]=clip_model.vertex_reg_out[i].entry3[j];
              VOB[3][j]=clip_model.vertex_reg_out[i].entry4[j];
           }
           
           fprintf(V_O,"entry1 = ");
           
           for(j=0;j<4;j++)
           {
              fprintf(V_O,"%f\t",VOB[0][j]);
           }
           fprintf(V_O,"\nentry2 = ");
           
           for(j=0;j<4;j++)
           {
              fprintf(V_O,"%f\t",VOB[1][j]);
           }
           fprintf(V_O,"\nentry3 = ");
           
           for(j=0;j<4;j++)
           {
              fprintf(V_O,"%f\t",VOB[2][j]);
           }
           fprintf(V_O,"\nentry4 = ");
           
           for(j=0;j<4;j++)
           {
              fprintf(V_O,"%f\t",VOB[3][j]);
           }
       }
       //system("pause");
       }
       
       }
       else
       {
            fprintf(V_O,"\nvertex %d out\n",++vertex_count);
            
            fprintf(V_O,"entry1 = ");
           
           for(j=0;j<4;j++)
           {
              fprintf(V_O,"%f\t",VOB[0][j]);
           }
           fprintf(V_O,"\nentry2 = ");
           
           for(j=0;j<4;j++)
           {
              fprintf(V_O,"%f\t",VOB[1][j]);
           }
           fprintf(V_O,"\nentry3 = ");
           
           for(j=0;j<4;j++)
           {
              fprintf(V_O,"%f\t",VOB[2][j]);
           }
           fprintf(V_O,"\nentry4 = ");
           
           for(j=0;j<4;j++)
           {
              fprintf(V_O,"%f\t",VOB[3][j]);
           }
            
           
       }
    }    

    
    
   fclose( fp_Instruction );
   fclose( fp_Vertex );
   fclose( fp_Uniform );
   fclose( fp_VST );
   fclose( fp_Result );
   fclose(V_O);
   //system("pause");


}
