//#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "link_var.h"
#include "inlineFunction.h"
#include "linker.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//#define combine_test
#define LD_BY_ONE 

#define ASSEMBLY_STRING_SIZE 8192

#define perr(X) {printf("Error: %s\n",X);exit(1);}

unsigned long long temp_b_code[64];
unsigned long long temp_b_code_NOP[128];
unsigned long long nop_nine_instruction=0xFC80000000000000LL;

//#ifndef combine_test
//int num_temp_b;
//unsigned long long all_temp_b[65536];
//#endif
//#ifdef combine_test

extern long long unsigned int BinCode[65536];
extern int LastBinLine;
extern struct variable_form r_form[256];
extern int regular_variable_address;

int num_temp_b;
unsigned long long all_temp_b[65536];
//#endif
char temp_string[4096];
char combine_ass_name[8]={'a','s','s','F','i','l','e','\0'};

#define perror(X) {printf("\"%s\" is Error",X);exit(1);}
#define unified_branch
//#define unified_shader_register
#define Opt_any
#define Opt_all
#define remove_nop //remove the nop instruction in GL_Start, GL_END

#ifdef LoadAttributeFromSCT
  extern int x;
  extern int y;
  extern int width;
  extern int height;
#endif

void MVR_instr( int r1, int r2, int r1_sw, int r2_sw, int neg, int WM, int SV ,int index){ //Fill MVR instruction.

  temp_b_code[index] = 7;
  temp_b_code[index] = temp_b_code[index] << 6;
  temp_b_code[index] |= r1;
  temp_b_code[index] = temp_b_code[index] << 6;
  temp_b_code[index] |= r2;
  temp_b_code[index] = temp_b_code[index] << 14;
  temp_b_code[index] |= r1_sw;
  temp_b_code[index] = temp_b_code[index] << 8;
  temp_b_code[index] |= r2_sw;
  temp_b_code[index] = temp_b_code[index] << 11;
  temp_b_code[index] |= neg;
  temp_b_code[index] = temp_b_code[index] << 4;
  temp_b_code[index] |= WM;
  temp_b_code[index] = temp_b_code[index] << 3;
  temp_b_code[index] |= SV;
  temp_b_code[index] = temp_b_code[index] << 7;

}

void insert_nop_nine(int insert_number);
void insert_N_nop9(int insert_number,int number_of_nop);

int link_Vec3ToVec2(int input_regi,
		    int return_regi,
		    FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"VEC3TOVEC2_R%d_R%d_ass_file",input_regi,return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tMVR R%d.1111, R0.xyzw, <4>\n",temp_string, return_regi);
  sprintf(temp_string,"%s\tMVR R%d.1100, R%d.xyzw, <2>\n",temp_string, return_regi, input_regi);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //binary
  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  
  temp_b_code[1]=temp_b_code[0];
  temp_b_code[1]&=(long long unsigned)0xFFE07FFFFFFFC27FLL;
  temp_b_code[1]|=(long long unsigned)input_regi<<47; 
  temp_b_code[1]|=(long long unsigned)12<<10; //WM: 1100
  temp_b_code[1]|=(long long unsigned)1<<7; //SV: 01 (1)

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),2,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 2*2-1;
#else
  return 2;
#endif

}
int link_Vec2ToVec2(int input_regi,
		    int return_regi,
		    FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"VEC2TOVEC2_R%d_R%d_ass_file",input_regi,return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tMVR R%d.1111, R0.xyzw, <4>\n",temp_string, return_regi);
  sprintf(temp_string,"%s\tMVR R%d.1100, R%d.xyzw, <2>\n",temp_string, return_regi, input_regi);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //binary
  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  
  temp_b_code[1]=temp_b_code[0];
  temp_b_code[1]&=(long long unsigned)0xFFE07FFFFFFFC27FLL;
  temp_b_code[1]|=(long long unsigned)input_regi<<47; 
  temp_b_code[1]|=(long long unsigned)12<<10; //WM: 1100
  temp_b_code[1]|=(long long unsigned)1<<7; //SV: 01 (1)

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),2,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 2*2-1;
#else
  return 2;
#endif

}

int link_ConVec4(int input_regi, 
		 int input_regi2, 
		 int input_regi3, 
		 int input_regi4, 
		 int return_regi,
		 char type,
		 FILE* output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"CONVEC4_R%d_R%d_R%d_R%d_R%d_ass_file", input_regi, input_regi2, input_regi3, input_regi4, return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.xyzw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi);
  sprintf(temp_string,"%s\tMVR R%d.0100, R%d.xxzw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi2);
  sprintf(temp_string,"%s\tMVR R%d.0010, R%d.xyxw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi3);
  sprintf(temp_string,"%s\tMVR R%d.0001, R%d.xyzx, <4>\n",temp_string, OUR_TEMP_REGI, input_regi4);
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string, return_regi, OUR_TEMP_REGI);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //binary
  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=OUR_TEMP_REGI;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=input_regi;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939; //xyzw xyzw
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  
  temp_b_code[1]=temp_b_code[0];
  temp_b_code[1]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
  temp_b_code[1]|=(long long unsigned)input_regi2<<47;
  temp_b_code[1]|=(long long unsigned)6923<<25; //xyzw xxzw 
  temp_b_code[1]|=(long long unsigned)4<<10; //WM: 0100

  temp_b_code[2]=temp_b_code[0];
  temp_b_code[2]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
  temp_b_code[2]|=(long long unsigned)input_regi3<<47; 
  temp_b_code[2]|=(long long unsigned)6931<<25; //xyzw xyxw 
  temp_b_code[2]|=(long long unsigned)2<<10; //WM: 0010

  temp_b_code[3]=temp_b_code[0];
  temp_b_code[3]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
  temp_b_code[3]|=(long long unsigned)input_regi4<<47; 
  temp_b_code[3]|=(long long unsigned)6936<<25; //xyzw xyzx 
  temp_b_code[3]|=(long long unsigned)1<<10; //WM: 0001

  temp_b_code[4]=temp_b_code[0];
  temp_b_code[4]&=(long long unsigned)0xF8007FFFFFFFC3FFLL;
  temp_b_code[4]|=(long long unsigned)return_regi<<53; 
  temp_b_code[4]|=(long long unsigned)OUR_TEMP_REGI<<47; 
  temp_b_code[4]|=(long long unsigned)15<<10; //WM: 1111

 
  if( type == 'v'){  
#ifdef SET_NOP_NINE
  insert_nop_nine(5);
  for(i=0;i<5*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<5;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 5*2-1;
#else
  return 5;
#endif
  }
  else{
 
#ifdef SET_NOP_NINE
    insert_N_nop9(5,2);
    for(i=0;i<5*3-2;i++)
      BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
    for(i=0;i<5;i++)
      BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
    fwrite(temp_b_code,sizeof(long long),4,output_file);  
#endif

#ifdef SET_NOP_NINE
    return 5*3-2;
#else
    return 5;
#endif
  }
}
int link_ConVec4_Vec2_F_F(int input_regi, 
		 int input_regi2, 
		 int input_regi3, 
		 int return_regi,
		 FILE* output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"CONVEC4_VEC2_F_F_R%d_R%d_R%d_R%d_ass_file", input_regi, input_regi2, input_regi3, return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tMVR R%d.1100, R%d.xyzw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi);
  sprintf(temp_string,"%s\tMVR R%d.0010, R%d.xyxw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi2);
  sprintf(temp_string,"%s\tMVR R%d.0001, R%d.xyzx, <4>\n",temp_string, OUR_TEMP_REGI, input_regi3);
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string, return_regi, OUR_TEMP_REGI);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  temp_b_code[0] = 7;                  //MVR opcode
  temp_b_code[0] = temp_b_code[0]<<6;
  temp_b_code[0] |= OUR_TEMP_REGI;     //R1 = 3 (R1 <= R3)
  temp_b_code[0] = temp_b_code[0]<<6;
  temp_b_code[0] |= input_regi;        //R2 = input_regi
  temp_b_code[0] = temp_b_code[0]<<22;
  temp_b_code[0] |= 6939;              // 0x001b1b , begining 6 bits are any bits(don't care). Next 8 bits means xyzw(0x1b1b). Next 8 bits are also. 
  temp_b_code[0] = temp_b_code[0]<<15;
  temp_b_code[0] |= 12;                //Begining 8 bits are any bits(don't cate). Next 3 bits are 000. Next 4 bits are WM bits.(1100).
  temp_b_code[0] = temp_b_code[0]<<3;
  temp_b_code[0] |= 3;                 //SV bits.
  temp_b_code[0] = temp_b_code[0]<<7;

  //MVR_instr( OUR_TEMP_REGI, input_regi, 0x1b, 0x1b, 0, 12, 3, 0);


  temp_b_code[1] = 7;
  temp_b_code[1] = temp_b_code[1]<<6;
  temp_b_code[1] |= OUR_TEMP_REGI;
  temp_b_code[1] = temp_b_code[1]<<6;
  temp_b_code[1] |= input_regi2;
  temp_b_code[1] = temp_b_code[1]<<22;
  temp_b_code[1] |= 6931;             // 0x001b13 , begining 6 bits are any bits(don't care). Next 8 bits means xyxw(0x1b13). Next 8 bits are also. 
  temp_b_code[1] = temp_b_code[1]<<15;
  temp_b_code[1] |= 2;                // WM (0010)
  temp_b_code[1] = temp_b_code[1]<<3;
  temp_b_code[1] |= 3;
  temp_b_code[1] = temp_b_code[1]<<7;

  //MVR_instr( OUR_TEMP_REGI, input_regi2, 0x1b, 0x13, 0, 2, 3, 1);

  temp_b_code[2] = 7;
  temp_b_code[2] = temp_b_code[2]<<6;
  temp_b_code[2] |= OUR_TEMP_REGI;
  temp_b_code[2] = temp_b_code[2]<<6;
  temp_b_code[2] |= input_regi3;
  temp_b_code[2] = temp_b_code[2]<<22;
  temp_b_code[2] |= 6936;             // 0x001b18 , begining 6 bits are any bits(don't care). Next 8 bits means xyzx(0x1b18). Next 8 bits are also.   
  temp_b_code[2] = temp_b_code[2]<<15;
  temp_b_code[2] |= 1;                // WM (0001)
  temp_b_code[2] = temp_b_code[2]<<3;
  temp_b_code[2] |= 3;
  temp_b_code[2] = temp_b_code[2]<<7;

  //MVR_instr( OUR_TEMP_REGI, input_regi3, 0x1b, 0x18, 0, 1, 3, 2);

  temp_b_code[3] = 7;
  temp_b_code[3] = temp_b_code[3]<<6;
  temp_b_code[3] |= return_regi;
  temp_b_code[3] = temp_b_code[3]<<6;
  temp_b_code[3] |= OUR_TEMP_REGI;
  temp_b_code[3] = temp_b_code[3]<<22;
  temp_b_code[3] |= 6939;             // 0x001b1b , begining 6 bits are any bits(don't care). Next 8 bits means xyzw(0x1b1b). Next 8 bits are also. 
  temp_b_code[3] = temp_b_code[3]<<15;
  temp_b_code[3] |= 15;
  temp_b_code[3] = temp_b_code[3]<<3;
  temp_b_code[3] |= 3;
  temp_b_code[3] = temp_b_code[3]<<7;

  //MVR_instr( return_regi, OUR_TEMP_REGI, 0x1b, 0x1b, 0, 15, 3, 3);

#ifdef SET_NOP_NINE
  insert_nop_nine(4);
  for(i=0;i<4*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<4;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 4*2-1;
#else
  return 4;
#endif
  
}

int link_ConVec4_Vec2_Vec2(int input_regi, 
			   int input_regi2, 
			   int return_regi,
			   FILE* output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"CONVEC4_VEC2_VEC2_R%d_R%d_R%d_ass_file", input_regi, input_regi2, return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tMVR R%d.1100, R%d.xyzw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi);
  sprintf(temp_string,"%s\tMVR R%d.0011, R%d.xyxy, <4>\n",temp_string, OUR_TEMP_REGI, input_regi2);
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string, return_regi, OUR_TEMP_REGI);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);


  MVR_instr( OUR_TEMP_REGI, input_regi, 0x1b, 0x1b, 0, 12, 3, 0);
  MVR_instr( OUR_TEMP_REGI, input_regi2, 0x1b, 0x11, 0, 3, 3, 1);
  MVR_instr( return_regi, OUR_TEMP_REGI, 0x1b, 0x1b, 0, 15, 3, 2);




#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 3*2-1;
#else
  return 3;
#endif
}

int link_ConVec4_Vec3_F(int input_regi, 
		 int input_regi2, 
		 int return_regi,
		 FILE* output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;
  
  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"CONVEC4_VEC3_F_R%d_R%d_R%d_ass_file", input_regi, input_regi2, return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tMVR R%d.1110, R%d.xyzw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi);
  sprintf(temp_string,"%s\tMVR R%d.0001, R%d.xyzx, <4>\n",temp_string, OUR_TEMP_REGI, input_regi2);
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string, return_regi, OUR_TEMP_REGI);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);





  MVR_instr( OUR_TEMP_REGI, input_regi, 0x1b, 0x1b, 0, 14, 3, 0);
  MVR_instr( OUR_TEMP_REGI, input_regi2, 0x1b, 0x18, 0, 1, 3, 1);
  MVR_instr( return_regi, OUR_TEMP_REGI, 0x1b, 0x1b, 0, 15, 3, 2);




#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 3*2-1;
#else
  return 3;
#endif

}
int link_ConVec3_Vec2_F(int input_regi, 
			int input_regi2, 
			int return_regi,
			FILE* output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"CONVEC3_Vec2_F_R%d_R%d_R%d_ass_file", input_regi, input_regi2,return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tMVR R%d.1100, R%d.xyzw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi);
  sprintf(temp_string,"%s\tMVR R%d.0010, R%d.xyxw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi2);
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string, return_regi, OUR_TEMP_REGI);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);


  MVR_instr( OUR_TEMP_REGI, input_regi, 0x1b, 0x1b, 0, 12, 3, 0);
  MVR_instr( OUR_TEMP_REGI, input_regi2, 0x1b, 0x13, 0, 2, 3, 1);
  MVR_instr( return_regi, OUR_TEMP_REGI, 0x1b, 0x1b, 0, 15, 3, 3);



#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 3*2-1;
#else
  return 3;
#endif
}

int link_ConVec3_F_Vec2(int input_regi, 
			int input_regi2, 
			int return_regi,
			FILE* output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"CONVEC3_F_Vec2R%d_R%d_R%d_ass_file", input_regi, input_regi2,return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.xyzw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi);
  sprintf(temp_string,"%s\tMVR R%d.0110, R%d.xxyw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi2);
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string, return_regi, OUR_TEMP_REGI);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);


  MVR_instr( OUR_TEMP_REGI, input_regi, 0x1b, 0x1b, 0, 8, 3, 0);
  MVR_instr( OUR_TEMP_REGI, input_regi2, 0x1b, 0x07, 0, 6, 3, 1);
  MVR_instr( return_regi, OUR_TEMP_REGI, 0x1b, 0x1b, 0, 15, 3, 3);



#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 3*2-1;
#else
  return 3;
#endif
}

int link_ConVec3_F_F_F(int input_regi, 
		       int input_regi2, 
		       int input_regi3, 
		       int return_regi,
		       FILE* output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"CONVEC3_F_F_F_R%d_R%d_R%d_R%d_ass_file", input_regi, input_regi2, input_regi3, return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.xyzw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi);
  sprintf(temp_string,"%s\tMVR R%d.0100, R%d.xxzw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi2);
  sprintf(temp_string,"%s\tMVR R%d.0010, R%d.xyxw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi3);
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string, return_regi, OUR_TEMP_REGI);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);


  MVR_instr( OUR_TEMP_REGI, input_regi, 0x1b, 0x1b, 0, 8, 3, 0);
  MVR_instr( OUR_TEMP_REGI, input_regi2, 0x1b, 0x0b, 0, 4, 3, 1);
  MVR_instr( OUR_TEMP_REGI, input_regi3, 0x1b, 0x13, 0, 2, 3, 2);
  MVR_instr( return_regi, OUR_TEMP_REGI, 0x1b, 0x1b, 0, 15, 3, 3);



#ifdef SET_NOP_NINE
  insert_nop_nine(4);
  for(i=0;i<4*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<4;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 4*2-1;
#else
  return 4;
#endif
}

int link_ConVec2_F_F(int input_regi, 
		     int input_regi2, 
		     int return_regi,
		     FILE* output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"CONVEC2_F_F_R%d_R%d_R%d_ass_file", input_regi, input_regi2, return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.xyzw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi);
  sprintf(temp_string,"%s\tMVR R%d.0100, R%d.xxzw, <4>\n",temp_string, OUR_TEMP_REGI, input_regi2);
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string, return_regi, OUR_TEMP_REGI);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);


  MVR_instr( OUR_TEMP_REGI, input_regi, 0x1b, 0x1b, 0, 8, 3, 0);
  MVR_instr( OUR_TEMP_REGI, input_regi2, 0x1b, 0x0b, 0, 4, 3, 1);
  MVR_instr( return_regi, OUR_TEMP_REGI, 0x1b, 0x1b, 0, 15, 3, 2);




#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 3*2-1;
#else
  return 3;
#endif
}
int link_Convert_ItoB(int return_regi,
		      int input_regi, 
		      FILE* output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"CONVERT_ItoB_R%d_R%d_ass_file", input_regi, return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.xyzw, <4>\n",temp_string, return_regi, input_regi);


  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  MVR_instr( return_regi , input_regi , 0X1b1b, 0X1b1b, 0 , 8 , 3 ,0);  //move angle to remain_regi.


#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 1*2-1;
#else
  return 1;
#endif
}

int link_Convert_ItoF(int return_regi,
		      int input_regi, 
		      FILE* output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"CONVERT_ItoF_R%d_R%d_ass_file", input_regi, return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tITOF R%d.1000, R%d.xyzw, <4>\n",temp_string, return_regi, input_regi);


  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  temp_b_code[0]=13; //opcode: ITOF
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=input_regi;
  temp_b_code[0]=temp_b_code[0]<<33;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<4;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3; //SV=4
  temp_b_code[0]=temp_b_code[0]<<7;


#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 1*2-1;
#else
  return 1;
#endif
}
int link_Convert_FtoI(int return_regi,
		      int input_regi, 
		      FILE* output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"CONVERT_FtoI_R%d_R%d_ass_file", input_regi, return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tFTOI R%d.1000, R%d.xyzw, <4>\n",temp_string, return_regi, input_regi);


  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  temp_b_code[0]=13; //opcode: FTOI
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=input_regi;
  temp_b_code[0]=temp_b_code[0]<<33;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<4;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1; //mode for FTOI or ITOF
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3; //SV=4
  temp_b_code[0]=temp_b_code[0]<<7;


#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 1*2-1;
#else
  return 1;
#endif
}


int link_VectorNegate(int input_regi,
		      int return_regi,
		      FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"VECTORNEGATE_R%d_R%d_ass_file",input_regi,return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tADDF R%d.1111, R0.xyzw, -R%d.xyzw, <4>\n",temp_string, return_regi, input_regi);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //binary
  temp_b_code[0]=16;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=input_regi;
  temp_b_code[0]=temp_b_code[0]<<24;
  temp_b_code[0]|=1776411;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<4;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  
#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 1*2-1;
#else
  return 1;
#endif

}

int link_Vec4ToVec3(int input_regi,
		    int return_regi,
		    FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"VEC4TOVEC3_R%d_R%d_ass_file",input_regi,return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tMVR R%d.1111, R0.xyzw, <4>\n",temp_string, return_regi);
  sprintf(temp_string,"%s\tMVR R%d.1110, R%d.xyzw, <3>\n",temp_string, return_regi, input_regi);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //binary
  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  
  temp_b_code[1]=temp_b_code[0];
  temp_b_code[1]&=(long long unsigned)0xFFE07FFFFFFFC27FLL;
  temp_b_code[1]|=(long long unsigned)input_regi<<47; 
  temp_b_code[1]|=(long long unsigned)14<<10; //WM: 1110
  temp_b_code[1]|=(long long unsigned)2<<7; //SV: 10 (2)

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),2,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 2*2-1;
#else
  return 2;
#endif

}

int link_FToVec3(int input_regi,
		 int return_regi,
		 FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"FTOVEC3_R%d_R%d_ass_file",input_regi,return_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"%s\tMVR R%d.1111, R0.xyzw, <4>\n",temp_string, return_regi);
  sprintf(temp_string,"%s\tMVR R%d.1110, R%d.xxxw, <3>\n",temp_string, return_regi, input_regi);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //binary
  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  
  temp_b_code[1]=temp_b_code[0];
  //temp_b_code[1]&=(long long unsigned)0xFFE07FFFFFFFC27FLL;
  temp_b_code[1]&=(long long unsigned)0xFFE07FFE07FFC27FLL;
  temp_b_code[1]|=(long long unsigned)input_regi<<47; 
  temp_b_code[1]|=(long long unsigned)14<<10; //WM: 1110
  temp_b_code[1]|=(long long unsigned)2<<7; //SV: 10 (2)

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),2,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 2*2-1;
#else
  return 2;
#endif

}

int GL_Sin(int input_angle_regi,
	   int remain_regi,
	   int remain_regi2,
	   int remain_regi3,
	   int remain_regi4,
	   int return_regi,
	   int type,//1:float, 2:vec2, 3:vec3, and 4:vec4
	   char *assembly_string,
	   FILE *output_ass)
{
  int i, g;
  char v[5]="1000";
  int instruction_count=0;
  float temp_float;
  unsigned int temp_int;
  unsigned int temp_wm=8;

  FILE *ass_file;

  if(type>1)
    {  
      temp_wm|=4;
      if(type>2)
	{
	  temp_wm|=2;
	  if(type>3)
	    {
	      temp_wm|=1;
	    }
	}
    }


  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"Sin_R%d_R%d_R%d_R%d_R%d_R%d_%d_ass_file",
	      input_angle_regi,remain_regi,remain_regi2,remain_regi3,remain_regi4,
	      return_regi,type);
      ass_file=fopen(temp_string,"w");
    }

  //make the angle map from (-lim,lim) to (90,-90)
  //assembly
/*
  if(assembly_string!=NULL)
    {
      sprintf(assembly_string,"");
      sprintf(assembly_string,"%s\tLDI R%d.1000, %d\n",
	      assembly_string,remain_regi4,type+1);
      sprintf(assembly_string,"%s\tLDIF R%d.0100, 90.0\n",
	      assembly_string,remain_regi3);
      sprintf(assembly_string,"%s\tLDI R%d.0010, 1\n",
	      assembly_string,remain_regi3);
      sprintf(assembly_string,"%s\tLDIF R%d.0001, 180.0\n",
	      assembly_string,remain_regi3);
      //BB0
      sprintf(assembly_string,"%s\tMVR R%d.1%d%d%d, R%d.xxyz, <4>\n",
	      assembly_string,OUR_TEMP_REGI,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI);
      sprintf(assembly_string,"%s\tADD R%d.1000, R%d.xyzw, -R%d.zyzw, <4>\n",
	      assembly_string,remain_regi4,remain_regi4,remain_regi3);
      sprintf(assembly_string,"%s\tLDI R%d.1000, %d\n",
	      assembly_string,remain_regi3,type-1);

      switch(type)
	{
	case 4:
	  sprintf(assembly_string,"%s\tBLT R%d, R%d, 4, <4>\n",
		  assembly_string,remain_regi4,remain_regi3);
	  sprintf(assembly_string,"%s\tBEQ R%d, R%d, 3, <4>\n",
		  assembly_string,remain_regi4,remain_regi3);
	  sprintf(assembly_string,"%s\tMVR R%d.1000, R%d.wyzw, <4>\n",
		  assembly_string,remain_regi,input_angle_regi);
	  sprintf(assembly_string,"%s\tJR %d\n", //goto BB1
		  assembly_string,12);
	  sprintf(assembly_string,"%s\tADD R%d.1000, R%d.xyzw, -R%d.zyzw, <4>\n",
		  assembly_string,remain_regi3,remain_regi3,remain_regi3);
	case 3:
	  sprintf(assembly_string,"%s\tBLT R%d, R%d, 4, <4>\n",
		  assembly_string,remain_regi4,remain_regi3);
	  sprintf(assembly_string,"%s\tBEQ R%d, R%d, 3, <4>\n",
		  assembly_string,remain_regi4,remain_regi3);
	  sprintf(assembly_string,"%s\tMVR R%d.1000, R%d.zyzw, <4>\n",
		  assembly_string,remain_regi,input_angle_regi);
	  sprintf(assembly_string,"%s\tJR %d\n", //goto BB1
		  assembly_string,7);
	  sprintf(assembly_string,"%s\tADD R%d.1000, R%d.xyzw, -R%d.zyzw, <4>\n",
		  assembly_string,remain_regi3,remain_regi3,remain_regi3);
	case 2:
	  sprintf(assembly_string,"%s\tBLT R%d, R%d, 4, <4>\n",
		  assembly_string,remain_regi4,remain_regi3);
	  sprintf(assembly_string,"%s\tBEQ R%d, R%d, 3, <4>\n",
		  assembly_string,remain_regi4,remain_regi3);
	  sprintf(assembly_string,"%s\tMVR R%d.1000, R%d.yyzw, <4>\n",
		  assembly_string,remain_regi,input_angle_regi);
	  sprintf(assembly_string,"%s\tJR %d\n", //goto BB1
		  assembly_string,2);
	case 1:
	  sprintf(assembly_string,"%s\tMVR R%d.1000, R%d.xyzw, <4>\n",
		  assembly_string,remain_regi,input_angle_regi);
	  break;
	default:
	  perr("GL_Sin type");
	}
      
      //BB1
      sprintf(assembly_string,"%s\tADDF R%d.1000, R0.xyzw, R%d.yyzw, <4>\n",
	      assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBLTF R%d, R%d, 4, <4>\n",
	      assembly_string,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tBEQV R%d, R%d, 3, <4>\n", //Kaymon
	      assembly_string,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.1000, R%d.xyzw, -R%d.wyzw, <4>\n",
	      assembly_string,remain_regi,remain_regi,remain_regi3);
      sprintf(assembly_string,"%s\tJR %d\n", //goto BB1
	      assembly_string,-4);
      //BB2
      sprintf(assembly_string,"%s\tADDF R%d.1000, R0.xyzw, -R%d.yyzw, <4>\n",
	      assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBGTF R%d, R%d, 4, <4>\n",
	      assembly_string,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tBEQV R%d, R%d, 3, <4>\n", //Kaymon
	      assembly_string,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.1000, R%d.wyzw, R%d.xyzw, <4>\n",
	      assembly_string,remain_regi,remain_regi3,remain_regi);
      sprintf(assembly_string,"%s\tJR %d\n", //goto BB2
	      assembly_string,-4);
      sprintf(assembly_string,"%s\tMVR R%d.1000, R%d.xyzw, <4>\n",
	      assembly_string,OUR_TEMP_REGI,remain_regi);
      sprintf(assembly_string,"%s\tMVR R%d.1000, R%d.zyzw, <4>\n",
	      assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBLT R%d, R%d, 3, <4>\n",
	      assembly_string,remain_regi4,remain_regi2);
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, 2, <4>\n",
	      assembly_string,remain_regi4,remain_regi2);
      sprintf(assembly_string,"%s\tJR %d\n", //goto BB0
	      assembly_string,type==1?-18:type==2?-22:type==3?-27:-32);
    }

  fputs(assembly_string,ass_file);
  bzero(assembly_string,sizeof(assembly_string));
  
  fflush(ass_file);


  //binary
  
  temp_int=type+1;
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi4;
  temp_b_code[0]=temp_b_code[0]<<38;
  temp_b_code[0]|=(unsigned)temp_int;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  instruction_count++;
  
  temp_float=90.0;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[1]=10;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi3;
  temp_b_code[1]=temp_b_code[1]<<38;
  temp_b_code[1]|=(unsigned)temp_int;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=4;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<7;
  instruction_count++;
  
  temp_int=1;
  temp_b_code[2]=temp_b_code[0];
  temp_b_code[2]&=0xF81F8000000043FFLL;
  temp_b_code[2]|=(unsigned long long)(remain_regi3)<<53;
  temp_b_code[2]|=(unsigned long long)((unsigned)temp_int)<<15;
  temp_b_code[2]|=(unsigned long long)2<<10;
  instruction_count++;
  
  temp_float=180.0;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[3]=temp_b_code[1];
  temp_b_code[3]&=0xFFFF8000000043FFLL;
  temp_b_code[3]|=(unsigned long long)((unsigned)temp_int)<<15;
  temp_b_code[3]|=(unsigned long long)1<<10;
  instruction_count++;
  
  temp_b_code[4]=7;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=OUR_TEMP_REGI;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=OUR_TEMP_REGI;
  temp_b_code[4]=temp_b_code[4]<<22;
  temp_b_code[4]|=0x1B06;
  temp_b_code[4]=temp_b_code[4]<<15;      
  temp_b_code[4]|=temp_wm;
  temp_b_code[4]=temp_b_code[4]<<3;
  temp_b_code[4]|=3;
  temp_b_code[4]=temp_b_code[4]<<7;
  instruction_count++;
  
  temp_b_code[5]=16;
  temp_b_code[5]=temp_b_code[5]<<6;
  temp_b_code[5]|=remain_regi4;
  temp_b_code[5]=temp_b_code[5]<<6;
  temp_b_code[5]|=remain_regi4;
  temp_b_code[5]=temp_b_code[5]<<6;
  temp_b_code[5]|=remain_regi3;
  temp_b_code[5]=temp_b_code[5]<<24;
  temp_b_code[5]|=0x1B1B9B;
  temp_b_code[5]=temp_b_code[5]<<3;      
  temp_b_code[5]|=2;
  temp_b_code[5]=temp_b_code[5]<<4;
  temp_b_code[5]|=8;
  temp_b_code[5]=temp_b_code[5]<<3;
  temp_b_code[5]|=3;
  temp_b_code[5]=temp_b_code[5]<<7;
  instruction_count++;
  
  temp_int=type-1;
  temp_b_code[6]=temp_b_code[2];
  temp_b_code[6]&=0xFFFF8000000043FFLL;
  temp_b_code[6]|=(unsigned long long)((unsigned)temp_int)<<15;
  temp_b_code[6]|=(unsigned long long)8<<10;
  instruction_count++;


  
  long long unsigned temp_code[5];
  
  temp_code[0]=26;
  temp_code[0]=temp_code[0]<<6;
  temp_code[0]|=remain_regi4;
  temp_code[0]=temp_code[0]<<6;
  temp_code[0]|=remain_regi3;
  temp_code[0]=temp_code[0]<<16;
#ifdef SET_NOP_NINE
  temp_code[0]|=(unsigned short)8;
#else
  temp_code[0]|=(unsigned short)4;
#endif
  temp_code[0]=temp_code[0]<<6;
  temp_code[0]|=remain_regi4;
  temp_code[0]=temp_code[0]<<5;
  temp_code[0]|=remain_regi3;
  temp_code[0]=temp_code[0]<<10;
  temp_code[0]|=8;
  temp_code[0]=temp_code[0]<<3;
  temp_code[0]|=3;
  temp_code[0]=temp_code[0]<<7;
  
  temp_code[1]=temp_code[0];
  temp_code[1]&=0x07FF80007FFFFFFFLL;
  temp_code[1]|=(unsigned long long)24<<59;
#ifdef SET_NOP_NINE
  temp_code[1]|=(unsigned long long)6<<31;
#else
  temp_code[1]|=(unsigned long long)3<<31;
#endif
  printf("%llx\n",temp_code[1]);
  
  temp_code[2]=7;
  temp_code[2]=temp_code[2]<<6;
  temp_code[2]|=remain_regi;
  temp_code[2]=temp_code[2]<<6;
  temp_code[2]|=input_angle_regi;
  temp_code[2]=temp_code[2]<<22;
  temp_code[2]|=0x1BDB;
  temp_code[2]=temp_code[2]<<15;      
  temp_code[2]|=8;
  temp_code[2]=temp_code[2]<<3;
  temp_code[2]|=3;
  temp_code[2]=temp_code[2]<<7;	  
  
  temp_code[3]=24;
  temp_code[3]=temp_code[3]<<28;
#ifdef SET_NOP_NINE
  temp_code[3]|=(unsigned short)24;
#else
  temp_code[3]|=(unsigned short)12;
#endif
  temp_code[3]=temp_code[3]<<21;
  temp_code[3]|=8;
  temp_code[3]=temp_code[3]<<10;
  
  temp_code[4]=temp_b_code[5];
  temp_code[4]&=0xF8007FFFFFFFFFFFLL;
  temp_code[4]|=(unsigned long long)(remain_regi3)<<53;
  temp_code[4]|=(unsigned long long)(remain_regi3)<<47;
  printf("type:%d\n",type);

  switch(type)
    {
    case 4:
      temp_b_code[instruction_count]=temp_code[0];
      instruction_count++;
      
      temp_b_code[instruction_count]=temp_code[1];
      instruction_count++;
      
      temp_b_code[instruction_count]=temp_code[2];
      instruction_count++;
      
      temp_b_code[instruction_count]=temp_code[3];
      instruction_count++;
      
      temp_b_code[instruction_count]=temp_code[4];
      instruction_count++;
    case 3:
      temp_b_code[instruction_count]=temp_code[0];
      instruction_count++;
      
      temp_b_code[instruction_count]=temp_code[1];
      instruction_count++;
      
      temp_b_code[instruction_count]=temp_code[2];
      temp_b_code[instruction_count]&=0xFFFFFFFE01FFFFFFLL;
      temp_b_code[instruction_count]|=(unsigned long long)0x9B<<25;
      instruction_count++;
      
      temp_b_code[instruction_count]=temp_code[3];
      temp_b_code[instruction_count]&=0xFFFF80007FFFFFFFLL;
      temp_b_code[instruction_count]|=(unsigned long long)7<<31;
      instruction_count++;
      
      temp_b_code[instruction_count]=temp_code[4];
      instruction_count++;
    case 2:
      temp_b_code[instruction_count]=temp_code[0];
      instruction_count++;
      
      temp_b_code[instruction_count]=temp_code[1];
      instruction_count++;
      
      temp_b_code[instruction_count]=temp_code[2];
      temp_b_code[instruction_count]&=0xFFFFFFFE01FFFFFFLL;
      temp_b_code[instruction_count]|=(unsigned long long)0x5B<<25;
      instruction_count++;
      
      temp_b_code[instruction_count]=temp_code[3];
      temp_b_code[instruction_count]&=0xFFFF80007FFFFFFFLL;
      temp_b_code[instruction_count]|=(unsigned long long)2<<31;
      instruction_count++;
    case 1:
      temp_b_code[instruction_count]=temp_code[2];
      temp_b_code[instruction_count]&=0xFFFFFFFE01FFFFFFLL;
      temp_b_code[instruction_count]|=(unsigned long long)0x1B<<25;
      instruction_count++;
      break;
    default:
      perr("GL_Sin type");
    }

  temp_b_code[instruction_count]=16;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<6;
  temp_b_code[instruction_count]|=remain_regi2;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<12;
  temp_b_code[instruction_count]|=remain_regi3;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<24;
  temp_b_code[instruction_count]|=0x1B1B5B;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<7;
  temp_b_code[instruction_count]|=8;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<1;
  temp_b_code[instruction_count]|=1;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<2;
  temp_b_code[instruction_count]|=3;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<7;
  instruction_count++;
  
  temp_b_code[instruction_count]=temp_code[0];
  temp_b_code[instruction_count]&=0xF8007FFF8007FDFFLL;
  temp_b_code[instruction_count]|=(unsigned long long)remain_regi<<53;
  temp_b_code[instruction_count]|=(unsigned long long)remain_regi2<<47;
  temp_b_code[instruction_count]|=(unsigned long long)remain_regi<<25;
  temp_b_code[instruction_count]|=(unsigned long long)remain_regi2<<20;
  temp_b_code[instruction_count]|=(unsigned long long)1<<9;
  instruction_count++;
  
  temp_b_code[instruction_count]=temp_b_code[instruction_count-1];
  temp_b_code[instruction_count]&=0x07FFFFFFFFFFFFFFLL;
  temp_b_code[instruction_count]|=(unsigned long long)24<<59;
  instruction_count++;
  
  temp_b_code[instruction_count]=16;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<6;
  temp_b_code[instruction_count]|=remain_regi;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<6;
  temp_b_code[instruction_count]|=remain_regi;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<6;
  temp_b_code[instruction_count]|=remain_regi3;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<24;
  temp_b_code[instruction_count]|=0x1B1BDB;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<3;      
  temp_b_code[instruction_count]|=2;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<4;
  temp_b_code[instruction_count]|=8;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<1;
  temp_b_code[instruction_count]|=1;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<2;
  temp_b_code[instruction_count]|=3;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<7;
  instruction_count++;
  
  temp_b_code[instruction_count]=temp_code[3];
  temp_b_code[instruction_count]&=0xFFFF80007FFFFFFFLL;
  temp_b_code[instruction_count]|=(unsigned long long)((unsigned short)-4)<<31;
  instruction_count++;
  
  temp_b_code[instruction_count]=temp_b_code[instruction_count-5];
  temp_b_code[instruction_count]&=0xFFFFFFFFFFFF7FFFLL;
  temp_b_code[instruction_count]|=(unsigned long long)1<<15;
  instruction_count++;
  
  temp_b_code[instruction_count]=temp_b_code[instruction_count-5];
  temp_b_code[instruction_count]&=0x07FFFFFFFFFFFFFFLL;
  temp_b_code[instruction_count]|=(unsigned long long)27<<59;
  instruction_count++;
  
  temp_b_code[instruction_count]=temp_b_code[instruction_count-5];
  instruction_count++;
  
  temp_b_code[instruction_count]=temp_b_code[instruction_count-5];
  temp_b_code[instruction_count]&=0xFFE001FE00017FFFLL;
  temp_b_code[instruction_count]|=(unsigned long long)remain_regi3<<47;
  temp_b_code[instruction_count]|=(unsigned long long)remain_regi<<41;
  temp_b_code[instruction_count]|=(unsigned long long)0xDB1B<<17;
  instruction_count++;
  
  temp_b_code[instruction_count]=temp_b_code[instruction_count-5];
  instruction_count++;
  
  temp_b_code[instruction_count]=7;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<6;
  temp_b_code[instruction_count]|=OUR_TEMP_REGI;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<6;
  temp_b_code[instruction_count]|=remain_regi;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<22;
  temp_b_code[instruction_count]|=0x1B1B;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<15;      
  temp_b_code[instruction_count]|=8;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<3;
  temp_b_code[instruction_count]|=3;
  temp_b_code[instruction_count]=temp_b_code[instruction_count]<<7;
  instruction_count++;
  
  temp_b_code[instruction_count]=temp_b_code[instruction_count-1];
  temp_b_code[instruction_count]&=0xF8001FFE01FFFFFFLL;
  temp_b_code[instruction_count]|=(unsigned long long)remain_regi2<<53;
  temp_b_code[instruction_count]|=(unsigned long long)remain_regi3<<47;
  temp_b_code[instruction_count]|=(unsigned long long)0x9B<<25;
  instruction_count++;
  
  temp_b_code[instruction_count]=temp_code[0];
  temp_b_code[instruction_count]&=0xF80000000007FFFFLL;
  temp_b_code[instruction_count]|=(unsigned long long)remain_regi4<<53;
  temp_b_code[instruction_count]|=(unsigned long long)remain_regi2<<47;
  temp_b_code[instruction_count]|=(unsigned long long)remain_regi4<<25;
  temp_b_code[instruction_count]|=(unsigned long long)remain_regi2<<20;
  temp_b_code[instruction_count]|=(unsigned long long)3<<31;
  instruction_count++;
  
  temp_b_code[instruction_count]=temp_b_code[instruction_count-1];
  temp_b_code[instruction_count]&=0x07FF80007FFFFFFFLL;
  temp_b_code[instruction_count]|=(unsigned long long)24<<59;
  temp_b_code[instruction_count]|=(unsigned long long)2<<31;
  instruction_count++;
  
  temp_b_code[instruction_count]=temp_code[3];
  temp_b_code[instruction_count]&=0xFFFF80007FFFFFFFLL;
  temp_b_code[instruction_count]|=(unsigned long long)((unsigned short)(type==1?-18:type==2?-22:type==3?-27:-32))<<31;
  instruction_count++;

#ifdef SET_NOP_NINE
  insert_nop_nine(instruction_count);
  for(i=0;i<instruction_count*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<instruction_count;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
*/
  //pi=3.14159265359
  if(assembly_string!=NULL){
    //assembly_string[0]=0;
    sprintf(assembly_string,"%s\tLDIF R%d.0100, 1.5707963268\n",assembly_string,remain_regi3);//pi
    sprintf(assembly_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",assembly_string,remain_regi,input_angle_regi);
    sprintf(assembly_string,"%s\tLDIF R%d.0100, 3.14159265359\n",assembly_string,remain_regi3);//pi
    sprintf(assembly_string,"%s\tLDIF R%d.0010, -3.14159265359\n",assembly_string,remain_regi3);
    sprintf(assembly_string,"%s\tLDIF R%d.0001, 6.28318530718\n",assembly_string,remain_regi3);//2*pi

    /*
      L1: ADDI R16.1000, R16.x, -6.283185 
          BGTVF R16.x, R18.y, L1
          ADDI R16.1000, R16.x, -6.283185 
      L2: ADDI R16.1000, R16.x, 6.283185 
          BLTVF R16.x, -R18.y, L2
    */
#ifdef unified_branch
    for (g=0;g<type;g++)
    { char p=(g==3)?'w':'x'+g;
      v[g]='1';
      sprintf(assembly_string,"%s\tADDF R%d.%s, R%d.%c%c%c%c, -R%d.wwww, <4>\n",assembly_string,remain_regi,v,remain_regi,p,p,p,p,remain_regi3);
      sprintf(assembly_string,"%s\tBGTSF R%d.%c%c%c%c, R%d.yyyy, -1, <4>\n",assembly_string,remain_regi,p,p,p,p,remain_regi3);
      sprintf(assembly_string,"%s\tADDF R%d.%s, R%d.%c%c%c%c, -R%d.wwww\n",assembly_string,remain_regi,v,remain_regi,p,p,p,p,remain_regi3);
      sprintf(assembly_string,"%s\tADDF R%d.%s, R%d.%c%c%c%c, R%d.wwww, <4>\n",assembly_string,remain_regi,v,remain_regi,p,p,p,p,remain_regi3);
      sprintf(assembly_string,"%s\tBLTSF R%d.%c%c%c%c, R%d.zzzz, -1, <4>\n",assembly_string,remain_regi,p,p,p,p,remain_regi3);
      v[g]='0';
    }

#else

    switch(type){
    case 4:
      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, R%d.yyyy, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 4, 0001, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle>pi) then angle -= 2*pi
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 3, 0001, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.0001, R%d.xyzw, -R%d.wwww, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3);
      sprintf(assembly_string,"%s\tJR %d\n", assembly_string,-3);
      
      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, -R%d.yyyy, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 4, 0001, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle<-pi) then angle += 2*pi
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 3, 0001, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.0001, R%d.xyzw, R%d.wwww, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3);
      sprintf(assembly_string,"%s\tJR %d\n", assembly_string,-3); 
    case 3:
      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, R%d.yyyy, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 4, 0010, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle>pi) then angle -= 2*pi
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 3, 0010, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.0010, R%d.xyzw, -R%d.wwww, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3);
      sprintf(assembly_string,"%s\tJR %d\n", assembly_string,-3);
      
      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, -R%d.yyyy, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 4, 0010, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle<-pi) then angle += 2*pi
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 3, 0010, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.0010, R%d.xyzw, R%d.wwww, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3);
      sprintf(assembly_string,"%s\tJR %d\n", assembly_string,-3); 
    case 2:
      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, R%d.yyyy, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 4, 0100, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle>pi) then angle -= 2*pi
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 3, 0100, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.0100, R%d.xyzw, -R%d.wwww, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3);
      sprintf(assembly_string,"%s\tJR %d\n", assembly_string,-3);
      
      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, -R%d.yyyy, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 4, 0100, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle<-pi) then angle += 2*pi
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 3, 0100, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.0100, R%d.xyzw, R%d.wwww, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3);
      sprintf(assembly_string,"%s\tJR %d\n", assembly_string,-3); 
    case 1:
      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, R%d.yyyy, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 4, 1000, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle>pi) then angle -= 2*pi
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 3, 1000, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.1000, R%d.xyzw, -R%d.wwww, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3);
      sprintf(assembly_string,"%s\tJR %d\n", assembly_string,-3);
      
      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, -R%d.yyyy, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 4, 1000, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle<-pi) then angle += 2*pi
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 3, 1000, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.1000, R%d.xyzw, R%d.wwww, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3);
      sprintf(assembly_string,"%s\tJR %d\n", assembly_string,-3); 
    }
#endif    
  }


  temp_float=3.14159265359/2;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi3;
  temp_b_code[0]=temp_b_code[0]<<38;
  temp_b_code[0]|=(unsigned)temp_int;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  

  
  MVR_instr( remain_regi , input_angle_regi, 0X1b1b, 0X1b1b, 0 , 15 , 3 ,0);  //move angle to remain_regi.
  
  temp_float=3.14159265359;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[1]=10;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi3;
  temp_b_code[1]=temp_b_code[1]<<38;
  temp_b_code[1]|=(unsigned)temp_int;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=4;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<7;

  
  temp_float=-3.14159265359;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[2]=10;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi3;
  temp_b_code[2]=temp_b_code[2]<<38;
  temp_b_code[2]|=(unsigned)temp_int;
  temp_b_code[2]=temp_b_code[2]<<5;
  temp_b_code[2]|=2;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=3;
  temp_b_code[2]=temp_b_code[2]<<7;

  temp_float=6.28318530718;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[3]=temp_b_code[1];
  temp_b_code[3]&=0xFFFF8000000043FFLL;
  temp_b_code[3]|=(unsigned long long)((unsigned)temp_int)<<15;
  temp_b_code[3]|=1<<10;


#ifdef SET_NOP_NINE
  insert_nop_nine(4);
  for(i=0;i<4*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<4;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  


#ifdef unified_branch

  temp_b_code[0]=16;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi3;
  temp_b_code[0]=temp_b_code[0]<<24;
  temp_b_code[0]|=0x1bFFFF;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<4;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=7;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=27;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi3;
  temp_b_code[1]=temp_b_code[1]<<16;
#ifdef SET_NOP_NINE
  temp_b_code[1]|=0xFFFE;
#else
  temp_b_code[1]|=0xFFFF;
#endif
  temp_b_code[1]=temp_b_code[1]<<16;
  temp_b_code[1]|=0xFF55;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=temp_b_code[0];
  temp_b_code[3]=temp_b_code[0]&0xFFFFFFFFFFFF7FFF;

  temp_b_code[4]=temp_b_code[1]&0x07FFFFFFFF807FFF;
  temp_b_code[4]|=(unsigned long long)26<<59;
  temp_b_code[4]|=0xAA<<15;

  switch(type){
  case 4:
#ifdef SET_NOP_NINE
  insert_nop_nine(5);
  for(i=0;i<5*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<5;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  
  case 3:
    temp_b_code[0]=temp_b_code[0]&0xFFFFFFFE01FFC3FF;
    temp_b_code[0]|=(unsigned long long)0xAA<<25;
    temp_b_code[0]|=2<<10;
    temp_b_code[1]=temp_b_code[1]&0xFFFFFFFF807FFFFF;
    temp_b_code[1]|=(unsigned long long)0xAA<<23;
    temp_b_code[2]=temp_b_code[2]&0xFFFFFFFE01FFC3FF;
    temp_b_code[2]|=(unsigned long long)0xAA<<25;
    temp_b_code[2]|=2<<10;
    temp_b_code[3]=temp_b_code[3]&0xFFFFFFFE01FFC3FF;
    temp_b_code[3]|=(unsigned long long)0xAA<<25;
    temp_b_code[3]|=2<<10;
    temp_b_code[4]=temp_b_code[4]&0xFFFFFFFF807FFFFF;
    temp_b_code[4]|=(unsigned long long)0xAA<<23;
#ifdef SET_NOP_NINE
  insert_nop_nine(5);
  for(i=0;i<5*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<5;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  
  case 2:
    temp_b_code[0]=temp_b_code[0]&0xFFFFFFFE01FFC3FF;
    temp_b_code[0]|=(unsigned long long)0x55<<25;
    temp_b_code[0]|=4<<10;
    temp_b_code[1]=temp_b_code[1]&0xFFFFFFFF807FFFFF;
    temp_b_code[1]|=(unsigned long long)0x55<<23;
    temp_b_code[2]=temp_b_code[2]&0xFFFFFFFE01FFC3FF;
    temp_b_code[2]|=(unsigned long long)0x55<<25;
    temp_b_code[2]|=4<<10;
    temp_b_code[3]=temp_b_code[3]&0xFFFFFFFE01FFC3FF;
    temp_b_code[3]|=(unsigned long long)0x55<<25;
    temp_b_code[3]|=4<<10;
    temp_b_code[4]=temp_b_code[4]&0xFFFFFFFF807FFFFF;
    temp_b_code[4]|=(unsigned long long)0x55<<23;
#ifdef SET_NOP_NINE
  insert_nop_nine(5);
  for(i=0;i<5*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<5;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  
  case 1:
    temp_b_code[0]=temp_b_code[0]&0xFFFFFFFE01FFC3FF;
    temp_b_code[0]|=8<<10;
    temp_b_code[1]=temp_b_code[1]&0xFFFFFFFF807FFFFF;
    temp_b_code[2]=temp_b_code[2]&0xFFFFFFFE01FFC3FF;
    temp_b_code[2]|=8<<10;
    temp_b_code[3]=temp_b_code[3]&0xFFFFFFFE01FFC3FF;
    temp_b_code[3]|=8<<10;
    temp_b_code[4]=temp_b_code[4]&0xFFFFFFFF807FFFFF;
#ifdef SET_NOP_NINE
  insert_nop_nine(5);
  for(i=0;i<5*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<5;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  
  break;
  default:
    perr("Sin inlinefunction error\n");
    break;

  }
#else

//if(angle>pi) then angle -= 2*pi
  temp_b_code[0]=16;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi2;
  temp_b_code[0]=temp_b_code[0]<<12;
  temp_b_code[0]|=remain_regi3;
  temp_b_code[0]=temp_b_code[0]<<24;
  temp_b_code[0]|=0x1b1b55;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

//if(angle>pi) then angle -= 2*pi
  temp_b_code[1]=26;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi2;
  temp_b_code[1]=temp_b_code[1]<<16;
#ifdef SET_NOP_NINE
  temp_b_code[1]|=(unsigned long long)8;
#else
  temp_b_code[1]|=(unsigned long long)4;
#endif
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=remain_regi2;
  temp_b_code[1]=temp_b_code[1]<<10;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=temp_b_code[1]&0x07FF80007FFFFFFF;
  temp_b_code[2]|=(unsigned long long)24<<59;
#ifdef SET_NOP_NINE
  temp_b_code[2]|=(unsigned long long)6<<31;
#else
  temp_b_code[2]|=(unsigned long long)3<<31;
#endif

  temp_b_code[3]=temp_b_code[0]&0xF8007FFFFEF003FF;
  temp_b_code[3]|=(unsigned long long)remain_regi<<53;
  temp_b_code[3]|=(unsigned long long)remain_regi<<47;
  temp_b_code[3]|=0xFF<<17;//wwww
  temp_b_code[3]|=1<<15; //neg
  temp_b_code[3]|=1<<10; //writed mask

  temp_b_code[4]=24;
  temp_b_code[4]=temp_b_code[4]<<28;
#ifdef SET_NOP_NINE
  temp_b_code[4]|=0xFFFA;
#else
  temp_b_code[4]|=0xFFFD;
#endif
  temp_b_code[4]=temp_b_code[4]<<31;
  temp_b_code[4]|=1<<13;


//if(angle<-pi) then angle += 2*pi

  temp_b_code[5] = temp_b_code[0];
  temp_b_code[5] |= 1<<15;
  temp_b_code[6] = temp_b_code[1]&0X07FFFFFFFFFFFFFF;
  temp_b_code[6]|=(unsigned long long)27<<59;
  temp_b_code[7] = temp_b_code[2];
  temp_b_code[8] = temp_b_code[3]&0xFFFFFFFFFFFF7FFF;
  temp_b_code[9] = temp_b_code[4];


  
  switch(type){//all change the writed mask.
  case 4:
#ifdef SET_NOP_NINE
    insert_nop_nine(10);
    for(i=0;i<10*2;i++)
      BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
    for(i=0;i<10;i++)
      BinCode[LastBinLine++]=temp_b_code[i];
#endif
  case 3:
    temp_b_code[1]=temp_b_code[1]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[1]|=2<<10;
    temp_b_code[2]=temp_b_code[2]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[2]|=2<<10;
    temp_b_code[3]=temp_b_code[3]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[3]|=2<<10;
    temp_b_code[6]=temp_b_code[6]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[6]|=2<<10;
    temp_b_code[7]=temp_b_code[7]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[7]|=2<<10;
    temp_b_code[8]=temp_b_code[8]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[8]|=2<<10;
#ifdef SET_NOP_NINE
    insert_nop_nine(10);
    for(i=0;i<10*2;i++)
      BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
    for(i=0;i<10;i++)
      BinCode[LastBinLine++]=temp_b_code[i];
#endif
  case 2:
    temp_b_code[1]=temp_b_code[1]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[1]|=4<<10;
    temp_b_code[2]=temp_b_code[2]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[2]|=4<<10;
    temp_b_code[3]=temp_b_code[3]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[3]|=4<<10;
    temp_b_code[6]=temp_b_code[6]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[6]|=4<<10;
    temp_b_code[7]=temp_b_code[7]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[7]|=4<<10;
    temp_b_code[8]=temp_b_code[8]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[8]|=4<<10;
#ifdef SET_NOP_NINE
    insert_nop_nine(10);
    for(i=0;i<10*2;i++)
      BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
    for(i=0;i<10;i++)
      BinCode[LastBinLine++]=temp_b_code[i];
#endif
  case 1:
    temp_b_code[1]=temp_b_code[1]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[1]|=8<<10;
    temp_b_code[2]=temp_b_code[2]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[2]|=8<<10;
    temp_b_code[3]=temp_b_code[3]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[3]|=8<<10;
    temp_b_code[6]=temp_b_code[6]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[6]|=8<<10;
    temp_b_code[7]=temp_b_code[7]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[7]|=8<<10;
    temp_b_code[8]=temp_b_code[8]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[8]|=8<<10;
#ifdef SET_NOP_NINE
    insert_nop_nine(10);
    for(i=0;i<10*2;i++)
      BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
    for(i=0;i<10;i++)
      BinCode[LastBinLine++]=temp_b_code[i];
#endif
    break;
  default:
    perr("Sin inlinefunction error\n");
  }
#endif //end of unified_branch

  //shift the radians between pi/2 and -pi/*2.
#ifdef unified_branch
  
  sprintf(assembly_string,"%s\tADDF R%d.0001, R0.xyzw, -R%d.xxxx\n",assembly_string,remain_regi2,remain_regi3);

  switch(type){
  case 4:
    sprintf(assembly_string,"%s\tBGTSF R%d.wwww, R%d.xxxx, 3, <4>\n",assembly_string,remain_regi,remain_regi3);
    sprintf(assembly_string,"%s\tBLTSF R%d.wwww, R%d.wwww, 4, <4>\n",assembly_string,remain_regi,remain_regi2);
    sprintf(assembly_string,"%s\tJR 5\n",assembly_string); 
    sprintf(assembly_string,"%s\tADDF R%d.0001, R%d.yyyy, -R%d.xyzw <4>\n",assembly_string,remain_regi,remain_regi3,remain_regi); //pi - angle
    sprintf(assembly_string,"%s\tJR 3\n",assembly_string); 
    sprintf(assembly_string,"%s\tADDF R%d.0001, R%d.xyzw, R%d.yyyy, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3); //pi + angle
    sprintf(assembly_string,"%s\tADDF R%d.0001, R0.xyzw, -R%d.xyzw, <4>\n",assembly_string,remain_regi,remain_regi); 
  case 3:
    sprintf(assembly_string,"%s\tBGTSF R%d.yyyy, R%d.xxxx, 3, <4>\n",assembly_string,remain_regi,remain_regi3);
    sprintf(assembly_string,"%s\tBLTSF R%d.yyyy, R%d.wwww, 4, <4>\n",assembly_string,remain_regi,remain_regi2);
    sprintf(assembly_string,"%s\tJR 5\n",assembly_string); 
    sprintf(assembly_string,"%s\tADDF R%d.0010, R%d.yyyy, -R%d.xyzw <4>\n",assembly_string,remain_regi,remain_regi3,remain_regi); //pi - angle
    sprintf(assembly_string,"%s\tJR 3\n",assembly_string); 
    sprintf(assembly_string,"%s\tADDF R%d.0010, R%d.xyzw, R%d.yyyy, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3); //pi + angle
    sprintf(assembly_string,"%s\tADDF R%d.0010, R0.xyzw, -R%d.xyzw, <4>\n",assembly_string,remain_regi,remain_regi);
  case 2:
    sprintf(assembly_string,"%s\tBGTSF R%d.yyyy, R%d.xxxx, 3, <4>\n",assembly_string,remain_regi,remain_regi3);
    sprintf(assembly_string,"%s\tBLTSF R%d.yyyy, R%d.wwww, 4, <4>\n",assembly_string,remain_regi,remain_regi2);
    sprintf(assembly_string,"%s\tJR 5\n",assembly_string); 
    sprintf(assembly_string,"%s\tADDF R%d.0100, R%d.yyyy, -R%d.xyzw <4>\n",assembly_string,remain_regi,remain_regi3,remain_regi); //pi - angle
    sprintf(assembly_string,"%s\tJR 3\n",assembly_string); 
    sprintf(assembly_string,"%s\tADDF R%d.0100, R%d.xyzw, R%d.yyyy, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3); //pi + angle
    sprintf(assembly_string,"%s\tADDF R%d.0100, R0.xyzw, -R%d.xyzw, <4>\n",assembly_string,remain_regi,remain_regi);
  case 1:
    sprintf(assembly_string,"%s\tBGTSF R%d.xxxx, R%d.xxxx, 3, <4>\n",assembly_string,remain_regi,remain_regi3);
    sprintf(assembly_string,"%s\tBLTSF R%d.xxxx, R%d.wwww, 4, <4>\n",assembly_string,remain_regi,remain_regi2);
    sprintf(assembly_string,"%s\tJR 5\n",assembly_string); 
    sprintf(assembly_string,"%s\tADDF R%d.1000, R%d.yyyy, -R%d.xyzw <4>\n",assembly_string,remain_regi,remain_regi3,remain_regi); //pi - angle
    sprintf(assembly_string,"%s\tJR 3\n",assembly_string); 
    sprintf(assembly_string,"%s\tADDF R%d.1000, R%d.xyzw, R%d.yyyy, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3); //pi + angle
    sprintf(assembly_string,"%s\tADDF R%d.1000, R0.xyzw, -R%d.xyzw, <4>\n",assembly_string,remain_regi,remain_regi);
    break;
  default:
    perr("Sin inlinefunction error\n");
  }
#else
  switch(type){
  case 4:
      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, R%d.xxxx, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 3, 0001, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle>pi/2) then angle = pi - angle;
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 2, 0001, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.0001, R%d.yyyy, -R%d.xyzw, <4>\n",assembly_string,remain_regi,remain_regi3,remain_regi);

      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, -R%d.xxxx, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 4, 0001, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle<-pi/2) then angle = (pi + angle )*-1
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 3, 0001, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.0001, R%d.xyzw, R%d.yyyy, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3);
      sprintf(assembly_string,"%s\tADDF R%d.0001, R0.xyzw, -R%d.xyzw, <4>\n",assembly_string,remain_regi,remain_regi);

  case 3:
      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, R%d.xxxx, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 3, 0010, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle>pi/2) then angle = pi - angle;
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 2, 0010, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.0010, R%d.yyyy, -R%d.xyzw, <4>\n",assembly_string,remain_regi,remain_regi3,remain_regi);

      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, -R%d.xxxx, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 4, 0010, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle<-pi/2) then angle = (pi + angle )*-1
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 3, 0010, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.0010, R%d.xyzw, R%d.yyyy, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3);
      sprintf(assembly_string,"%s\tADDF R%d.0010, R0.xyzw, -R%d.xyzw, <4>\n",assembly_string,remain_regi,remain_regi);
  case 2:
      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, R%d.xxxx, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 3, 0100, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle>pi/2) then angle = pi - angle;
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 2, 0100, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.0100, R%d.yyyy, -R%d.xyzw, <4>\n",assembly_string,remain_regi,remain_regi3,remain_regi);

      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, -R%d.xxxx, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 4, 0100, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle<-pi/2) then angle = (pi + angle )*-1
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 3, 0100, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.0100, R%d.xyzw, R%d.yyyy, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3);
      sprintf(assembly_string,"%s\tADDF R%d.0100, R0.xyzw, -R%d.xyzw, <4>\n",assembly_string,remain_regi,remain_regi);
  case 1:
      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, R%d.xxxx, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 3, 1000, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle>pi/2) then angle = pi - angle;
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 2, 1000, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.1000, R%d.yyyy, -R%d.xyzw, <4>\n",assembly_string,remain_regi,remain_regi3,remain_regi);

      sprintf(assembly_string,"%s\tADDF R%d.1111, R0.xyzw, -R%d.xxxx, <4>\n",assembly_string,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 4, 1000, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2); //if(angle<-pi/2) then angle = (pi + angle )*-1
      sprintf(assembly_string,"%s\tBEQ R%d, R%d, R%d, R%d, 3, 10000, <4>\n",assembly_string,remain_regi,remain_regi2,remain_regi,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.1000, R%d.xyzw, R%d.yyyy, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3);
      sprintf(assembly_string,"%s\tADDF R%d.1000, R0.xyzw, -R%d.xyzw, <4>\n",assembly_string,remain_regi,remain_regi);
    break;
  default:
    perr("Sin inlinefunction error\n");
  }
#endif

#ifdef unified_branch
  temp_b_code[0]=16;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi2;
  temp_b_code[0]=temp_b_code[0]<<12;
  temp_b_code[0]|=remain_regi3;
  temp_b_code[0]=temp_b_code[0]<<24;
  temp_b_code[0]|=0x1b1b00;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<4;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  

  temp_b_code[0]=27;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi3;
  temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
  temp_b_code[0]|=6;
#else
  temp_b_code[0]|=3;
#endif
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=0xFF00;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=26;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi2;
  temp_b_code[1]=temp_b_code[1]<<16;
#ifdef SET_NOP_NINE
  temp_b_code[1]|=8;
#else
  temp_b_code[1]|=4;
#endif
  temp_b_code[1]=temp_b_code[1]<<16;
  temp_b_code[1]|=0xFFFF;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=24;
  temp_b_code[2]=temp_b_code[2]<<28;
#ifdef SET_NOP_NINE
  temp_b_code[2]|=10;
#else
  temp_b_code[2]|=5;
#endif
  temp_b_code[2]=temp_b_code[2]<<31;
  temp_b_code[2]|=1<<13;

  temp_b_code[3]=16;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=remain_regi;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=remain_regi3;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=remain_regi;
  temp_b_code[3]=temp_b_code[3]<<24;
  temp_b_code[3]|=0x1b551b;
  temp_b_code[3]=temp_b_code[3]<<3;
  temp_b_code[3]|=2;
  temp_b_code[3]=temp_b_code[3]<<4;
  temp_b_code[3]|=1;
  temp_b_code[3]=temp_b_code[3]<<1;
  temp_b_code[3]|=1;
  temp_b_code[3]=temp_b_code[3]<<2;
  temp_b_code[3]|=3;
  temp_b_code[3]=temp_b_code[3]<<7;
  
  temp_b_code[4] = temp_b_code[2] & 0xFFFF80007FFFFFFF;
#ifdef SET_NOP_NINE
  temp_b_code[4]|=(unsigned long long)6<<31;
#else
  temp_b_code[4]|=(unsigned long long)3<<31;
#endif

  temp_b_code[5] = temp_b_code[3] & 0xFFE001FE00017FFF;
  temp_b_code[5] |= (unsigned long long)remain_regi<< 47;
  temp_b_code[5] |= (unsigned long long)remain_regi3<<41;
  temp_b_code[5] |= (unsigned long long)0x1b55<<17;

  temp_b_code[6] = temp_b_code[3] & 0xFFE07FFE0001FFFF;
  temp_b_code[6] |= (unsigned long long)0x1b1b<<17;
  
  /*
    sprintf(assembly_string,"%s\tBGTSF R%d.wwww, R%d.xxxx, 3, <4>\n",assembly_string,remain_regi,remain_regi3);
    sprintf(assembly_string,"%s\tBLTSF R%d.wwww, R%d.wwww, 4, <4>\n",assembly_string,remain_regi,remain_regi2);
    sprintf(assembly_string,"%s\tJR 5\n",assembly_string); 
    sprintf(assembly_string,"%s\tADDF R%d.0001, R%d.yyyy, -R%d.xyzw <4>\n",assembly_string,remain_regi,remain_regi3,remain_regi); //pi - angle
    sprintf(assembly_string,"%s\tJR 3\n",assembly_string); 
    sprintf(assembly_string,"%s\tADDF R%d.0001, R%d.xyzw, R%d.yyyy, <4>\n",assembly_string,remain_regi,remain_regi,remain_regi3); //pi + angle
    sprintf(assembly_string,"%s\tADDF R%d.0001, R0.xyzw, -R%d.xyzw, <4>\n",assembly_string,remain_regi,remain_regi); 
  */
  switch(type){
  case 4:
#ifdef SET_NOP_NINE
  insert_nop_nine(7);
  for(i=0;i<7*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<7;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  
  case 3:
    temp_b_code[0] = temp_b_code[0] & 0xFFFFFFFF807FFFFF;
    temp_b_code[0] |= (unsigned long long)0xAA<<23;
    temp_b_code[1] = temp_b_code[1] & 0xFFFFFFFF807FFFFF;
    temp_b_code[1] |= (unsigned long long)0xAA<<23;
    temp_b_code[3] = temp_b_code[3] & 0xFFFFFFFFFFFFC3FF;
    temp_b_code[3] |= (unsigned long long)2<<10;
    temp_b_code[5] = temp_b_code[5] & 0xFFFFFFFFFFFFC3FF;
    temp_b_code[5] |= (unsigned long long)2<<10;
    temp_b_code[6] = temp_b_code[6] & 0xFFFFFFFFFFFFC3FF;
    temp_b_code[6] |= (unsigned long long)2<<10;
#ifdef SET_NOP_NINE
  insert_nop_nine(7);
  for(i=0;i<7*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<7;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  
  case 2:
    temp_b_code[0] = temp_b_code[0] & 0xFFFFFFFF807FFFFF;
    temp_b_code[0] |= (unsigned long long)0x55<<23;
    temp_b_code[1] = temp_b_code[1] & 0xFFFFFFFF807FFFFF;
    temp_b_code[1] |= (unsigned long long)0x55<<23;
    temp_b_code[3] = temp_b_code[3] & 0xFFFFFFFFFFFFC3FF;
    temp_b_code[3] |= (unsigned long long)4<<10;
    temp_b_code[5] = temp_b_code[5] & 0xFFFFFFFFFFFFC3FF;
    temp_b_code[5] |= (unsigned long long)4<<10;
    temp_b_code[6] = temp_b_code[6] & 0xFFFFFFFFFFFFC3FF;
    temp_b_code[6] |= (unsigned long long)4<<10;
#ifdef SET_NOP_NINE
  insert_nop_nine(7);
  for(i=0;i<7*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<7;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  
  case 1:
    temp_b_code[0] = temp_b_code[0] & 0xFFFFFFFF807FFFFF;
    temp_b_code[1] = temp_b_code[1] & 0xFFFFFFFF807FFFFF;
    temp_b_code[3] = temp_b_code[3] & 0xFFFFFFFFFFFFC3FF;
    temp_b_code[3] |= (unsigned long long)8<<10;
    temp_b_code[5] = temp_b_code[5] & 0xFFFFFFFFFFFFC3FF;
    temp_b_code[5] |= (unsigned long long)8<<10;
    temp_b_code[6] = temp_b_code[6] & 0xFFFFFFFFFFFFC3FF;
    temp_b_code[6] |= (unsigned long long)8<<10;
#ifdef SET_NOP_NINE
  insert_nop_nine(7);
  for(i=0;i<7*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<7;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  
    break;
  default:
    perr("Sin inlinefunction error\n");    
  }
#else
  temp_b_code[0]=16;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi2;
  temp_b_code[0]=temp_b_code[0]<<12;
  temp_b_code[0]|=remain_regi3;
  temp_b_code[0]=temp_b_code[0]<<24;
  temp_b_code[0]|=0x1b1b00;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

//if(angle>pi) then angle -= 2*pi
  temp_b_code[1]=26;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi2;
  temp_b_code[1]=temp_b_code[1]<<16;
#ifdef SET_NOP_NINE
  temp_b_code[1]|=(unsigned long long)6;
#else
  temp_b_code[1]|=(unsigned long long)3;
#endif
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=remain_regi2;
  temp_b_code[1]=temp_b_code[1]<<10;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=temp_b_code[1]&0x07FF80007FFFFFFF;
  temp_b_code[2]|=(unsigned long long)24<<59;
#ifdef SET_NOP_NINE
  temp_b_code[2]|=(unsigned long long)4<<31;
#else
  temp_b_code[2]|=(unsigned long long)2<<31;
#endif

  temp_b_code[3]=temp_b_code[0]&0xF80001FE000003FF;
  temp_b_code[3]|=(unsigned long long)remain_regi<<53;
  temp_b_code[3]|=(unsigned long long)remain_regi3<<47;
  temp_b_code[3]|=(unsigned long long)remain_regi<<41;
  temp_b_code[3]|=(unsigned long long)0x551b<<17;//wwww
  temp_b_code[3]|=1<<15; //neg
  temp_b_code[3]|=1<<10; //writed mask


  temp_b_code[4] = temp_b_code[0]&0xFFFFFFFFFFFF7FFF;
  temp_b_code[4] |= 1<<15; //enable neg 

  temp_b_code[5] = temp_b_code[1]&0x07FF80007FFFFFFF;
  temp_b_code[5]|=(unsigned long long)27<<59;
#ifdef SET_NOP_NINE
  temp_b_code[5]|=(unsigned long long)8<<31;
#else
  temp_b_code[5]|=(unsigned long long)4<<31;
#endif
  temp_b_code[6] = temp_b_code[2]&0xFFFF80007FFFFFFF;
#ifdef SET_NOP_NINE
  temp_b_code[6]|=(unsigned long long)6<<31;
#else
  temp_b_code[6]|=(unsigned long long)3<<31;
#endif
  temp_b_code[7] = temp_b_code[0]&0xF8007FFE000003FF;
  temp_b_code[7]|=(unsigned long long)remain_regi<<53;
  temp_b_code[7]|=(unsigned long long)remain_regi<<47;
  temp_b_code[7]|=0x1b55<<17;//wwww
  temp_b_code[7]|=1<<10; //writed mask
  temp_b_code[8] = temp_b_code[0]&0xF80001FE000003FF;
  temp_b_code[8]|=(unsigned long long)remain_regi<<53;
  temp_b_code[8]|=(unsigned long long)remain_regi<<41;
  temp_b_code[8]|=0x1b1b<<17;//wwww
  temp_b_code[8]|=1<<10; //writed mask
  temp_b_code[8]|=1<<15; //writed mask

  switch(type){
  case 4:
#ifdef SET_NOP_NINE
  insert_nop_nine(9);
  for(i=0;i<9*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<9;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  
  case 3:
    temp_b_code[1]=temp_b_code[1]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[1]|=2<<10;
    temp_b_code[2]=temp_b_code[2]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[2]|=2<<10;
    temp_b_code[3]=temp_b_code[3]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[3]|=2<<10;
    temp_b_code[5]=temp_b_code[5]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[5]|=2<<10;
    temp_b_code[6]=temp_b_code[6]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[6]|=2<<10;
    temp_b_code[7]=temp_b_code[7]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[7]|=2<<10;
    temp_b_code[8]=temp_b_code[8]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[8]|=2<<10;
#ifdef SET_NOP_NINE
  insert_nop_nine(9);
  for(i=0;i<9*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<9;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  
  case 2:
    temp_b_code[1]=temp_b_code[1]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[1]|=4<<10;
    temp_b_code[2]=temp_b_code[2]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[2]|=4<<10;
    temp_b_code[3]=temp_b_code[3]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[3]|=4<<10;
    temp_b_code[5]=temp_b_code[5]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[5]|=4<<10;
    temp_b_code[6]=temp_b_code[6]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[6]|=4<<10;
    temp_b_code[7]=temp_b_code[7]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[7]|=4<<10;
    temp_b_code[8]=temp_b_code[8]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[8]|=4<<10;

#ifdef SET_NOP_NINE
  insert_nop_nine(9);
  for(i=0;i<9*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<9;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  
  case 1:
    temp_b_code[1]=temp_b_code[1]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[1]|=8<<10;
    temp_b_code[2]=temp_b_code[2]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[2]|=8<<10;
    temp_b_code[3]=temp_b_code[3]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[3]|=8<<10;
    temp_b_code[5]=temp_b_code[5]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[5]|=8<<10;
    temp_b_code[6]=temp_b_code[6]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[6]|=8<<10;
    temp_b_code[7]=temp_b_code[7]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[7]|=8<<10;
    temp_b_code[8]=temp_b_code[8]&0xFFFFFFFFFFFFC3FF;
    temp_b_code[8]|=8<<10;

#ifdef SET_NOP_NINE
  insert_nop_nine(9);
  for(i=0;i<9*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<9;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif  
  break;
  default:
    perr("Sin inlinefunction error\n");
    break;

  }
#endif


  //Taylor series
  //assembly
  if(assembly_string!=NULL) //angle is expressed as radians.
    {
      //1sprintf(assembly_string,"%s\tLDIF R%d.1%d%d%d, 0.017453\n",assembly_string,remain_regi,type>1?1:0,type>2?1:0,type>3?1:0);//0
      sprintf(assembly_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",assembly_string,OUR_TEMP_REGI,remain_regi);
      sprintf(assembly_string,"%s\tLDIF R%d.1%d%d%d, 1.000000\n",assembly_string,remain_regi,type>1?1:0,type>2?1:0,type>3?1:0);//0
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",
	      assembly_string,OUR_TEMP_REGI,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",
	      assembly_string,remain_regi,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,OUR_TEMP_REGI);
      sprintf(assembly_string,"%s\tLDIF R%d.1000, 362880.0\n",//3
	      assembly_string,remain_regi2);
      sprintf(assembly_string,"%s\tLDIF R%d.0100, 60480.0\n",
	      assembly_string,remain_regi2);
      sprintf(assembly_string,"%s\tLDIF R%d.0010, 3024.0\n",
	      assembly_string,remain_regi2);
      sprintf(assembly_string,"%s\tLDIF R%d.0001, 72.0\n",
	      assembly_string,remain_regi2);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xxxx, <4>\n",//7
	      assembly_string,remain_regi3,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi2);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",
	      assembly_string,OUR_TEMP_REGI,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.yyyy, <4>\n",
	      assembly_string,remain_regi4,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, -R%d.xyzw, <4>\n",//10
	      assembly_string,remain_regi3,type>1?1:0,type>2?1:0,type>3?1:0,remain_regi3,remain_regi4);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",
	      assembly_string,OUR_TEMP_REGI,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.zzzz, <4>\n",
	      assembly_string,remain_regi4,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",//13
	      assembly_string,remain_regi3,type>1?1:0,type>2?1:0,type>3?1:0,remain_regi3,remain_regi4);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",
	      assembly_string,OUR_TEMP_REGI,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.wwww, <4>\n",
	      assembly_string,remain_regi4,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, -R%d.xyzw, <4>\n",//16
	      assembly_string,remain_regi3,type>1?1:0,type>2?1:0,type>3?1:0,remain_regi3,remain_regi4);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",
	      assembly_string,remain_regi4,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi);
      sprintf(assembly_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",//18
	      assembly_string,remain_regi3,type>1?1:0,type>2?1:0,type>3?1:0,remain_regi3,remain_regi4);
      sprintf(assembly_string,"%s\tRCP R%d.1111, R%d.xyzw, <4>\n",
	      assembly_string,remain_regi2,remain_regi2);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",
	      assembly_string,OUR_TEMP_REGI,type>1?1:0,type>2?1:0,type>3?1:0,remain_regi2,remain_regi3);
      sprintf(assembly_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",
	      assembly_string,return_regi,input_angle_regi);
      sprintf(assembly_string,"%s\tMVR R%d.1%d%d%d, R%d.xyzw, <4>\n",
	      assembly_string,return_regi,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI);
    }

  fputs(assembly_string,ass_file);
  bzero(assembly_string,sizeof(assembly_string));
  
  fflush(ass_file);
  if(!output_ass)
    fclose(ass_file);

  MVR_instr( 3 , 16, 0X1b1b, 0X1b1b, 0 , 15 , 3 ,0);  //Kaymon add thisinstr.
#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
  //binary
  
  temp_float=0.017453;
  temp_float=1.0;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=temp_wm;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  
  temp_b_code[1]=17;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=OUR_TEMP_REGI;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=OUR_TEMP_REGI;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=0x1B1B;
  temp_b_code[1]=temp_b_code[1]<<7;
  temp_b_code[1]|=temp_wm;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=temp_b_code[1];
  temp_b_code[2]&=0xF80001FFFFFFFFFFLL;
  temp_b_code[2]|=(unsigned long long)(remain_regi)<<53;
  temp_b_code[2]|=(unsigned long long)(OUR_TEMP_REGI)<<47;
  temp_b_code[2]|=(unsigned long long)(OUR_TEMP_REGI)<<41;
    
  temp_float=362880.0;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[3]=temp_b_code[0];
  temp_b_code[3]&=0xF81F8000000043FFLL;
  temp_b_code[3]|=(unsigned long long)(remain_regi2)<<53;
  temp_b_code[3]|=(unsigned long long)(temp_int)<<15;
  temp_b_code[3]|=(unsigned long long)8<<10;

  temp_float=60480.0;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[4]=temp_b_code[3];
  temp_b_code[4]&=0xFFFF8000000043FFLL;
  temp_b_code[4]|=(unsigned long long)(temp_int)<<15;
  temp_b_code[4]|=(unsigned long long)4<<10;
  
  temp_float=3024.0;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[5]=temp_b_code[3];
  temp_b_code[5]&=0xFFFF8000000043FFLL;
  temp_b_code[5]|=(unsigned long long)(temp_int)<<15;
  temp_b_code[5]|=(unsigned long long)2<<10;

  temp_float=72.0;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[6]=temp_b_code[3];
  temp_b_code[6]&=0xFFFF8000000043FFLL;
  temp_b_code[6]|=(unsigned long long)(temp_int)<<15;
  temp_b_code[6]|=(unsigned long long)1<<10;
      
  temp_b_code[7]=temp_b_code[2];
  temp_b_code[7]&=0xF81F81FFFE01FFFFLL;
  temp_b_code[7]|=(unsigned long long)(remain_regi3)<<53;
  temp_b_code[7]|=(unsigned long long)(remain_regi2)<<41;

  temp_b_code[8]=temp_b_code[1];
  temp_b_code[8]&=0xFFE07FFFFFFFFFFFLL;
  temp_b_code[8]|=(unsigned long long)(OUR_TEMP_REGI)<<47;

  temp_b_code[9]=temp_b_code[1];
  temp_b_code[9]&=0xF80001FFFE01FFFFLL;
  temp_b_code[9]|=(unsigned long long)(remain_regi4)<<53;
  temp_b_code[9]|=(unsigned long long)(OUR_TEMP_REGI)<<47;
  temp_b_code[9]|=(unsigned long long)(remain_regi2)<<41;
  temp_b_code[9]|=(unsigned long long)(0x55)<<17;

  temp_b_code[10]=16;
  temp_b_code[10]=temp_b_code[10]<<6;
  temp_b_code[10]|=remain_regi3;
  temp_b_code[10]=temp_b_code[10]<<6;
  temp_b_code[10]|=remain_regi3;
  temp_b_code[10]=temp_b_code[10]<<6;
  temp_b_code[10]|=remain_regi4;
  temp_b_code[10]=temp_b_code[10]<<24;
  temp_b_code[10]|=0x1B1B1B;
  temp_b_code[10]=temp_b_code[10]<<3;      
  temp_b_code[10]|=2;
  temp_b_code[10]=temp_b_code[10]<<4;
  temp_b_code[10]|=temp_wm;
  temp_b_code[10]=temp_b_code[10]<<1;
  temp_b_code[10]|=1;
  temp_b_code[10]=temp_b_code[10]<<2;
  temp_b_code[10]|=3;
  temp_b_code[10]=temp_b_code[10]<<7;

  temp_b_code[11]=temp_b_code[8];

  temp_b_code[12]=temp_b_code[9];
  temp_b_code[12]&=0xFFFFFFFFFE01FFFFLL;
  temp_b_code[12]|=(unsigned long long)(0xAA)<<17;
      
  temp_b_code[13]=temp_b_code[10];
  temp_b_code[13]&=0xFFFFFFFFFFFE3FFFLL;
      
  temp_b_code[14]=temp_b_code[8];

  temp_b_code[15]=temp_b_code[9];
  temp_b_code[15]&=0xFFFFFFFFFE01FFFFLL;
  temp_b_code[15]|=(unsigned long long)(0xFF)<<17;

  temp_b_code[16]=temp_b_code[10];

  temp_b_code[17]=temp_b_code[8];
  temp_b_code[17]&=0xF81FFFFFFFFFFFFFLL;
  temp_b_code[17]|=(unsigned long long)(remain_regi4)<<53;

  temp_b_code[18]=temp_b_code[13];

  temp_b_code[19]=2;
  temp_b_code[19]=temp_b_code[19]<<6;
  temp_b_code[19]|=remain_regi2;
  temp_b_code[19]=temp_b_code[19]<<6;
  temp_b_code[19]|=remain_regi2;
  temp_b_code[19]=temp_b_code[19]<<22;
  temp_b_code[19]|=0x1B1B;
  temp_b_code[19]=temp_b_code[19]<<15;      
  temp_b_code[19]|=temp_wm;
  temp_b_code[19]=temp_b_code[19]<<1;
  temp_b_code[19]|=1;
  temp_b_code[19]=temp_b_code[19]<<9;

  temp_b_code[20]=temp_b_code[1];
  temp_b_code[20]&=0xF80001FFFFFFFFFFLL;
  temp_b_code[20]|=(unsigned long long)(OUR_TEMP_REGI)<<53;
  temp_b_code[20]|=(unsigned long long)(remain_regi2)<<47;
  temp_b_code[20]|=(unsigned long long)(remain_regi3)<<41;

  temp_b_code[21]=7;
  temp_b_code[21]=temp_b_code[21]<<6;
  temp_b_code[21]|=return_regi;
  temp_b_code[21]=temp_b_code[21]<<6;
  temp_b_code[21]|=input_angle_regi;
  temp_b_code[21]=temp_b_code[21]<<22;
  temp_b_code[21]|=0x1B1B;
  temp_b_code[21]=temp_b_code[21]<<15;      
  temp_b_code[21]|=15;
  temp_b_code[21]=temp_b_code[21]<<3;
  temp_b_code[21]|=3;
  temp_b_code[21]=temp_b_code[21]<<7;

  temp_b_code[22]=temp_b_code[21];
  temp_b_code[22]&=0xFFE07FFFFFFFC3FFLL;
  temp_b_code[22]|=(unsigned long long)(OUR_TEMP_REGI)<<47;
  temp_b_code[22]|=(unsigned long long)(temp_wm)<<10;
  

#ifdef SET_NOP_NINE
  insert_nop_nine(23);
  for(i=0;i<23*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<23;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),23+instruction_count,output_file);  
#endif

#ifdef SET_NOP_NINE
  //printf("sin instr count:%d\n",(23+instruction_count)*2-1);
  return (23+instruction_count)*2-1;
#else
  return 23+instruction_count;
#endif

}



int GL_Cos(int input_angle_regi,
	   int remain_regi,
	   int remain_regi2,
	   int remain_regi3,
	   int remain_regi4,
	   int remain_regi5,
	   int return_regi,
	   int type,
	   char *assembly_string,
	   FILE *output_ass)
{
  int i;
  int instruction_count = 0;
  float temp_float;
  unsigned int temp_int;
  unsigned int temp_wm=8;
  FILE *ass_file;

  if(type>1)
    {  
      temp_wm|=4;
      if(type>2)
	{
	  temp_wm|=2;
	  if(type>3)
	    {
	      temp_wm|=1;
	    }
	}
    }

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"Cos_R%d_R%d_R%d_R%d_R%d_R%d_R%d_%d_ass_file",
	      input_angle_regi,remain_regi,remain_regi2,remain_regi3,
	      remain_regi4, remain_regi5, return_regi,type);
      ass_file=fopen(temp_string,"w");
    }

  //angle = 90 - angle
  //assembly
  if(assembly_string!=NULL)
    {
      sprintf(assembly_string,"");
      sprintf(assembly_string,"%s\tLDIF R%d.1%d%d%d, 1.5707963268\n",assembly_string,remain_regi,type>1?1:0,type>2?1:0,type>3?1:0);
      sprintf(assembly_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, -R%d.xyzw, <4>\n",assembly_string,remain_regi5,type>1?1:0,type>2?1:0,type>3?1:0,remain_regi,input_angle_regi);
    }

  fputs(assembly_string,ass_file);
  bzero(assembly_string,sizeof(assembly_string));
  
  fflush(ass_file);


  //binary
  temp_float=1.5707963268;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=temp_wm;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  instruction_count++;

  temp_b_code[1]=16;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi5;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=input_angle_regi;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=0x1B1B1B;
  temp_b_code[1]=temp_b_code[1]<<3;      
  temp_b_code[1]|=2;
  temp_b_code[1]=temp_b_code[1]<<4;
  temp_b_code[1]|=temp_wm;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<7;
  instruction_count++;

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

  //Sin(90-angle)
  instruction_count+=GL_Sin(remain_regi5,remain_regi,remain_regi2,remain_regi3,remain_regi4,return_regi,type,assembly_string,output_ass);

  if(!output_ass)
    fclose(ass_file);

#ifdef SET_NOP_NINE
  return instruction_count*2-1;
#else
  return instruction_count;
#endif

}

/* Taylor series cos
int GL_Cos(int input_angle_regi,
	   int remain_regi,
	   int remain_regi2,
	   int remain_regi3,
	   int remain_regi4,
	   int return_regi,
	   int type,
	   char *assembly_string)
{
  int i;

  if(assembly_string!=NULL)
    {
      sprintf(assembly_string,"%s\tLDIF R%d.1%d%d%d, 0.017453\n",//0
	      assembly_string,remain_regi,type>1?1:0,type>2?1:0,type>3?1:0);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",
	      assembly_string,OUR_TEMP_REGI,type>1?1:0,type>2?1:0,type>3?1:0,input_angle_regi,remain_regi);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",
	      assembly_string,remain_regi,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,OUR_TEMP_REGI);
      sprintf(assembly_string,"%s\tMVR R%d.1%d%d%d, R%d.xyzw, <4>\n",
	      assembly_string,OUR_TEMP_REGI,type>1?1:0,type>2?1:0,type>3?1:0,remain_regi);
      sprintf(assembly_string,"%s\tLDIF R%d.1000, 3628800.0\n",//4
	      assembly_string,remain_regi2);
      sprintf(assembly_string,"%s\tLDIF R%d.0100, 1814400.0\n",
	      assembly_string,remain_regi2);
      sprintf(assembly_string,"%s\tLDIF R%d.0010, 151200.0\n",
	      assembly_string,remain_regi2);
      sprintf(assembly_string,"%s\tLDIF R%d.0001, 5040.0\n",
	      assembly_string,remain_regi2);
      sprintf(assembly_string,"%s\tMVR R%d.1%d%d%d, R%d.xxxx, <4>\n",//8
	      assembly_string,remain_regi3,type>1?1:0,type>2?1:0,type>3?1:0,remain_regi2);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.yyyy, <4>\n",
	      assembly_string,remain_regi4,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, -R%d.xyzw, <4>\n",//10
	      assembly_string,remain_regi3,type>1?1:0,type>2?1:0,type>3?1:0,remain_regi3,remain_regi4);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",
	      assembly_string,OUR_TEMP_REGI,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.zzzz, <4>\n",
	      assembly_string,remain_regi4,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",//13
	      assembly_string,remain_regi3,type>1?1:0,type>2?1:0,type>3?1:0,remain_regi3,remain_regi4);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",
	      assembly_string,OUR_TEMP_REGI,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.wwww, <4>\n",
	      assembly_string,remain_regi4,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, -R%d.xyzw, <4>\n",//16
	      assembly_string,remain_regi3,type>1?1:0,type>2?1:0,type>3?1:0,remain_regi3,remain_regi4);
      sprintf(assembly_string,"%s\tLDIF R%d.0100, 90.0\n",//17
	      assembly_string,remain_regi2);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",//18
	      assembly_string,OUR_TEMP_REGI,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.yyyy, <4>\n",//19 
	      assembly_string,remain_regi4,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi2);
      sprintf(assembly_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",//20
	      assembly_string,remain_regi3,type>1?1:0,type>2?1:0,type>3?1:0,remain_regi3,remain_regi4);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",  //21
	      assembly_string,remain_regi4,type>1?1:0,type>2?1:0,type>3?1:0,OUR_TEMP_REGI,remain_regi);
      sprintf(assembly_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, -R%d.xyzw, <4>\n",//22
	      assembly_string,remain_regi3,type>1?1:0,type>2?1:0,type>3?1:0,remain_regi3,remain_regi4);
      sprintf(assembly_string,"%s\tRCP R%d.1111, R%d.xyzw, <4>\n",  //23
	      assembly_string,remain_regi2,remain_regi2);
      sprintf(assembly_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <4>\n",  //24
	      assembly_string,return_regi,type>1?1:0,type>2?1:0,type>3?1:0,remain_regi2,remain_regi3);

    }

  float temp_float;
  unsigned int temp_int;
  unsigned int temp_wm=8;
  
  if(type>1)
    {  
      temp_wm|=4;
      if(type>2)
	{
	  temp_wm|=2;
	  if(type>3)
	    {
	      temp_wm|=1;
	    }
	}
    }

  temp_float=0.017453;
  memcpy(&temp_int,&temp_float,4);
  
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=temp_wm;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=17;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=OUR_TEMP_REGI;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=input_angle_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=0x1B1B;
  temp_b_code[1]=temp_b_code[1]<<7;
  temp_b_code[1]|=temp_wm;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=temp_b_code[1];
  temp_b_code[2]&=0xF80001FFFFFFFFFFLL;
  temp_b_code[2]|=(unsigned long long)(remain_regi)<<53;
  temp_b_code[2]|=(unsigned long long)(OUR_TEMP_REGI)<<47;
  temp_b_code[2]|=(unsigned long long)(OUR_TEMP_REGI)<<41;

  temp_b_code[3]=7;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=OUR_TEMP_REGI;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=remain_regi;
  temp_b_code[3]=temp_b_code[3]<<22;
  temp_b_code[3]|=0x1B1B;
  temp_b_code[3]=temp_b_code[3]<<15;
  temp_b_code[3]|=temp_wm;
  temp_b_code[3]=temp_b_code[3]<<3;
  temp_b_code[3]|=3;
  temp_b_code[3]=temp_b_code[3]<<7;

  temp_float=3628800.0;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[4]=temp_b_code[0];
  temp_b_code[4]&=0xF81F800000007FFFLL;
  temp_b_code[4]|=(unsigned long long)(remain_regi2)<<53;
  temp_b_code[4]|=(unsigned long long)(temp_int)<<15;

  temp_float=1814400.0;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[5]=temp_b_code[4];
  temp_b_code[5]&=0xFFFF800000007FFFLL;
  temp_b_code[5]|=(unsigned long long)(temp_int)<<15;
  
  temp_float=151200.0;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[6]=temp_b_code[4];
  temp_b_code[6]&=0xFFFF800000007FFFLL;
  temp_b_code[6]|=(unsigned long long)(temp_int)<<15;
  
  temp_float=5040.0;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[7]=temp_b_code[4];
  temp_b_code[7]&=0xFFFF800000007FFFLL;
  temp_b_code[7]|=(unsigned long long)(temp_int)<<15;

  temp_b_code[8]=temp_b_code[3];
  temp_b_code[8]&=0xF8007FFE01FFFFFFLL;
  temp_b_code[8]|=(unsigned long long)(remain_regi3)<<53;
  temp_b_code[8]|=(unsigned long long)(remain_regi2)<<47;

  temp_b_code[9]=temp_b_code[1];
  temp_b_code[9]&=0xF80001FFFE01FFFFLL;
  temp_b_code[9]|=(unsigned long long)(remain_regi4)<<53;
  temp_b_code[9]|=(unsigned long long)(OUR_TEMP_REGI)<<47;
  temp_b_code[9]|=(unsigned long long)(remain_regi2)<<41;
  temp_b_code[9]|=(unsigned long long)(0x55)<<17;

  temp_b_code[10]=16;
  temp_b_code[10]=temp_b_code[10]<<6;
  temp_b_code[10]|=remain_regi3;
  temp_b_code[10]=temp_b_code[10]<<6;
  temp_b_code[10]|=remain_regi3;
  temp_b_code[10]=temp_b_code[10]<<6;
  temp_b_code[10]|=remain_regi4;
  temp_b_code[10]=temp_b_code[10]<<24;
  temp_b_code[10]|=0x1B1B1B;
  temp_b_code[10]=temp_b_code[10]<<3;      
  temp_b_code[10]|=2;
  temp_b_code[10]=temp_b_code[10]<<4;
  temp_b_code[10]|=temp_wm;
  temp_b_code[10]=temp_b_code[10]<<1;
  temp_b_code[10]|=1;
  temp_b_code[10]=temp_b_code[10]<<2;
  temp_b_code[10]|=3;
  temp_b_code[10]=temp_b_code[10]<<7;
  
  temp_b_code[11]=temp_b_code[1];
  temp_b_code[11]&=0xFFE07FFFFFFFFFFFLL;
  temp_b_code[11]|=(unsigned long long)(OUR_TEMP_REGI)<<47;
  
  temp_b_code[12]=temp_b_code[9];
  temp_b_code[12]&=0xFFFFFFFFFE01FFFFLL;
  temp_b_code[12]|=(unsigned long long)(0xAA)<<17;
      
  temp_b_code[13]=temp_b_code[10];
  temp_b_code[13]&=0xFFFFFFFFFFFE3FFFLL;
      
  temp_b_code[14]=temp_b_code[11];

  temp_b_code[15]=temp_b_code[9];
  temp_b_code[15]&=0xFFFFFFFFFE01FFFFLL;
  temp_b_code[15]|=(unsigned long long)(0xFF)<<17;

  temp_b_code[16]=temp_b_code[10];

  temp_float=90.0;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[17]=temp_b_code[5];
  temp_b_code[17]&=0xFFFF800000007FFFLL;
  temp_b_code[17]|=(unsigned long long)(temp_int)<<15;

  temp_b_code[18]=temp_b_code[14];

  temp_b_code[19]=temp_b_code[9];

  temp_b_code[20]=temp_b_code[13];

  temp_b_code[21]=temp_b_code[11];
  temp_b_code[21]&=0xF81FFFFFFFFFFFFFLL;
  temp_b_code[21]|=(unsigned long long)(remain_regi4)<<53;

  temp_b_code[22]=temp_b_code[16];

  temp_b_code[23]=2;
  temp_b_code[23]=temp_b_code[23]<<6;
  temp_b_code[23]|=remain_regi2;
  temp_b_code[23]=temp_b_code[23]<<6;
  temp_b_code[23]|=remain_regi2;
  temp_b_code[23]=temp_b_code[23]<<22;
  temp_b_code[23]|=0x1B1B;
  temp_b_code[23]=temp_b_code[23]<<15;      
  temp_b_code[23]|=temp_wm;
  temp_b_code[23]=temp_b_code[23]<<1;
  temp_b_code[23]|=1;
  temp_b_code[23]=temp_b_code[23]<<9;

  temp_b_code[24]=temp_b_code[1];
  temp_b_code[24]&=0xF80001FFFFFFFFFFLL;
  temp_b_code[24]|=(unsigned long long)(return_regi)<<53;
  temp_b_code[24]|=(unsigned long long)(remain_regi2)<<47;
  temp_b_code[24]|=(unsigned long long)(remain_regi3)<<41;


#ifdef SET_NOP_NINE
  insert_nop_nine(21);
  for(i=0;i<24*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<24;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 24*2-1;
#else
  return 24;
#endif

}
*/

int GL_Tan(int input_angle_regi,
	   int remain_regi,
	   int remain_regi2,
	   int remain_regi3,
	   int remain_regi4,
	   int remain_regi5,
	   int return_regi,
	   int type,
	   char* assembly_string,
	   FILE *output_ass)
{
  FILE *ass_file;
  int instruction_count = 0,i;
  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"Tan_R%d_R%d_R%d_R%d_R%d_R%d_R%d_%d_ass_file",
	      input_angle_regi,remain_regi,remain_regi2,remain_regi3,
	      remain_regi4, remain_regi5, return_regi,type);
      ass_file=fopen(temp_string,"w");
    }
  sprintf(temp_string," ");

  instruction_count+=GL_Cos(input_angle_regi,remain_regi,remain_regi2,remain_regi3,remain_regi4,remain_regi5,return_regi,type,assembly_string,output_ass);

  
  MVR_instr( remain_regi5,return_regi, 0x1b, 0x1b, 0, 15, type ,0);
#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
  instruction_count++;

  instruction_count+=GL_Sin(input_angle_regi,remain_regi,remain_regi2,remain_regi3,remain_regi4,return_regi,type,assembly_string,output_ass);

  temp_b_code[0]=6;//DIV remain_regi5 return_regi
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi5;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1b1b;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<9;


#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

  fputs(assembly_string,ass_file);
  bzero(assembly_string,sizeof(assembly_string));
  
  fflush(ass_file);  
}

int GL_CallRegister();

int inline_fneg(int input_Rd,
		int input_Rs,
		FILE *output_ass)
{
  int i;
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"inline_fneg_R%d_R%d_ass_file",input_Rd,input_Rs);
      ass_file=fopen(temp_string,"w");
    }


  sprintf(temp_string," ");
  /*
  sprintf(temp_string,"%s\tMVR R%d.1111, -R%d.xyzw, <4>\n",temp_string,input_Rd,input_Rs);
  //printf("\tMVR R%d.1111, -R%d.1111, <4>\n",input_Rd,input_Rs);


  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=input_Rd;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=input_Rs;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<8;
  temp_b_code[0]|=0x1b;                //xywz
  temp_b_code[0]=temp_b_code[0]<<8;
  temp_b_code[0]|=17;                  //xyxy but only use the first element. It doesn't matter.
  temp_b_code[0]=temp_b_code[0]<<8;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<4;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[0]|=0;
  */
  sprintf(temp_string,"%s\tADDF R16.1111,R0.xyzw, -R%d.xyzw, <4>\n",temp_string,input_Rd,input_Rs);
  //printf("\tMVR R%d.1111, -R%d.1111, <4>\n",input_Rd,input_Rs);


  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  temp_b_code[0]=16;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=input_Rd;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=input_Rs;
  temp_b_code[0]=temp_b_code[0]<<8;
  temp_b_code[0]|=0x1b;                //xywz
  temp_b_code[0]=temp_b_code[0]<<8;
  temp_b_code[0]|=0x1b;                  //xyxy but only use the first element. It doesn't matter.
  temp_b_code[0]=temp_b_code[0]<<8;
  temp_b_code[0]|=0x1b;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<4;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=7;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[0]|=0;
#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  //  for(i=0;i<3;i++)
  //BinCode[LastBinLine++]=temp_b_code[i];
  BinCode[LastBinLine++]=temp_b_code[0];
#endif
  return 1;
}

int inline_insertelement(int input_Rd,
			 int input_Rs,
			 int input_Rt,
			 int swi,
			 int SV,
			 FILE *output_ass)
{
  int i;
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"inline_insertelement_R%d_R%d_ass_file",input_Rd,input_Rs);
      ass_file=fopen(temp_string,"w");
    }
  char swizzle2[4][4][5];
  int swizzle3[4][4];
  strcpy(swizzle2[0][0],"xyzw"); swizzle3[0][0] = 0x1b;  
  strcpy(swizzle2[1][0],"xxzw"); swizzle3[1][0] = 0x0b;  
  strcpy(swizzle2[2][0],"xyxw"); swizzle3[2][0] = 0x13;  
  strcpy(swizzle2[3][0],"xyzx"); swizzle3[3][0] = 0x18;  

  strcpy(swizzle2[0][1],"yyzw"); swizzle3[0][1] = 0x5b;
  strcpy(swizzle2[1][1],"xyzw"); swizzle3[1][1] = 0x1b;
  strcpy(swizzle2[2][1],"xyyw"); swizzle3[2][1] = 0x17;
  strcpy(swizzle2[3][1],"xyzy"); swizzle3[3][1] = 0x19;

  strcpy(swizzle2[0][2],"zyzw"); swizzle3[0][2] = 0x9b;
  strcpy(swizzle2[1][2],"xzzw"); swizzle3[1][2] = 0x2b;
  strcpy(swizzle2[2][2],"xyzw"); swizzle3[2][2] = 0x1b;
  strcpy(swizzle2[3][2],"xyzz"); swizzle3[3][2] = 0x1a;

  strcpy(swizzle2[0][3],"wyzw"); swizzle3[0][3] = 0xdb;
  strcpy(swizzle2[1][3],"xwzw"); swizzle3[1][3] = 0x3b;
  strcpy(swizzle2[2][3],"xyww"); swizzle3[2][3] = 0x1f;
  strcpy(swizzle2[3][3],"xyzw"); swizzle3[3][3] = 0x1b;

  sprintf(temp_string," ");
  
  int swizzle[4]={0};
  int index = 4;
  while( index ){
    swizzle[index-1] = swi & 0x03;
    //printf("swi[%d] : %d\n",index-1,swizzle[index-1]);
    swi = swi >> 2;
    index--;
  }


  index = 0;
  while(index < SV){
    switch(swizzle[index]){
    case 0:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.%s, <%d>\n",temp_string,input_Rd,input_Rs,swizzle2[0][index],SV);
      break;
    case 1:
      sprintf(temp_string,"%s\tMVR R%d.0100, R%d.%s, <%d>\n",temp_string,input_Rd,input_Rs,swizzle2[1][index],SV);
      break;
    case 2:
      sprintf(temp_string,"%s\tMVR R%d.0010, R%d.%s, <%d>\n",temp_string,input_Rd,input_Rs,swizzle2[2][index],SV);
      break;
    case 3:
      sprintf(temp_string,"%s\tMVR R%d.0001, R%d.%s, <%d>\n",temp_string,input_Rd,input_Rs,swizzle2[3][index],SV);
      break ;
    }
    index++;
  }
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  index= 0;
  while(index<SV){
    switch(swizzle[index]){
    case 0:
      temp_b_code[index]=7;
      temp_b_code[index]=temp_b_code[index]<<6;
      temp_b_code[index]|=input_Rd;
      temp_b_code[index]=temp_b_code[index]<<6;
      temp_b_code[index]|=input_Rs;
      temp_b_code[index]=temp_b_code[index]<<6;
      temp_b_code[index]|=0;
      temp_b_code[index]=temp_b_code[index]<<8;
      temp_b_code[index]|=0x1b;
      temp_b_code[index]=temp_b_code[index]<<8;
      temp_b_code[index]|=swizzle3[0][index];
      temp_b_code[index]=temp_b_code[index]<<8;
      temp_b_code[index]|=0;          
      temp_b_code[index]=temp_b_code[index]<<1;
      temp_b_code[index]|=0;
      temp_b_code[index]=temp_b_code[index]<<2;
      temp_b_code[index]|=0;
      temp_b_code[index]=temp_b_code[index]<<4;
      temp_b_code[index]|=8;
      temp_b_code[index]=temp_b_code[index]<<3;
      temp_b_code[index]|=SV-1;
      temp_b_code[index]=temp_b_code[index]<<7;
      temp_b_code[index]|=0;
      break;
    case 1:
      temp_b_code[index]=7;
      temp_b_code[index]=temp_b_code[index]<<6;
      temp_b_code[index]|=input_Rd;
      temp_b_code[index]=temp_b_code[index]<<6;
      temp_b_code[index]|=input_Rs;
      temp_b_code[index]=temp_b_code[index]<<6;
      temp_b_code[index]|=0;
      temp_b_code[index]=temp_b_code[index]<<8;
      temp_b_code[index]|=0x1b;
      temp_b_code[index]=temp_b_code[index]<<8;
      temp_b_code[index]|=swizzle3[1][index];
      temp_b_code[index]=temp_b_code[index]<<8;
      temp_b_code[index]|=0;          
      temp_b_code[index]=temp_b_code[index]<<1;
      temp_b_code[index]|=0;
      temp_b_code[index]=temp_b_code[index]<<2;
      temp_b_code[index]|=0;
      temp_b_code[index]=temp_b_code[index]<<4;
      temp_b_code[index]|=4;
      temp_b_code[index]=temp_b_code[index]<<3;
      temp_b_code[index]|=SV-1;
      temp_b_code[index]=temp_b_code[index]<<7;
      temp_b_code[index]|=0;
      break;
    case 2:
      temp_b_code[index]=7;
      temp_b_code[index]=temp_b_code[index]<<6;
      temp_b_code[index]|=input_Rd;
      temp_b_code[index]=temp_b_code[index]<<6;
      temp_b_code[index]|=input_Rs;
      temp_b_code[index]=temp_b_code[index]<<6;
      temp_b_code[index]|=0;
      temp_b_code[index]=temp_b_code[index]<<8;
      temp_b_code[index]|=0x1b;
      temp_b_code[index]=temp_b_code[index]<<8;
      temp_b_code[index]|=swizzle3[2][index];
      temp_b_code[index]=temp_b_code[index]<<8;
      temp_b_code[index]|=0;          
      temp_b_code[index]=temp_b_code[index]<<1;
      temp_b_code[index]|=0;
      temp_b_code[index]=temp_b_code[index]<<2;
      temp_b_code[index]|=0;
      temp_b_code[index]=temp_b_code[index]<<4;
      temp_b_code[index]|=2;
      temp_b_code[index]=temp_b_code[index]<<3;
      temp_b_code[index]|=SV-1;
      temp_b_code[index]=temp_b_code[index]<<7;
      temp_b_code[index]|=0;
      break;
    case 3:
      temp_b_code[index]=7;
      temp_b_code[index]=temp_b_code[index]<<6;
      temp_b_code[index]|=input_Rd;
      temp_b_code[index]=temp_b_code[index]<<6;
      temp_b_code[index]|=input_Rs;
      temp_b_code[index]=temp_b_code[index]<<6;
      temp_b_code[index]|=0;
      temp_b_code[index]=temp_b_code[index]<<8;
      temp_b_code[index]|=0x1b;
      temp_b_code[index]=temp_b_code[index]<<8;
      temp_b_code[index]|=swizzle3[3][index];
      temp_b_code[index]=temp_b_code[index]<<8;
      temp_b_code[index]|=0;          
      temp_b_code[index]=temp_b_code[index]<<1;
      temp_b_code[index]|=0;
      temp_b_code[index]=temp_b_code[index]<<2;
      temp_b_code[index]|=0;
      temp_b_code[index]=temp_b_code[index]<<4;
      temp_b_code[index]|=1;
      temp_b_code[index]=temp_b_code[index]<<3;
      temp_b_code[index]|=SV-1;
      temp_b_code[index]=temp_b_code[index]<<7;
      temp_b_code[index]|=0;
      break;
    }
    index++;
  }
  //printf("swi[0] : %d\n",swizzle[0]);
  //printf("swi[1] : %d\n",swizzle[1]);
  //printf("swi[2] : %d\n",swizzle[2]);
  //printf("swi[3] : %d\n",swizzle[3]);

#ifdef SET_NOP_NINE
  insert_nop_nine(SV);
  for(i=0;i<(SV)*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<SV;i++)
    BinCode[LastBinLine++]=temp_b_code[i];

#endif
#ifdef SET_NOP_NINE
  return (SV)*2-1;
#else
  return SV;
#endif
}
int inline_extractelement(int input_Rd,
			  int input_Rs,
			  int swi,
			  int SV,
			  FILE *output_ass)
{
  int i;
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"inline_extractelement_R%d_R%d_ass_file",input_Rd,input_Rs);
      ass_file=fopen(temp_string,"w");
    }


  sprintf(temp_string," ");
  
  int swizzle[4]={0};
  int index = 4;
  while( index ){
    swizzle[index-1] = swi & 0x03;
    //printf("swi[%d] : %d\n",index-1,swizzle[index-1]);
    swi = swi >> 2;
    index--;
  }

  index = 4;

  int swizzle2[4]={0};
  while( index ){

    switch( swizzle[index-1] ){
    case 0:
      swizzle2[index-1] = 120;
      break;
    case 1:
      swizzle2[index-1] = 121;
      break;
    case 2:
      swizzle2[index-1] = 122;
      break;
    case 3:
      swizzle2[index-1] = 119;
      break;

    }
    //printf("index:%d   %c\n",index-1,swizzle2[index-1]);

    index--;
  }

  
  switch(SV){
  case 4:
    sprintf(temp_string,"%s\tMVR R%d.1111, R%d.%c%c%c%c, <%d>\n",temp_string,input_Rd,input_Rs,swizzle2[0],swizzle2[1],swizzle2[2],swizzle2[3],SV);
    break;
  case 3:
    sprintf(temp_string,"%s\tMVR R%d.1111, R%d.%c%c%cx, <%d>\n",temp_string,input_Rd,input_Rs,swizzle2[0],swizzle2[1],swizzle2[2],SV);
    break;
  case 2:
    sprintf(temp_string,"%s\tMVR R%d.1111, R%d.%c%cxx, <%d>\n",temp_string,input_Rd,input_Rs,swizzle2[0],swizzle2[1],SV);
    break;
  case 1:
    sprintf(temp_string,"%s\tMVR R%d.1111, R%d.%cyxx, <%d>\n",temp_string,input_Rd,input_Rs,swizzle2[0],SV);
    break ;
  }
  
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);
  
  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=input_Rd;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=input_Rs;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<8;
  temp_b_code[0]|=0x1b;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=swizzle[0];          //Fit the first swizzle. 
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=swizzle[1];          //second
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=swizzle[2];          //third
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=swizzle[3];          //fourth
  temp_b_code[0]=temp_b_code[0]<<8;
  temp_b_code[0]|=0;          
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<4;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[0]|=0;
  
  //printf("swi[0] : %d\n",swizzle[0]);
  //printf("swi[1] : %d\n",swizzle[1]);
  //printf("swi[2] : %d\n",swizzle[2]);
  //printf("swi[3] : %d\n",swizzle[3]);

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  //  for(i=0;i<3;i++)
  //BinCode[LastBinLine++]=temp_b_code[i];
  BinCode[LastBinLine++]=temp_b_code[0];
#endif
  return 1;
}

int inline_vsldoi(int input_Rd,
		  int input_Rs,
		  int input_Rt,
		  int input_sa,
		  FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"inline_vsldoi_R%d_R%d_R%d_ass_file",input_Rd,input_Rs,input_Rt);
      ass_file=fopen(temp_string,"w");
    }


  sprintf(temp_string," ");
  if(input_sa==4)
    {
      sprintf(temp_string,"%s\tMVR R%d.1110, R%d.yzww, <4>\n",temp_string, 3, input_Rs);
      sprintf(temp_string,"%s\tMVR R%d.0001, R%d.xyzx, <4>\n",temp_string, 3, input_Rt);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string, input_Rd, 3);
    }
  else if(input_sa==8)
    {
      sprintf(temp_string,"%s\tMVR R%d.1100, R%d.zwzw, <4>\n",temp_string, 3, input_Rs);
      sprintf(temp_string,"%s\tMVR R%d.0011, R%d.xyxy, <4>\n",temp_string, 3, input_Rt);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string, input_Rd, 3);
    }
  else if(input_sa==12)
    {
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.wyzw, <4>\n",temp_string, 3, input_Rs);
      sprintf(temp_string,"%s\tMVR R%d.0111, R%d.xxyz, <4>\n",temp_string, 3, input_Rt);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string, input_Rd, 3);
    }
  else
    {
      perror("inline_vsldoi: input_sa");
    }

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine

  if(input_sa==4)
    {
      temp_b_code[0]=7;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=3;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_Rs;
      temp_b_code[0]=temp_b_code[0]<<22;
      temp_b_code[0]|=7023;
      temp_b_code[0]=temp_b_code[0]<<15;
      temp_b_code[0]|=14;
      temp_b_code[0]=temp_b_code[0]<<3;
      temp_b_code[0]|=3;
      temp_b_code[0]=temp_b_code[0]<<7;
      
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0xFFE07FFE01FFC3FFLL;
      temp_b_code[1]|=(long long unsigned)input_Rt<<47; 
      temp_b_code[1]|=(long long unsigned)24<<25; // swizzle: xyzx  00011000 
      temp_b_code[1]|=(long long unsigned)1<<10; //WM: 0001
    }
  else if(input_sa==8)
    {
      temp_b_code[0]=7;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=3;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_Rs;
      temp_b_code[0]=temp_b_code[0]<<22;
      temp_b_code[0]|=7099;
      temp_b_code[0]=temp_b_code[0]<<15;
      temp_b_code[0]|=12;
      temp_b_code[0]=temp_b_code[0]<<3;
      temp_b_code[0]|=3;
      temp_b_code[0]=temp_b_code[0]<<7;
      
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0xFFE07FFE01FFC3FFLL;
      temp_b_code[1]|=(long long unsigned)input_Rt<<47; 
      temp_b_code[1]|=(long long unsigned)17<<25; // swizzle: xyxy  00010001 
      temp_b_code[1]|=(long long unsigned)3<<10; //WM: 0011
    }
  else if(input_sa==12)
    {
      temp_b_code[0]=7;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=3;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_Rs;
      temp_b_code[0]=temp_b_code[0]<<22;
      temp_b_code[0]|=7131;
      temp_b_code[0]=temp_b_code[0]<<15;
      temp_b_code[0]|=8;
      temp_b_code[0]=temp_b_code[0]<<3;
      temp_b_code[0]|=3;
      temp_b_code[0]=temp_b_code[0]<<7;
      
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0xFFE07FFE01FFC3FFLL;
      temp_b_code[1]|=(long long unsigned)input_Rt<<47; 
      temp_b_code[1]|=(long long unsigned)6<<25; // swizzle: xxyz  00000110 
      temp_b_code[1]|=(long long unsigned)7<<10; //WM: 0111
    }

  temp_b_code[2]=7;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=input_Rd;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=3;
  temp_b_code[2]=temp_b_code[2]<<22;
  temp_b_code[2]|=6939;
  temp_b_code[2]=temp_b_code[2]<<15;
  temp_b_code[2]|=15;
  temp_b_code[2]=temp_b_code[2]<<3;
  temp_b_code[2]|=3;
  temp_b_code[2]=temp_b_code[2]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 3*2-1;
#else
  return 3;
#endif

}

int inline_vmrghw(int input_Rd,
		  int input_Rs,
		  int input_Rt,
		  FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"inline_vmrghw_R%d_R%d_R%d_ass_file",input_Rd,input_Rs,input_Rt);
      ass_file=fopen(temp_string,"w");
    }


  sprintf(temp_string," ");
  sprintf(temp_string,"%s\tMVR R%d.1010, R%d.xxyy, <2>\n",temp_string, 3, input_Rs);
  sprintf(temp_string,"%s\tMVR R%d.0101, R%d.xxyy, <2>\n",temp_string, 3, input_Rt);
  //sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <2>\n",temp_string, input_Rd, 3);
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string, input_Rd, 3); //Kaymon

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=input_Rs;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6917;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=10;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=temp_b_code[0];
  temp_b_code[1]&=(long long unsigned)0xFFE07FFFFFFFC3FFLL;
  temp_b_code[1]|=(long long unsigned)input_Rt<<47; 
  temp_b_code[1]|=(long long unsigned)5<<10; //WM: 0101

  temp_b_code[2]=7;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=input_Rd;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=3;
  temp_b_code[2]=temp_b_code[2]<<22;
  temp_b_code[2]|=6939;
  temp_b_code[2]=temp_b_code[2]<<15;
  temp_b_code[2]|=15;
  temp_b_code[2]=temp_b_code[2]<<3;
  temp_b_code[2]|=3;
  temp_b_code[2]=temp_b_code[2]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 3*2-1;
#else
  return 3;
#endif



}

int inline_SUBF(int input_Rd,int input_Rs,int input_Rt,int input_WM,int input_SV)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"inline_SUB_R%d_R%d_R%d_ass_file",Rd,Rs,Rt);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string," ");
  sprintf(temp_string,"%s\tADDI R%d.%d%d%d%d, R0, 2147483648, <%d>\n",temp_string,OUR_TEMP_REGI,input_WM&8?1:0,input_WM&4?1:0,input_WM&2?1:0,input_WM&1?1:0,input_SV);
  sprintf(temp_string,"%s\tXOR R%d.%d%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,OUR_TEMP_REGI,input_WM&8?1:0,input_WM&4?1:0,input_WM&2?1:0,input_WM&1?1:0,OUR_TEMP_REGI,input_Rt,input_SV);
  sprintf(temp_string,"%s\tADDF R%d.%d%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,input_Rd,input_WM&8?1:0,input_WM&4?1:0,input_WM&2?1:0,input_WM&1?1:0,input_Rs,OUR_TEMP_REGI,input_SV);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  
  fflush(ass_file);

#endif

  fclose(ass_file);

  //machine code

  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=OUR_TEMP_REGI;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<32;
  temp_b_code[0]|=2147483648;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=input_WM;
  temp_b_code[0]=temp_b_code[0]<<1;
  //temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=input_SV;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=23;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=OUR_TEMP_REGI;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=OUR_TEMP_REGI;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=input_Rt;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=6939;
  temp_b_code[1]=temp_b_code[1]<<7;
  temp_b_code[1]|=input_WM;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=input_SV;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=16;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=input_Rd;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=input_Rs;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=OUR_TEMP_REGI;
  temp_b_code[2]=temp_b_code[2]<<24;
  temp_b_code[2]|=6939;
  temp_b_code[2]=temp_b_code[2]<<7;
  temp_b_code[2]|=input_WM;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=input_SV;
  temp_b_code[2]=temp_b_code[2]<<7;


#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 3*2-1;
#else
  return 3;
#endif


}

void insert_nop_nine(int insert_number)
{
  int i;
  for(i=0;i<insert_number;i++)
    {
      temp_b_code_NOP[i*2]=temp_b_code[i];
      temp_b_code_NOP[i*2+1]=nop_nine_instruction;
    }
}

void insert_N_nop9(int insert_number,int number_of_nop)
{
  int i;
  number_of_nop += 1;
  for(i=0;i<insert_number;i++)
    {
      int i2 = number_of_nop-1;
      temp_b_code_NOP[i*number_of_nop]=temp_b_code[i];
      //printf("%d_%d:\n",i,i*number_of_nop);
      temp_b_code_NOP[i*number_of_nop+1]=nop_nine_instruction;
      //printf("%d_%d:\n",i,i*number_of_nop+1);
      while( i2 > 1 ){
	//printf("in_loop\n");
	temp_b_code_NOP[i*number_of_nop+i2]=nop_nine_instruction;
	//printf("%d_%d:\n",i,i*number_of_nop+i2);
	i2--;
      }
    }
}

int GL_VectorTimesScalar(int Vec_regi,
			 int Sca_regi,
			 int bool_float,
			 int Vec_len,
			 FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"VectorTimesScalar_R%d_R%d_R%d_ass_file",RETU_REGI,Vec_regi,Sca_regi);
      ass_file=fopen(temp_string,"w");
    }


  if(Vec_len<1 || Vec_len>4)
    perror("VectorTimesScalar Vec_len");
  if(bool_float==1)
    {
      sprintf(temp_string,"\tMULF R%d.%d%d%d%d, R%d.xyzw, R%d.xxxx, <%d>\n",RETU_REGI,1,Vec_len>1?1:0,Vec_len>2?1:0,Vec_len>3?1:0,Vec_regi,Sca_regi,Vec_len);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
    }
  else
    perror("VectorTimesScalar bool_float");

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code

  temp_b_code[0]=17;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=RETU_REGI;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=Vec_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=Sca_regi;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=0x1B1B;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=(Vec_len>3?15:Vec_len>2?14:Vec_len>1?12:8);
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=(Vec_len-1);
  temp_b_code[0]=temp_b_code[0]<<7;


#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_file);  
#endif

#ifdef SET_NOP_NINE
  return 1*2-1;
#else
  return 1;
#endif
}

int GL_Start(FILE *output_ass,
	     char VOrF)
{
  struct variable_form *p_cur_r_form=&r_form[0];
  int var_count=0,extra_count_for_vector=0;
  int i;
  int j;

  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"Start_ass_file");
      ass_file=fopen(temp_string,"w");
    }

  if(!output_ass)
    fclose(ass_file);

  //Count the number of the variable
  //First save the gl_FragCoord to mem.
  while(1)
  {
    if(p_cur_r_form->next==NULL)
      {
	break;
      }
    else
      {
	var_count++;
	if(p_cur_r_form->vector_size==2)
	  extra_count_for_vector++;
	if(p_cur_r_form->vector_size==3)
	  extra_count_for_vector+=2;
	if(p_cur_r_form->vector_size==4)
	  extra_count_for_vector+=3;
	p_cur_r_form=p_cur_r_form->next;
      }
  }

  //Store the value of the variables into the memory
  
  //printf("VorF:%c regular_variable_address: %d\n",VOrF,regular_variable_address);
  //printf("VorF:%c regular_variable_address: %llx\n",VOrF,regular_variable_address);

  
  temp_b_code[0]=10;         //Set the beginning mememory address to R2(stack regi).
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=OUR_STACK_REGI;
  temp_b_code[0]=temp_b_code[0]<<38;
#ifdef FS_DMEM_SET
  if(VOrF == 'v')
    temp_b_code[0]|=regular_variable_address;//31 might be better than 32.(Because regular_variable_address indicate the empty address.) //32 is for the registers befor jumping to userdefinedfunction.(Before jumping to UserDefinedFunction,save the main function's registers first.)
  //temp_b_code[0]|=regular_variable_address+32;//31 might be better than 32.(Because regular_variable_address indicate the empty address.) //32 is for the registers befor jumping to userdefinedfunction.(Before jumping to UserDefinedFunction,save the main function's registers first.)
  else if(VOrF == 'f')
    //temp_b_code[0]|=regular_variable_address+32; //FS_DMEM_START is the beginning address. Regular_variable_address is beginning address of non-used address.  
    temp_b_code[0]|=regular_variable_address; //FS_DMEM_START is the beginning address. Regular_variable_address is beginning address of non-used address.  
    //temp_b_code[0]|=FS_DMEM_START+32;
  else
    perror("GL_Start(): VOrF is wrong.");
#else
  temp_b_code[0]|=regular_variable_address+32;
#endif
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<10;

  if(VOrF == 'v'){
#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(j=0;j<1*2;j++)
    BinCode[LastBinLine++]=temp_b_code_NOP[j];
#else
  for(j=0;j<1;j++)
    BinCode[LastBinLine++]=temp_b_code[j];
#endif
  }
  if(VOrF == 'f'){
    temp_b_code[1]=9; //STore the r32 to the beginning addres. Kaymon
    temp_b_code[1]=temp_b_code[1]<<12;
#ifdef unified_shader_register
    temp_b_code[1]|=24;
#else
    temp_b_code[1]|=32;
#endif
    temp_b_code[1]=temp_b_code[1]<<32;
    temp_b_code[1]|=FS_DMEM_START;
    temp_b_code[1]=temp_b_code[1]<<5;
    temp_b_code[1]|=15;
    temp_b_code[1]=temp_b_code[1]<<3;
    temp_b_code[1]|=3;
    temp_b_code[1]=temp_b_code[1]<<7;
  
#ifdef FOR_REAL_FS
#ifndef remove_nop
  temp_b_code[3] = temp_b_code[1];       //reorder the instruction squence.
  temp_b_code[1] = nop_nine_instruction;
  temp_b_code[2] = nop_nine_instruction;

#ifdef SET_NOP_NINE
      insert_nop_nine(4);
      for(j=0;j<4*2;j++)
        BinCode[LastBinLine++]=temp_b_code_NOP[j];
#else
      for(j=0;j<4;j++)
        BinCode[LastBinLine++]=temp_b_code[j];
#endif
#else //remove_nop's else
      for(j=0;j<2;j++)
        BinCode[LastBinLine++]=temp_b_code[j];
#endif
#endif
  }

  for(i=0;i<256;i++)
    {
      unsigned input_imm;
      int j;

      //printf("i: %d\n",i);
      //printf("r_form[%d].next: %p\n",i,r_form[i].next);
      //printf("r_form[%d].address: %d\n",i,r_form[i].address);
      if(VOrF=='f'){
	if(r_form[i].address==1024||i==0) //For the reserved variable. e.g. gl_FragCoord //i==0 => for the first calculating address.
	  continue;
      }
      if(r_form[i].next==NULL)
	break;

      if(r_form[i].vector_size==4)
	{
	  memcpy(&input_imm,&r_form[i].value3,4);
	  
	  temp_b_code[0]=10;
	  temp_b_code[0]=temp_b_code[0]<<6;
	  temp_b_code[0]|=ARGU_REGI;
	  temp_b_code[0]=temp_b_code[0]<<38;
	  temp_b_code[0]|=input_imm;
	  temp_b_code[0]=temp_b_code[0]<<5;
	  temp_b_code[0]|=1;
	  temp_b_code[0]=temp_b_code[0]<<1;
#ifdef FOR_REAL_FS
          temp_b_code[0]|=0;
#else
          temp_b_code[0]|=1;
#endif
	  temp_b_code[0]=temp_b_code[0]<<2;
	  temp_b_code[0]|=3;
	  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
	  insert_nop_nine(1);
	  for(j=0;j<1*2;j++)
	    BinCode[LastBinLine++]=temp_b_code_NOP[j];
#else
	  for(j=0;j<1;j++)
	    BinCode[LastBinLine++]=temp_b_code[j];
#endif
	}

      if(r_form[i].vector_size>=3)
	{
	  memcpy(&input_imm,&r_form[i].value2,4);
	  
	  temp_b_code[0]=10;
	  temp_b_code[0]=temp_b_code[0]<<6;
	  temp_b_code[0]|=ARGU_REGI;
	  temp_b_code[0]=temp_b_code[0]<<38;
	  temp_b_code[0]|=input_imm;
	  temp_b_code[0]=temp_b_code[0]<<5;
	  temp_b_code[0]|=2;
	  temp_b_code[0]=temp_b_code[0]<<1;
#ifdef FOR_REAL_FS
          temp_b_code[0]|=0;
#else
          temp_b_code[0]|=1;
#endif
	  temp_b_code[0]=temp_b_code[0]<<2;
	  temp_b_code[0]|=2;
	  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
	  insert_nop_nine(1);
	  for(j=0;j<1*2;j++)
	    BinCode[LastBinLine++]=temp_b_code_NOP[j];
#else
	  for(j=0;j<1;j++)
	    BinCode[LastBinLine++]=temp_b_code[j];
#endif
	}

      if(r_form[i].vector_size>=2)
	{
	  memcpy(&input_imm,&r_form[i].value1,4);
	  
	  temp_b_code[0]=10;
	  temp_b_code[0]=temp_b_code[0]<<6;
	  temp_b_code[0]|=ARGU_REGI;
	  temp_b_code[0]=temp_b_code[0]<<38;
	  temp_b_code[0]|=input_imm;
	  temp_b_code[0]=temp_b_code[0]<<5;
	  temp_b_code[0]|=4;
	  temp_b_code[0]=temp_b_code[0]<<1;
#ifdef FOR_REAL_FS
          temp_b_code[0]|=1;
#else
          temp_b_code[0]|=0;
#endif
	  temp_b_code[0]=temp_b_code[0]<<2;
	  temp_b_code[0]|=1;
	  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
	  insert_nop_nine(1);
	  for(j=0;j<1*2;j++)
	    BinCode[LastBinLine++]=temp_b_code_NOP[j];
#else
	  for(j=0;j<1;j++)
	    BinCode[LastBinLine++]=temp_b_code[j];
#endif
	}

      memcpy(&input_imm,&r_form[i].value,4);

      temp_b_code[0]=10;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=ARGU_REGI;
      temp_b_code[0]=temp_b_code[0]<<38;
      temp_b_code[0]|=input_imm;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=8;
      temp_b_code[0]=temp_b_code[0]<<1;
#ifdef FOR_REAL_FS
      temp_b_code[0]|=0;
#else
      temp_b_code[0]|=1;
#endif
      temp_b_code[0]=temp_b_code[0]<<9;


      temp_b_code[1]=9;
      temp_b_code[1]=temp_b_code[1]<<12;
      temp_b_code[1]|=ARGU_REGI;
      temp_b_code[1]=temp_b_code[1]<<32;
      //#ifdef FS_DMEM_SET
      //if(VOrF == 'v')
      //temp_b_code[1]|=r_form[i].address;
      //else if(VOrF == 'f')
      //temp_b_code[1]|=r_form[i].address+FS_DMEM_START;
      //else
      //perror("GL_Start(): VOrF is wrong.");
      //#else
      //temp_b_code[1]|=r_form[i].address;
      //#endif

      temp_b_code[1]|=r_form[i].address;

      temp_b_code[1]=temp_b_code[1]<<5;
      temp_b_code[1]|=15;
      temp_b_code[1]=temp_b_code[1]<<3;
      temp_b_code[1]|=3;
      temp_b_code[1]=temp_b_code[1]<<7;

#ifdef FOR_REAL_FS
#ifndef remove_nop
      temp_b_code[3] = temp_b_code[1];
      temp_b_code[1] = nop_nine_instruction;
      temp_b_code[2] = nop_nine_instruction;

#ifdef SET_NOP_NINE
      insert_nop_nine(4);
      for(j=0;j<4*2;j++)
        BinCode[LastBinLine++]=temp_b_code_NOP[j];
#else
      for(j=0;j<4;j++)
        BinCode[LastBinLine++]=temp_b_code[j];
#endif
#else
      for(j=0;j<2;j++)
        BinCode[LastBinLine++]=temp_b_code[j];
#endif
#else
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(j=0;j<2*2;j++)
	BinCode[LastBinLine++]=temp_b_code_NOP[j];
#else
      for(j=0;j<2;j++)
	BinCode[LastBinLine++]=temp_b_code[j];
#endif
#endif
    }

#ifdef SET_NOP_NINE
  //delete one NOP
  LastBinLine--;
#endif

#ifdef SET_NOP_NINE
  return 2*(2+var_count+1+extra_count_for_vector)-1;
#else
  //printf("%d\n",2+var_count+1+extra_count_for_vector);
  return 2+var_count+1+extra_count_for_vector;
#endif  
}

int GL_FloatDiv(int Y_regi,
		int X_regi,
		int remain_regi,
		int remain_regi2,
		int vector_num,
		FILE* output_ass) // Y=Y/X
{

  int i,inst_count=0;
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"FloatDiv_%d_%d_%d_%d_ass_file",Y_regi,X_regi,remain_regi,remain_regi2);
      ass_file=fopen(temp_string,"w");
    }

  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string," ");
  switch(vector_num)
    {
    case 4:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.wyzw\n",temp_string,remain_regi,Y_regi);
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.wyzw\n",temp_string,remain_regi2,X_regi);
      sprintf(temp_string,"%s\tDIV R%d.0001, R%d, R%d\n",temp_string,remain_regi,remain_regi,remain_regi2);
    case 3:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.zyzw\n",temp_string,remain_regi,Y_regi);
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.zyzw\n",temp_string,remain_regi2,X_regi);
      sprintf(temp_string,"%s\tDIV R%d.0010, R%d, R%d\n",temp_string,remain_regi,remain_regi,remain_regi2);
    case 2:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.yyzw\n",temp_string,remain_regi,Y_regi);
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.yyzw\n",temp_string,remain_regi2,X_regi);
      sprintf(temp_string,"%s\tDIV R%d.0100, R%d, R%d\n",temp_string,remain_regi,remain_regi,remain_regi2);
      sprintf(temp_string,"%s\tMVR R%d.0111, R%d.xyzw\n",temp_string,Y_regi,remain_regi);
    case 1:
      sprintf(temp_string,"%s\tDIV R%d.1000, R%d, R%d\n",temp_string,Y_regi,Y_regi,X_regi);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw\n",temp_string,RETU_REGI,Y_regi);
      break;
    default:
      perror("GL_FloatDiv vector_num");
      break;
    }

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  fflush(ass_file);
  
  if(!output_ass)
    fclose(ass_file);

  //assembly setup

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=Y_regi;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=7131;//xyzw wyzw
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=8;//M
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3;//SV
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=temp_b_code[0];
  temp_b_code[1]&=(long long unsigned)0xF8007FFFFFFFFFFFLL;
  temp_b_code[1]|=(long long unsigned)remain_regi2<<53;
  temp_b_code[1]|=(long long unsigned)X_regi<<47;

  temp_b_code[2]=6;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi2;
  temp_b_code[2]=temp_b_code[2]<<22;
  temp_b_code[2]|=6939;//xyzw xyzw
  temp_b_code[2]=temp_b_code[2]<<15;
  temp_b_code[2]|=1;//M
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;//F
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=0;//SV
  temp_b_code[2]=temp_b_code[2]<<7;

  switch(vector_num)
    {
    case 4:
      inst_count+=3;

#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 3:
      inst_count+=3;

      temp_b_code[0]&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)155<<25;//Sw zyzw 
      temp_b_code[1]&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
      temp_b_code[1]|=(long long unsigned)155<<25;//Sw zyzw
      temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[2]|=(long long unsigned)2<<10;//M 0010
 
#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 2:
      inst_count+=4;

      temp_b_code[0]&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)91<<25;//Sw yyzw 
      temp_b_code[1]&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
      temp_b_code[1]|=(long long unsigned)91<<25;//Sw yyzw
      temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[2]|=(long long unsigned)4<<10;//M 0100

      temp_b_code[3]=7;
      temp_b_code[3]=temp_b_code[3]<<6;
      temp_b_code[3]|=Y_regi;
      temp_b_code[3]=temp_b_code[3]<<6;
      temp_b_code[3]|=remain_regi;
      temp_b_code[3]=temp_b_code[3]<<22;
      temp_b_code[3]|=6939;//xyzw xyzw
      temp_b_code[3]=temp_b_code[3]<<15;
      temp_b_code[3]|=7;//M
      temp_b_code[3]=temp_b_code[3]<<3;
      temp_b_code[3]|=3;//SV
      temp_b_code[3]=temp_b_code[3]<<7;
 
#ifdef SET_NOP_NINE
  insert_nop_nine(4);
  for(i=0;i<4*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<4;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 1:
      inst_count+=2;
      
      temp_b_code[0]=temp_b_code[2];
      temp_b_code[0]&=(long long unsigned)0xF8007FFFFFFFC3FFLL;
      temp_b_code[0]|=(long long unsigned)Y_regi<<53;
      temp_b_code[0]|=(long long unsigned)X_regi<<47;
      temp_b_code[0]|=(long long unsigned)8<<10;
      //temp_b_code[1]=temp_b_code[3];                               //if entrying case 1, then temp_b_code[3] is null. it will occur an error.
      //temp_b_code[1]&=(long long unsigned)0xF8007FFFFFFFC3FFLL;
      //temp_b_code[1]|=(long long unsigned)RETU_REGI<<53;
      //temp_b_code[1]|=(long long unsigned)Y_regi<<47;
      //temp_b_code[1]|=(long long unsigned)15<<10;//M 1111
      MVR_instr( RETU_REGI, Y_regi, 0x1b, 0x1b, 0, 15, 3,1);
      //printf("%llx\n",temp_b_code[1]);
#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
      break;
    default:
      perror("GL_FloatDiv vector_num");
      break;
    }

#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

#ifdef SET_NOP_NINE
  return inst_count*2-1;
#else
  return inst_count;
#endif

}

int GL_CallRegister(FILE *output_ass)
{

  int i;
  FILE *ass_file;

  if(output_ass)
    output_ass=ass_file;
  else
    {
      sprintf(temp_string,"CallRegister_ass_file");
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string,"MVR R%d.1111 R0.xyzw, <4>\n",RETU_REGI);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine instructions
  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=RETU_REGI;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=15;

  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;


#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

#ifdef SET_NOP_NINE
  return 1*2-1;
#else
  return 1;
#endif

}


int GL_texture2D()
{

  int i;
  FILE *ass_file;

#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"Texture2D_ass_file");
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"TEX R%d.1111, R%d.xyzw, Sampler: %d\n",RETU_REGI,ARGU_REGI+1,0);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  fflush(ass_file);

#endif  
#ifndef For_Attila
  temp_b_code[0]=15;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=RETU_REGI;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=ARGU_REGI+1;
  temp_b_code[0]=temp_b_code[0]<<8;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<14;
  temp_b_code[0]|=0x1B;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
#else

  temp_b_code[0]=15;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=RETU_REGI;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=ARGU_REGI+1;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=15; //regi1
  temp_b_code[0]=temp_b_code[0]<<31;
  temp_b_code[0]|=15; //wm
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
#endif
#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

#ifdef SET_NOP_NINE
  return 1*2-1;
#else
  return 1;
#endif

}
int GL_textureCube()
{

  int i;
  FILE *ass_file;

#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"Texture2D_ass_file");
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"TEX R%d.1111, R%d.xyzw, Sampler: %d\n",RETU_REGI,ARGU_REGI+1,0);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  fflush(ass_file);

#endif  
  /*  
  temp_b_code[0]=15;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=RETU_REGI;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=ARGU_REGI+1;
  temp_b_code[0]=temp_b_code[0]<<8;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<14;
  temp_b_code[0]|=0x1B;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  */

  temp_b_code[0]=15;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=RETU_REGI;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=ARGU_REGI+1;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=15; //regi1
  temp_b_code[0]=temp_b_code[0]<<31;
  temp_b_code[0]|=15; //wm
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<1; //add this bit for temporary texcube instruction.
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<6;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

#ifdef SET_NOP_NINE
  return 1*2-1;
#else
  return 1;
#endif

}

int GL_LoadVarying(int varying_regi)
{
  int i;
  FILE *ass_file;

#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"LoadVarying_R%d_R%d_ass_file",RETU_REGI,varying_regi);
  ass_file=fopen(temp_string,"w");
#endif
#endif

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=RETU_REGI;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=varying_regi;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1B1B;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=31;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

#ifdef SET_NOP_NINE
  return 1*2-1;
#else
  return 1;
#endif
}

int GL_ENDFrag(FILE* output_ass) //The interface register will be changed in unified shader. See the FS team's this function. R26<=R0(color)  R24<=R1(position) R25<=R2(position.z) 
{
  int i;
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"ENDFrag_ass_file");
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4> (Rd(R%d): Fragment interface register)\n",temp_string,0,ARGU_REGI,0);
  sprintf(temp_string,"%s\tMVR R%d.1100, R%d.xyzw, <4> (Rd(R%d): Fragment interface register)\n",temp_string,1,32,1);
  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.zyzw, <4> (Rd(R%d): Fragment interface register)\n",temp_string,2,32,2);
  sprintf(temp_string,"%s\tFIN (Start fragment)\n",temp_string);
  sprintf(temp_string,"%s\tFIN",temp_string);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
#ifdef unified_shader_register
  temp_b_code[0]|=24;
#else
  temp_b_code[0]|=0;
#endif
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=ARGU_REGI;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1B1B;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=31;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<1;
  //Fragment interface register bit
#ifndef unified_shader_register
  temp_b_code[0]|=1;
#endif
  temp_b_code[0]=temp_b_code[0]<<6;

#ifdef FOR_REAL_FS
  temp_b_code[1]=7;
  temp_b_code[1]=temp_b_code[1]<<6;
#ifdef unified_shader_register
  temp_b_code[1]|=25;
#else
  temp_b_code[1]|=1;
#endif
  temp_b_code[1]=temp_b_code[1]<<6;
#ifdef unified_shader_register
  temp_b_code[1]|=24;
#else
  temp_b_code[1]|=32;
#endif
  temp_b_code[1]=temp_b_code[1]<<22;
  temp_b_code[1]|=0x1B1B;
  temp_b_code[1]=temp_b_code[1]<<16;
  temp_b_code[1]|=25;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<1;
  //Fragment interface register bit
#ifndef unified_shader_register
  temp_b_code[1]|=1;
#endif
  temp_b_code[1]=temp_b_code[1]<<6;

  temp_b_code[2]=7;
  temp_b_code[2]=temp_b_code[2]<<6;
#ifdef unified_shader_register
  temp_b_code[2]|=26;
#else
  temp_b_code[2]|=2;
#endif
  temp_b_code[2]=temp_b_code[2]<<6;
#ifdef unified_shader_register
  temp_b_code[2]|=24;
#else
  temp_b_code[2]|=32;
#endif
  temp_b_code[2]=temp_b_code[2]<<22;
  temp_b_code[2]|=0x1BAA;
  temp_b_code[2]=temp_b_code[2]<<16;
  temp_b_code[2]|=17;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=3;
  temp_b_code[2]=temp_b_code[2]<<1;
  //Fragment interface register bit
#ifndef unified_shader_register
  temp_b_code[2]|=1;
#endif
  temp_b_code[2]=temp_b_code[2]<<6;

  temp_b_code[4]=14;
  temp_b_code[4]=temp_b_code[4]<<59;

  temp_b_code[3]=temp_b_code[4];
  temp_b_code[3]|=(1<<6);

  //#ifdef SET_NOP_NINE
  //insert_nop_nine(5);
  //for(i=0;i<5*2-1;i++)
  //BinCode[LastBinLine++]=temp_b_code_NOP[i];
  //#else
  //for(i=0;i<5;i++)
  //BinCode[LastBinLine++]=temp_b_code[i];
  //#endif

  //Set NOP in front of the instructions that write the interface register
  
  temp_b_code[7] = temp_b_code[4];
  temp_b_code[6] = temp_b_code[3];
  temp_b_code[5] = temp_b_code[2];
  temp_b_code[3] = temp_b_code[1];
  temp_b_code[1] = temp_b_code[0];
  temp_b_code[0] = temp_b_code[2] = temp_b_code[4] = nop_nine_instruction;

  for(i=0;i<8;i++)
    BinCode[LastBinLine++]=temp_b_code[i];

  //#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),1,output_file);
  //#endif


  //#ifdef SET_NOP_NINE
  //return 5*2-1;
  //#else
  //return 5;
  //#endif

  return 8;
#else

  temp_b_code[2]=14;
  temp_b_code[2]=temp_b_code[2]<<59;

  temp_b_code[1]=temp_b_code[2];
  temp_b_code[1]|=(1<<6);


  //#ifdef SET_NOP_NINE
  //insert_nop_nine(3);
  //for(i=0;i<3*2-1;i++)
  //BinCode[LastBinLine++]=temp_b_code_NOP[i];
  //#else
  //for(i=0;i<3;i++)
  //BinCode[LastBinLine++]=temp_b_code[i];
  //#endif

  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];

#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif


  //#ifdef SET_NOP_NINE
  //return 3*2-1;
  //#else
  //return 3;
  //#endif

  return 3;

#endif

}

int GL_END(int return_regi,
	   FILE* output_ass)
{

#ifdef For_Attila
  int i;
  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=ARGU_REGI;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1B1B;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=30;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=14;
  temp_b_code[1]=temp_b_code[1]<<59;


#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef SET_NOP_NINE
  return 3;
#else
  return 2;
#endif

#else //insert the viewport code on the bottom.
#ifdef LoadAttributeFromSCT

#define view_w width
#define view_h height
#define view_x x
#define view_y y
#define temp_z 1.0
#define temp_w 1.0

#else

#define view_w 512.0
#define view_h 512.0
#define view_x 0.0
#define view_y 0.0
#define temp_z 1.0
#define temp_w 1.0

#endif
  int i;
  float temp_float;
  unsigned temp_int;
  FILE *ass_file;
  int return_value=0;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"END_R%d_ass_file",return_regi);
      ass_file=fopen(temp_string,"w");
    }

  if(!output_ass)
    fclose(ass_file);

  //machine code

  //dealing with w = 0
  //assembly:
  //  ADDIF R(ARGU_REGI1).1000, R0, 0.01, <4>
  //  BGTSF R(ARGU_REGI).wwww, R(ARGU_REGI1).xxxx,2 ,<4>
  //  ADDIF R(ARGU_REGI).0001, R0, 0.01, <4>
  temp_float=0.01;
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=ARGU_REGI+1;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<32;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=27;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=ARGU_REGI;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=ARGU_REGI+1;
  temp_b_code[1]=temp_b_code[1]<<16;
  temp_b_code[1]|=2;
  temp_b_code[1]=temp_b_code[1]<<16;
  temp_b_code[1]|=0xFF00;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=10;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=ARGU_REGI;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=0;
  temp_b_code[2]=temp_b_code[2]<<32;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[2]|=temp_int;
  temp_b_code[2]=temp_b_code[2]<<5;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=3;
  temp_b_code[2]=temp_b_code[2]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  return_value=3*2-1;
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  return_value=3;
#endif

  temp_float=1.0;

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=ARGU_REGI+1;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=ARGU_REGI;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1BDB;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;


#ifdef W_RECIPROCAL

  temp_b_code[2]=7;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=ARGU_REGI+2;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=ARGU_REGI+1;
  temp_b_code[2]=temp_b_code[2]<<22;
  temp_b_code[2]|=0x1B1B;
  temp_b_code[2]=temp_b_code[2]<<16;
  temp_b_code[2]|=31;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=3;
  temp_b_code[2]=temp_b_code[2]<<7;

  temp_b_code[1]=2;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=ARGU_REGI+1;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=ARGU_REGI+1;
  temp_b_code[1]=temp_b_code[1]<<22;
  temp_b_code[1]|=6939;
  temp_b_code[1]=temp_b_code[1]<<16;
  temp_b_code[1]|=31;
  temp_b_code[1]=temp_b_code[1]<<9;  

#else

  temp_b_code[1]=7;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=ARGU_REGI+2;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=ARGU_REGI;
  temp_b_code[1]=temp_b_code[1]<<22;
  temp_b_code[1]|=0x1B1B;
  temp_b_code[1]=temp_b_code[1]<<16;
  temp_b_code[1]|=31;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=2;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=ARGU_REGI+1;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=ARGU_REGI+1;
  temp_b_code[2]=temp_b_code[2]<<22;
  temp_b_code[2]|=6939;
  temp_b_code[2]=temp_b_code[2]<<16;
  temp_b_code[2]|=31;
  temp_b_code[2]=temp_b_code[2]<<9;  

#endif


  temp_b_code[3]=17;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=ARGU_REGI;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=ARGU_REGI;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=ARGU_REGI+1;
  temp_b_code[3]=temp_b_code[3]<<24;
  temp_b_code[3]|=0x1B1B;
  temp_b_code[3]=temp_b_code[3]<<7;
  temp_b_code[3]|=15;
  temp_b_code[3]=temp_b_code[3]<<1;
  temp_b_code[3]|=1;
  temp_b_code[3]=temp_b_code[3]<<2;
  temp_b_code[3]|=3;
  temp_b_code[3]=temp_b_code[3]<<7;

  temp_b_code[4]=10;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=ARGU_REGI;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=ARGU_REGI;
  temp_b_code[4]=temp_b_code[4]<<32;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[4]|=temp_int;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=25;
  //temp_b_code[4]|=24;
  temp_b_code[4]=temp_b_code[4]<<2;
  temp_b_code[4]|=2;
  temp_b_code[4]=temp_b_code[4]<<7;

  temp_b_code[5]=temp_b_code[4];
  temp_b_code[5]&=(long long unsigned)0xF80000000000427FLL;
  temp_b_code[5]|=(long long unsigned)(ARGU_REGI+1)<<53;
  temp_float=view_w/2;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[5]|=(long long unsigned)temp_int<<15;
  temp_b_code[5]|=(long long unsigned)8<<10;

  temp_b_code[6]=temp_b_code[5];
  temp_b_code[6]&=(long long unsigned)0xFFF700000000427FLL;
  temp_float=view_h/2;
  memcpy(&temp_int,&temp_float,4);
  //temp_int=240/2;
  temp_b_code[6]|=(long long unsigned)temp_int<<15;
  temp_b_code[6]|=(long long unsigned)4<<10;
  temp_b_code[6]|=(long long unsigned)1<<7;

  temp_b_code[7]=17;
  temp_b_code[7]=temp_b_code[7]<<6;
  temp_b_code[7]|=ARGU_REGI;
  temp_b_code[7]=temp_b_code[7]<<6;
  temp_b_code[7]|=ARGU_REGI;
  temp_b_code[7]=temp_b_code[7]<<6;
  temp_b_code[7]|=ARGU_REGI+1;
  temp_b_code[7]=temp_b_code[7]<<24;
  temp_b_code[7]|=0x1B1B;
  temp_b_code[7]=temp_b_code[7]<<7;
  temp_b_code[7]|=12;
  temp_b_code[7]=temp_b_code[7]<<1;
  temp_b_code[7]|=1;
  temp_b_code[7]=temp_b_code[7]<<2;
  temp_b_code[7]|=1;
  temp_b_code[7]=temp_b_code[7]<<7;

  temp_b_code[8]=temp_b_code[5];
  temp_b_code[8]&=(long long unsigned)0xFFF700000000427FLL;
  temp_float=view_x;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[8]|=(long long unsigned)temp_int<<15;
  temp_b_code[8]|=(long long unsigned)8<<10;

  temp_b_code[9]=temp_b_code[5];
  temp_b_code[9]&=(long long unsigned)0xFFF700000000427FLL;
  temp_float=view_y;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[9]|=(long long unsigned)temp_int<<15;
  temp_b_code[9]|=(long long unsigned)4<<10;
  temp_b_code[9]|=(long long unsigned)1<<7;

  temp_b_code[10]=16;
  temp_b_code[10]=temp_b_code[10]<<6;
  temp_b_code[10]|=ARGU_REGI;
  temp_b_code[10]=temp_b_code[10]<<6;
  temp_b_code[10]|=ARGU_REGI;
  temp_b_code[10]=temp_b_code[10]<<6;
  temp_b_code[10]|=ARGU_REGI+1;
  temp_b_code[10]=temp_b_code[10]<<24;
  temp_b_code[10]|=6939;
  temp_b_code[10]=temp_b_code[10]<<7;
  temp_b_code[10]|=12;
  temp_b_code[10]=temp_b_code[10]<<1;
  temp_b_code[10]|=1;
  temp_b_code[10]=temp_b_code[10]<<2;
  temp_b_code[10]|=1;
  temp_b_code[10]=temp_b_code[10]<<7;

  temp_b_code[11]=temp_b_code[5];
  temp_b_code[11]&=(long long unsigned)0xFFF700000000427FLL;
  temp_float=temp_z;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[11]|=(long long unsigned)temp_int<<15;
  temp_b_code[11]|=(long long unsigned)2<<10;
  temp_b_code[11]|=(long long unsigned)3<<7;

  temp_b_code[12]=temp_b_code[5];
  temp_b_code[12]&=(long long unsigned)0xFFF700000000427FLL;
  temp_float=temp_w;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[12]|=(long long unsigned)temp_int<<15;
  temp_b_code[12]|=(long long unsigned)1<<10;
  temp_b_code[12]|=(long long unsigned)3<<7;

  temp_b_code[13]=17;
  temp_b_code[13]=temp_b_code[13]<<6;
  temp_b_code[13]|=ARGU_REGI+1;
  temp_b_code[13]=temp_b_code[13]<<6;
  temp_b_code[13]|=ARGU_REGI;
  temp_b_code[13]=temp_b_code[13]<<6;
  temp_b_code[13]|=ARGU_REGI+1;
  temp_b_code[13]=temp_b_code[13]<<24;
  temp_b_code[13]|=0x1B1B;
  temp_b_code[13]=temp_b_code[13]<<7;
  temp_b_code[13]|=3;
  temp_b_code[13]=temp_b_code[13]<<1;
  temp_b_code[13]|=1;
  temp_b_code[13]=temp_b_code[13]<<2;
  temp_b_code[13]|=3;
  temp_b_code[13]=temp_b_code[13]<<7;

  temp_b_code[14]=16;
  temp_b_code[14]=temp_b_code[14]<<6;
  temp_b_code[14]|=ARGU_REGI;
  temp_b_code[14]=temp_b_code[14]<<6;
  temp_b_code[14]|=ARGU_REGI+1;
  temp_b_code[14]=temp_b_code[14]<<6;
  temp_b_code[14]|=ARGU_REGI+1;
  temp_b_code[14]=temp_b_code[14]<<24;
  temp_b_code[14]|=0x1B1F;
  temp_b_code[14]=temp_b_code[14]<<7;
  temp_b_code[14]|=2;
  temp_b_code[14]=temp_b_code[14]<<1;
  temp_b_code[14]|=1;
  temp_b_code[14]=temp_b_code[14]<<2;
  temp_b_code[14]|=3;
  temp_b_code[14]=temp_b_code[14]<<7;

  temp_b_code[15]=temp_b_code[5];
  temp_b_code[15]&=(long long unsigned)0xFFF700000000427FLL;
  temp_float=0.5;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[15]|=(long long unsigned)temp_int<<15;
  temp_b_code[15]|=(long long unsigned)8<<10;
  temp_b_code[15]|=(long long unsigned)1<<7;

  temp_b_code[16]=17;
  temp_b_code[16]=temp_b_code[16]<<6;
  temp_b_code[16]|=ARGU_REGI;
  temp_b_code[16]=temp_b_code[16]<<6;
  temp_b_code[16]|=ARGU_REGI;
  temp_b_code[16]=temp_b_code[16]<<6;
  temp_b_code[16]|=ARGU_REGI+1;
  temp_b_code[16]=temp_b_code[16]<<24;
  temp_b_code[16]|=0x1B13;
  temp_b_code[16]=temp_b_code[16]<<7;
  temp_b_code[16]|=2;
  temp_b_code[16]=temp_b_code[16]<<1;
  temp_b_code[16]|=1;
  temp_b_code[16]=temp_b_code[16]<<2;
  temp_b_code[16]|=3;
  temp_b_code[16]=temp_b_code[16]<<7;

  temp_b_code[17]=7;
  temp_b_code[17]=temp_b_code[17]<<6;
  temp_b_code[17]|=ARGU_REGI;
  temp_b_code[17]=temp_b_code[17]<<6;
  temp_b_code[17]|=ARGU_REGI+2;
  temp_b_code[17]=temp_b_code[17]<<22;
  temp_b_code[17]|=0x1B1B;
  temp_b_code[17]=temp_b_code[17]<<16;
  temp_b_code[17]|=2;
  temp_b_code[17]=temp_b_code[17]<<2;
  temp_b_code[17]|=3;
  temp_b_code[17]=temp_b_code[17]<<7;

  temp_b_code[18]=7;
  temp_b_code[18]=temp_b_code[18]<<6;
  temp_b_code[18]|=return_regi;
  temp_b_code[18]=temp_b_code[18]<<6;
  temp_b_code[18]|=ARGU_REGI;
  temp_b_code[18]=temp_b_code[18]<<22;
  temp_b_code[18]|=0x1B1B;
  temp_b_code[18]=temp_b_code[18]<<16;
  temp_b_code[18]|=30;
  temp_b_code[18]=temp_b_code[18]<<2;
  temp_b_code[18]|=3;
  temp_b_code[18]=temp_b_code[18]<<7;

  temp_b_code[19]=14;
  temp_b_code[19]=temp_b_code[19]<<59;


#ifdef SET_NOP_NINE
  insert_nop_nine(20);
  for(i=0;i<20*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<20;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif


#ifdef SET_NOP_NINE
  return return_value+20*2-1;
#else
  return return_value+20;
#endif
#endif //end of For_Attila
}
int GL_ChangeMatrix(int return_regi,
		    int argu_regi,
		    int argu_regi_2,
		    int argu_regi_3,
		    int argu_regi_4,
		    int remain_regi,
		    int step,
		    int vector_num,
		    FILE* output_ass)
// argu_regi_4.x   argu_regi_4.y   argu_regi_4.z   argu_regi_4.w
// argu_regi_3.x   argu_regi_3.y   argu_regi_3.z   argu_regi_3.w
// argu_regi_2.x   argu_regi_2.y   argu_regi_2.z   argu_regi_2.w
// argu_regi_1.x   argu_regi_1.y   argu_regi_1.z   argu_regi_1.w
//                              ||
//                              ||
//                              \/
// argu_regi_4.x   argu_regi_3.x   argu_regi_2.x   argu_regi_1.x
// argu_regi_4.y   argu_regi_3.y   argu_regi_2.y   argu_regi_1.y
// argu_regi_4.z   argu_regi_3.z   argu_regi_2.z   argu_regi_1.z
// argu_regi_4.w   argu_regi_3.w   argu_regi_2.w   argu_regi_1.w
{
  int i;//,argu_regi=ARGU_REGI;
  int instr_count=0;
  FILE *ass_file;
  
  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"ChangeMatrix%d_R%d_R%d_R%d_R%d_R%d_R%d_%d_ass_file",vector_num,return_regi,argu_regi,argu_regi_2,argu_regi_3,argu_regi_4,remain_regi,step);
      ass_file=fopen(temp_string,"w");
    }

  //assembly code
  sprintf(temp_string,"");
  switch(step)
    {
    case 1:
      if(vector_num>3)
	sprintf(temp_string,"%s\tMVR R%d.0001, R%d.xyzx, <%d>\n",temp_string,remain_regi,argu_regi,vector_num);
      if(vector_num>2)
	sprintf(temp_string,"%s\tMVR R%d.0010, R%d.xyxw, <%d>\n",temp_string,remain_regi,argu_regi_2,vector_num);
      if(vector_num>1)
	sprintf(temp_string,"%s\tMVR R%d.0100, R%d.xxzw, <%d>\n",temp_string,remain_regi,argu_regi_3,vector_num);
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.xyzw, <%d>\n",temp_string,remain_regi,argu_regi_4,vector_num);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi,vector_num);
      break;
    case 2:
      if(vector_num>3)
	sprintf(temp_string,"%s\tMVR R%d.0001, R%d.xyzy, <%d>\n",temp_string,remain_regi,argu_regi,vector_num);
      if(vector_num>2)
	sprintf(temp_string,"%s\tMVR R%d.0010, R%d.xyyw, <%d>\n",temp_string,remain_regi,argu_regi_2,vector_num);
      if(vector_num>1)
	sprintf(temp_string,"%s\tMVR R%d.0100, R%d.xyzw, <%d>\n",temp_string,remain_regi,argu_regi_3,vector_num);
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.yyzw, <%d>\n",temp_string,remain_regi,argu_regi_4,vector_num);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi,vector_num);
      break;
    case 3:
      if(vector_num>3)
	sprintf(temp_string,"%s\tMVR R%d.0001, R%d.xyzz, <%d>\n",temp_string,remain_regi,argu_regi,vector_num);
      if(vector_num>2)
	sprintf(temp_string,"%s\tMVR R%d.0010, R%d.xyzw, <%d>\n",temp_string,remain_regi,argu_regi_2,vector_num);
      if(vector_num>1)
	sprintf(temp_string,"%s\tMVR R%d.0100, R%d.xzzw, <%d>\n",temp_string,remain_regi,argu_regi_3,vector_num);
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.zyzw, <%d>\n",temp_string,remain_regi,argu_regi_4,vector_num);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi,vector_num);
      break;
    case 4:
      if(vector_num>3)
	sprintf(temp_string,"%s\tMVR R%d.0001, R%d.xyzw, <%d>\n",temp_string,remain_regi,argu_regi,vector_num);
      if(vector_num>2)
	sprintf(temp_string,"%s\tMVR R%d.0010, R%d.xyww, <%d>\n",temp_string,remain_regi,argu_regi_2,vector_num);
      if(vector_num>1)
	sprintf(temp_string,"%s\tMVR R%d.0100, R%d.xwzw, <%d>\n",temp_string,remain_regi,argu_regi_3,vector_num);
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.wyzw, <%d>\n",temp_string,remain_regi,argu_regi_4,vector_num);
      sprintf(temp_string,"%s\tMVR R%d.1%d%d%d, R%d.xyzw, <%d>\n",temp_string,return_regi,vector_num>1?1:0,vector_num>2?1:0,vector_num>3?1:0,remain_regi,vector_num);
      break;
    default:
      perror("GL_ChangeMatrix(): variable step error");
      break;
    }

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //printf("step: %d\n",step);
  //printf("vector_num: %d\n",vector_num);
  //argu_regi-=(4-vector_num);

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=argu_regi;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1B18;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[4]=7;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=return_regi;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=remain_regi;
  temp_b_code[4]=temp_b_code[4]<<22;
  temp_b_code[4]|=0x1B1B;
  temp_b_code[4]=temp_b_code[4]<<15;
  temp_b_code[4]|=vector_num>3?15:vector_num>2?14:vector_num>1?12:8;
  temp_b_code[4]=temp_b_code[4]<<3;
  temp_b_code[4]|=vector_num-1;
  temp_b_code[4]=temp_b_code[4]<<7;

  switch (step)
    {
    case 1:
      temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFFFFFLL;
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[1]|=(long long unsigned)(argu_regi_2)<<47;
      temp_b_code[1]|=(long long unsigned)0x1B13<<25;
      temp_b_code[1]|=(long long unsigned)2<<10;
      temp_b_code[2]=temp_b_code[0];
      temp_b_code[2]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[2]|=(long long unsigned)(argu_regi_3)<<47;
      temp_b_code[2]|=(long long unsigned)0x1B0B<<25;
      temp_b_code[2]|=(long long unsigned)4<<10;
      temp_b_code[3]=temp_b_code[0];
      temp_b_code[3]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[3]|=(long long unsigned)(argu_regi_4)<<47;
      temp_b_code[3]|=(long long unsigned)0x1B1B<<25;
      temp_b_code[3]|=(long long unsigned)8<<10;
      break;
    case 2:
      temp_b_code[0]&=(long long unsigned)0xFFFFFE0001FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x1B19<<25;
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[1]|=(long long unsigned)(argu_regi_2)<<47;
      temp_b_code[1]|=(long long unsigned)0x1B17<<25;
      temp_b_code[1]|=(long long unsigned)2<<10;
      temp_b_code[2]=temp_b_code[0];
      temp_b_code[2]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[2]|=(long long unsigned)(argu_regi_3)<<47;
      temp_b_code[2]|=(long long unsigned)0x1B1B<<25;
      temp_b_code[2]|=(long long unsigned)4<<10;
      temp_b_code[3]=temp_b_code[0];
      temp_b_code[3]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[3]|=(long long unsigned)(argu_regi_4)<<47;
      temp_b_code[3]|=(long long unsigned)0x1B5B<<25;
      temp_b_code[3]|=(long long unsigned)8<<10;
      break;
    case 3:
      temp_b_code[0]&=(long long unsigned)0xFFFFFE0001FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x1B1A<<25;
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[1]|=(long long unsigned)(argu_regi_2)<<47;
      temp_b_code[1]|=(long long unsigned)0x1B1B<<25;
      temp_b_code[1]|=(long long unsigned)2<<10;
      temp_b_code[2]=temp_b_code[0];
      temp_b_code[2]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[2]|=(long long unsigned)(argu_regi_3)<<47;
      temp_b_code[2]|=(long long unsigned)0x1B2B<<25;
      temp_b_code[2]|=(long long unsigned)4<<10;
      temp_b_code[3]=temp_b_code[0];
      temp_b_code[3]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[3]|=(long long unsigned)(argu_regi_4)<<47;
      temp_b_code[3]|=(long long unsigned)0x1B9B<<25;
      temp_b_code[3]|=(long long unsigned)8<<10;
      break;
    case 4:
      temp_b_code[0]&=(long long unsigned)0xFFFFFE0001FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x1B1B<<25;
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[1]|=(long long unsigned)(argu_regi_2)<<47;
      temp_b_code[1]|=(long long unsigned)0x1B1F<<25;
      temp_b_code[1]|=(long long unsigned)2<<10;
      temp_b_code[2]=temp_b_code[0];
      temp_b_code[2]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[2]|=(long long unsigned)(argu_regi_3)<<47;
      temp_b_code[2]|=(long long unsigned)0x1B3B<<25;
      temp_b_code[2]|=(long long unsigned)4<<10;
      temp_b_code[3]=temp_b_code[0];
      temp_b_code[3]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[3]|=(long long unsigned)(argu_regi_4)<<47;
      temp_b_code[3]|=(long long unsigned)0x1BDB<<25;
      temp_b_code[3]|=(long long unsigned)8<<10;
      break;
    default:

      //perror("step");
      break;
    }

  //return register is the second argument register ==> exchange instruction 0 and 1

  switch(vector_num)
    {
    case 1:
      temp_b_code[0]=temp_b_code[3];
      temp_b_code[1]=temp_b_code[4];
      break;
    case 2:
      temp_b_code[0]=temp_b_code[2];
      temp_b_code[1]=temp_b_code[3];
      temp_b_code[2]=temp_b_code[4];
      break;
    case 3:
      temp_b_code[0]=temp_b_code[1];
      temp_b_code[1]=temp_b_code[2];
      temp_b_code[2]=temp_b_code[3];
      temp_b_code[3]=temp_b_code[4];
    break;
    case 4:
      break;
    default:
      perror("ChangeMatrix vector_num")
      break;
    }

  /*
  long long unsigned temp_b=temp_b_code[0];
  temp_b_code[0]=temp_b_code[1];
  temp_b_code[1]=temp_b;
  */

  
#ifdef SET_NOP_NINE
  insert_nop_nine(vector_num+1);
  for(i=0;i<(vector_num+1)*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<(vector_num+1);i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef SET_NOP_NINE
  return (vector_num+1)*2-1;
#else
  return vector_num+1;
#endif

}

int GL_LoadAttribute(int att_regi,
		     int vector_size,
		     FILE *output_ass)
{
  int i;
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"LoadAttribute_R%d_R%d_ass_file",RETU_REGI,att_regi);
      ass_file=fopen(temp_string,"w");
    }

  //printf("att_regi: %d\tvector_size: %d\n",att_regi,vector_size);

  if(!output_ass)
    fclose(ass_file);

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=RETU_REGI;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=att_regi;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=15;//((1<<3)|((vector_size>1?1:0)<<2)|((vector_size>2?1:0)<<1)|(vector_size>3?1:0));
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;


#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

#ifdef SET_NOP_NINE
  return 1*2-1;
#else
  return 1;
#endif

}


int GL_VaryingPut(int att_regi,
		  int vector_size,
		  FILE *output_ass)
{
  int i;
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"VaryingPut_R%d_R%d_ass_file",RETU_REGI,att_regi);
      ass_file=fopen(temp_string,"w");
    }

  if(!output_ass)
    fclose(ass_file);

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=att_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=ARGU_REGI;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=((1<<3)|((vector_size>1?1:0)<<2)|((vector_size>2?1:0)<<1)|(vector_size>3?1:0));
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

  //temp_b_code_VOB[VOB_count++]=temp_b_code[0];

  //return 0;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif


#ifdef test_only
     fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

#ifdef SET_NOP_NINE
return 1*2-1;
#else
return 1;
#endif

}

int GL_FaceForward(int return_regi,
		   int N_regi,
		   int I_regi,
		   int Nref_regi,
		   int remain_regi,
		   char VOrF,
		   int SV,
		   FILE *output_ass)
{
  int i;
  int instr_count=0;
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"FACEFORWARD_%d_%d_%d_%d_%d_ass_file",return_regi,N_regi,I_regi,Nref_regi,remain_regi);
      ass_file=fopen(temp_string,"w");
    }

  instr_count=GL_Dot(remain_regi,Nref_regi,I_regi,SV,output_ass);

  //assembly code
  sprintf(temp_string,"");
  if(VOrF=='v')
    sprintf(temp_string,"%s\tBGTVF R0, R%d, R0, R%d, 2, 1000, <%d>\n",temp_string,remain_regi,remain_regi,SV);
  else if(VOrF=='f')
    sprintf(temp_string,"%s\tBGTSF R0.xxxx, R%d.xxxx, 2, <%d>\n",temp_string,remain_regi,SV);
  else
    perror("GL_FaceForward(): VOrF error");

  sprintf(temp_string,"%s\tADDF R%d.1%d%d%d, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,N_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,N_regi,SV);
  sprintf(temp_string,"%s\tMVR R%d.1%d%d%d, R%d.xyzw, <4>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,N_regi);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

#ifdef SET_NOP_NINE
  BinCode[LastBinLine++]=nop_nine_instruction;
  instr_count+=1;
#endif
  //machine code

  if(VOrF=='v')
    {
#ifndef unified_branch
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=remain_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<11;
      temp_b_code[0]|=remain_regi;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[0]|=8;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#else 
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=remain_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<17;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=8;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#endif
    }
  else if(VOrF=='f')
    {
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=remain_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<17;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=8;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
    }
  else
    perror("GL_FaceForward(): VOrF error");

  temp_b_code[1]=16;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=N_regi;
  temp_b_code[1]=temp_b_code[1]<<12;
  temp_b_code[1]|=N_regi;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=0x1b1b1b;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=2;
  temp_b_code[1]=temp_b_code[1]<<4;
  temp_b_code[1]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=7;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=return_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=N_regi;
  temp_b_code[2]=temp_b_code[2]<<22;
  temp_b_code[2]|=0x1B1B;
  temp_b_code[2]=temp_b_code[2]<<15;
  temp_b_code[2]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[2]=temp_b_code[2]<<3;
  temp_b_code[2]|=SV-1;
  temp_b_code[2]=temp_b_code[2]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=5;
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=3;
#endif

  return instr_count;
}

int GL_Normalize(int vector_reg,
		 int remain_reg,
		 int remain_reg2,
		 int return_reg,
		 int vector_num,
		 FILE *output_ass)
{
  int i;
  //Kenny provide a faster (vec3)method than below thing.
  //DP3 result.w vector vector
  //RSQ result.w result.w
  //MUL result.xyz result.w vector
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"NORMALIZE_%d_%d_ass_file",vector_reg,remain_reg);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");

  sprintf(temp_string,"\tLDIF R%d.0%d%d%d, 0.0\n",vector_reg,vector_num<2?1:0,vector_num<3?1:0,vector_num<4?1:0);

#ifndef unified_branch
  sprintf(temp_string,"%s\tBEQV R%d, R0, R%d, R0, 17, 1111,<4>\n",temp_string,vector_reg,vector_reg);//Kaymon addd it
#else
  sprintf(temp_string,"%s\tBEQS R%d.xyzw, R0.xyzw, 17,<4>\n",temp_string,vector_reg);//Kaymon addd it
#endif
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string,remain_reg2,vector_reg);//0
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string,remain_reg,vector_reg);//1
  //sprintf(temp_string,"%s\tLDI R%d.0100, 2.0\n",temp_string,remain_reg);
  /*
  sprintf(temp_string,"%s\tPOW2 R%d.1000, R%d\n",temp_string,vector_reg,vector_reg);//2
  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.yyzw, <4>\n",temp_string,remain_reg,vector_reg);
  sprintf(temp_string,"%s\tPOW2 R%d.0100, R%d\n",temp_string,vector_reg,remain_reg);
  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.zyzw, <4>\n",temp_string,remain_reg,vector_reg);
  sprintf(temp_string,"%s\tPOW2 R%d.0010, R%d\n",temp_string,vector_reg,remain_reg);
  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.wyzw, <4>\n",temp_string,remain_reg,vector_reg);
  sprintf(temp_string,"%s\tPOW2 R%d.0001, R%d\n",temp_string,vector_reg,remain_reg);

  sprintf(temp_string,"%s\tLOG2 R%d.1000, R%d\n",temp_string,vector_reg,vector_reg);//9
  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.yyzw, <4>\n",temp_string,remain_reg,vector_reg);
  sprintf(temp_string,"%s\tLOG2 R%d.0100, R%d\n",temp_string,vector_reg,remain_reg);
  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.zyzw, <4>\n",temp_string,remain_reg,vector_reg);
  sprintf(temp_string,"%s\tLOG2 R%d.0010, R%d\n",temp_string,vector_reg,remain_reg);
  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.wyzw, <4>\n",temp_string,remain_reg,vector_reg);
  sprintf(temp_string,"%s\tLOG2 R%d.0001, R%d\n",temp_string,vector_reg,remain_reg);
*/

  //sprintf(temp_string,"%s\tMULF R%d, R%d.xyzw, R%d.xyzw\n",temp_string,vector_reg,vector_reg,remain_reg2);//16  0

  sprintf(temp_string,"%s\tMULF R%d, R%d.xyzw, R%d.xyzw\n",temp_string,vector_reg,vector_reg,vector_reg);//16  0

  sprintf(temp_string,"%s\tADDF R%d.1000, R%d.xyzw, R%d.yyzw, <4>\n",temp_string,vector_reg,vector_reg,vector_reg);//17  1
  sprintf(temp_string,"%s\tADDF R%d.1000, R%d.xyzw, R%d.zyzw, <4>\n",temp_string,vector_reg,vector_reg,vector_reg);
  sprintf(temp_string,"%s\tADDF R%d.1000, R%d.xyzw, R%d.wyzw, <4>\n",temp_string,vector_reg,vector_reg,vector_reg);


  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <4>\n",temp_string,remain_reg,remain_reg2);//20   4
  sprintf(temp_string,"%s\tDSQ R%d.1000, R%d, R%d\n",temp_string,remain_reg2,remain_reg2,vector_reg);
  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.yyzw, <4>\n",temp_string,remain_reg,remain_reg2);
  sprintf(temp_string,"%s\tDSQ R%d.1000, R%d, R%d\n",temp_string,remain_reg,remain_reg,vector_reg);
  sprintf(temp_string,"%s\tMVR R%d.0100, R%d.xxzw, <4>\n",temp_string,remain_reg2,remain_reg);
  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.zyzw, <4>\n",temp_string,remain_reg,remain_reg2);
  sprintf(temp_string,"%s\tDSQ R%d.1000, R%d, R%d\n",temp_string,remain_reg,remain_reg,vector_reg);
  sprintf(temp_string,"%s\tMVR R%d.0010, R%d.xyxw, <4>\n",temp_string,remain_reg2,remain_reg);
  sprintf(temp_string,"%s\tMVR R%d.1000, R%d.wyzw, <4>\n",temp_string,remain_reg,remain_reg2);
  sprintf(temp_string,"%s\tDSQ R%d.1000, R%d, R%d\n",temp_string,remain_reg,remain_reg,vector_reg);
  sprintf(temp_string,"%s\tMVR R%d.0001, R%d.xyzx, <4>\n",temp_string,remain_reg2,remain_reg);

  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzx, <4>\n",temp_string,return_reg,remain_reg2);//31  15

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code

  float temp_float=0.0;
  unsigned temp_int;
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=vector_reg;
  temp_b_code[0]=temp_b_code[0]<<38;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=vector_num>3?0:vector_num>2?1:vector_num>1?3:7;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2;i++)
      BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
      BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifndef unified_branch
  temp_b_code[0]=24;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=vector_reg;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
  temp_b_code[0]|=34;
#else
  temp_b_code[0]|=17;
#endif
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=vector_reg;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<10;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
#else
  temp_b_code[0]=24;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=vector_reg;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
  temp_b_code[0]|=34;
#else
  temp_b_code[0]|=17;
#endif
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=0x1b1b;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
#endif

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2;i++)
      BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
      BinCode[LastBinLine++]=temp_b_code[i];
#endif

  unsigned long long temp;

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_reg2;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=vector_reg;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1B1B;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=0xF;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  
  temp=temp_b_code[0];

  temp&=(long long unsigned)0xF81FFFFE01FFC3FFLL;
  temp|=(1<<13);
  temp|=((long long unsigned)(0x5B)<<25);
  temp|=((long long unsigned)remain_reg<<53);
  temp_b_code[3]=temp;

  temp&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
  temp|=((long long unsigned)(0x9B)<<25);
  temp_b_code[5]=temp;

  temp&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
  temp|=((long long unsigned)(0xDB)<<25);
  temp_b_code[7]=temp;

  temp_b_code[10]=temp_b_code[3];
  temp_b_code[12]=temp_b_code[5];
  temp_b_code[14]=temp_b_code[7];

  temp&=(long long unsigned)0xFFE07FFE01FFC3FFLL;
  temp|=(long long unsigned)remain_reg2<<47;
  temp|=(long long unsigned)(0x1B)<<25;
  temp|=(15<<10);
  temp_b_code[20]=temp;

  temp=temp_b_code[10];
  temp&=(long long unsigned)0xFFE07FFFFFFFFDFFLL;
  temp|=((long long unsigned)remain_reg2<<47);
  temp|=((long long unsigned)1<<9);
  temp_b_code[22]=temp;

  temp&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
  temp|=((long long unsigned)(0x9B)<<25);
  temp_b_code[25]=temp;

  temp&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
  temp|=((long long unsigned)(0xDB)<<25);
  temp_b_code[28]=temp;

  temp&=(long long unsigned)0xF8007FFE01FFC3FFLL;
  temp|=((long long unsigned)remain_reg2<<53);
  temp|=((long long unsigned)remain_reg<<47);
  temp|=((long long unsigned)(0xB)<<25);
  temp|=(4<<10);
  temp_b_code[24]=temp;

  temp&=(long long unsigned)0xFFFFFFFE01FFC3FFLL;
  temp|=((long long unsigned)(0x13)<<25);
  temp|=(2<<10);
  temp_b_code[27]=temp;

  temp&=(long long unsigned)0xFFFFFFFE01FFC3FFLL;
  temp|=((long long unsigned)(0x18)<<25);
  temp|=(1<<10);
  temp_b_code[30]=temp;

  temp_b_code[31]=temp_b_code[20];
  temp_b_code[31]&=(long long unsigned)0xF8007FFFFFFFFFFFLL;
  temp_b_code[31]|=(long long unsigned)return_reg<<53;
  temp_b_code[31]|=(long long unsigned)remain_reg2<<47;

  temp_float=2.0;

  memcpy(&temp_int,&temp_float,4);

  //printf("temp_float:%f\n",temp_float);
  //printf("temp_int:%f\n",temp_int);
  //printf("temp_int:%d\n",temp_int);


  //need to fix
  /*
  temp_b_code[1]=7;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_reg;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=4;
  temp_b_code[1]=temp_b_code[1]<<22;
  temp_b_code[1]|=0x1B1B;
  temp_b_code[1]=temp_b_code[1]<<15;
  temp_b_code[1]|=4;
  temp_b_code[1]=temp_b_code[1]<<10;
  */


  //temp_b_code[1]=10;
  //temp_b_code[1]=temp_b_code[1]<<6;
  //temp_b_code[1]|=remain_reg;
  //temp_b_code[1]=temp_b_code[1]<<38;
  //temp_b_code[1]|=temp_int;
  //temp_b_code[1]=temp_b_code[1]<<6;
  //temp_b_code[1]|=9;
  //temp_b_code[1]=temp_b_code[1]<<2;
  //temp_b_code[1]|=1;
  //temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[1]=7;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_reg;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=vector_reg;
  temp_b_code[1]=temp_b_code[1]<<22;
  temp_b_code[1]|=0x1B1B;
  temp_b_code[1]=temp_b_code[1]<<15;
  temp_b_code[1]|=0xF;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=5;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=vector_reg;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=vector_reg;
  temp_b_code[2]=temp_b_code[2]<<38;
  temp_b_code[2]|=17;
  temp_b_code[2]=temp_b_code[2]<<9;
  temp=temp_b_code[2];

  temp&=(long long unsigned)0xFFE07FFFFFFFC3FFLL;
  temp|=((long long unsigned)remain_reg<<47);
  temp|=(4<<10);
  temp_b_code[4]=temp;

  temp&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
  temp|=(2<<10);
  temp_b_code[6]=temp;

  temp&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
  temp|=(1<<10);
  temp_b_code[8]=temp;

  temp_b_code[16]=17;
  temp_b_code[16]=temp_b_code[16]<<6;
  temp_b_code[16]|=vector_reg;
  temp_b_code[16]=temp_b_code[16]<<6;
  temp_b_code[16]|=vector_reg;
  temp_b_code[16]=temp_b_code[16]<<6;
  temp_b_code[16]|=vector_reg;
  temp_b_code[16]=temp_b_code[16]<<24;
  temp_b_code[16]|=0x1B1B;
  temp_b_code[16]=temp_b_code[16]<<7;
  temp_b_code[16]|=15;
  temp_b_code[16]=temp_b_code[16]<<3;
  temp_b_code[16]|=7;
  temp_b_code[16]=temp_b_code[16]<<7;

  temp_b_code[9]=4;
  temp_b_code[9]=temp_b_code[9]<<6;
  temp_b_code[9]|=vector_reg;
  temp_b_code[9]=temp_b_code[9]<<6;
  temp_b_code[9]|=vector_reg;
  temp_b_code[9]=temp_b_code[9]<<38;
  temp_b_code[9]|=17;
  temp_b_code[9]=temp_b_code[9]<<9;
  temp=temp_b_code[9];
  
  temp&=(long long unsigned)0xFFE07FFFFFFFC3FFLL;
  temp|=((long long unsigned)remain_reg<<47);
  temp|=(4<<10);
  temp_b_code[11]=temp;

  temp&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
  temp|=(2<<10);
  temp_b_code[13]=temp;

  temp&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
  temp|=(1<<10);
  temp_b_code[15]=temp;
  
  temp_b_code[17]=16;
  temp_b_code[17]=temp_b_code[17]<<6;
  temp_b_code[17]|=vector_reg;
  temp_b_code[17]=temp_b_code[17]<<6;
  temp_b_code[17]|=vector_reg;
  temp_b_code[17]=temp_b_code[17]<<6;
  temp_b_code[17]|=vector_reg;
  temp_b_code[17]=temp_b_code[17]<<24;
  temp_b_code[17]|=0x1B1B5B;
  temp_b_code[17]=temp_b_code[17]<<7;
  temp_b_code[17]|=8;
  temp_b_code[17]=temp_b_code[17]<<3;
  temp_b_code[17]|=7;
  temp_b_code[17]=temp_b_code[17]<<7;
  temp=temp_b_code[17];

  temp&=(long long unsigned)0xFFFFFFFFFE01FFFFLL;
  temp|=((long long unsigned)(0x9B)<<17);
  temp_b_code[18]=temp;

  temp&=(long long unsigned)0xFFFFFFFFFE01FFFFLL;
  temp|=((long long unsigned)(0xDB)<<17);
  temp_b_code[19]=temp;

  temp_b_code[21]=1;
  temp_b_code[21]=temp_b_code[21]<<6;
  temp_b_code[21]|=remain_reg2;
  temp_b_code[21]=temp_b_code[21]<<6;
  temp_b_code[21]|=vector_reg;
  temp_b_code[21]=temp_b_code[21]<<38;
  temp_b_code[21]|=17;
  temp_b_code[21]=temp_b_code[21]<<9;
  temp=temp_b_code[21];  

  temp&=(long long unsigned)0xF81FFFFFFFFFFFFFLL;
  temp|=((long long unsigned)remain_reg<<53);
  temp_b_code[23]=temp;
  temp_b_code[26]=temp;
  temp_b_code[29]=temp;


  for(i=0;i<16;i++)
    temp_b_code[i+2]=temp_b_code[i+16];


#ifdef SET_NOP_NINE
  insert_nop_nine(18);
  for(i=0;i<18*2-1;i++)
      BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<18;i++)
      BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),31,output_file);
#endif

#ifdef SET_NOP_NINE
  return (18+1)*2-1;
#else
  return 18+1;
#endif
}

int GL_Fract(int return_regi,
	     int regiX,
	     int remain_regi,
	     int remain_regi2,
	     char VOrF,
	     int SV,
	     FILE *output_ass)
{
  int i;
  int instr_count=0;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"FRACT_R%d_R%d_R%d_R%d_ass_file",return_regi,regiX,remain_regi,remain_regi2);
      ass_file=fopen(temp_string,"w");
    }

  instr_count+=GL_Floor(remain_regi2,regiX,remain_regi,VOrF,SV,output_ass);

  //assembly code
  sprintf(temp_string,"\tADDF R%d.1%d%d%d, R%d.xyzw, -R%d.xyzw, <%d>\n",return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,regiX,remain_regi2,SV);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
  if(!output_ass)
    fclose(ass_file);


  //machine code
  temp_b_code[0]=16;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi2;
  temp_b_code[0]=temp_b_code[0]<<24;
  temp_b_code[0]|=0x1b1b1b;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<4;
  temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  BinCode[LastBinLine++]=nop_nine_instruction;
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count += 2;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count += 1;
#endif

  return instr_count;

}

int GL_Ceil(int return_regi,
	    int regiX,
	    char VOrF,
	    int SV,
	    FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"CEIL_R%d_R%d_ass_file",return_regi,regiX);
      ass_file=fopen(temp_string,"w");
    }
  

  sprintf(temp_string,"\tMVR R%d.1%d%d%d, R%d.xyzw, <%d>\n",return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,regiX,SV);
  if(VOrF == 'v')
    {
      switch(SV)
	{
	  /*
	case 4:
	  sprintf(temp_string,"%s\tBGTVF R0, R%d, R0, R%d, 2, 0001, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0001, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.0001, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0001, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTVF R0, R%d, R0, R%d, 3, 0001, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0001, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 0001, <%d>\n",temp_string,return_regi,regiX,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0001, R%d, -1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	case 3:
	  sprintf(temp_string,"%s\tBGTVF R0, R%d, R0, R%d, 2, 0010, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0010, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.0010, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0010, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTVF R0, R%d, R0, R%d, 3, 0010, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0010, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 0010, <%d>\n",temp_string,return_regi,regiX,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0010, R%d, -1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	case 2:
	  sprintf(temp_string,"%s\tBGTVF R0, R%d, R0, R%d, 2, 0100, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0100, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.0100, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0100, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTVF R0, R%d, R0, R%d, 3, 0100, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0100, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 0100, <%d>\n",temp_string,return_regi,regiX,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0100, R%d, -1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	case 1:
	  sprintf(temp_string,"%s\tBGTVF R0, R%d, R0, R%d, 2, 1000, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.1000, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.1000, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.1000, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTVF R0, R%d, R0, R%d, 3, 1000, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.1000, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 1000, <%d>\n",temp_string,return_regi,regiX,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.1000, R%d, -1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	  break;
	default:
	  perror("GL_Ceil(): SV error.");
	}
	  */
	case 4:
	  sprintf(temp_string,"%s\tFTOI R%d.0001, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0001, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTVF R0, R%d, R0, R%d, 3, 0001, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 0001, <%d>\n",temp_string,return_regi,regiX,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0001, R%d, 1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	case 3:
	  sprintf(temp_string,"%s\tFTOI R%d.0010, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0010, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTVF R0, R%d, R0, R%d, 3, 0010, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 0010, <%d>\n",temp_string,return_regi,regiX,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0010, R%d, 1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	case 2:
	  sprintf(temp_string,"%s\tFTOI R%d.0100, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0100, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTVF R0, R%d, R0, R%d, 3, 0100, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 0100, <%d>\n",temp_string,return_regi,regiX,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0100, R%d, 1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	case 1:
	  sprintf(temp_string,"%s\tFTOI R%d.1000, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.1000, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTVF R0, R%d, R0, R%d, 3, 1000, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 1000, <%d>\n",temp_string,return_regi,regiX,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.1000, R%d, 1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	  break;
	default:
	  perror("GL_Ceil(): SV error.");
	}
    }
  else
    {
      switch(SV)
	{
	  /*
	case 4:
	  sprintf(temp_string,"%s\tBGTSF R0.wwww, R%d.wwww, 2, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0001, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.0001, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0001, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTSF R0.wwww, R%d.wwww, 3, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0001, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBEQS R%d.wwww, R%d.wwww, 2, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0001, R%d, -1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	case 3:
	  sprintf(temp_string,"%s\tBGTSF R0.zzzz, R%d.zzzz, 2, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0010, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.0010, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0010, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTSF R0.zzzz, R%d.zzzz, 3, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0010, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBEQS R%d.zzzz, R%d.zzzz, 2, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0010, R%d, -1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	case 2:
	  sprintf(temp_string,"%s\tBGTSF R0.yyyy, R%d.yyyy, 2, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0100, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.0100, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0100, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTSF R0.yyyy, R%d.yyyy, 3, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0100, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBEQS R%d.yyyy, R%d.yyyy, 2, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0100, R%d, -1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	case 1:
	  sprintf(temp_string,"%s\tBGTSF R0.xxxx, R%d.xxxx, 2, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.1000, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.1000, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.1000, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTSF R0.xxxx, R%d.xxxx, 3, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.1000, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBEQS R%d.xxxx, R%d.xxxx, 2, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.1000, R%d, -1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	  break;
	default:
	  perror("GL_Ceil(): SV error.");
	  break;
	  */
	case 4:
	  sprintf(temp_string,"%s\tFTOI R%d.0001, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0001, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTSF R0.wwww, R%d.wwww, 3, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tBEQS R%d.wwww, R%d.wwww, 2, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0001, R%d, 1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	case 3:
	  sprintf(temp_string,"%s\tFTOI R%d.0010, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0010, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTSF R0.zzzz, R%d.zzzz, 3, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tBEQS R%d.zzzz, R%d.zzzz, 2, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0010, R%d, 1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	case 2:
	  sprintf(temp_string,"%s\tFTOI R%d.0100, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0100, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTSF R0.yyyy, R%d.yyyy, 3, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tBEQS R%d.yyyy, R%d.yyyy, 2, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0100, R%d, 1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	case 1:
	  sprintf(temp_string,"%s\tFTOI R%d.1000, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.1000, R%d, <%d>\n",temp_string,return_regi,return_regi,SV);
	  sprintf(temp_string,"%s\tBGTSF R0.xxxx, R%d.xxxx, 3, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tBEQS R%d.xxxx, R%d.xxxx, 2, <%d>\n",temp_string,return_regi,regiX,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.1000, R%d, 1.0, <%d>\n",temp_string,return_regi,return_regi,SV);
	  break;
	default:
	  perror("GL_Ceil(): SV error.");
	  break;
	}
    }

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
  if(!output_ass)
    fclose(ass_file);
  
  //machine code
  /************************************************old************************ and it's wrong.
  //float temp_float=-1.0;
  float temp_float=1.0; //It should be +1.
  unsigned temp_int;

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1b1b;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=SV>1?SV>2?SV>3?15:14:12:8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

  temp_b_code[1]=16;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=return_regi;
  temp_b_code[1]=temp_b_code[1]<<12;
  temp_b_code[1]|=regiX;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=0x1b1b1b;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=2;
  temp_b_code[1]=temp_b_code[1]<<4;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=13;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=return_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=return_regi;
  temp_b_code[2]=temp_b_code[2]<<37;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=SV-1;
  temp_b_code[2]=temp_b_code[2]<<7;

  temp_b_code[3]=temp_b_code[2];
  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFFDFFLL;

  temp_b_code[5]=temp_b_code[1];
  temp_b_code[5]&=(long long unsigned)0xFFFF81FFFFFFFFFFLL;  
  temp_b_code[5]|=(long long unsigned)((long long unsigned)return_regi<<41);  

  temp_b_code[7]=10;
  temp_b_code[7]=temp_b_code[7]<<6;
  temp_b_code[7]|=return_regi;
  temp_b_code[7]=temp_b_code[7]<<6;
  temp_b_code[7]|=return_regi;
  temp_b_code[7]=temp_b_code[7]<<32;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[7]|=temp_int;
  temp_b_code[7]=temp_b_code[7]<<5;
  temp_b_code[7]|=1;
  temp_b_code[7]=temp_b_code[7]<<1;
  temp_b_code[7]|=1;
  temp_b_code[7]=temp_b_code[7]<<2;
  temp_b_code[7]|=SV-1;
  temp_b_code[7]=temp_b_code[7]<<7;  

  if(VOrF=='v')
    {
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<16;
      //temp_b_code[0]|=2;
      temp_b_code[0]|=4;  //change 2 to 4
      temp_b_code[0]=temp_b_code[0]<<11;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

      temp_b_code[4]=temp_b_code[0];
      temp_b_code[4]&=(long long unsigned)0xFFFF80007FFFFFFFLL;  
      //temp_b_code[4]|=(long long unsigned)((long long unsigned)3<<31);
      temp_b_code[4]|=(long long unsigned)((long long unsigned)6<<31); //change 3 to 6

      temp_b_code[6]=temp_b_code[0];
      temp_b_code[6]&=(long long unsigned)0x001FFFFF81FFFDFFLL;
      temp_b_code[6]|=(long long unsigned)((long long unsigned)24<<59);
      temp_b_code[6]|=(long long unsigned)((long long unsigned)return_regi<<53);
      temp_b_code[6]|=(long long unsigned)((long long unsigned)return_regi<<25);

      switch(SV)
	{
	case 4:
#ifdef SET_NOP_NINE
  insert_nop_nine(8);
  for(i=0;i<8*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<8;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 3:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[5]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[5]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[6]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[7]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[7]|=(long long unsigned)((long long unsigned)2<<10);

#ifdef SET_NOP_NINE
  insert_nop_nine(8);
  for(i=0;i<8*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<8;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 2:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[5]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[5]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[6]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[7]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[7]|=(long long unsigned)((long long unsigned)4<<10);

#ifdef SET_NOP_NINE
  insert_nop_nine(8);
  for(i=0;i<8*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<8;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 1:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[5]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[5]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[6]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[7]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[7]|=(long long unsigned)((long long unsigned)8<<10);

#ifdef SET_NOP_NINE
  insert_nop_nine(8);
  for(i=0;i<8*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<8;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	  break;
	default:
	  perror("GL_Ceil(): SV error.");
	  break;
	}
    }
  else
    {

      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=2;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xffff;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

      temp_b_code[4]=temp_b_code[0];
      temp_b_code[4]&=(long long unsigned)0xFFFF80007FFFFFFFLL;  
      temp_b_code[4]|=(long long unsigned)((long long unsigned)3<<31);

      temp_b_code[6]=temp_b_code[0];
      temp_b_code[6]&=(long long unsigned)0x001FFFFFFFFFFDFFLL;
      temp_b_code[6]|=(long long unsigned)((long long unsigned)24<<59);
      temp_b_code[6]|=(long long unsigned)((long long unsigned)return_regi<<53);

      switch(SV)
	{
	case 4:
#ifdef SET_NOP_NINE
  insert_nop_nine(8);
  for(i=0;i<8*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<8;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 3:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)0xAAAA<<15);
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)0xAAAA<<15);
	  temp_b_code[5]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[5]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[6]|=(long long unsigned)((long long unsigned)0xAAAA<<15);
	  temp_b_code[7]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[7]|=(long long unsigned)((long long unsigned)2<<10);

#ifdef SET_NOP_NINE
  insert_nop_nine(8);
  for(i=0;i<8*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<8;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 2:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)0x5555<<15);
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)0x5555<<15);
	  temp_b_code[5]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[5]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[6]|=(long long unsigned)((long long unsigned)0x5555<<15);
	  temp_b_code[7]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[7]|=(long long unsigned)((long long unsigned)4<<10);

#ifdef SET_NOP_NINE
  insert_nop_nine(8);
  for(i=0;i<8*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<8;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 1:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[5]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[5]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[7]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[7]|=(long long unsigned)((long long unsigned)8<<10);

#ifdef SET_NOP_NINE
  insert_nop_nine(8);
  for(i=0;i<8*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<8;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	  break;
	default:
	  perror("GL_Ceil(): SV error.");
	  break;
	}
    }

#ifdef SET_NOP_NINE
  return (1+SV*8)*2-1;
#else
  return 1+SV*8;
#endif
  */
  float temp_float=1.0; //It should be +1.
  unsigned temp_int;

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1b1b;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=SV>1?SV>2?SV>3?15:14:12:8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif



  temp_b_code[0]=13;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<37;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=temp_b_code[0];
  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFFDFFLL;


  temp_b_code[4]=10;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=return_regi;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=return_regi;
  temp_b_code[4]=temp_b_code[4]<<32;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[4]|=temp_int;
  temp_b_code[4]=temp_b_code[4]<<5;
  temp_b_code[4]|=1;
  temp_b_code[4]=temp_b_code[4]<<1;
  temp_b_code[4]|=1;
  temp_b_code[4]=temp_b_code[4]<<2;
  temp_b_code[4]|=SV-1;
  temp_b_code[4]=temp_b_code[4]<<7;  

  if(VOrF=='v')
    {
#ifndef unified_branch
      temp_b_code[2]=27;
      temp_b_code[2]=temp_b_code[2]<<12;
      temp_b_code[2]|=regiX;
      temp_b_code[2]=temp_b_code[2]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[2]|=6;  //change 3 to 6
#else
      temp_b_code[2]|=3;
#endif
      temp_b_code[2]=temp_b_code[2]<<11;
      temp_b_code[2]|=regiX;
      temp_b_code[2]=temp_b_code[2]<<10;
      temp_b_code[2]|=1;
      temp_b_code[2]=temp_b_code[2]<<1;
      temp_b_code[2]|=1;
      temp_b_code[2]=temp_b_code[2]<<2;
      temp_b_code[2]|=SV-1;
      temp_b_code[2]=temp_b_code[2]<<7;

      temp_b_code[3]=temp_b_code[2];
      temp_b_code[3]&=(long long unsigned)0x001F800001FFFDFFLL;
      temp_b_code[3]|=(long long unsigned)((long long unsigned)24<<59);
      temp_b_code[3]|=(long long unsigned)((long long unsigned)return_regi<<53);
      temp_b_code[3]|=(long long unsigned)((long long unsigned)return_regi<<25);
#ifdef SET_NOP_NINE
      temp_b_code[3]|=(long long unsigned)((long long unsigned)4<<31); //change 2 to 4
#else
      temp_b_code[3]|=(long long unsigned)((long long unsigned)2<<31); //change 2 to 4
#endif
#else     
      //****unified_branch********************
      temp_b_code[2]=27;
      temp_b_code[2]=temp_b_code[2]<<12;
      temp_b_code[2]|=regiX;
      temp_b_code[2]=temp_b_code[2]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[2]|=6;  //change 3 to 6
#else
      temp_b_code[2]|=3;
#endif
      temp_b_code[2]=temp_b_code[2]<<16;
      temp_b_code[2]|=0XFFFF;
      temp_b_code[2]=temp_b_code[2]<<1;
      temp_b_code[2]|=1;
      temp_b_code[2]=temp_b_code[2]<<5;
      temp_b_code[2]|=1;
      temp_b_code[2]=temp_b_code[2]<<2;
      temp_b_code[2]|=SV-1;
      temp_b_code[2]=temp_b_code[2]<<7;

      temp_b_code[3]=temp_b_code[2];
      temp_b_code[3]&=(long long unsigned)0x001F80007FFFFDFFLL;
      temp_b_code[3]|=(long long unsigned)((long long unsigned)24<<59);
      temp_b_code[3]|=(long long unsigned)((long long unsigned)return_regi<<53);
#ifdef SET_NOP_NINE
      temp_b_code[3]|=(long long unsigned)((long long unsigned)4<<31); //change 2 to 4
#else
      temp_b_code[3]|=(long long unsigned)((long long unsigned)2<<31); //change 2 to 4
#endif
#endif
      switch(SV)
	{
	case 4:
#ifdef SET_NOP_NINE
  insert_nop_nine(5);
  for(i=0;i<5*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<5;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 3:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)2<<10);
#ifndef unified_branch
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)2<<10);
#else
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)0xAAAA<<15);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)0xAAAA<<15);
#endif
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)2<<10);

#ifdef SET_NOP_NINE
  insert_nop_nine(5);
  for(i=0;i<5*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<5;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 2:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)4<<10);
#ifndef unified_branch
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)4<<10);
#else
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)0x5555<<15);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)0x5555<<15);
#endif
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)4<<10);

#ifdef SET_NOP_NINE
  insert_nop_nine(5);
  for(i=0;i<5*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<5;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 1:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)8<<10);
#ifndef unified_branch
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)8<<10);
#else
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
#endif
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)8<<10);

#ifdef SET_NOP_NINE
  insert_nop_nine(5);
  for(i=0;i<5*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<5;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	  break;
	default:
	  perror("GL_Ceil(): SV error.");
	  break;
	}
    }
  else
    {

      temp_b_code[2]=27;
      temp_b_code[2]=temp_b_code[2]<<12;
      temp_b_code[2]|=regiX;
      temp_b_code[2]=temp_b_code[2]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[2]|=6; //change 3 to 6
#else
      temp_b_code[2]|=3; 
#endif
      temp_b_code[2]=temp_b_code[2]<<16;
      temp_b_code[2]|=0xffff;
      temp_b_code[2]=temp_b_code[2]<<1;
      temp_b_code[2]|=1;
      temp_b_code[2]=temp_b_code[2]<<5;
      temp_b_code[2]|=1;
      temp_b_code[2]=temp_b_code[2]<<2;
      temp_b_code[2]|=SV-1;
      temp_b_code[2]=temp_b_code[2]<<7;

      temp_b_code[3]=temp_b_code[2];
      //temp_b_code[3]&=(long long unsigned)0x001FFFFFFFFFFDFFLL;
      temp_b_code[3]&=(long long unsigned)0x001F800001FFFDFFLL; //set zero to the offset
      temp_b_code[3]|=(long long unsigned)((long long unsigned)24<<59);
      temp_b_code[3]|=(long long unsigned)((long long unsigned)return_regi<<53);
#ifdef SET_NOP_NINE
      temp_b_code[3]|=(long long unsigned)((long long unsigned)4<<31);
#else
      temp_b_code[3]|=(long long unsigned)((long long unsigned)2<<31);
#endif

      switch(SV)
	{
	case 4:
#ifdef SET_NOP_NINE
  insert_nop_nine(5);
  for(i=0;i<5*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<5;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 3:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)0xAAAA<<15);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)0xAAAA<<15);
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)2<<10);

#ifdef SET_NOP_NINE
  insert_nop_nine(5);
  for(i=0;i<5*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<5;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 2:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)0x5555<<15);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)0x5555<<15);
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)4<<10);

#ifdef SET_NOP_NINE
  insert_nop_nine(5);
  for(i=0;i<5*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<5;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 1:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)8<<10);

#ifdef SET_NOP_NINE
  insert_nop_nine(5);
  for(i=0;i<5*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<5;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	  break;
	default:
	  perror("GL_Ceil(): SV error.");
	  break;
	}
    }

#ifdef SET_NOP_NINE
  return (1+SV*5)*2-1;
#else
  return 1+SV*5;
#endif
  
}

int GL_Floor(int return_regi,
	     int regiX,
	     int remain_regi,
	     char VOrF,
	     int SV,
	     FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"FLOOR_R%d_R%d_R%d_ass_file",return_regi,regiX,remain_regi);
      ass_file=fopen(temp_string,"w");
    }
  

  sprintf(temp_string,"\tMVR R%d.1%d%d%d, R%d.xyzw, <%d>\n",remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,regiX,SV);
  if(VOrF == 'v')
    {
      switch(SV)
	{
	case 4:
	  sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 2, 0001, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0001, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.0001, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0001, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 4, 0001, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0001, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 0001, <%d>\n",temp_string,regiX,remain_regi,regiX,remain_regi,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0001, R%d, -1.0, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	case 3:
	  sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 2, 0010, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0010, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.0010, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0010, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 4, 0010, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0010, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 0010, <%d>\n",temp_string,regiX,remain_regi,regiX,remain_regi,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0010, R%d, -1.0, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	case 2:
	  sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 2, 0100, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0100, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.0100, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0100, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 4, 0100, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0100, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 0100, <%d>\n",temp_string,regiX,remain_regi,regiX,remain_regi,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0100, R%d, -1.0, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	case 1:
	  sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 2, 1000, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.1000, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.1000, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.1000, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 4, 1000, <%d>\n",temp_string,regiX,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.1000, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 1000, <%d>\n",temp_string,regiX,remain_regi,regiX,remain_regi,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.1000, R%d, -1.0, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  break;
	default:
	  perror("GL_Floor(): SV error.");
	}
    }
  else
    {
      switch(SV)
	{
	case 4:
	  sprintf(temp_string,"%s\tBGTSF R%d.wwww, R0.wwww, 2, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0001, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.0001, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0001, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBGTSF R%d.wwww, R0.wwww, 4, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0001, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBEQS R%d.wwww, R%d.wwww, 2, <%d>\n",temp_string,regiX,remain_regi,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0001, R%d, -1.0, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	case 3:
	  sprintf(temp_string,"%s\tBGTSF R%d.zzzz, R0.zzzz, 2, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0010, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.0010, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0010, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBGTSF R%d.zzzz, R0.zzzz, 4, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0010, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBEQS R%d.zzzz, R%d.zzzz, 2, <%d>\n",temp_string,regiX,remain_regi,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0010, R%d, -1.0, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	case 2:
	  sprintf(temp_string,"%s\tBGTSF R%d.yyyy, R0.yyyy, 2, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0100, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.0100, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.0100, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBGTSF R%d.yyyy, R0.yyyy, 4, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.0100, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBEQS R%d.yyyy, R%d.yyyy, 2, <%d>\n",temp_string,regiX,remain_regi,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.0100, R%d, -1.0, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	case 1:
	  sprintf(temp_string,"%s\tBGTSF R%d.xxxx, R0.xxxx, 2, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.1000, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
	  sprintf(temp_string,"%s\tFTOI R%d.1000, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tITOF R%d.1000, R%d, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBGTSF R%d.xxxx, R0.xxxx, 4, <%d>\n",temp_string,regiX,SV);
	  sprintf(temp_string,"%s\tADDF R%d.1000, R0.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  sprintf(temp_string,"%s\tBEQS R%d.xxxx, R%d.xxxx, 2, <%d>\n",temp_string,regiX,remain_regi,SV);
	  sprintf(temp_string,"%s\tADDIF R%d.1000, R%d, -1.0, <%d>\n",temp_string,remain_regi,remain_regi,SV);
	  break;
	default:
	  perror("GL_Floor(): SV error.");
	  break;
	}
    }

  sprintf(temp_string,"%s\tMVR R%d.1%d%d%d, R%d.xyzw, <%d>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,remain_regi,SV);


  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
  if(!output_ass)
    fclose(ass_file);
  
  //machine code

  float temp_float=-1.0;
  unsigned temp_int;

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1b1b;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=SV>1?SV>2?SV>3?15:14:12:8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

  temp_b_code[1]=16;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<12;
  temp_b_code[1]|=regiX;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=0x1b1b1b;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=2;
  temp_b_code[1]=temp_b_code[1]<<4;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=13;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<37;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=SV-1;
  temp_b_code[2]=temp_b_code[2]<<7;

  temp_b_code[3]=temp_b_code[2];
  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFFDFFLL;

  temp_b_code[5]=temp_b_code[1];
  temp_b_code[5]&=(long long unsigned)0xFFFF81FFFFFFFFFFLL;  
  temp_b_code[5]|=(long long unsigned)((long long unsigned)remain_regi<<41);  

  temp_b_code[7]=10;
  temp_b_code[7]=temp_b_code[7]<<6;
  temp_b_code[7]|=remain_regi;
  temp_b_code[7]=temp_b_code[7]<<6;
  temp_b_code[7]|=remain_regi;
  temp_b_code[7]=temp_b_code[7]<<32;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[7]|=temp_int;
  temp_b_code[7]=temp_b_code[7]<<5;
  temp_b_code[7]|=1;
  temp_b_code[7]=temp_b_code[7]<<1;
  temp_b_code[7]|=1;
  temp_b_code[7]=temp_b_code[7]<<2;
  temp_b_code[7]|=SV-1;
  temp_b_code[7]=temp_b_code[7]<<7;  

  if(VOrF=='v')
    {
#ifndef unified_branch
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<22;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4; //change 2 to 4
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<15;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

      temp_b_code[4]=temp_b_code[0];
      temp_b_code[4]&=(long long unsigned)0xFFFF80007FFFFFFFLL;  
      //temp_b_code[4]|=(long long unsigned)((long long unsigned)4<<31);
#ifdef SET_NOP_NINE
      temp_b_code[4]|=(long long unsigned)((long long unsigned)8<<31);//change 4 to 8
#else
      temp_b_code[4]|=(long long unsigned)((long long unsigned)4<<31);
#endif
      temp_b_code[6]=temp_b_code[0];
      temp_b_code[6]&=(long long unsigned)0x07E07FFFFE0FFDFFLL;
      temp_b_code[6]|=(long long unsigned)((long long unsigned)24<<59);
      temp_b_code[6]|=(long long unsigned)((long long unsigned)remain_regi<<47);
      temp_b_code[6]|=(long long unsigned)((long long unsigned)remain_regi<<20); 
#else 
      //**********************unified_branch**************
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<22;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4; //change 2 to 4
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xffff;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

      temp_b_code[4]=temp_b_code[0];
      temp_b_code[4]&=(long long unsigned)0xFFFF80007FFFFFFFLL;  
#ifdef SET_NOP_NINE
      temp_b_code[4]|=(long long unsigned)((long long unsigned)8<<31);
#else
      temp_b_code[4]|=(long long unsigned)((long long unsigned)4<<31);
#endif
      temp_b_code[6]=temp_b_code[0];
      temp_b_code[6]&=(long long unsigned)0x07E07FFFFFFFFDFFLL;
      temp_b_code[6]|=(long long unsigned)((long long unsigned)24<<59);
      temp_b_code[6]|=(long long unsigned)((long long unsigned)remain_regi<<47);
#endif
      switch(SV)
	{
	case 4:
#ifdef SET_NOP_NINE
	  insert_nop_nine(8);
	  for(i=0;i<8*2;i++)
	    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
	  for(i=0;i<8;i++)
	    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 3:
#ifndef unified_branch	  
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)2<<10);
#else
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)0xAAAA<<15);
#endif
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)2<<10);
#ifndef unified_branch
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)2<<10);
#else
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)0xAAAA<<15);
#endif
	  temp_b_code[5]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[5]|=(long long unsigned)((long long unsigned)2<<10);
#ifndef unified_branch
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[6]|=(long long unsigned)((long long unsigned)2<<10);
#else
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[6]|=(long long unsigned)((long long unsigned)0xAAAA<<15);
#endif
	  temp_b_code[7]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[7]|=(long long unsigned)((long long unsigned)2<<10);

#ifdef SET_NOP_NINE
	  insert_nop_nine(8);
	  for(i=0;i<8*2;i++)
	    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
	  for(i=0;i<8;i++)
	    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 2:

#ifndef unified_branch
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)4<<10);
#else
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)0x5555<<15);
#endif
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)4<<10);
#ifndef unified_branch
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)4<<10);
#else
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)0x5555<<15);
#endif
	  temp_b_code[5]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[5]|=(long long unsigned)((long long unsigned)4<<10);
#ifndef unified_branch
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[6]|=(long long unsigned)((long long unsigned)4<<10);
#else
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[6]|=(long long unsigned)((long long unsigned)0x5555<<15);
#endif
	  temp_b_code[7]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[7]|=(long long unsigned)((long long unsigned)4<<10);

#ifdef SET_NOP_NINE
	  insert_nop_nine(8);
	  for(i=0;i<8*2;i++)
	    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
	  for(i=0;i<8;i++)
	    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 1:

#ifndef unified_branch
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)8<<10);
#else
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
#endif
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)8<<10);
#ifndef unified_branch
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)8<<10);
#else
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
#endif
	  temp_b_code[5]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[5]|=(long long unsigned)((long long unsigned)8<<10);
#ifndef unified_branch
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[6]|=(long long unsigned)((long long unsigned)8<<10);
#else
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
#endif
	  temp_b_code[7]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[7]|=(long long unsigned)((long long unsigned)8<<10);

#ifdef SET_NOP_NINE
	  insert_nop_nine(8);
	  for(i=0;i<8*2;i++)
	    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
	  for(i=0;i<8;i++)
	    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	  break;
	default:
	  perror("GL_Floor(): SV error.");
	  break;
	}
    }
  else
    {

      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<22;
      temp_b_code[0]|=2;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xffff;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

      temp_b_code[4]=temp_b_code[0];
      temp_b_code[4]&=(long long unsigned)0xFFFF80007FFFFFFFLL;  
      temp_b_code[4]|=(long long unsigned)((long long unsigned)4<<31);

      temp_b_code[6]=temp_b_code[0];
      temp_b_code[6]&=(long long unsigned)0x07E07FFFFFFFFDFFLL;
      temp_b_code[6]|=(long long unsigned)((long long unsigned)24<<59);
      temp_b_code[6]|=(long long unsigned)((long long unsigned)remain_regi<<47);

      switch(SV)
	{
	case 4:
#ifdef SET_NOP_NINE
	  insert_nop_nine(8);
	  for(i=0;i<8*2;i++)
	    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
	  for(i=0;i<8;i++)
	    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 3:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)0xAAAA<<15);
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)0xAAAA<<15);
	  temp_b_code[5]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[5]|=(long long unsigned)((long long unsigned)2<<10);
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[6]|=(long long unsigned)((long long unsigned)0xAAAA<<15);
	  temp_b_code[7]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[7]|=(long long unsigned)((long long unsigned)2<<10);

#ifdef SET_NOP_NINE
	  insert_nop_nine(8);
	  for(i=0;i<8*2;i++)
	    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
	  for(i=0;i<8;i++)
	    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 2:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=(long long unsigned)((long long unsigned)0x5555<<15);
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[4]|=(long long unsigned)((long long unsigned)0x5555<<15);
	  temp_b_code[5]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[5]|=(long long unsigned)((long long unsigned)4<<10);
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[6]|=(long long unsigned)((long long unsigned)0x5555<<15);
	  temp_b_code[7]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[7]|=(long long unsigned)((long long unsigned)4<<10);

#ifdef SET_NOP_NINE
	  insert_nop_nine(8);
	  for(i=0;i<8*2;i++)
	    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
	  for(i=0;i<8;i++)
	    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	case 1:

	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[2]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[3]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[4]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[5]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[5]|=(long long unsigned)((long long unsigned)8<<10);
	  temp_b_code[6]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[7]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[7]|=(long long unsigned)((long long unsigned)8<<10);

#ifdef SET_NOP_NINE
	  insert_nop_nine(8);
	  for(i=0;i<8*2;i++)
	    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
	  for(i=0;i<8;i++)
	    BinCode[LastBinLine++]=temp_b_code[i];
#endif
	  break;
	default:
	  perror("GL_Floor(): SV error.");
	  break;
	}
    }


  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1b1b;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=SV>1?SV>2?SV>3?15:14:12:8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef SET_NOP_NINE
  return (2+SV*8)*2-1;
#else
  return 2+SV*8;
#endif

}

int GL_Sign(int return_regi,
	    int regiX,
	    char VOrF,
	    int SV,
	    FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"SIGN_R%d_R%d_ass_file",return_regi,regiX);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string,"");

  switch(SV)
    {
    case 4:
      if(VOrF == 'v')
	{
	  sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 4, 0001\n",temp_string,regiX,regiX); 
	  sprintf(temp_string,"%s\tBLTVF R%d, R0, R%d, R0, 5, 0001\n",temp_string,regiX,regiX);
	}
      else if(VOrF == 'f')
	{
	  sprintf(temp_string,"%s\tBGTSF R%d.wwww, R0.wwww, 4\n",temp_string,regiX);
	  sprintf(temp_string,"%s\tBLTSF R%d.wwww, R0.wwww, 5\n",temp_string,regiX);
	}
      else
	perror("GL_Sign(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVIF R%d.0001, 0.0\n",temp_string,return_regi);
      sprintf(temp_string,"%s\tJR 4\n",temp_string);
      sprintf(temp_string,"%s\tMVIF R%d.0001, 1.0\n",temp_string,return_regi);
      sprintf(temp_string,"%s\tJR 2\n",temp_string);
      sprintf(temp_string,"%s\tMVIF R%d.0001, -1.0\n",temp_string,return_regi);
    case 3:
      if(VOrF == 'v')
	{
	  sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 4, 0010\n",temp_string,regiX,regiX);
	  sprintf(temp_string,"%s\tBLTVF R%d, R0, R%d, R0, 5, 0010\n",temp_string,regiX,regiX);
	}
      else if(VOrF == 'f')
	{
	  sprintf(temp_string,"%s\tBGTSF R%d.zzzz, R0.zzzz, 4\n",temp_string,regiX);
	  sprintf(temp_string,"%s\tBLTSF R%d.zzzz, R0.zzzz, 5\n",temp_string,regiX);
	}
      else
	perror("GL_Sign(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVIF R%d.0010, 0.0\n",temp_string,return_regi);
      sprintf(temp_string,"%s\tJR 4\n",temp_string);
      sprintf(temp_string,"%s\tMVIF R%d.0010, 1.0\n",temp_string,return_regi);
      sprintf(temp_string,"%s\tJR 2\n",temp_string);
      sprintf(temp_string,"%s\tMVIF R%d.0010, -1.0\n",temp_string,return_regi);
    case 2:
      if(VOrF == 'v')
	{
	  sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 4, 0100\n",temp_string,regiX,regiX);
	  sprintf(temp_string,"%s\tBLTVF R%d, R0, R%d, R0, 5, 0100\n",temp_string,regiX,regiX);
	}
      else if(VOrF == 'f')
	{
	  sprintf(temp_string,"%s\tBGTSF R%d.yyyy, R0.yyyy, 4\n",temp_string,regiX);
	  sprintf(temp_string,"%s\tBLTSF R%d.yyyy, R0.yyyy, 5\n",temp_string,regiX);
	}
      else
	perror("GL_Sign(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVIF R%d.0100, 0.0\n",temp_string,return_regi);
      sprintf(temp_string,"%s\tJR 4\n",temp_string);
      sprintf(temp_string,"%s\tMVIF R%d.0100, 1.0\n",temp_string,return_regi);
      sprintf(temp_string,"%s\tJR 2\n",temp_string);
      sprintf(temp_string,"%s\tMVIF R%d.0100, -1.0\n",temp_string,return_regi);
    case 1:
      if(VOrF == 'v')
	{
	  sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 4, 1000\n",temp_string,regiX,regiX);
	  sprintf(temp_string,"%s\tBLTVF R%d, R0, R%d, R0, 5, 1000\n",temp_string,regiX,regiX);
	}
      else if(VOrF == 'f')
	{
	  sprintf(temp_string,"%s\tBGTSF R%d.xxxx, R0.xxxx, 4\n",temp_string,regiX);
	  sprintf(temp_string,"%s\tBLTSF R%d.xxxx, R0.xxxx, 5\n",temp_string,regiX);
	}
      else
	perror("GL_Sign(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVIF R%d.1000, 0.0\n",temp_string,return_regi);
      sprintf(temp_string,"%s\tJR 4\n",temp_string);
      sprintf(temp_string,"%s\tMVIF R%d.1000, 1.0\n",temp_string,return_regi);
      sprintf(temp_string,"%s\tJR 2\n",temp_string);
      sprintf(temp_string,"%s\tMVIF R%d.1000, -1.0\n",temp_string,return_regi);
      break;
    default:
      perror("GL_Sign(): SV is wrong.");
    }

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
  if(!output_ass)
    fclose(ass_file);

  //machine code

  float temp_float=0.0;
  unsigned temp_int;

  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<22;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=8; //modify 4 to 8 because we will insert nop
#else
      temp_b_code[0]|=4;
#endif
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<15;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
      
      temp_b_code[1]=temp_b_code[0];
      //temp_b_code[1]&=(long long unsigned)0x07FF80007FFFFFFFLL;
      temp_b_code[1]&=(long long unsigned)0x07FF80007FFFFFFFLL; 
      //printf("bin[0]: %llx\n",temp_b_code[0]);
      //printf("bin[1]: %llx\n",temp_b_code[1]);
      temp_b_code[1]|=(long long unsigned)((long long unsigned)26<<59);//opcode : BLTVF
#ifdef SET_NOP_NINE
      temp_b_code[1]|=(long long unsigned)((long long unsigned)10<<31); //modify 5 to 10 because we will insert nop
#else
      temp_b_code[1]|=(long long unsigned)((long long unsigned)5<<31); 
#endif
      //printf("bin[1]: %llx\n",temp_b_code[1]);
#else
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<22;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=8; //modify 4 to 8 because we will insert nop
#else
      temp_b_code[0]|=4;
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
      
      temp_b_code[1]=temp_b_code[0];
      //temp_b_code[1]&=(long long unsigned)0x07FF80007FFFFFFFLL;
      temp_b_code[1]&=(long long unsigned)0x07FF80007FFFFFFFLL; 
      //printf("bin[0]: %llx\n",temp_b_code[0]);
      //printf("bin[1]: %llx\n",temp_b_code[1]);
      temp_b_code[1]|=(long long unsigned)((long long unsigned)26<<59);//opcode : BLTVF
#ifdef SET_NOP_NINE
      temp_b_code[1]|=(long long unsigned)((long long unsigned)10<<31); //modify 5 to 10 because we will insert nop
#else
      temp_b_code[1]|=(long long unsigned)((long long unsigned)5<<31); 
#endif
#endif
    }
  else
    {
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<22;
      temp_b_code[0]|=4;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
      
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0x07FF80007FFFFFFFLL;
      temp_b_code[1]|=(long long unsigned)((long long unsigned)26<<59);
      temp_b_code[1]|=(long long unsigned)((long long unsigned)5<<31);
    }
  
  temp_b_code[2]=10;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=return_regi;
  temp_b_code[2]=temp_b_code[2]<<38;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[2]|=temp_int;
  temp_b_code[2]=temp_b_code[2]<<5;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<9;  

  temp_b_code[3]=24;
  temp_b_code[3]=temp_b_code[3]<<28;
  //temp_b_code[3]|=4;
#ifdef SET_NOP_NINE
  temp_b_code[3]|=8; //modify 4 to 8 because we will insert nop
#else
  temp_b_code[3]|=4;
#endif
  if(VOrF=='v')
      temp_b_code[3]=temp_b_code[3]<<21;
  else if(VOrF == 'f')
    {
      temp_b_code[3]=temp_b_code[3]<<17;
      temp_b_code[3]|=1;
      temp_b_code[3]=temp_b_code[3]<<4;
    }
  else
    perror("GL_Sign(): VOrF is wrong.");
  temp_b_code[3]|=8;
  temp_b_code[3]=temp_b_code[3]<<10;
    
  temp_float=1.0;
  temp_b_code[4]=temp_b_code[2];
  temp_b_code[4]&=(long long unsigned)0xFFFF800000007FFFLL;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[4]|=(long long unsigned)((long long unsigned)temp_int<<15);
  
  temp_b_code[5]=temp_b_code[3];
  temp_b_code[5]&=(long long unsigned)0xFFFF80007FFFFFFFLL;
#ifdef SET_NOP_NINE
  temp_b_code[5]|=(long long unsigned)((long long unsigned)4<<31); //modify 2 to 4 because we will insert nop
#else
  temp_b_code[5]|=(long long unsigned)((long long unsigned)2<<31); 
#endif
  
  temp_float=-1.0;
  temp_b_code[6]=temp_b_code[2];
  temp_b_code[6]&=(long long unsigned)0xFFFF800000007FFFLL;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[6]|=((long long unsigned)temp_int<<15);
  
  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
  insert_nop_nine(7);
  for(i=0;i<7*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<7;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 3:
      if(VOrF=='v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)(0x2) << 10;
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)(0x2) << 10;
#else
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=(long long unsigned)(0xAAAA) << 15;
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[1]|=(long long unsigned)(0xAAAA) << 15;
#endif
	}
      else if(VOrF=='f')
	{
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=(long long unsigned)(0xAAAA) << 15;
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[1]|=(long long unsigned)(0xAAAA) << 15;
	}
      temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[2]|=(long long unsigned)(0x2) << 10;
      temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[4]|=(long long unsigned)(0x2) << 10;
      temp_b_code[6]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[6]|=(long long unsigned)(0x2) << 10;
#ifdef SET_NOP_NINE
  insert_nop_nine(7);
  for(i=0;i<7*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<7;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 2:
      if(VOrF=='v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)(0x4) << 10;
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)(0x4) << 10;
#else
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=(long long unsigned)(0x5555) << 15;
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[1]|=(long long unsigned)(0x5555) << 15;
#endif
	}
      else if(VOrF=='f')
	{
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=(long long unsigned)(0x5555) << 15;
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[1]|=(long long unsigned)(0x5555) << 15;
	}
      temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[2]|=(long long unsigned)(0x4) << 10;
      temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[4]|=(long long unsigned)(0x4) << 10;
      temp_b_code[6]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[6]|=(long long unsigned)(0x4) << 10;
#ifdef SET_NOP_NINE
  insert_nop_nine(7);
  for(i=0;i<7*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<7;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 1:
      if(VOrF=='v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=(long long unsigned)(0x8) << 10;
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[1]|=(long long unsigned)(0x8) << 10;
#else
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=(long long unsigned)(0x0) << 15;
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[1]|=(long long unsigned)(0x0) << 15;
#endif
	}
      else if(VOrF=='f')
	{
	  temp_b_code[0]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=(long long unsigned)(0x0) << 15;
	  temp_b_code[1]&=(long long unsigned)0xFFFFFFFF80007FFFLL;
	  temp_b_code[1]|=(long long unsigned)(0x0) << 15;
	}
      temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[2]|=(long long unsigned)(0x8) << 10;
      temp_b_code[4]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[4]|=(long long unsigned)(0x8) << 10;
      temp_b_code[6]&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[6]|=(long long unsigned)(0x8) << 10;
#ifdef SET_NOP_NINE
  insert_nop_nine(7);
  for(i=0;i<7*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<7;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
      break;
    default:
      perror("GL_Sign(): SV is wrong.");
      break;
    }

#ifdef SET_NOP_NINE
  return 7*SV*2-1;
#else
  return 7*SV;
#endif

  
}

int GL_Abs(int return_regi,
	   int regiX,
	   char VOrF,
	   int SV,
	   FILE *output_ass)
{
  int i;
  int instr_count=0;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"ABS_R%d_R%d_ass_file",return_regi,regiX);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string,"\tMVR R%d.1111, R%d.xyzw, <%d>\n",return_regi,regiX,SV);
  switch(SV)
    {
    case 4:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 2, 0001, <%d>\n",temp_string,regiX,regiX,SV); //modify 2 to 4. Since we will insert nop into next two instruction.
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.wwww, R0.wwww, 2, <%d>\n",temp_string,regiX,SV);
      else
	perror("GL_Abs(): VOrF is wrong.");
      sprintf(temp_string,"%s\tADDF R%d.0001, R%d.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,0,return_regi,SV);
    case 3:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 2, 0010, <%d>\n",temp_string,regiX,regiX,SV);
      else if(VOrF == 'f') 
	sprintf(temp_string,"%s\tBGTSF R%d.zzzz, R0.zzzz, 2, <%d>\n",temp_string,regiX,SV);
      else
	perror("GL_Abs(): VOrF is wrong.");
      sprintf(temp_string,"%s\tADDF R%d.0010, R%d.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,0,return_regi,SV);
    case 2:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 2, 0100, <%d>\n",temp_string,regiX,regiX,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.yyyy, R0.yyyy, 2, <%d>\n",temp_string,regiX,SV);
      else
	perror("GL_Abs(): VOrF is wrong.");
      sprintf(temp_string,"%s\tADDF R%d.0100, R%d.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,0,return_regi,SV);
    case 1:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R0, R%d, R0, 2, 1000, <%d>\n",temp_string,regiX,regiX,SV); //Kaymon
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.xxxx, R0.xxxx, 2, <%d>\n",temp_string,regiX,SV);
      else
	perror("GL_Abs(): VOrF is wrong.");
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,0,return_regi,SV);
      break;
    default:
      perror("GL_Abs(): SV error");
      break;
    }

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  if(!output_ass)
    fclose(ass_file);


  // machine code
  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=31;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=2;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=1;
#endif

  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[0]=27;                  //BFTV opcode
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiX;              //R1 = regix
      temp_b_code[0]=temp_b_code[0]<<22;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;                  //R2 = 0, and jump 2 instructions //mark here => adjust 2 to 4.
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiX;              //R3 = regix
      temp_b_code[0]=temp_b_code[0]<<15;
      temp_b_code[0]|=1;                  //R4 = 0, 5 bits for don't care , 1 dummy bit , scc = 0001
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;                  //enable float bit
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;               //vector count = sv-1
      temp_b_code[0]=temp_b_code[0]<<7;
#else
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<22;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;                  //R2 = 0, and jump 2 instructions //mark here => adjust 2 to 4.  
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#endif
    }
  else if(VOrF == 'f')
    {
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<22;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=3;                   //R2 = 0, and jump 2 instructions //mark here => adjust 2 to 3.
#else
      temp_b_code[0]|=2;                   //R2 = 0, and jump 2 instructions //mark here => adjust 2 to 3.
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
    }
  else
    perror("GL_Abs(): VOrF variable is wrong.");
  //printf("==============start===============\n");
  //printf("dddddddddddddddd:%llx\n",temp_b_code[0]);
  temp_b_code[1]=16;                         //ADD opcode
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=return_regi;               //R1 = return_regi
  temp_b_code[1]=temp_b_code[1]<<12;
  temp_b_code[1]|=return_regi;               //R2 = 0, R3=return_regi
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=0x1b1b1b;                  //set the swizzle for R1,R2 and R3
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=2;                         //010 => R3 => -R3
  temp_b_code[1]=temp_b_code[1]<<4;
  temp_b_code[1]|=1;                         //set WM = 0001
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;                         //set float bit
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=SV-1;                      //vector count = SV-1
  temp_b_code[1]=temp_b_code[1]<<7;
  //printf("SV:%d\n",SV);
  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 3:
      if(VOrF=='v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=((long long unsigned)2)<<10; //readjust the scc bit.
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=((long long unsigned)0xAAAA)<<15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=((long long unsigned)0xAAAA)<<15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=((long long unsigned)2)<<10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 2:
      if(VOrF=='v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=((long long unsigned)4)<<10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=((long long unsigned)0x5555)<<15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=((long long unsigned)0x5555)<<15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=((long long unsigned)4)<<10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 1:
      if(VOrF=='v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=((long long unsigned)8)<<10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=((long long unsigned)8)<<10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=3;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
      break;
    default:
      perror("GL_Abs(): SV error");
      break;
    }
  //printf("==============end:%d===============\n",instr_count);
  return instr_count;

}

int GL_InverseSqrt(int return_regi,
		   int regiX,
		   int remain_regi,
		   int SV,
		   FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"INVERSESQRT_R%d_R%d_ass_file",return_regi,regiX);
      ass_file=fopen(temp_string,"w");
    }

  switch(SV)
    {
    case 4:
      sprintf(temp_string,"\tMVR R%d.1000, R%d.wyzw, <%d>\n",remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tRSQ R%d.0001, R%d\n",temp_string,return_regi,remain_regi);
    case 3:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.zyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tRSQ R%d.0010, R%d\n",temp_string,return_regi,remain_regi);
    case 2:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.yyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tRSQ R%d.0100, R%d\n",temp_string,return_regi,remain_regi);
    case 1:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.xyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tRSQ R%d.1000, R%d\n",temp_string,return_regi,remain_regi);
      break;
    default:
      perror("GL_InverseSqrt(): SV error");
      break;      
    }

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  if(!output_ass)
    fclose(ass_file);


  // machine code

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1bdb;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=3;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=return_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<22;
  temp_b_code[1]|=6939;
  temp_b_code[1]=temp_b_code[1]<<15;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<9;


  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 3:

      temp_b_code[0]&=0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x9b<<25;
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=(long long unsigned)2<<10;

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 2:

      temp_b_code[0]&=0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x5b<<25;
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=(long long unsigned)4<<10;

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 1:

      temp_b_code[0]&=0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x1b<<25;
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=(long long unsigned)8<<10;

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
      break;
    default:
      perror("GL_InverseSqrt(): SV error");
      break;      
    }


#ifdef SET_NOP_NINE
  return SV*2*2-1;
#else
  return SV*2;
#endif

}

int GL_Sqrt(int return_regi,
	    int regiX,
	    int remain_regi,
	    int remain_regi2,
	    int SV,
	    FILE *output_ass)
{
  int i,line_record=0;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"SQRT_R%d_R%d_ass_file",return_regi,regiX);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string,"\tLDIF R%d.1111, 0.5\n",remain_regi); //Kaymon

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  if(!output_ass)
    fclose(ass_file);

  //machine code

  float temp_float=0.5;
  unsigned temp_int;
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<6;
  //temp_b_code[0]|=17;  //WM should be 1111
  temp_b_code[0]|=31;
  temp_b_code[0]=temp_b_code[0]<<9;  

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  line_record+=2;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  line_record++;
#endif

  line_record+=GL_Pow(return_regi,regiX,remain_regi,remain_regi2,SV,output_ass);
  
  return line_record;
}

int GL_Log2(int return_regi,
	    int regiX,
	    int remain_regi,
	    int SV,
	    FILE *output_ass)
//remain_regi can be used OUR_TEMP_REGI
{
  int i;
  //assembly code
  FILE *ass_file;
  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"LOG2_R%d_R%d_ass_file",return_regi,regiX);
      ass_file=fopen(temp_string,"w");
    }
  switch(SV)
    {
    case 4:
      sprintf(temp_string,"\tMVR R%d.1000, R%d.wyzw, <%d>\n",remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tLOG2 R%d.0001, R%d\n",temp_string,return_regi,remain_regi);
    case 3:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.zyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tLOG2 R%d.0010, R%d\n",temp_string,return_regi,remain_regi);
    case 2:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.yyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tLOG2 R%d.0100, R%d\n",temp_string,return_regi,remain_regi);
    case 1:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.xyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tLOG2 R%d.1000, R%d\n",temp_string,return_regi,remain_regi);
      break;
    default:
      perror("GL_Log2(): SV error");
      break;      
    }
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  // machine code

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1bdb;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=4;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=return_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<22;
  temp_b_code[1]|=6939;
  temp_b_code[1]=temp_b_code[1]<<15;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<9;


  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 3:

      temp_b_code[0]&=0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x9b<<25;
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=(long long unsigned)2<<10;

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 2:

      temp_b_code[0]&=0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x5b<<25;
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=(long long unsigned)4<<10;

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 1:

      temp_b_code[0]&=0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x1b<<25;
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=(long long unsigned)8<<10;

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
      break;
    default:
      perror("GL_Exp2(): SV error");
      break;      
    }


#ifdef SET_NOP_NINE
  return SV*2*2-1;
#else
  return SV*2;
#endif

}

int GL_Exp2(int return_regi,
	    int regiX,
	    int remain_regi,
	    int SV,
	    FILE *output_ass)
//remain_regi can be used OUR_TEMP_REGI
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"EXP2_R%d_R%d_ass_file",return_regi,regiX);
      ass_file=fopen(temp_string,"w");
    }

  switch(SV)
    {
    case 4:
      sprintf(temp_string,"\tMVR R%d.1000, R%d.wyzw, <%d>\n",remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tPOW2 R%d.0001, R%d\n",temp_string,return_regi,remain_regi);
    case 3:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.zyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tPOW2 R%d.0010, R%d\n",temp_string,return_regi,remain_regi);
    case 2:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.yyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tPOW2 R%d.0100, R%d\n",temp_string,return_regi,remain_regi);
    case 1:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.xyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tPOW2 R%d.1000, R%d\n",temp_string,return_regi,remain_regi);
      break;
    default:
      perror("GL_Exp2(): SV error");
      break;      
    }

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  // machine code

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1bdb;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=5;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=return_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<22;
  temp_b_code[1]|=6939;
  temp_b_code[1]=temp_b_code[1]<<15;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<9;


  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 3:

      temp_b_code[0]&=0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x9b<<25;
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=(long long unsigned)2<<10;

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 2:

      temp_b_code[0]&=0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x5b<<25;
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=(long long unsigned)4<<10;

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 1:

      temp_b_code[0]&=0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x1b<<25;
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=(long long unsigned)8<<10;

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
      break;
    default:
      perror("GL_Exp2(): SV error");
      break;      
    }


#ifdef SET_NOP_NINE
  return SV*2*2-1;
#else
  return SV*2;
#endif

}

int GL_Log(int return_regi,
	   int regiX,
	   int remain_regi,
	   int remain_regi2,
	   int SV,
	   FILE *output_ass)
{//remain_regi can be used OUR_TEMP_REGI
  int i;
  //assembly code
  int b_output_ass=1;
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      b_output_ass=0;
      sprintf(temp_string,"LOG_R%d_R%d_R%d_ass_file",return_regi,regiX,remain_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string,"\tADDIF R%d.1000, R0, 2.718281\n",remain_regi2);
  sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi2,remain_regi2);
  sprintf(temp_string,"%s\tRCP R%d, R%d\n",temp_string,remain_regi2,remain_regi2);
  switch (SV)
    {
    case 4:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.wyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      //sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi2,remain_regi2);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      //sprintf(temp_string,"%s\tRCP R%d, R%d\n",temp_string,remain_regi2,remain_regi2);
      sprintf(temp_string,"%s\tMULF R%d.0001, R%d.xyzx, R%d.xyzx, <%d>\n",temp_string,return_regi,remain_regi2,remain_regi,SV);
    case 3:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.zyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      //sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi2,remain_regi2);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      //sprintf(temp_string,"%s\tRCP R%d, R%d\n",temp_string,remain_regi2,remain_regi2);
      sprintf(temp_string,"%s\tMULF R%d.0010, R%d.xyxw, R%d.xyxw, <%d>\n",temp_string,return_regi,remain_regi2,remain_regi,SV);
    case 2:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.yyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      //sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi2,remain_regi2);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      //sprintf(temp_string,"%s\tRCP R%d, R%d\n",temp_string,remain_regi2,remain_regi2);
      sprintf(temp_string,"%s\tMULF R%d.0100, R%d.xxzw, R%d.xxzw\n",temp_string,return_regi,remain_regi2,remain_regi);
    case 1:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.xyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      //sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi2,remain_regi2);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      //sprintf(temp_string,"%s\tRCP R%d, R%d\n",temp_string,remain_regi2,remain_regi2);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi2,remain_regi,SV);
      break;
    default:
      perror("LOG: SV error");
      break;
    }
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  if(!b_output_ass)
    fclose(ass_file);


  //machine code
  float temp_float=2.718281;
  unsigned temp_int;

  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi2;
  temp_b_code[0]=temp_b_code[0]<<38;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<9;

  temp_b_code[1]=4;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi2;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi2;
  temp_b_code[1]=temp_b_code[1]<<22;
  temp_b_code[1]|=6939;
  temp_b_code[1]=temp_b_code[1]<<16;
  temp_b_code[1]|=17;
  temp_b_code[1]=temp_b_code[1]<<9;

  temp_b_code[2]=temp_b_code[1];
  temp_b_code[2]&=(long long unsigned)0x07FFFFFFFFFFFFFFLL;
  temp_b_code[2]|=(long long unsigned)2<<59;

#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=7131;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

  //temp_b_code[1]=4;
  //temp_b_code[1]=temp_b_code[1]<<6;
  //temp_b_code[1]|=remain_regi2;
  //temp_b_code[1]=temp_b_code[1]<<6;
  //temp_b_code[1]|=remain_regi2;
  //temp_b_code[1]=temp_b_code[1]<<22;
  //temp_b_code[1]|=6939;
  //temp_b_code[1]=temp_b_code[1]<<16;
  //temp_b_code[1]|=17;
  //temp_b_code[1]=temp_b_code[1]<<9;

  //temp_b_code[2]=temp_b_code[1];
  temp_b_code[1]&=(long long unsigned)0xF8007FFFFFFFFFFFLL;
  temp_b_code[1]|=(long long unsigned)remain_regi<<53;
  temp_b_code[1]|=(long long unsigned)remain_regi<<47;

  //temp_b_code[3]=temp_b_code[1];
  //temp_b_code[3]&=(long long unsigned)0x07FFFFFFFFFFFFFFLL;
  //temp_b_code[3]|=(long long unsigned)2<<59;

  temp_b_code[2]=17;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=return_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi2;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<24;
  temp_b_code[2]|=1775640;//0x1b1818
  //temp_b_code[2]|=6168;//0x001818
  temp_b_code[2]=temp_b_code[2]<<7;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=SV-1;
  temp_b_code[2]=temp_b_code[2]<<7;

  switch(SV)
    {
    case 4:

#ifdef SET_NOP_NINE
      insert_nop_nine(3);
      for(i=0;i<3*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<3;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif
      

    case 3:
      temp_b_code[0]&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)155<<25;
      temp_b_code[2]&=(long long unsigned)0xFFFFFFFE0001C3FFLL;
      temp_b_code[2]|=(long long unsigned)4883<<17;
      temp_b_code[2]|=(long long unsigned)2<<10;

#ifdef SET_NOP_NINE
      insert_nop_nine(3);
      for(i=0;i<3*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<3;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif

    case 2:
      temp_b_code[0]&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)91<<25;
      temp_b_code[2]&=(long long unsigned)0xFFFFFFFE0001C3FFLL;
      temp_b_code[2]|=(long long unsigned)2827<<17;
      temp_b_code[2]|=(long long unsigned)4<<10;

#ifdef SET_NOP_NINE
      insert_nop_nine(5);
      for(i=0;i<3*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<3;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif

    case 1:
      temp_b_code[0]&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)27<<25;
      temp_b_code[2]&=(long long unsigned)0xFFFFFFFE0001C3FFLL;
      temp_b_code[2]|=(long long unsigned)6939<<17;
      temp_b_code[2]|=(long long unsigned)8<<10;

#ifdef SET_NOP_NINE
      insert_nop_nine(3);
      for(i=0;i<3*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<3;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif

    }


#ifdef SET_NOP_NINE
  return (SV*3+3)*2-1;
#else
  return SV*3+3;
#endif

}

int GL_Exp(int return_regi,
	   int regiX,
	   int remain_regi,
	   int remain_regi2,
	   int SV,
	   FILE *output_ass)
//one remain regi can be used OUR_TEMP_REGI
{

  int i,line_record=0;
  //assembly code
  int b_output_ass=1;
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      b_output_ass=0;
      sprintf(temp_string,"EXP_R%d_R%d_ass_file",return_regi,regiX);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string,"\tLDIF R%d.1%d%d%d, 2.718281\n",remain_regi,SV>=2?1:0,SV>=3?1:0,SV>=4?1:0);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  //machine code


  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  float temp_float=2.718281;
  unsigned temp_int;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=(1<<3)|((SV>=2?1:0)<<2)|((SV>=3?1:0)<<1)|(SV>=4?1:0);
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<9;  


#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2*1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef SET_NOP_NINE
  line_record+=2;
#else
  line_record++;
#endif

  line_record+=GL_Pow(return_regi,remain_regi,regiX,remain_regi2,SV,output_ass);

  if(!b_output_ass)
    fclose(ass_file);

  return line_record;
}

int GL_Degrees(int return_regi,int regi1,int remain_regi,int SV,FILE *output_ass)
{//remain_regi can be used OUR_TEMP_REGI

  int i;
  //assembly code
  FILE *ass_file;
  int b_output_ass=1;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      b_output_ass=0;
      sprintf(temp_string,"DEGREES_R%d_R%d_ass_file",return_regi,regi1);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string,"\tLDIF R%d.1000, 57.295718482\n",remain_regi);
  sprintf(temp_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xxxx, <%d>\n",temp_string,return_regi,SV>=2?1:0,SV>=3?1:0,SV>=4?1:0,regi1,remain_regi,SV);
  
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!b_output_ass)
    fclose(ass_file);
  
  //machine code

  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  float temp_float=57.295718482;
  unsigned temp_int;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<9;

  temp_b_code[1]=17;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=return_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=regi1;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=6912;
  temp_b_code[1]=temp_b_code[1]<<7;
  temp_b_code[1]|=(1<<3)|((SV>=2?1:0)<<2)|((SV>=3?1:0)<<1)|(SV>=4?1:0);
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;  


#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  return 2*2-1;
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  return 2;
#endif

}

int GL_Radians(int return_regi,int regi1,int remain_regi,int SV,FILE *output_ass)
{//remain_regi can be used OUT_TEMP_REGI

  int i;
  //assembly code
  FILE *ass_file;
  int b_output_ass=1;
  //printf("return_regi:%d,regi1:%d,remain_regi:%d,SV:%d\n",return_regi,regi1,remain_regi,SV);
  if(output_ass)
    ass_file=output_ass;
  else
    {
      b_output_ass=0;
      sprintf(temp_string,"RADIANS_R%d_R%d_ass_file",return_regi,regi1);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string,"\tLDIF R%d.1000, 0.017453\n",remain_regi);
  sprintf(temp_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xxxx, <%d>\n",temp_string,return_regi,SV>=2?1:0,SV>=3?1:0,SV>=4?1:0,regi1,remain_regi,SV);
  
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!b_output_ass)
    fclose(ass_file);
  
  //machine code

  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  float temp_float=0.017453311;
  unsigned temp_int;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<9;

  temp_b_code[1]=17;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=return_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=regi1;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=6912;
  temp_b_code[1]=temp_b_code[1]<<7;
  temp_b_code[1]|=(1<<3)|((SV>=2?1:0)<<2)|((SV>=3?1:0)<<1)|(SV>=4?1:0);
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;  


#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef SET_NOP_NINE
  return 2*2-1;
#else
  return 2;
#endif

}

int GL_MatrixCompMult(int return_regi,
		      int return_regi2,
		      int return_regi3,
		      int return_regi4,
		      int x_mat_regi,
		      int x_mat_regi2,
		      int x_mat_regi3,
		      int x_mat_regi4,
		      int y_mat_regi,
		      int y_mat_regi2,
		      int y_mat_regi3,
		      int y_mat_regi4,
		      int remain_regi,
		      int SV,
		      FILE *output_ass)
{

}

int GL_Refract(int return_regi,
	       int I_regi,
	       int N_regi,
	       int eta_regi,
	       int remain_regi,
	       int remain_regi2,
	       int remain_regi3,
	       int remain_regi4,
	       char VOrF,
	       int SV,
	       FILE *output_ass)
{
  int i,instr_count=0;

  instr_count+=GL_Dot(remain_regi,N_regi,I_regi,SV,output_ass);

  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"REFRACT_R%d_R%d_R%d_R%d_ass_file",return_regi,I_regi,N_regi,remain_regi);
      ass_file=fopen(temp_string,"w");
    }

  //assembly code
  sprintf(temp_string,"");
  sprintf(temp_string,"%s\tLDIF R%d.0100, 1.0\n",temp_string,remain_regi2);
  sprintf(temp_string,"%s\tMULF R%d.0010, R%d.xxxx, R%d.xxxx, <3>\n",temp_string,remain_regi3,remain_regi,remain_regi);
  sprintf(temp_string,"%s\tADDF R%d.0010, R%d.yyyy, -R%d.zzzz, <3>\n",temp_string,remain_regi3,remain_regi2,remain_regi3);
  sprintf(temp_string,"%s\tMULF R%d.0010, R%d.xxxx, R%d.zzzz, <3>\n",temp_string,remain_regi3,eta_regi,remain_regi3);
  sprintf(temp_string,"%s\tMULF R%d.0010, R%d.xxxx, R%d.zzzz, <3>\n",temp_string,remain_regi3,eta_regi,remain_regi3);
  sprintf(temp_string,"%s\tADDF R%d.1000, R%d.yyyy, -R%d.zzzz, <1>\n",temp_string,remain_regi2,remain_regi2,remain_regi3);

  if(VOrF == 'v')
    sprintf(temp_string,"%s\tBLTVF R%d, R0, R%d, R0, 12, 1000, <%d>\n",temp_string,remain_regi2,remain_regi2,SV);
  else if(VOrF == 'f')
    sprintf(temp_string,"%s\tBLTSF R%d.xxxx, R0.xxxx, 12, <%d>\n",temp_string,remain_regi2,SV);
  else
    perror("GL_Refract(): VOrF error.");

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

#ifdef SET_NOP_NINE
  BinCode[LastBinLine++]=nop_nine_instruction;
  instr_count+=1;
#endif

  //machine code
  float temp_float=1.0;
  unsigned temp_int;

  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi2;
  temp_b_code[0]=temp_b_code[0]<<38;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=4;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;  

  temp_b_code[1]=17;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi3;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=0x1B0000;
  temp_b_code[1]=temp_b_code[1]<<7;
  temp_b_code[1]|=2;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<9;

  temp_b_code[2]=16;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi3;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi2;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi3;
  temp_b_code[2]=temp_b_code[2]<<24;
  temp_b_code[2]|=0x1B55AA;
  temp_b_code[2]=temp_b_code[2]<<3;
  temp_b_code[2]|=2;
  temp_b_code[2]=temp_b_code[2]<<4;
  temp_b_code[2]|=2;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=2;
  temp_b_code[2]=temp_b_code[2]<<7;

  temp_b_code[3]=temp_b_code[1];
  temp_b_code[3]&=0xFFE001FFFC03FE7FLL;
  temp_b_code[3]|=((long long unsigned)eta_regi) << 47;
  temp_b_code[3]|=((long long unsigned)remain_regi3) << 41;
  temp_b_code[3]|=((long long unsigned)0xAA) << 17;
  temp_b_code[3]|=((long long unsigned)2) << 7;
  
  temp_b_code[4]=temp_b_code[3];

  temp_b_code[5]=temp_b_code[2];
  temp_b_code[5]&=0xF81FFFFFFFFFC3FFLL;
  temp_b_code[5]|=((long long unsigned)remain_regi2) << 53;
  temp_b_code[5]|=((long long unsigned)8) << 10;

  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[6]=26;
      temp_b_code[6]=temp_b_code[6]<<6;
      temp_b_code[6]|=remain_regi2;
      temp_b_code[6]=temp_b_code[6]<<22;
#ifdef SET_NOP_NINE
      temp_b_code[6]|=24;
#else
      temp_b_code[6]|=12;
#endif
      temp_b_code[6]=temp_b_code[6]<<6;
      temp_b_code[6]|=remain_regi2;
      temp_b_code[6]=temp_b_code[6]<<15;
      temp_b_code[6]|=8;
      temp_b_code[6]=temp_b_code[6]<<1;
      temp_b_code[6]|=1;
      temp_b_code[6]=temp_b_code[6]<<2;
      temp_b_code[6]|=SV-1;
      temp_b_code[6]=temp_b_code[6]<<7;
#else
      temp_b_code[6]=26;
      temp_b_code[6]=temp_b_code[6]<<6;
      temp_b_code[6]|=remain_regi2;
      temp_b_code[6]=temp_b_code[6]<<22;
#ifdef SET_NOP_NINE
      temp_b_code[6]|=24;
#else
      temp_b_code[6]|=12;
#endif
      temp_b_code[6]=temp_b_code[6]<<17;
      temp_b_code[6]|=1;
      temp_b_code[6]=temp_b_code[6]<<5;
      temp_b_code[6]|=1;
      temp_b_code[6]=temp_b_code[6]<<2;
      temp_b_code[6]|=SV-1;
      temp_b_code[6]=temp_b_code[6]<<7;
#endif
    }
  else if(VOrF == 'f')
    {
      temp_b_code[6]=26;
      temp_b_code[6]=temp_b_code[6]<<6;
      temp_b_code[6]|=remain_regi2;
      temp_b_code[6]=temp_b_code[6]<<22;
      temp_b_code[6]|=12;
      temp_b_code[6]=temp_b_code[6]<<17;
      temp_b_code[6]|=1;
      temp_b_code[6]=temp_b_code[6]<<5;
      temp_b_code[6]|=1;
      temp_b_code[6]=temp_b_code[6]<<2;
      temp_b_code[6]|=SV-1;
      temp_b_code[6]=temp_b_code[6]<<7;
    }
  else
    perror("GL_Refract(): VOrF variable is wrong.");

#ifdef SET_NOP_NINE
  insert_nop_nine(7);
  for(i=0;i<7*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=14;
#else
  for(i=0;i<7;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=7;
#endif

  instr_count+=GL_Sqrt(remain_regi2,remain_regi2,remain_regi3,remain_regi4,1,output_ass);  //5 instructions

#ifdef SET_NOP_NINE
  BinCode[LastBinLine++]=nop_nine_instruction;
  instr_count+=1;
#endif

  //assembly code
  sprintf(temp_string,"");
  sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xxxx, R%d.xxxx, <1>\n",temp_string,remain_regi,eta_regi,remain_regi);
  sprintf(temp_string,"%s\tADDF R%d.1000, R%d.xxxx, R%d.xxxx, <1>\n",temp_string,remain_regi,remain_regi,remain_regi2);
  sprintf(temp_string,"%s\tMULF R%d.1111, R%d.xxxx, R%d.xyzw, <4>\n",temp_string,remain_regi,remain_regi,N_regi);
  sprintf(temp_string,"%s\tMULF R%d.1111, R%d.xxxx, R%d.xyzw, <4>\n",temp_string,remain_regi2,eta_regi,I_regi);
  sprintf(temp_string,"%s\tADDF R%d.1111, R%d.xyzw, -R%d.xyzw, <4>\n",temp_string,return_regi,remain_regi2,remain_regi);
  sprintf(temp_string,"%s\tJR 2\n",temp_string);
  sprintf(temp_string,"%s\tMVR R%d.1%d%d%d, R0.xyzw, <%d>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,SV);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  //machine code

  temp_b_code[0]=17;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=eta_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<24;
  temp_b_code[0]|=0x1B0000;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<9;

  temp_b_code[1]=16;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi2;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=0x1B0000;
  temp_b_code[1]=temp_b_code[1]<<7;
  temp_b_code[1]|=8;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<9;

  temp_b_code[2]=17;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=N_regi;
  temp_b_code[2]=temp_b_code[2]<<24;
  temp_b_code[2]|=0x1B001B;
  temp_b_code[2]=temp_b_code[2]<<7;
  temp_b_code[2]|=15;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=3;
  temp_b_code[2]=temp_b_code[2]<<7;

  temp_b_code[3]=temp_b_code[2];
  temp_b_code[3]&=0xF80001FFFFFFFFFFLL;
  temp_b_code[3]|=((long long unsigned)remain_regi2) << 53;
  temp_b_code[3]|=((long long unsigned)eta_regi) << 47;
  temp_b_code[3]|=((long long unsigned)I_regi) << 41;

  temp_b_code[4]=16;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=return_regi;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=remain_regi2;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=remain_regi;
  temp_b_code[4]=temp_b_code[4]<<24;
  temp_b_code[4]|=0x1B1B1B;
  temp_b_code[4]=temp_b_code[4]<<3;
  temp_b_code[4]|=2;
  temp_b_code[4]=temp_b_code[4]<<4;
  temp_b_code[4]|=15;
  temp_b_code[4]=temp_b_code[4]<<1;
  temp_b_code[4]|=1;
  temp_b_code[4]=temp_b_code[4]<<2;
  temp_b_code[4]|=3;
  temp_b_code[4]=temp_b_code[4]<<7;

  temp_b_code[5]=24;
  temp_b_code[5]=temp_b_code[5]<<28;
  if(VOrF=='v'){
#ifdef SET_NOP_NINE
    temp_b_code[5]|=4;
#else
    temp_b_code[5]|=2;
#endif
  }
  else if(VOrF == 'f')
    temp_b_code[5]|=2;
  if(VOrF=='v')
      temp_b_code[5]=temp_b_code[5]<<21;
  else if(VOrF == 'f')
    {
      temp_b_code[5]=temp_b_code[5]<<17;
      temp_b_code[5]|=1;
      temp_b_code[5]=temp_b_code[5]<<4;
    }
  else
    perror("GL_Refract(): VOrF is wrong.");
  temp_b_code[5]|=8;
  temp_b_code[5]=temp_b_code[5]<<10;

  temp_b_code[6]=7;
  temp_b_code[6]=temp_b_code[6]<<6;
  temp_b_code[6]|=return_regi;
  temp_b_code[6]=temp_b_code[6]<<28;
  temp_b_code[6]|=0x1B1B;
  temp_b_code[6]=temp_b_code[6]<<15;
  temp_b_code[6]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[6]=temp_b_code[6]<<3;
  temp_b_code[6]|=SV-1;
  temp_b_code[6]=temp_b_code[6]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(7);
  for(i=0;i<7*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=13;
#else
  for(i=0;i<7;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=7;
#endif
  
  if(!output_ass)
    fclose(ass_file);

  return instr_count;
}

int GL_Reflect(int return_regi,
	       int I_regi,
	       int N_regi,
	       int remain_regi,
	       int SV,
	       FILE *output_ass)
{
  int i,instr_count=0;

  //assembly code

  instr_count+=GL_Dot(remain_regi,I_regi,N_regi,SV,output_ass);
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"REFLECT_R%d_R%d_R%d_R%d_ass_file",return_regi,I_regi,N_regi,remain_regi);
      ass_file=fopen(temp_string,"w");
    }


  sprintf(temp_string,"%s\tADDF R%d.1000, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi,remain_regi,remain_regi,SV);
  sprintf(temp_string,"%s\tMULF R%d.1111, R%d.xyzw, R%d.xxxx, <%d>\n",temp_string,remain_regi,N_regi,remain_regi,SV);
  sprintf(temp_string,"%s\tADDF R%d.1111, R%d.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,I_regi,remain_regi,SV);


  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code
#ifdef SET_NOP_NINE
  BinCode[LastBinLine++]=nop_nine_instruction;
  instr_count+=1;
#endif
  temp_b_code[0]=16;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<24;
  temp_b_code[0]|=0x1b1b1b;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=17;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=N_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=6912;
  temp_b_code[1]=temp_b_code[1]<<7;
  temp_b_code[1]|=15;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=16;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=return_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=I_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<24;
  temp_b_code[2]|=6939;
  temp_b_code[2]=temp_b_code[2]<<3;
  //temp_b_code[2]|=1;
  temp_b_code[2]|=2;//rs is neg.
  temp_b_code[2]=temp_b_code[2]<<4;
  temp_b_code[2]|=15;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=SV-1;
  temp_b_code[2]=temp_b_code[2]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=5;
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=3;
#endif
  return instr_count;
}


int GL_Length(int return_regi,
	      int input_regi,
	      int remain_regi,
	      int remain_regi2,
	      int SV,
	      FILE *output_ass)
{
  int i;
  //assembly code
  int line_record=0;
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"LENGTH_R%d_R%d_ass_file",return_regi,input_regi);
      ass_file=fopen(temp_string,"w");
    }

  if(SV>4 || SV<1)
    perror("GL_Length: SV");

  sprintf(temp_string,"\tLDIF R%d.1%d%d%d, 0.5\n",remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0);
  sprintf(temp_string,"%s\tMULF R%d.1111, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,input_regi,input_regi,input_regi,SV);
  switch(SV)
    {
    case 4:
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.xyzw, R%d.wyzw, <%d>\n",temp_string,input_regi,input_regi,input_regi,SV);
    case 3:
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.xyzw, R%d.zyzw, <%d>\n",temp_string,input_regi,input_regi,input_regi,SV);
    case 2:
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.xyzw, R%d.yyzw, <%d>\n",temp_string,input_regi,input_regi,input_regi,SV);
    case 1:
      break;
    default:
      break;
    }
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code
  
  float temp_float=0.5;
  unsigned temp_int;
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<9;

  temp_b_code[1]=17;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=input_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=input_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=input_regi;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=6939;
  temp_b_code[1]=temp_b_code[1]<<8;
  temp_b_code[1]|=31;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  line_record=4;
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  line_record=2;
#endif

  //machine code
  switch(SV)
    {
    case 4:
      temp_b_code[0]=16;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<24;
      //temp_b_code[0]|=7131;//0x001beb
      temp_b_code[0]|=1776603;//0x1b1beb//simulator's behavior.....
      temp_b_code[0]=temp_b_code[0]<<8;
      temp_b_code[0]|=17;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<2*1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      line_record+=2;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      line_record+=1;
#endif
    case 3:
      temp_b_code[0]=16;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<24;
      //temp_b_code[0]|=7067;//0x001b9b
      temp_b_code[0]|=1776539;//0x1b1b9b
      temp_b_code[0]=temp_b_code[0]<<8;
      temp_b_code[0]|=17;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<2*1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      line_record+=2;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      line_record+=1;
#endif
    case 2:
      temp_b_code[0]=16;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<24;
      //temp_b_code[0]|=7003;//0x001b5b
      temp_b_code[0]|=1776475;//0x1b1b5b
      temp_b_code[0]=temp_b_code[0]<<8;
      temp_b_code[0]|=17;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<2*1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      line_record+=2;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      line_record+=1;
#endif
      break;

    case 1:
      break;

    default:
      perror("GL_Length(): SV is wrong.");
      break;
    }

#ifdef SET_NOP_NINE
  line_record=(2+SV-1)*2;
#else
  line_record=2+SV-1;
#endif

  //call GL_Pow
  line_record+=GL_Pow(return_regi,input_regi,remain_regi,remain_regi2,1,output_ass);

  return line_record;
}

int GL_Distance(int return_regi,
		int p0_regi,
		int p1_regi,
		int remain_regi,
		int remain_regi2,
		int SV,
		FILE *output_ass)
{
  int i;
  //assembly code
  int line_record=0;
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"DISTANCE_R%d_R%d_R%d_ass_file",return_regi,p0_regi,p1_regi);
      ass_file=fopen(temp_string,"w");
    }

  switch(SV)
    {
    case 4:
    case 3:
    case 2:
    case 1:
      sprintf(temp_string,"\tADDF R%d.1%d%d%d, R%d.xyzw, -R%d.xyzw, <%d>\n",p0_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,p0_regi,p1_regi,SV);
      break;
    default:
      perror("GL_Distance(): SV is wrong.");
      break;
    }
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code

  temp_b_code[0]=16;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=p0_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=p0_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=p1_regi;
  temp_b_code[0]=temp_b_code[0]<<24;
  temp_b_code[0]|=0x1b1b1b;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<4;
  temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<2*1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      line_record+=2;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      line_record+=1;
#endif

  line_record+=GL_Length(return_regi,p0_regi,remain_regi,remain_regi2,SV,output_ass);

  return line_record;
  //machine code
}

int VectorNegate(int return_regi, int input_regi, int bool_float,int SV, FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"VectorNegate_R%d_R%d_ass_file",return_regi,input_regi);
  ass_file=fopen(temp_string,"w");
#endif
  if(SV>4 || SV<1)
    perror("VectorNegate component");
  if(bool_float==0)
    {
      sprintf(temp_string,"\tADD R%d.1111, -R%d.xyzw, R0.xyzw, <%d>\n",return_regi,input_regi,SV);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
    }
  else if(bool_float==1)
    {
      sprintf(temp_string,"\tADDF R%d.1111, -R%d.xyzw, R0.xyzw, <%d>\n",return_regi,input_regi,SV);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
    }
  else
    perror("VectorNegate bool_float");
  fflush(ass_file);
#endif

    //machine code

  temp_b_code[0]=16;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=input_regi;
  temp_b_code[0]=temp_b_code[0]<<30;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[0]|=47;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=bool_float;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;
  
  for(i=0;i<1;i++)
      all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_ass);
#endif
  return 1;
}

int GL_Clamp(int return_regi,
	     int x_regi,
	     int min_regi,
	     int max_regi,
	     int remain_regi,
	     int SV,
	     int type,   //type 0 : genType clamp (genType x, genType minVal, genType maxVal)
	                 //type 1 : genType clamp (genType x, float minVal, float maxVal)
	     FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"CLAMP_R%d_R%d_R%d_R%d_ass_file",return_regi,x_regi,min_regi,max_regi);
      ass_file=fopen(temp_string,"w");
    }

  if(type==0)
    {
      sprintf(temp_string,"\tMAXF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,x_regi,min_regi,SV);
      sprintf(temp_string,"%s\tMINF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,return_regi,max_regi,SV);
    }
  else if(type==1)
    {
      sprintf(temp_string,"\tMVR R%d.1111, R%d.xxxx, <%d>\n",return_regi,min_regi,SV);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xxxx, <%d>\n",temp_string,remain_regi,max_regi,SV);
      sprintf(temp_string,"%s\tMAXF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,return_regi,x_regi,SV);
      sprintf(temp_string,"%s\tMINF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,return_regi,remain_regi,SV);
    }
  else
    perror("GL_Clamp(): type");

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code

  if(type==0)
    {
      temp_b_code[0]=12;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=return_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=min_regi;
      temp_b_code[0]=temp_b_code[0]<<24;
      temp_b_code[0]|=0x1b1b;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[0]|=((1<<3)|((SV>1?1:0)<<2)|((SV>2?1:0)<<1)|((SV>3?1:0)));
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0x07E001FFFFFFFFFFLL;
      temp_b_code[1]|=(long long unsigned)11<<59;
      temp_b_code[1]|=(long long unsigned)return_regi<<47;
      temp_b_code[1]|=(long long unsigned)max_regi<<41;


#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif
      
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),1,output_ass);  
#endif
      
#ifdef SET_NOP_NINE
      return 2*2-1;
#else
      return 2;
#endif
    }
  else
    {
      temp_b_code[0]=7;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=return_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=min_regi;
      temp_b_code[0]=temp_b_code[0]<<22;
      temp_b_code[0]|=0x1B00;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0x1F;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0xF8007FFFFFFFFFFFLL;
      temp_b_code[1]|=(long long unsigned)remain_regi<<53;
      temp_b_code[1]|=(long long unsigned)max_regi<<47;
      temp_b_code[2]=12;
      temp_b_code[2]=temp_b_code[2]<<6;
      temp_b_code[2]|=return_regi;
      temp_b_code[2]=temp_b_code[2]<<6;
      temp_b_code[2]|=return_regi;
      temp_b_code[2]=temp_b_code[2]<<6;
      temp_b_code[2]|=x_regi;
      temp_b_code[2]=temp_b_code[2]<<24;
      temp_b_code[2]|=0x1b1b;
      temp_b_code[2]=temp_b_code[2]<<7;
      temp_b_code[2]|=((1<<3)|((SV>1?1:0)<<2)|((SV>2?1:0)<<1)|((SV>3?1:0)));
      temp_b_code[2]=temp_b_code[2]<<1;
      temp_b_code[2]|=1;
      temp_b_code[2]=temp_b_code[2]<<2;
      temp_b_code[2]|=SV-1;
      temp_b_code[2]=temp_b_code[2]<<7;
      temp_b_code[3]=temp_b_code[2];
      temp_b_code[3]&=(long long unsigned)0x07FF81FFFFFFFFFFLL;
      temp_b_code[3]|=(long long unsigned)11<<59;
      temp_b_code[3]|=(long long unsigned)remain_regi<<41;

#ifdef SET_NOP_NINE
      insert_nop_nine(4);
      for(i=0;i<4*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<4;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif
      
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),4,output_ass);  
#endif
      
#ifdef SET_NOP_NINE
      return 4*2-1;
#else
      return 4;
#endif
    }

}

int GL_Pow(int return_regi,
	   int regiX,
	   int regiY,
	   int remain_regi,
	   int SV,
	   FILE *output_ass)
//remain_regi can be used by OUR_TEMP_REGI
{
  int i;
  //assembly code
  int b_output_ass=1;
  FILE *ass_file;

  if(output_ass)
    ass_file = output_ass;
  else
    {
      b_output_ass =0;
      sprintf(temp_string,"POW_R%d_R%d_R%d_ass_file",return_regi,regiX,regiY);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");
  switch(SV)
    {
    case 4:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.wyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.wyzw, <%d>\n",temp_string,remain_regi,remain_regi,regiY,SV);
      sprintf(temp_string,"%s\tPOW2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      sprintf(temp_string,"%s\tMVR R%d.0001, R%d.xyzx, <%d>\n",temp_string,return_regi,remain_regi,SV);
    case 3:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.zyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.zyzw, <%d>\n",temp_string,remain_regi,remain_regi,regiY,SV);
      sprintf(temp_string,"%s\tPOW2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      sprintf(temp_string,"%s\tMVR R%d.0010, R%d.xyxw, <%d>\n",temp_string,return_regi,remain_regi,SV);
    case 2:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.yyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.yyzw, <%d>\n",temp_string,remain_regi,remain_regi,regiY,SV);
      sprintf(temp_string,"%s\tPOW2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      sprintf(temp_string,"%s\tMVR R%d.0100, R%d.xxzw, <%d>\n",temp_string,return_regi,remain_regi,SV);
    case 1:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.xyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi,remain_regi,regiY,SV);
      sprintf(temp_string,"%s\tPOW2 R%d, R%d\n",temp_string,return_regi,remain_regi);
      break;
    default:
      perror("GL_Pow: SV");
    }

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!b_output_ass)
    fclose(ass_file);

  //machine code
  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=7131;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;
  
  temp_b_code[1]=4;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<22;
  temp_b_code[1]|=6939;
  temp_b_code[1]=temp_b_code[1]<<16;
  temp_b_code[1]|=17;
  temp_b_code[1]=temp_b_code[1]<<9;
  
  temp_b_code[2]=17;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=regiY;
  temp_b_code[2]=temp_b_code[2]<<24;
  temp_b_code[2]|=7131;
  temp_b_code[2]=temp_b_code[2]<<8;
  temp_b_code[2]|=17;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=SV-1;
  temp_b_code[2]=temp_b_code[2]<<7;
  
  temp_b_code[3]=5;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=remain_regi;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=remain_regi;
  temp_b_code[3]=temp_b_code[3]<<22;
  temp_b_code[3]|=6939;
  temp_b_code[3]=temp_b_code[3]<<16;
  temp_b_code[3]|=17;
  temp_b_code[3]=temp_b_code[3]<<2;
  //temp_b_code[3]|=SV-1; //simulator says it should be zero, so I fix it below.
  temp_b_code[3]|=0;
  temp_b_code[3]=temp_b_code[3]<<7;
  
  temp_b_code[4]=temp_b_code[0];
  //temp_b_code[4]&=(long long unsigned)0xF81FFFFE000003FFLL; 
  //temp_b_code[4]|=(long long unsigned)((long long unsigned)return_regi<<53)|((long long unsigned)((24<<15)+1)<<10); //didn't modify the Rs part.
  temp_b_code[4]&=(long long unsigned)0xF8007FFE000003FFLL;
  temp_b_code[4]|=(long long unsigned)((long long unsigned)return_regi<<53)|(long long unsigned)((long long unsigned)remain_regi<<47)|((long long unsigned)((24<<15)+1)<<10);
  //printf("%d\n",remain_regi);
  //printf("%llx\n",temp_b_code[4]);
  switch(SV)
    {
    case 4:
      
#ifdef SET_NOP_NINE
      insert_nop_nine(5);
      for(i=0;i<2*5;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<5;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif
      
    case 3:
      temp_b_code[0]&=(long long unsigned)0xFFFFFFFF7FFFFFFFLL;
      temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFF7FFFFFLL;
      temp_b_code[4]&=(long long unsigned)0xFFFFFFFE000003FFLL;
      temp_b_code[4]|=(long long unsigned)((long long unsigned)((19<<15)+2)<<10);
      
#ifdef SET_NOP_NINE
      insert_nop_nine(5);
      for(i=0;i<2*5;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<5;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif
      
    case 2:
      temp_b_code[0]&=(long long unsigned)0xFFFFFFFE7FFFFFFFLL;
      temp_b_code[0]|=(long long unsigned)((long long unsigned)1<<31);
      temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFE7FFFFFLL;
      temp_b_code[2]|=(long long unsigned)((long long unsigned)1<<23);
      temp_b_code[4]&=(long long unsigned)0xFFFFFFFE000003FFLL;
      temp_b_code[4]|=(long long unsigned)((long long unsigned)((11<<15)+4)<<10);

#ifdef SET_NOP_NINE
      insert_nop_nine(5);
      for(i=0;i<2*5;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<5;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif

    case 1:
      temp_b_code[0]&=(long long unsigned)0xFFFFFFFE7FFFFFFFLL;
      temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFE7FFFFFLL;
      temp_b_code[3]&=(long long unsigned)0xF81FFFFFFFFFFFFFLL;
      temp_b_code[3]|=(long long unsigned)((long long unsigned)return_regi<<53);

#ifdef SET_NOP_NINE
      insert_nop_nine(4);
      for(i=0;i<2*4-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<4;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif
      break;
    default:
      perror("GL_Pow SV");
    }

#ifdef SET_NOP_NINE
  return ((SV-1)*5+4)*2-1;
#else
  return (SV-1)*5+4;
#endif


}

int GL_Dot(int return_regi,
	   int regiX,
	   int regiY,
	   int SV,
	   FILE* output_ass)
{
  int i,write_mask=8;

  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"DOT_R%d_R%d_R%d_ass_file",return_regi,regiX,regiY);
      ass_file=fopen(temp_string,"w");
    }

  //printf("SV: %d\n",SV);

  if(SV>=1 && SV <=4)
    sprintf(temp_string,"\tDPF R%d.1000, R%d.xyzw, R%d.xyzw, <%d>\n",return_regi,regiX,regiY,SV);
    //sprintf(temp_string,"\tDPF R%d.%d%d%d%d, R%d, R%d, <%d>\n",return_regi,(write_mask&8)?1:0,(write_mask&4)?1:0,(write_mask&2)?1:0,(write_mask&1)?1:0,regiX,regiY,SV);
  else
    perror("GL_Dot component");
  
 
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code

  temp_b_code[0]=19;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiY;
  temp_b_code[0]=temp_b_code[0]<<24;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[0]|=write_mask;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=(SV-1);
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  return 1*2-1;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  return 1;
#endif

}

int GL_Cross(int return_regi,
	     int x_regi,
	     int y_regi,
	     int remain_regi,
	     int remain_regi2,
	     FILE* output_ass)
{
  int i;

  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"CROSS_R%d_R%d_R%d_ass_file",return_regi,x_regi,y_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string,"");

  sprintf(temp_string,"%s\tMULF R%d.1110, R%d.yzxz, R%d.zxyz, <%d>\n",temp_string,remain_regi,x_regi,y_regi,4);
  sprintf(temp_string,"%s\tMULF R%d.1110, R%d.zxyy, R%d.yzxx, <%d>\n",temp_string,remain_regi2,x_regi,y_regi,4);
  sprintf(temp_string,"%s\tADDF R%d.1110, R%d.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi,remain_regi2,3);


  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);


  //machine code

  temp_b_code[0]=17;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=x_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=y_regi;
  temp_b_code[0]=temp_b_code[0]<<24;
  temp_b_code[0]|=0x1b6286;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[0]|=15;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=17;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi2;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=x_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=y_regi;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=0x1b8560;
  temp_b_code[1]=temp_b_code[1]<<7;
  temp_b_code[1]|=15;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=2;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=16;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=return_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi2;
  temp_b_code[2]=temp_b_code[2]<<24;
  temp_b_code[2]|=0x1b1b1b;
  temp_b_code[2]=temp_b_code[2]<<3;
  temp_b_code[2]|=2;
  temp_b_code[2]=temp_b_code[2]<<4;
  temp_b_code[2]|=15;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=2;
  temp_b_code[2]=temp_b_code[2]<<7;


#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  return 3*2-1;
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  return 3;
#endif

}

int GL_Max(int return_regi,
	   int regiX,
	   int regiY,
	   int remain_regi,
	   int SV,
	   int type,   //type 0 : genType max (genType x, genType y)
	               //type 1 : genType max (genType x, float y)
	   FILE* output_ass)
{
  int i;

  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"MAX_R%d_R%d_R%d_ass_file",return_regi,regiX,regiY);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");
  if(type==1)
    {
      sprintf(temp_string,"\tMVR R%d.1111, R%d.xxxx, <%d>\n",remain_regi,regiY,SV);
      sprintf(temp_string,"%s\tMAXF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,regiX,remain_regi,SV);
    }
  else if(type==0)
    sprintf(temp_string,"%s\tMAXF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,regiX,regiY,SV);
  else
    perror("GL_Max: type");

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code

  if(type==1)
    {
      temp_b_code[0]=7;
      temp_b_code[0]=temp_b_code[0]<<6;
      //temp_b_code[0]|=regiY;             //fix the instructiob to be below.
      temp_b_code[0]|=remain_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiY;
      temp_b_code[0]=temp_b_code[0]<<22;
      temp_b_code[0]|=0x1B00;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0x1F;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

      temp_b_code[1]=12;
      temp_b_code[1]=temp_b_code[1]<<6;
      temp_b_code[1]|=return_regi;
      temp_b_code[1]=temp_b_code[1]<<6;
      temp_b_code[1]|=regiX;
      temp_b_code[1]=temp_b_code[1]<<6;
      //temp_b_code[1]|=return_regi;      //fix the instructiob to be below.
      temp_b_code[1]|=remain_regi;
      temp_b_code[1]=temp_b_code[1]<<24;
      temp_b_code[1]|=0x1b1b;
      temp_b_code[1]=temp_b_code[1]<<8;
      temp_b_code[1]|=((1<<4)|((SV>1?1:0)<<3)|((SV>2?1:0)<<2)|((SV>3?1:0)<<1)|1);
      temp_b_code[1]=temp_b_code[1]<<2;
      temp_b_code[1]|=SV-1;
      temp_b_code[1]=temp_b_code[1]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  return 2*2-1;
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  return 2;
#endif
    }
  else
    {
      temp_b_code[0]=12;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=return_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiY;
      temp_b_code[0]=temp_b_code[0]<<24;
      temp_b_code[0]|=0x1b1b;
      temp_b_code[0]=temp_b_code[0]<<8;
      temp_b_code[0]|=((1<<4)|((SV>1?1:0)<<3)|((SV>2?1:0)<<2)|((SV>3?1:0)<<1)|1);
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  return 1*2-1;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  return 1;
#endif
    }
}

int GL_Mod(int return_regi,
	   int regiX,
	   int regiY,
	   int remain_regi,
	   int remain_regi2,
	   int remain_regi3,
	   char VOrF,
	   int SV,
	   int type,   //type 0 : genType mod (genType x, genType y)
	               //type 1 : genType mod (genType x, float y)
	   FILE* output_ass)
{
  int i;
  int instr_count=0;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"MOD_R%d_R%d_R%d_R%d_R%d_ass_file",return_regi,regiX,regiY,remain_regi,remain_regi2);
      ass_file=fopen(temp_string,"w");
    }

  //assembly code
  sprintf(temp_string,"");
  if(type == 1)
    sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xxxx, <%d>\n",temp_string,regiY,regiY,SV);

  switch(SV)
    {
    case 4:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.wyzw, <%d>\n",temp_string,remain_regi,regiY,SV);
      sprintf(temp_string,"%s\tRCP R%d.0001, R%d\n",temp_string,remain_regi,remain_regi);
    case 3:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.zyzw, <%d>\n",temp_string,remain_regi,regiY,SV);
      sprintf(temp_string,"%s\tRCP R%d.0010, R%d\n",temp_string,remain_regi,remain_regi);
    case 2:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.yyzw, <%d>\n",temp_string,remain_regi,regiY,SV);
      sprintf(temp_string,"%s\tRCP R%d.0100, R%d\n",temp_string,remain_regi,remain_regi);
    case 1:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.xyzw, <%d>\n",temp_string,remain_regi,regiY,SV);
      sprintf(temp_string,"%s\tRCP R%d.1000, R%d\n",temp_string,remain_regi,remain_regi);
      break;
    default:
      perror("GL_Mod(): SV error!");
      break;
    }

  sprintf(temp_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,remain_regi,regiX,SV);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  //machine code

  if(type==1)
    {
      temp_b_code[0]=7;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiY;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiY;
      temp_b_code[0]=temp_b_code[0]<<22;
      temp_b_code[0]|=0x1B00;
      temp_b_code[0]=temp_b_code[0]<<15;
      temp_b_code[0]|=0xF;
      temp_b_code[0]=temp_b_code[0]<<3;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=2;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif      
    }

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiY;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1BDB;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=0x8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=2;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<22;
  temp_b_code[1]|=0x1B1B;
  temp_b_code[1]=temp_b_code[1]<<15;
  temp_b_code[1]|=0x1;
  temp_b_code[1]=temp_b_code[1]<<3;
  //temp_b_code[1]|=SV-1; //old
  temp_b_code[1]|=4; //new //RCP : F bit = 1 , and don't care SV.
  temp_b_code[1]=temp_b_code[1]<<7;
  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif      
    case 3:
      temp_b_code[0]&=0xFFFFFFF87FFFFFFFLL;
      temp_b_code[0]|=((long long unsigned)2) << 31;
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=((long long unsigned)2) << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif      
    case 2:
      temp_b_code[0]&=0xFFFFFFF87FFFFFFFLL;
      temp_b_code[0]|=((long long unsigned)1) << 31;
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=((long long unsigned)4) << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif      
    case 1:
      temp_b_code[0]&=0xFFFFFFF87FFFFFFFLL;
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=((long long unsigned)8) << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif      
      break;
    default:
      perror("GL_Mod(): SV error!");
      break;
    }

  temp_b_code[0]=17;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<24;
  temp_b_code[0]|=0x1b1b1b;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=2;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=1;
#endif      
  
  instr_count+=GL_Floor(remain_regi3,remain_regi,remain_regi2,VOrF,SV,output_ass);


  //assembly code
  sprintf(temp_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi2,SV>1?1:0,SV>2?1:0,SV>3?1:0,regiY,remain_regi3,SV);
  sprintf(temp_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,regiX,remain_regi2,SV);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code
  temp_b_code[0]=17;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi2;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiY;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi3;
  temp_b_code[0]=temp_b_code[0]<<24;
  temp_b_code[0]|=0x1b1b1b;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=16;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=return_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=regiX;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi2;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=0x1b1b1b;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=2;
  temp_b_code[1]=temp_b_code[1]<<4;
  temp_b_code[1]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  BinCode[LastBinLine++]=nop_nine_instruction; //add one nop to end of GL_Floor()
  instr_count+=1;

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=3;
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=2;
#endif      

  return instr_count;
}

int GL_Min(int return_regi,
	   int regiX,
	   int regiY,
	   int remain_regi,
	   int SV,
	   int type,   //type 0 : genType min (genType x, genType y)
	               //type 1 : genType min (genType x, float y)
	   FILE* output_ass)
{
  int i;

  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"MIN_%d_R%d_R%d_ass_file",return_regi,regiX,regiY);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");
  if(type==1)
    {
      sprintf(temp_string,"\tMVR R%d.1111, R%d.xxxx, <%d>\n",remain_regi,regiY,SV);   
      sprintf(temp_string,"%s\tMINF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,regiX,remain_regi,SV);
    }
  else if(type==0)
    sprintf(temp_string,"%s\tMINF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,regiX,regiY,SV);
  else
    perror("GL_Min: type");

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code

  if(type==1)
    {
      temp_b_code[0]=7;
      temp_b_code[0]=temp_b_code[0]<<6;
      //temp_b_code[0]|=regiY;  //fix the instructiob to be below.
      temp_b_code[0]|=remain_regi; 
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiY;
      temp_b_code[0]=temp_b_code[0]<<22;
      temp_b_code[0]|=0x1B00;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0x1F;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

      temp_b_code[1]=11;
      temp_b_code[1]=temp_b_code[1]<<6;
      temp_b_code[1]|=return_regi;
      temp_b_code[1]=temp_b_code[1]<<6;
      temp_b_code[1]|=regiX;
      temp_b_code[1]=temp_b_code[1]<<6;
      //temp_b_code[1]|=return_regi; //fix the instructiob to be below.
      temp_b_code[1]|=remain_regi;
      temp_b_code[1]=temp_b_code[1]<<24;
      temp_b_code[1]|=0x1b1b;
      temp_b_code[1]=temp_b_code[1]<<8;
      temp_b_code[1]|=((1<<4)|((SV>1?1:0)<<3)|((SV>2?1:0)<<2)|((SV>3?1:0)<<1)|1);
      temp_b_code[1]=temp_b_code[1]<<2;
      temp_b_code[1]|=SV-1;
      temp_b_code[1]=temp_b_code[1]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(2);
  for(i=0;i<2*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  return 2*2-1;
#else
  for(i=0;i<2;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  return 2;
#endif
    }
  else
    {
      temp_b_code[0]=11;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=return_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiX;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regiY;
      temp_b_code[0]=temp_b_code[0]<<24;
      temp_b_code[0]|=0x1b1b;
      temp_b_code[0]=temp_b_code[0]<<8;
      temp_b_code[0]|=((1<<4)|((SV>1?1:0)<<3)|((SV>2?1:0)<<2)|((SV>3?1:0)<<1)|1);
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  return 1*2-1;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  return 1;
#endif
    }
}

int GL_Mix(int return_regi,
	   int x_regi,
	   int y_regi,
	   int a_regi,
	   int remain_regi,
	   int type,          //type 0: genType x, genType y, genType a
                              //type 1: genType x, genType y, float a
	   int SV,
	   FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"MIX_R%d_R%d_R%d_R%d_ass_file",return_regi,x_regi,y_regi,a_regi);
      ass_file=fopen(temp_string,"w");
    }
  /*
  sprintf(temp_string,"\tLDIF R%d.1%d%d%d, 1.0\n",remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0);
  if(type==1)
    sprintf(temp_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, -R%d.xxxx, <%d>\n",temp_string,remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,remain_regi,a_regi,SV);
  else
    sprintf(temp_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,remain_regi,a_regi,SV);
  sprintf(temp_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,remain_regi,x_regi,SV);
  if(type==1)
    sprintf(temp_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xxxx, <%d>\n",temp_string,y_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,y_regi,a_regi,SV);
  else  
    sprintf(temp_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,y_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,y_regi,a_regi,SV);
  sprintf(temp_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,remain_regi,y_regi,SV);
  */
  

  sprintf(temp_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, -R%d.xyzw, <%d>\n",temp_string,remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,y_regi,x_regi,SV);
  
  if(type==1)
    sprintf(temp_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xxxx, <%d>\n",temp_string,remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,remain_regi,a_regi,SV);
  else  
    sprintf(temp_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,remain_regi,a_regi,SV);
  sprintf(temp_string,"%s\tADDF R%d.1%d%d%d, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,remain_regi,x_regi,SV);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code
  /*
  float temp_float=1.0;
  unsigned temp_int;
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=(long long unsigned)temp_int;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=16;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=a_regi;
  temp_b_code[1]=temp_b_code[1]<<24;
  if(type==1)
    temp_b_code[1]|=0x1b1b00;
  else
    temp_b_code[1]|=0x1b1b1b;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=2;
  temp_b_code[1]=temp_b_code[1]<<4;
  temp_b_code[1]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=17;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=x_regi;
  temp_b_code[2]=temp_b_code[2]<<24;
  temp_b_code[2]|=0x1b1b1b;
  temp_b_code[2]=temp_b_code[2]<<7;
  temp_b_code[2]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=SV-1;
  temp_b_code[2]=temp_b_code[2]<<7;

  temp_b_code[3]=temp_b_code[2];
  temp_b_code[3]&=0xF80001FFFFFFFFFFLL;
  temp_b_code[3]|=(long long unsigned) y_regi <<53;
  temp_b_code[3]|=(long long unsigned) y_regi <<47;
  temp_b_code[3]|=(long long unsigned) a_regi <<41;

  temp_b_code[4]=temp_b_code[1];
  temp_b_code[4]&=0xF81F81FFFE003FFFLL;
  temp_b_code[4]|=(long long unsigned) return_regi <<53;
  temp_b_code[4]|=(long long unsigned) y_regi <<41;
  temp_b_code[4]|=(long long unsigned) 0x1b <<17;

#ifdef SET_NOP_NINE
      insert_nop_nine(5);
      for(i=0;i<2*5-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      return 2*5-1;
#else
      for(i=0;i<5;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      return 5;
#endif
  */
  //Kaymon rewrite it 
  temp_b_code[0]=16;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=y_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=x_regi;
  temp_b_code[0]=temp_b_code[0]<<24;
  temp_b_code[0]|=0x1b1b1b;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<4;
  temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=17;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=a_regi;
  temp_b_code[1]=temp_b_code[1]<<24;
  if(type==1)
    temp_b_code[1]|=0x1b1b00;
  else
    temp_b_code[1]|=0x1b1b1b;
  temp_b_code[1]=temp_b_code[1]<<7;
  temp_b_code[1]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=16;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=return_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=x_regi;
  temp_b_code[2]=temp_b_code[2]<<24;
  temp_b_code[2]|=0x1b1b1b;
  temp_b_code[2]=temp_b_code[2]<<3;
  temp_b_code[2]|=0;
  temp_b_code[2]=temp_b_code[2]<<4;
  temp_b_code[2]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=SV-1;
  temp_b_code[2]=temp_b_code[2]<<7;




#ifdef SET_NOP_NINE
      insert_nop_nine(3);
      for(i=0;i<2*3-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      return 2*3-1;
#else
      for(i=0;i<3;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      return 3;
#endif
}

int GL_SmoothStep(int return_regi,
		  int edge0_regi,
		  int edge1_regi,
		  int x_regi,
		  int remain_regi,
		  int remain_regi2,
		  int remain_regi3,
		  int type,        //type 0: genType edge0, genType edge1, genType x 
		                   //type 1: float edge0, float edge1, genType x 
		  int VOrF,
		  int SV,
		  FILE *output_ass)
{
  int i;
  int instr_count=0;
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"SMOOTHSTEP_R%d_R%d_R%d_R%d_R%d_R%d_R%d_ass_file",return_regi,edge0_regi,edge1_regi,x_regi,remain_regi,remain_regi2,remain_regi3);
      ass_file=fopen(temp_string,"w");
    }

  //assembly code
  sprintf(temp_string,"");

  if(type == 1)
    {
      sprintf(temp_string,"%s\tMVR R%d.1%d%d%d, R%d.xxxx, <%d>\n",temp_string,edge0_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,edge0_regi,SV);
      sprintf(temp_string,"%s\tMVR R%d.1%d%d%d, R%d.xxxx, <%d>\n",temp_string,edge1_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,edge1_regi,SV);
    }

  sprintf(temp_string,"%s\tADDIF R%d.0100, R0, 1.0, <%d>\n",temp_string,remain_regi,SV);
  sprintf(temp_string,"%s\tADDIF R%d.0010, R0, 2.0, <%d>\n",temp_string,remain_regi,SV);
  sprintf(temp_string,"%s\tADDIF R%d.0001, R0, 3.0, <%d>\n",temp_string,remain_regi,SV);

  switch(SV)
    {
    case 4:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 3, 0001, <%d>\n",temp_string,x_regi,edge0_regi,x_regi,edge0_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.wwww, R%d.wwww, 3, <%d>\n",temp_string,x_regi,edge0_regi,SV);
      else
	perror("GL_SmoothStep(): VOrF is wrong.");

      sprintf(temp_string,"%s\tMVR R%d.0001, R0.xxxx, <%d>\n",temp_string,return_regi,SV);   //1
      sprintf(temp_string,"%s\tJR 14\n",temp_string);

      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 3, 0001, <%d>\n",temp_string,x_regi,edge1_regi,x_regi,edge1_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBLTSF R%d.wwww, R%d.wwww, 3, <%d>\n",temp_string,x_regi,edge1_regi,SV);

      sprintf(temp_string,"%s\tMVR R%d.0001, R%d.yyyy, <%d>\n",temp_string,return_regi,remain_regi,SV);   //4
      sprintf(temp_string,"%s\tJR 11\n",temp_string);

      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.wyzw, -R%d.wyzw, <4>\n",temp_string, remain_regi2, edge1_regi, edge0_regi);  //6
      sprintf(temp_string,"%s\tRCP R%d, R%d\n",temp_string,remain_regi2,remain_regi2);
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.wyzw, -R%d.wyzw, <4>\n",temp_string, remain_regi3, x_regi, edge0_regi);      
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi2,remain_regi2,remain_regi3);
      sprintf(temp_string,"%s\tMAXF R%d.1000, R%d.xxxx, R0.xxxx, <%d>\n",temp_string,remain_regi2,remain_regi2,SV);    //10
      sprintf(temp_string,"%s\tMINF R%d.1000, R%d.xxxx, R%d.yyyy, <%d>\n",temp_string,remain_regi2,remain_regi2,remain_regi,SV);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.zzzz, R%d.xyzw, <4>\n",temp_string,remain_regi3,remain_regi,remain_regi2);
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.wwww, -R%d.xyzw, <4>\n",temp_string, remain_regi3, remain_regi, remain_regi3);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi3,remain_regi3,remain_regi2);
      sprintf(temp_string,"%s\tMULF R%d.0001, R%d.xyzx, R%d.xyzx, <4>\n",temp_string,return_regi,remain_regi3,remain_regi2);
    case 3:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 3, 0010, <%d>\n",temp_string,x_regi,edge0_regi,x_regi,edge0_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.zzzz, R%d.zzzz, 3, <%d>\n",temp_string,x_regi,edge0_regi,SV);
      else
	perror("GL_SmoothStep(): VOrF is wrong.");

      sprintf(temp_string,"%s\tMVR R%d.0010, R0.xxxx, <%d>\n",temp_string,return_regi,SV);   //1
      sprintf(temp_string,"%s\tJR 14\n",temp_string);

      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 3, 0010, <%d>\n",temp_string,x_regi,edge1_regi,x_regi,edge1_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBLTSF R%d.zzzz, R%d.zzzz, 3, <%d>\n",temp_string,x_regi,edge1_regi,SV);

      sprintf(temp_string,"%s\tMVR R%d.0010, R%d.yyyy, <%d>\n",temp_string,return_regi,remain_regi,SV);   //4
      sprintf(temp_string,"%s\tJR 11\n",temp_string);

      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.zyzw, -R%d.zyzw, <4>\n",temp_string, remain_regi2, edge1_regi, edge0_regi);  //6
      sprintf(temp_string,"%s\tRCP R%d, R%d\n",temp_string,remain_regi2,remain_regi2);
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.zyzw, -R%d.zyzw, <4>\n",temp_string, remain_regi3, x_regi, edge0_regi);      
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi2,remain_regi2,remain_regi3);
      sprintf(temp_string,"%s\tMAXF R%d.1000, R%d.xxxx, R0.xxxx, <%d>\n",temp_string,remain_regi2,remain_regi2,SV);    //10
      sprintf(temp_string,"%s\tMINF R%d.1000, R%d.xxxx, R%d.yyyy, <%d>\n",temp_string,remain_regi2,remain_regi2,remain_regi,SV);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.zzzz, R%d.xyzw, <4>\n",temp_string,remain_regi3,remain_regi,remain_regi2);
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.wwww, -R%d.xyzw, <4>\n",temp_string, remain_regi3, remain_regi, remain_regi3);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi3,remain_regi3,remain_regi2);
      sprintf(temp_string,"%s\tMULF R%d.0010, R%d.xyxw, R%d.xyxw, <4>\n",temp_string,return_regi,remain_regi3,remain_regi2);
    case 2:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 3, 0100, <%d>\n",temp_string,x_regi,edge0_regi,x_regi,edge0_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.yyyy, R%d.yyyy, 3, <%d>\n",temp_string,x_regi,edge0_regi,SV);
      else
	perror("GL_SmoothStep(): VOrF is wrong.");

      sprintf(temp_string,"%s\tMVR R%d.0100, R0.xxxx, <%d>\n",temp_string,return_regi,SV);   //1
      sprintf(temp_string,"%s\tJR 14\n",temp_string);

      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 3, 0100, <%d>\n",temp_string,x_regi,edge1_regi,x_regi,edge1_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBLTSF R%d.yyyy, R%d.yyyy, 3, <%d>\n",temp_string,x_regi,edge1_regi,SV);

      sprintf(temp_string,"%s\tMVR R%d.0100, R%d.yyyy, <%d>\n",temp_string,return_regi,remain_regi,SV);   //4
      sprintf(temp_string,"%s\tJR 11\n",temp_string);

      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.yyzw, -R%d.yyzw, <4>\n",temp_string, remain_regi2, edge1_regi, edge0_regi);  //6
      sprintf(temp_string,"%s\tRCP R%d, R%d\n",temp_string,remain_regi2,remain_regi2);
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.yyzw, -R%d.yyzw, <4>\n",temp_string, remain_regi3, x_regi, edge0_regi);      
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi2,remain_regi2,remain_regi3);
      sprintf(temp_string,"%s\tMAXF R%d.1000, R%d.xxxx, R0.xxxx, <%d>\n",temp_string,remain_regi2,remain_regi2,SV);    //10
      sprintf(temp_string,"%s\tMINF R%d.1000, R%d.xxxx, R%d.yyyy, <%d>\n",temp_string,remain_regi2,remain_regi2,remain_regi,SV);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.zzzz, R%d.xyzw, <4>\n",temp_string,remain_regi3,remain_regi,remain_regi2);
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.wwww, -R%d.xyzw, <4>\n",temp_string, remain_regi3, remain_regi, remain_regi3);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi3,remain_regi3,remain_regi2);
      sprintf(temp_string,"%s\tMULF R%d.0100, R%d.xxzw, R%d.xxzw, <4>\n",temp_string,return_regi,remain_regi3,remain_regi2);
    case 1:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 3, 1000, <%d>\n",temp_string,x_regi,edge0_regi,x_regi,edge0_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.xxxx, R%d.xxxx, 3, <%d>\n",temp_string,x_regi,edge0_regi,SV);
      else
	perror("GL_SmoothStep(): VOrF is wrong.");

      sprintf(temp_string,"%s\tMVR R%d.1000, R0.xxxx, <%d>\n",temp_string,return_regi,SV);   //1
      sprintf(temp_string,"%s\tJR 14\n",temp_string);

      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 3, 1000, <%d>\n",temp_string,x_regi,edge1_regi,x_regi,edge1_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBLTSF R%d.xxxx, R%d.xxxx, 3, <%d>\n",temp_string,x_regi,edge1_regi,SV);

      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.yyyy, <%d>\n",temp_string,return_regi,remain_regi,SV);   //4
      sprintf(temp_string,"%s\tJR 11\n",temp_string);

      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.xyzw, -R%d.xyzw, <4>\n",temp_string, remain_regi2, edge1_regi, edge0_regi);  //6
      sprintf(temp_string,"%s\tRCP R%d, R%d\n",temp_string,remain_regi2,remain_regi2);
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.xyzw, -R%d.xyzw, <4>\n",temp_string, remain_regi3, x_regi, edge0_regi);      
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi2,remain_regi2,remain_regi3);
      sprintf(temp_string,"%s\tMAXF R%d.1000, R%d.xxxx, R0.xxxx, <%d>\n",temp_string,remain_regi2,remain_regi2,SV);    //10
      sprintf(temp_string,"%s\tMINF R%d.1000, R%d.xxxx, R%d.yyyy, <%d>\n",temp_string,remain_regi2,remain_regi2,remain_regi,SV);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.zzzz, R%d.xyzw, <4>\n",temp_string,remain_regi3,remain_regi,remain_regi2);
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.wwww, -R%d.xyzw, <4>\n",temp_string, remain_regi3, remain_regi, remain_regi3);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi3,remain_regi3,remain_regi2);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi,remain_regi3,remain_regi2);
      break;
    default:
      perror("GL_SmoothStep(): SV error");
      break;
    }


  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code
  float temp_float;
  unsigned temp_int;
  if(type == 1)
    {
      temp_b_code[0]=7;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=edge0_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=edge0_regi;
      temp_b_code[0]=temp_b_code[0]<<22;
      temp_b_code[0]|=0x1B00;
      temp_b_code[0]=temp_b_code[0]<<15;
      temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
      temp_b_code[0]=temp_b_code[0]<<3;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=0xF8007FFFFFFFFFFFLL;
      temp_b_code[1]|=(long long unsigned) edge1_regi <<53;
      temp_b_code[1]|=(long long unsigned) edge1_regi <<47;

#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    }

  temp_float = 1.0;
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=4;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;  

  temp_float = 2.0;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[1]=temp_b_code[0];
  temp_b_code[1]&=0xFFFFC000000003FFLL;
  temp_b_code[1]|=((long long unsigned) temp_int) <<15;
  temp_b_code[1]|=((long long unsigned) 2) <<10;

  temp_float = 3.0;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[2]=temp_b_code[0];
  temp_b_code[2]&=0xFFFFC000000003FFLL;
  temp_b_code[2]|=((long long unsigned) temp_int) <<15;
  temp_b_code[2]|=((long long unsigned) 1) <<10;

#ifdef SET_NOP_NINE
  insert_nop_nine(3);
  for(i=0;i<3*2;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=6;
#else
  for(i=0;i<3;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=3;
#endif

  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=edge0_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=6; //insert the nop between two instructions, so jump double instructions.
#else
      temp_b_code[0]|=3;
#endif
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=edge0_regi;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

      temp_b_code[2]=24;
      temp_b_code[2]=temp_b_code[2]<<28;
#ifdef SET_NOP_NINE
      temp_b_code[2]|=28; //insert the nop between two instructions, so jump double instructions.
#else
      temp_b_code[2]|=14; //insert the nop between two instructions, so jump double instructions.
#endif
      temp_b_code[2]=temp_b_code[2]<<31;

      temp_b_code[3]=temp_b_code[0];
      temp_b_code[3]&=0x07E07FFFFE0FFFFFLL;
      temp_b_code[3]|=((long long unsigned) 26) <<59;
      temp_b_code[3]|=((long long unsigned) edge1_regi) <<47;
      temp_b_code[3]|=((long long unsigned) edge1_regi) <<20;
      
      temp_b_code[5]=temp_b_code[2];
      temp_b_code[5]&=0xFFFF80007FFFFFFFLL;
#ifdef SET_NOP_NINE
      temp_b_code[5]|=((long long unsigned)22) <<31;//insert the nop between two instructions, so jump double instructions.
#else
      temp_b_code[5]|=((long long unsigned)11) <<31;
#endif
#else
      //********************unified branch********************//
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=edge0_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=6; //insert the nop between two instructions, so jump double instructions.
#else
      temp_b_code[0]|=3;
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0XFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1; 
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

      temp_b_code[2]=24;
      temp_b_code[2]=temp_b_code[2]<<28; 
#ifdef SET_NOP_NINE
      temp_b_code[2]|=28; //insert the nop between two instructions, so jump double instructions.
#else
      temp_b_code[2]|=14; 
#endif
      temp_b_code[2]=temp_b_code[2]<<17;
      temp_b_code[2]|=1;
      temp_b_code[2]=temp_b_code[2]<<14;

      temp_b_code[3]=temp_b_code[0];
      temp_b_code[3]&=0x07E07FFFFFFFFFFFLL;
      temp_b_code[3]|=((long long unsigned) 26) <<59;
      temp_b_code[3]|=((long long unsigned) edge1_regi) <<47;

      temp_b_code[5]=temp_b_code[2];
      temp_b_code[5]&=0xFFFF80007FFFFFFFLL;
#ifdef SET_NOP_NINE
      temp_b_code[5]|=((long long unsigned)22) <<31;//insert the nop between two instructions, so jump double instructions.
#else
      temp_b_code[5]|=((long long unsigned)11) <<31;
#endif
#endif
    }
  else if(VOrF == 'f')
    {
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=edge0_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=3;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

      temp_b_code[2]=24;
      temp_b_code[2]=temp_b_code[2]<<28;
      temp_b_code[2]|=14;
      temp_b_code[2]=temp_b_code[2]<<17;
      temp_b_code[2]|=1;
      temp_b_code[2]=temp_b_code[2]<<14;

      temp_b_code[3]=temp_b_code[0];
      temp_b_code[3]&=0x07E07FFFFFFFFFFFLL;//0x07807FFFFFFFFFFFLL;
      temp_b_code[3]|=((long long unsigned) 26) <<59;
      temp_b_code[3]|=((long long unsigned) edge1_regi) <<47;

      temp_b_code[5]=temp_b_code[2];
      temp_b_code[5]&=0xFFFF80007FFFFFFFLL;
      temp_b_code[5]|=((long long unsigned) 11) <<31;
    }
  else
    perror("GL_SmoothStep(): VOrF is wrong.");

  temp_b_code[1]=7;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=return_regi;
  temp_b_code[1]=temp_b_code[1]<<28;
  temp_b_code[1]|=0x1B00;
  temp_b_code[1]=temp_b_code[1]<<15;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<3;
  //temp_b_code[1]|=SV-1;
  temp_b_code[1]|=3; //SV should be 3. and then the hw can choose the all swizzle. see the fifth instruction 
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[4]=temp_b_code[1];
  temp_b_code[4]&=0xFF807FFE01FFFFFFLL;
  temp_b_code[4]|=((long long unsigned)remain_regi)<<47;
  temp_b_code[4]|=((long long unsigned)0x55)<<25;

  temp_b_code[6]=16;
  temp_b_code[6]=temp_b_code[6]<<6;
  temp_b_code[6]|=remain_regi2;
  temp_b_code[6]=temp_b_code[6]<<6;
  temp_b_code[6]|=edge1_regi;
  temp_b_code[6]=temp_b_code[6]<<6;
  temp_b_code[6]|=edge0_regi;
  temp_b_code[6]=temp_b_code[6]<<24;
  temp_b_code[6]|=0x1bdbdb;
  temp_b_code[6]=temp_b_code[6]<<3;
  temp_b_code[6]|=2;
  temp_b_code[6]=temp_b_code[6]<<4;
  temp_b_code[6]|=8;
  temp_b_code[6]=temp_b_code[6]<<1;
  temp_b_code[6]|=1;
  temp_b_code[6]=temp_b_code[6]<<2;
  temp_b_code[6]|=3;
  temp_b_code[6]=temp_b_code[6]<<7;

  temp_b_code[7]=2;
  temp_b_code[7]=temp_b_code[7]<<6;
  temp_b_code[7]|=remain_regi2;
  temp_b_code[7]=temp_b_code[7]<<6;
  temp_b_code[7]|=remain_regi2;
  temp_b_code[7]=temp_b_code[7]<<22;
  temp_b_code[7]|=0x1B1B;
  temp_b_code[7]=temp_b_code[7]<<15;
  temp_b_code[7]|=0x8;
  temp_b_code[7]=temp_b_code[7]<<1; //F bit should be 1.
  temp_b_code[7]|=1; 
  temp_b_code[7]=temp_b_code[7]<<9;

  temp_b_code[8]=temp_b_code[6];
  temp_b_code[8]&=0xF8007FFFFFFFFFFFLL;
  temp_b_code[8]|=((long long unsigned)remain_regi3)<<53;
  temp_b_code[8]|=((long long unsigned)x_regi)<<47;  

  temp_b_code[9]=17;
  temp_b_code[9]=temp_b_code[9]<<6;
  temp_b_code[9]|=remain_regi2;
  temp_b_code[9]=temp_b_code[9]<<6;
  temp_b_code[9]|=remain_regi2;
  temp_b_code[9]=temp_b_code[9]<<6;
  temp_b_code[9]|=remain_regi3;
  temp_b_code[9]=temp_b_code[9]<<24;
  temp_b_code[9]|=0x1b1b1b;
  temp_b_code[9]=temp_b_code[9]<<7;
  temp_b_code[9]|=8;
  temp_b_code[9]=temp_b_code[9]<<1;
  temp_b_code[9]|=1;
  temp_b_code[9]=temp_b_code[9]<<2;
  temp_b_code[9]|=3;
  temp_b_code[9]=temp_b_code[9]<<7;
  
  temp_b_code[10]=12;
  temp_b_code[10]=temp_b_code[10]<<6;
  temp_b_code[10]|=remain_regi2;
  temp_b_code[10]=temp_b_code[10]<<6;
  temp_b_code[10]|=remain_regi2;
  temp_b_code[10]=temp_b_code[10]<<37;
  temp_b_code[10]|=8;
  temp_b_code[10]=temp_b_code[10]<<1;
  temp_b_code[10]|=1;
  temp_b_code[10]=temp_b_code[10]<<2;
  temp_b_code[10]|=3; //sv should be 3. reason is as above.
  temp_b_code[10]=temp_b_code[10]<<7;

  temp_b_code[11]=temp_b_code[10];
  temp_b_code[11]&=0x07FF81FFFC03FFFFLL;
  temp_b_code[11]|=((long long unsigned)11)<<59;
  temp_b_code[11]|=((long long unsigned)remain_regi)<<41;
  temp_b_code[11]|=((long long unsigned)0x55)<<17;  

  temp_b_code[12]=temp_b_code[9];
  temp_b_code[12]&=0xF80001FE01FFFFFFLL;
  temp_b_code[12]|=((long long unsigned)remain_regi3)<<53;
  temp_b_code[12]|=((long long unsigned)remain_regi)<<47;
  temp_b_code[12]|=((long long unsigned)remain_regi2)<<41;  
  temp_b_code[12]|=((long long unsigned)0xAA)<<25;  

  temp_b_code[13]=temp_b_code[8];
  temp_b_code[13]&=0xFFE001FE0001FFFFLL;
  temp_b_code[13]|=((long long unsigned)remain_regi)<<47;
  temp_b_code[13]|=((long long unsigned)remain_regi3)<<41;  
  temp_b_code[13]|=((long long unsigned)0xFF1B)<<17;

  temp_b_code[14]=temp_b_code[12];
  temp_b_code[14]&=0xFFE07FFE01FFFFFFLL;
  temp_b_code[14]|=((long long unsigned)remain_regi3)<<47;
  temp_b_code[14]|=((long long unsigned)0x1B)<<25;    

  temp_b_code[15]=temp_b_code[14];
  temp_b_code[15]&=0xF81FFFFE0001C3FFLL;
  temp_b_code[15]|=((long long unsigned)return_regi)<<53;
  temp_b_code[15]|=((long long unsigned)1)<<10;
  temp_b_code[15]|=((long long unsigned)0x1818)<<17;    

  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
      insert_nop_nine(16);
      for(i=0;i<16*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=32;
#else
      for(i=0;i<16;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=16;
#endif
    case 3:
      if(VOrF=='v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=((long long unsigned)2)<<10;
	  temp_b_code[3]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=((long long unsigned)2)<<10;
#else	 
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=((long long unsigned)0xAAAA)<<15;
	  temp_b_code[3]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[3]|=((long long unsigned)0xAAAA)<<15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=((long long unsigned)0xAAAA)<<15;
	  temp_b_code[3]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[3]|=((long long unsigned)0xAAAA)<<15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=((long long unsigned)2)<<10;
      temp_b_code[4]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[4]|=((long long unsigned)2)<<10;
      temp_b_code[6]&=0xFFFFFFFE7E7FFFFFLL;
      temp_b_code[6]|=((long long unsigned)2)<<23;
      temp_b_code[6]|=((long long unsigned)2)<<31;
      temp_b_code[8]&=0xFFFFFFFE7E7FFFFFLL;
      temp_b_code[8]|=((long long unsigned)2)<<23;
      temp_b_code[8]|=((long long unsigned)2)<<31;
      temp_b_code[15]&=0xFFFFFFFE0001C3FFLL;
      temp_b_code[15]|=((long long unsigned)0x1313)<<17;
      temp_b_code[15]|=((long long unsigned)2)<<10;

#ifdef SET_NOP_NINE
      insert_nop_nine(16);
      for(i=0;i<16*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=32;
#else
      for(i=0;i<16;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=16;
#endif
    case 2:
      if(VOrF=='v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=((long long unsigned)4)<<10;
	  temp_b_code[3]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=((long long unsigned)4)<<10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=((long long unsigned)0x5555)<<15;
	  temp_b_code[3]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[3]|=((long long unsigned)0x5555)<<15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=((long long unsigned)0x5555)<<15;
	  temp_b_code[3]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[3]|=((long long unsigned)0x5555)<<15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=((long long unsigned)4)<<10;
      temp_b_code[4]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[4]|=((long long unsigned)4)<<10;
      temp_b_code[6]&=0xFFFFFFFE7E7FFFFFLL;
      temp_b_code[6]|=((long long unsigned)1)<<23;
      temp_b_code[6]|=((long long unsigned)1)<<31;
      temp_b_code[8]&=0xFFFFFFFE7E7FFFFFLL;
      temp_b_code[8]|=((long long unsigned)1)<<23;
      temp_b_code[8]|=((long long unsigned)1)<<31;
      temp_b_code[15]&=0xFFFFFFFE0001C3FFLL;
      temp_b_code[15]|=((long long unsigned)0x0B0B)<<17;
      temp_b_code[15]|=((long long unsigned)4)<<10;

#ifdef SET_NOP_NINE
      insert_nop_nine(16);
      for(i=0;i<16*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=32;
#else
      for(i=0;i<16;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=16;
#endif
    case 1:
      if(VOrF=='v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=((long long unsigned)8)<<10;
	  temp_b_code[3]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[3]|=((long long unsigned)8)<<10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[3]&=0xFFFFFFFF80007FFFLL;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[3]&=0xFFFFFFFF80007FFFLL;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=((long long unsigned)8)<<10;
      temp_b_code[4]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[4]|=((long long unsigned)8)<<10;
      temp_b_code[6]&=0xFFFFFFFE7E7FFFFFLL;
      temp_b_code[8]&=0xFFFFFFFE7E7FFFFFLL;
      temp_b_code[15]&=0xFFFFFFFE0001C3FFLL;
      temp_b_code[15]|=((long long unsigned)0x1B1B)<<17;
      temp_b_code[15]|=((long long unsigned)8)<<10;

#ifdef SET_NOP_NINE
      insert_nop_nine(16);
      for(i=0;i<16*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=31;
#else
      for(i=0;i<16;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=16;
#endif
      break;
    default:
      perror("GL_SmoothStep(): SV error");
      break;
    }


  return instr_count;
}

int GL_Step(int return_regi,
	    int edge_regi,
	    int x_regi,
	    int remain_regi,
	    int remain_regi1,
	    int type,        //type 0: genType edge, genType x 
                             //type 1: float edge, genType x 
	    char VOrF,
	    int SV,
	    FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"STEP_R%d_R%d_R%d_ass_file",return_regi,edge_regi,x_regi);
      ass_file=fopen(temp_string,"w");
    }

  sprintf(temp_string," ");
  sprintf(temp_string,"\tLDI R%d.1%d%d%d, 0\n",remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0);
  if(type==1){
    sprintf(temp_string,"%s\tMVR R%d.1%d%d%d, R%d.xxxx, <%d>\n",temp_string,remain_regi1,SV>1?1:0,SV>2?1:0,SV>3?1:0,edge_regi,SV);
    edge_regi = remain_regi1;
  }

  switch(SV)
    {
    case 4:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 2, 0001, <%d>\n",temp_string,edge_regi,x_regi,edge_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.wwww, R%d.wwww, 2, <%d>\n",temp_string,edge_regi,x_regi,SV);
      else
	perror("GL_Step(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVIF R%d.0001, 1.0\n",temp_string,remain_regi);
    case 3:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 2, 0010, <%d>\n",temp_string,edge_regi,x_regi,edge_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.zzzz, R%d.zzzz, 2, <%d>\n",temp_string,edge_regi,x_regi,SV);
      else
	perror("GL_Step(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVIF R%d.0010, 1.0\n",temp_string,remain_regi);
    case 2:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 2, 0100, <%d>\n",temp_string,edge_regi,x_regi,edge_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.yyyy, R%d.yyyy, 2, <%d>\n",temp_string,edge_regi,x_regi,SV);
      else
	perror("GL_Step(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVIF R%d.0100, 1.0\n",temp_string,remain_regi);
    case 1:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 2, 1000, <%d>\n",temp_string,edge_regi,x_regi,edge_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.xxxx, R%d.xxxx, 2, <%d>\n",temp_string,edge_regi,x_regi,SV);
      else
	perror("GL_Step(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVIF R%d.1000, 1.0\n",temp_string,remain_regi);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi,SV);
      break;
    default:
      perror("GL_Step(): SV is wrong.");
    }
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code
  float temp_float=0.0;
  unsigned temp_int;
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  //temp_b_code[0]|=return_regi;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=(long long unsigned)temp_int;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2*1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
  edge_regi = 15;//set edge's value  back to original number.
  if(type == 1)
    {
      temp_b_code[0]=7;
      temp_b_code[0]=temp_b_code[0]<<6;
      //temp_b_code[0]|=edge_regi;
      temp_b_code[0]|=remain_regi1; //Kaymon
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=edge_regi;
      temp_b_code[0]=temp_b_code[0]<<22;
      temp_b_code[0]|=0x1B00;
      temp_b_code[0]=temp_b_code[0]<<15;
      temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
      temp_b_code[0]=temp_b_code[0]<<3;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
      edge_regi = remain_regi1; //Kaymon
#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2*1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#endif
    }

  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=edge_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4; //change 2 to 4
#else
      temp_b_code[0]|=2; 
#endif
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=edge_regi;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#else
      //****unified branch**********/
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=edge_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4; 
#else
      temp_b_code[0]|=2; 
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#endif
    }
  else if(VOrF == 'f')
    {
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=edge_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=2;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
    }
  else
    perror("GL_Step(): VOrF variable is wrong.");

  temp_float=1.0;
  temp_b_code[1]=10;
  temp_b_code[1]=temp_b_code[1]<<6;
  //temp_b_code[1]|=return_regi;
  temp_b_code[1]|=remain_regi; //Kaymon
  temp_b_code[1]=temp_b_code[1]<<38;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[1]|=temp_int;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<9;  

  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 3:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=2 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=2 << 10;
#ifdef SET_NOP_NINE
	insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 2:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=4 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=4 << 10;

#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif
    case 1:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=8 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x0 << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x0 << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=8 << 10;
      MVR_instr(return_regi,remain_regi, 0x1b, 0x1b, 0, SV>3?15:SV>2?14:SV>1?12:8 , SV-1 ,2);//Kaymon add it
#ifdef SET_NOP_NINE
      //insert_nop_nine(2);
      insert_nop_nine(3); //Kaymon add it for the mvr_instr.
      //for(i=0;i<2*2-1;i++)
      for(i=0;i<2*3-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      //for(i=0;i<2;i++)
      for(i=0;i<3;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif
      break;
    default:
      perror("GL_Step(): SV is wrong.");
    }
  /*
#ifdef SET_NOP_NINE
  if(type==1)
    return (2+SV*2)*2-1;
  else
    return (1+SV*2)*2-1;
#else
  if(type==1)
    return (2+SV*2);
  else
    return (1+SV*2);
#endif
  */
#ifdef SET_NOP_NINE
  if(type==1)
    return (3+SV*2)*2-1;
  else
    return (2+SV*2)*2-1;
#else
  if(type==1)
    return (3+SV*2);
  else
    return (2+SV*2);
#endif
}

int GL_LessThan(int return_regi,
		int x_regi,
		int y_regi,
		int remain_regi,
		char VOrF,
		int mode, //mode = 0 => arguments is vector. mode = 1 => arguments is ivector.
		int SV,
		FILE *output_ass)
{
  int i;
  int instr_count=0;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"LESSTHAN_R%d_R%d_R%d_ass_file",return_regi,x_regi,y_regi);
      ass_file=fopen(temp_string,"w");
    }

  //assembly code
  sprintf(temp_string,"\tLDI R%d.1%d%d%d, 1\n",remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0);

  switch(SV)
    {
    case 4:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 2, 0001, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.wwww, R%d.wwww, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_LessThan(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVR R%d.0001, R0.xyzw, <%d>\n",temp_string,remain_regi,SV);
    case 3:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 2, 0010, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.zzzz, R%d.zzzz, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_LessThan(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVR R%d.0010, R0.xyzw, <%d>\n",temp_string,remain_regi,SV);
    case 2:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 2, 0100, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.yyyy, R%d.yyyy, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_LessThan(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVR R%d.0100, R0.xyzw, <%d>\n",temp_string,remain_regi,SV);
    case 1:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 2, 1000, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.xxxx, R%d.xxxx, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_LessThan(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVR R%d.1000, R0.xyzw, <%d>\n",temp_string,remain_regi,SV);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi,SV);
      break;
    default:
      perror("GL_LessThan(): SV is wrong.");
    }
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2*1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=2;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=1;
#endif


  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4; //change 2 to 4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=mode>0?0:1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#else
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4; //change 2 to 4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=mode>0?0:1; //compare with mode.
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#endif
    }
  else if(VOrF == 'f')
    {
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=2;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=mode>0?0:1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
    }
  else
    perror("GL_LessThan(): VOrF variable is wrong.");

  temp_b_code[1]=10;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<38;
  temp_b_code[1]|=0;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7; 

  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 3:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=2 << 10;
#else 
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=2 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 2:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=4 << 10;
#else 
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=4 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 1:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=8 << 10;
#else 
	temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
#endif
	}
      else
	temp_b_code[0]&=0xFFFFFFFF80007FFFLL;

      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=8 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
      break;
    default:
      perror("GL_LessThan(): SV is error");
      break;
    }
  MVR_instr( return_regi , remain_regi, 0X1b1b, 0X1b1b, 0 , 15 , 3 ,0);  //Kaymon add the last mvr instr.
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=1;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
  return instr_count;
}

int GL_GreaterThan(int return_regi,
		   int x_regi,
		   int y_regi,
		   int remain_regi,
		   char VOrF,
		   int mode, //mode = 0 => arguments is vector. mode = 1 => arguments is ivector.
		   int SV,
		   FILE *output_ass)
{
  int i;
  int instr_count=0;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"GREATERTHAN_R%d_R%d_R%d_ass_file",return_regi,x_regi,y_regi);
      ass_file=fopen(temp_string,"w");
    }

  //assembly code
  sprintf(temp_string,"\tLDI R%d.1%d%d%d, 1\n",remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0);

  switch(SV)
    {
    case 4:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 2, 0001, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBLTSF R%d.wwww, R%d.wwww, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_GreaterThan(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVR R%d.0001, R0.xyzw, <%d>\n",temp_string,remain_regi,SV);
    case 3:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 2, 0010, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBLTSF R%d.zzzz, R%d.zzzz, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_Greater(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVR R%d.0010, R0.xyzw, <%d>\n",temp_string,remain_regi,SV);
    case 2:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 2, 0100, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBLTSF R%d.yyyy, R%d.yyyy, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_GreaterThan(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVR R%d.0100, R0.xyzw, <%d>\n",temp_string,remain_regi,SV);
    case 1:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 2, 1000, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBLTSF R%d.xxxx, R%d.xxxx, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_GreaterThan(): VOrF is wrong.");
      sprintf(temp_string,"%s\tMVR R%d.1000, R0.xyzw, <%d>\n",temp_string,remain_regi,SV);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi,SV);
      break;
    default:
      perror("GL_GreaterThan(): SV is wrong.");
    }
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2*1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=2;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=1;
#endif


  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[0]=26;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=mode>0?0:1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#else
      temp_b_code[0]=26;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=mode>0?0:1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#endif
    }
  else if(VOrF == 'f')
    {
      temp_b_code[0]=26;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=2;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=mode>0?0:1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
    }
  else
    perror("GL_GreaterThan(): VOrF variable is wrong.");

  temp_b_code[1]=10;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<38;
  temp_b_code[1]|=0;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 3:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=2 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=2 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 2:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=4 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=4 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 1:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=8 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
#endif
	}
      else
	temp_b_code[0]&=0xFFFFFFFF80007FFFLL;

      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=8 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
      break;
    default:
      perror("GL_GreaterThan(): SV is error");
      break;
    }
  MVR_instr( return_regi , remain_regi, 0X1b1b, 0X1b1b, 0 , 15 , 3 ,0);  //Kaymon add the last mvr instr.
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=1;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
  return instr_count;
}

int GL_LessThanEqual(int return_regi,
		     int x_regi,
		     int y_regi,
		     int remain_regi,
		     char VOrF,
		     int mode, //mode = 0 => arguments is vector. mode = 1 => arguments is ivector.
		     int SV,
		     FILE *output_ass)
{
  int i;
  int instr_count=0;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"LESSTHANEQUAL_R%d_R%d_R%d_ass_file",return_regi,x_regi,y_regi);
      ass_file=fopen(temp_string,"w");
    }

  //assembly code
  sprintf(temp_string,"\tLDI R%d.1%d%d%d, 0\n",remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0);

  switch(SV)
    {
    case 4:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 2, 0001, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBLTSF R%d.wwww, R%d.wwww, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_LessThanEqual(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.0001, 1\n",temp_string,remain_regi);
    case 3:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 2, 0010, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBLTSF R%d.zzzz, R%d.zzzz, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_LessThanEqual(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.0010, 1\n",temp_string,remain_regi);
    case 2:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 2, 0100, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBLTSF R%d.yyyy, R%d.yyyy, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_LessThanEqual(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.0100, 1\n",temp_string,remain_regi);
    case 1:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBLTVF R%d, R%d, R%d, R%d, 2, 1000, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBLTSF R%d.xxxx, R%d.xxxx, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_LessThanEqual(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.1000, 1\n",temp_string,remain_regi);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi,SV);
      break;
    default:
      perror("GL_LessThanEqual(): SV is wrong.");
    }
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<43;
  temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2*1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=2;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=1;
#endif


  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[0]=26;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4; //insert nop instr.
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=mode>0?0:1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#else
      /************unified_branch*************/
      temp_b_code[0]=26;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4; //insert nop instr.
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=mode>0?0:1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#endif
    }
  else if(VOrF == 'f')
    {
      temp_b_code[0]=26;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=2;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=mode>0?0:1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
    }
  else
    perror("GL_LessThanEqual(): VOrF variable is wrong.");

  temp_b_code[1]=10;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<38;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 3:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=2 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=2 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 2:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=4 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=4 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 1:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=8 << 10;
#else
	temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
#endif
	}
      else
	temp_b_code[0]&=0xFFFFFFFF80007FFFLL;

      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=8 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
      break;
    default:
      perror("GL_LessThanEqual(): SV is error");
      break;
    }
  MVR_instr( return_regi , remain_regi, 0X1b1b, 0X1b1b, 0 , 15 , 3 ,0);  //Kaymon add the last mvr instr.
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=1;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
  return instr_count;
}

int GL_GreaterThanEqual(int return_regi,
			int x_regi,
			int y_regi,
			int remain_regi,
			char VOrF,
			int mode, //mode = 0 => arguments is vector. mode = 1 => arguments is ivector.
			int SV,
			FILE *output_ass)
{
  int i;
  int instr_count=0;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"GREATERTHANEQUAL_R%d_R%d_R%d_ass_file",return_regi,x_regi,y_regi);
      ass_file=fopen(temp_string,"w");
    }

  //assembly code
  sprintf(temp_string,"\tLDI R%d.1%d%d%d, 0\n",remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0);

  switch(SV)
    {
    case 4:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 2, 0001, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.wwww, R%d.wwww, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_GreaterThanEqual(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.0001, 1\n",temp_string,remain_regi);
    case 3:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 2, 0010, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.zzzz, R%d.zzzz, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_GreaterThanEqual(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.0010, 1\n",temp_string,remain_regi);
    case 2:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 2, 0100, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.yyyy, R%d.yyyy, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_GreaterThanEqual(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.0100, 1\n",temp_string,remain_regi);
    case 1:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBGTVF R%d, R%d, R%d, R%d, 2, 1000, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBGTSF R%d.xxxx, R%d.xxxx, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_GreaterThanEqual(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.1000, 1\n",temp_string,remain_regi);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi,SV);
      break;
    default:
      perror("GL_GreaterThanEqual(): SV is wrong.");
    }
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<43;
  temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2*1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=2;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=1;
#endif


  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=mode>0?0:1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#else
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=mode>0?0:1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#endif
    }
  else if(VOrF == 'f')
    {
      temp_b_code[0]=27;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=2;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=mode>0?0:1;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
    }
  else
    perror("GL_GreaterThanEqual(): VOrF variable is wrong.");

  temp_b_code[1]=10;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<38;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 3:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=2 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=2 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 2:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=4 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=4 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 1:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=8 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
#endif
	}
      else
	temp_b_code[0]&=0xFFFFFFFF80007FFFLL;

      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=8 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
      break;
    default:
      perror("GL_GreaterThanEqual(): SV is error");
      break;
    }
  MVR_instr( return_regi , remain_regi, 0X1b1b, 0X1b1b, 0 , 15 , 3 ,0);  //Kaymon add the last mvr instr.
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=1;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
  return instr_count;
}

int GL_Equal(int return_regi,
	     int x_regi,
	     int y_regi,
	     int remain_regi,
	     char VOrF,
	     int SV,
	     FILE *output_ass)
{
  int i;
  int instr_count=0;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"EQUAL_R%d_R%d_R%d_ass_file",return_regi,x_regi,y_regi);
      ass_file=fopen(temp_string,"w");
    }

  //assembly code
  sprintf(temp_string,"\tLDI R%d.1%d%d%d, 0\n",remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0);

  switch(SV)
    {
    case 4:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBNEV R%d, R%d, R%d, R%d, 2, 0001, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBNES R%d.wwww, R%d.wwww, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_Equal(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.0001, 1\n",temp_string,remain_regi);
    case 3:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBNEV R%d, R%d, R%d, R%d, 2, 0010, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBNES R%d.zzzz, R%d.zzzz, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_Equal(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.0010, 1\n",temp_string,remain_regi);
    case 2:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBNEV R%d, R%d, R%d, R%d, 2, 0100, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBNES R%d.yyyy, R%d.yyyy, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_Equal(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.0100, 1\n",temp_string,remain_regi);
    case 1:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBNEV R%d, R%d, R%d, R%d, 2, 1000, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBNES R%d.xxxx, R%d.xxxx, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_Equal(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.1000, 1\n",temp_string,remain_regi);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi,SV);
      break;
    default:
      perror("GL_Equal(): SV is wrong.");
    }
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);
  
  //machine code
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<43;
  temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2*1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=2;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=1;
#endif


  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[0]=25;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<3;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#else
      temp_b_code[0]=25;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#endif
    }
  else if(VOrF == 'f')
    {
      temp_b_code[0]=25;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=2;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
    }
  else
    perror("GL_Equal(): VOrF variable is wrong.");

  temp_b_code[1]=10;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<38;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 3:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=2 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=2 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 2:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=4 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=4 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 1:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=8 << 10;
#else
	temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
#endif
	}
      else
	temp_b_code[0]&=0xFFFFFFFF80007FFFLL;

      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=8 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
      break;
    default:
      perror("GL_Equal(): SV is error");
      break;
    }
  MVR_instr( return_regi , remain_regi, 0X1b1b, 0X1b1b, 0 , 15 , 3 ,0);  //Kaymon add the last mvr instr.
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=1;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
  return instr_count;
}

int GL_NotEqual(int return_regi,
		int x_regi,
		int y_regi,
		int remain_regi,
		char VOrF,
		int SV,
		FILE *output_ass)
{
  int i;
  int instr_count=0;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"NOTEQUAL_R%d_R%d_R%d_ass_file",return_regi,x_regi,y_regi);
      ass_file=fopen(temp_string,"w");
    }

  //assembly code
  sprintf(temp_string,"\tLDI R%d.1%d%d%d, 0\n",remain_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0);

  switch(SV)
    {
    case 4:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 0001, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBEQS R%d.wwww, R%d.wwww, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_NotEqual(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.0001, 1\n",temp_string,remain_regi);
    case 3:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 0010, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBEQS R%d.zzzz, R%d.zzzz, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_NotEqual(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.0010, 1\n",temp_string,remain_regi);
    case 2:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 0100, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBEQS R%d.yyyy, R%d.yyyy, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_NotEqual(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.0100, 1\n",temp_string,remain_regi);
    case 1:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBEQV R%d, R%d, R%d, R%d, 2, 1000, <%d>\n",temp_string,y_regi,x_regi,y_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBEQS R%d.xxxx, R%d.xxxx, 2, <%d>\n",temp_string,y_regi,x_regi,SV);
      else
	perror("GL_NotEqual(): VOrF is wrong.");
      sprintf(temp_string,"%s\tLDI R%d.1000, 1\n",temp_string,remain_regi);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi,SV);
      break;
    default:
      perror("GL_NotEqual(): SV is wrong.");
    }
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);
  
  //machine code
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<43;
  temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2*1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=2;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=1;
#endif


  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[0]=24;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<3;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#else
      temp_b_code[0]=24;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif      
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#endif
    }
  else if(VOrF == 'f')
    {
      temp_b_code[0]=24;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=y_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=2;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
    }
  else
    perror("GL_NotEqual(): VOrF variable is wrong.");

  temp_b_code[1]=10;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<38;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 3:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=2 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=2 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 2:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=4 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
	}
      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=4 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
    case 1:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFFFFFFFFFFC3FFLL;
	  temp_b_code[0]|=8 << 10;
#else
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
#endif
	}
      else
	temp_b_code[0]&=0xFFFFFFFF80007FFFLL;

      temp_b_code[1]&=0xFFFFFFFFFFFFC3FFLL;
      temp_b_code[1]|=8 << 10;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
      break;
    default:
      perror("GL_NotEqual(): SV is error");
      break;
    }
  MVR_instr( return_regi , remain_regi, 0X1b1b, 0X1b1b, 0 , 15 , 3 ,0);  //Kaymon add the last mvr instr.
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=1;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
  return instr_count;
}

int GL_Any(int return_regi,
	   int x_regi,
	   int remain_regi,
	   char VOrF,
	   int SV,
	   FILE *output_ass)
{
  int i;
  int instr_count=0;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"ANY_R%d_R%d_ass_file",return_regi,x_regi);
      ass_file=fopen(temp_string,"w");
    }

  //assembly code
#ifndef Opt_any
  sprintf(temp_string,"\tLDI R%d.1000, 1\n",remain_regi);

  switch(SV)
    {
    case 4:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBNEV R0, R%d, R0, R%d, 5, 0001, <%d>\n",temp_string,x_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBNES R0.wwww, R%d.wwww, 5, <%d>\n",temp_string,x_regi,SV);
      else
	perror("GL_Any(): VOrF is wrong.");
    case 3:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBNEV R0, R%d, R0, R%d, 4, 0010, <%d>\n",temp_string,x_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBNES R0.zzzz, R%d.zzzz, 4, <%d>\n",temp_string,x_regi,SV);
      else
	perror("GL_Any(): VOrF is wrong.");
    case 2:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBNEV R0, R%d, R0, R%d, 3, 0100, <%d>\n",temp_string,x_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBNES R0.yyyy, R%d.yyyy, 3, <%d>\n",temp_string,x_regi,SV);
      else
	perror("GL_Any(): VOrF is wrong.");
    case 1:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBNEV R0, R%d, R0, R%d, 2, 1000, <%d>\n",temp_string,x_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBNES R0.xxxx, R%d.xxxx, 2, <%d>\n",temp_string,x_regi,SV);
      else
	perror("GL_Any(): VOrF is wrong.");

      sprintf(temp_string,"%s\tLDI R%d.1000, 0\n",temp_string,remain_regi);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi,SV);
      break;
    default:
      perror("GL_Any(): SV is wrong.");
    }
#else
  sprintf(temp_string,"\tLDI R%d.1000, 0\n",remain_regi);
  sprintf(temp_string,"%s\tBEQ R0, R%d, R0, R%d, 2, 1%d%d%d, <%d>\n",temp_string,x_regi,x_regi,SV>1,SV>2,SV>3,SV);
  sprintf(temp_string,"%s\tLDI R%d.1000, 1\n",temp_string,remain_regi);
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi,SV);
#endif
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

#ifndef Opt_any
  //machine code
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2*1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=2;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=1;
#endif


  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[0]=25;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=10;
#else
      temp_b_code[0]|=5;
#endif
      temp_b_code[0]=temp_b_code[0]<<11;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<3;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#else
      temp_b_code[0]=25;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=10;
#else
      temp_b_code[0]|=5;
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#endif
    }
  else if(VOrF == 'f')
    {
      temp_b_code[0]=25;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=5;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
    }
  else
    perror("GL_Any(): VOrF variable is wrong.");

  temp_b_code[1]=10;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<38;
  temp_b_code[1]|=0;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=8;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=2;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
    case 3:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFF80007FFFC3FFLL;
#ifdef SET_NOP_NINE
	  temp_b_code[0]|=(long long unsigned)8 << 31;
	  temp_b_code[0]|=2 << 10;
#else
	  temp_b_code[0]|=(long long unsigned)4 << 31;
	  temp_b_code[0]|=2 << 10;
#endif
#else
	  temp_b_code[0]&=0xFFFF800000007FFFLL;
#ifdef SET_NOP_NINE
	  temp_b_code[0]|=(long long unsigned)8<<31;
	  temp_b_code[0]|=0xaaaa << 15;
#else
	  temp_b_code[0]|=(long long unsigned)4 << 31;
	  temp_b_code[0]|=0xaaaa << 15;
#endif
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
	}

#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=2;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
    case 2:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFF80007FFFC3FFLL;
#ifdef SET_NOP_NINE
	  temp_b_code[0]|=(long long unsigned)6<<31;
	  temp_b_code[0]|=4 << 10;
#else
	  temp_b_code[0]|=(long long unsigned)3 << 31;
	  temp_b_code[0]|=4 << 10;
#endif
#else
	  temp_b_code[0]&=0xFFFF800000007FFFLL;
#ifdef SET_NOP_NINE
	  temp_b_code[0]|=(long long unsigned)6<<31;
	  temp_b_code[0]|=0x5555 << 15;
#else
	  temp_b_code[0]|=(long long unsigned)3 << 31;
	  temp_b_code[0]|=0x5555 << 15;
#endif
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
	}
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=2;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
    case 1:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFF80007FFFC3FFLL;
#ifdef SET_NOP_NINE
	  temp_b_code[0]|=(long long unsigned)4 << 31;
	  temp_b_code[0]|=8 << 10;
#else
	  temp_b_code[0]|=(long long unsigned)2 << 31;
	  temp_b_code[0]|=8 << 10;
#endif
#else
	  temp_b_code[0]&=0xFFFF800000007FFFLL;
#ifdef SET_NOP_NINE
	  temp_b_code[0]|=(long long unsigned)4<<31;
#else
	  temp_b_code[0]|=(long long unsigned)2 << 31;
#endif
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFF800000007FFFLL;
	  temp_b_code[0]|=2 << 31;
	}

#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
      break;
    default:
      perror("GL_Any(): SV is error");
      break;
      }

  MVR_instr( return_regi , remain_regi, 0X1b1b, 0X1b1b, 0 , 15 , 3 ,0);  //Kaymon add the last mvr instr.
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=1;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
#else //Opt_any
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2*1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=2;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=1;
#endif


  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[0]=24; //Use 1 beq instruction instead of 4 bne instructions.
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<11;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
      temp_b_code[0]=temp_b_code[0]<<3;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#else
      temp_b_code[0]=24;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=SV>3?0x1b1b:SV>2?0x1818:SV>1?0x1111:0;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#endif
    }
  else if(VOrF == 'f')
    {
      temp_b_code[0]=24;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=2;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0x1b1b;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
    }
  else
    perror("GL_Any(): VOrF variable is wrong.");

  temp_b_code[1]=10;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<38;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=8;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;
#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif

  MVR_instr( return_regi , remain_regi, 0X1b1b, 0X1b1b, 0 , 15 , 3 ,0);  //Kaymon add the last mvr instr.
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=1;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
#endif
  return instr_count;
}

int GL_All(int return_regi,
	   int x_regi,
	   int remain_regi,
	   char VOrF,
	   int SV,
	   FILE *output_ass)
{
  int i;
  int instr_count=0;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"ALL_R%d_R%d_ass_file",return_regi,x_regi);
      ass_file=fopen(temp_string,"w");
    }
#ifndef Opt_all
  //assembly code
  sprintf(temp_string,"\tLDI R%d.1000, 0\n",remain_regi);

  switch(SV)
    {
    case 4:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBEQV R0, R%d, R0, R%d, 5, 0001, <%d>\n",temp_string,x_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBEQS R0.wwww, R%d.wwww, 5, <%d>\n",temp_string,x_regi,SV);
      else
	perror("GL_All(): VOrF is wrong.");
    case 3:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBEQV R0, R%d, R0, R%d, 4, 0010, <%d>\n",temp_string,x_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBEQS R0.zzzz, R%d.zzzz, 4, <%d>\n",temp_string,x_regi,SV);
      else
	perror("GL_All(): VOrF is wrong.");
    case 2:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBEQV R0, R%d, R0, R%d, 3, 0100, <%d>\n",temp_string,x_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBEQS R0.yyyy, R%d.yyyy, 3, <%d>\n",temp_string,x_regi,SV);
      else
	perror("GL_All(): VOrF is wrong.");
    case 1:
      if(VOrF == 'v')
	sprintf(temp_string,"%s\tBEQV R0, R%d, R0, R%d, 2, 1000, <%d>\n",temp_string,x_regi,x_regi,SV);
      else if(VOrF == 'f')
	sprintf(temp_string,"%s\tBEQS R0.xxxx, R%d.xxxx, 2, <%d>\n",temp_string,x_regi,SV);
      else
	perror("GL_All(): VOrF is wrong.");

      sprintf(temp_string,"%s\tLDI R%d.1000, 1\n",temp_string,remain_regi);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_regi,remain_regi,SV);
      break;
    default:
      perror("GL_All(): SV is wrong.");
    }
#else
  sprintf(temp_string,"\tLDI R%d.1000, 1\n",remain_regi);
  sprintf(temp_string,"%s\tBNE R0, R%d, R0, R%d, 2, 1%d%d%d, <%d>\n",temp_string,x_regi,x_regi,SV>1,SV>2,SV>3,SV);
  sprintf(temp_string,"%s\tLDI R%d.1000, 0\n",temp_string,remain_regi);
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xxxx, <%d>\n",temp_string,return_regi,remain_regi,SV);
#endif
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);
#ifndef Opt_all  
  //machine code
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  temp_b_code[0]|=0;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2*1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=2;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=1;
#endif


  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[0]=24;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=10;
#else
      temp_b_code[0]|=5;
#endif
      temp_b_code[0]=temp_b_code[0]<<11;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<3;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#else
      temp_b_code[0]=24;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=10;
#else
      temp_b_code[0]|=5;
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#endif
    }
  else if(VOrF == 'f')
    {
      temp_b_code[0]=24;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=3;
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=0xFFFF;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
    }
  else
    perror("GL_All(): VOrF variable is wrong.");

  temp_b_code[1]=10;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<38;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=8;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  switch(SV)
    {
    case 4:
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=2;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
    case 3:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFF80007FFFC3FFLL;
#ifdef SET_NOP_NINE
	  temp_b_code[0]|=(long long unsigned)8<<31;
	  temp_b_code[0]|=2 << 10;
#else
	  temp_b_code[0]|=(long long unsigned)4 << 31;
	  temp_b_code[0]|=2 << 10;
#endif
#else
	  temp_b_code[0]&=0xFFFF800000007FFFLL;
#ifdef SET_NOP_NINE
	  temp_b_code[0]|=(long long unsigned)8<<31;
	  temp_b_code[0]|=0xaaaa << 15;
#else
	  temp_b_code[0]|=(long long unsigned)4 << 31;
	  temp_b_code[0]|=0xaaaa << 15;
#endif
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0xaaaa << 15;
	}

#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=2;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
    case 2:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFF80007FFFC3FFLL;
#ifdef SET_NOP_NINE
	  temp_b_code[0]|=(long long unsigned)6<<31;
	  temp_b_code[0]|=4 << 10;
#else
	  temp_b_code[0]|=(long long unsigned)3 << 31;
	  temp_b_code[0]|=4 << 10;
#endif
#else
	  temp_b_code[0]&=0xFFFF800000007FFFLL;
#ifdef SET_NOP_NINE
	  temp_b_code[0]|=(long long unsigned)6<<31;
	  temp_b_code[0]|=0x5555 << 15;
#else
	  temp_b_code[0]|=(long long unsigned)3 << 31;
	  temp_b_code[0]|=0x5555 << 15;
#endif
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFFFFFF80007FFFLL;
	  temp_b_code[0]|=0x5555 << 15;
	}
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=2;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
    case 1:
      if(VOrF == 'v')
	{
#ifndef unified_branch
	  temp_b_code[0]&=0xFFFF80007FFFC3FFLL;
#ifdef SET_NOP_NINE
	  temp_b_code[0]|=(long long unsigned)4<<31;
	  temp_b_code[0]|=8 << 10;
#else
	  temp_b_code[0]|=(long long unsigned)2 << 31;
	  temp_b_code[0]|=8 << 10;
#endif
#else
	  temp_b_code[0]&=0xFFFF800000007FFFLL;
#ifdef SET_NOP_NINE
	  temp_b_code[0]|=(long long unsigned)4<<31;
#else
	  temp_b_code[0]|=(long long unsigned)2 << 31;
#endif
#endif
	}
      else
	{
	  temp_b_code[0]&=0xFFFF800000007FFFLL;
	  temp_b_code[0]|=2 << 31;
	}

#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif
      break;
    default:
      perror("GL_All(): SV is error");
      break;
    }
  MVR_instr( return_regi , remain_regi, 0X1b1b, 0X1b1b, 0 , 15 , 3 ,0);  //Kaymon add the last mvr instr.
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=1;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
#else //Opt_all
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<3;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<2*1;i++)
    BinCode[LastBinLine++]=temp_b_code_NOP[i];
  instr_count+=2;
#else
  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
  instr_count+=1;
#endif

  if(VOrF == 'v')
    {
#ifndef unified_branch
      temp_b_code[0]=25;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<11;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[0]|=SV>3?15:SV>2?14:SV>1?12:8;
      temp_b_code[0]=temp_b_code[0]<<3;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#else
      temp_b_code[0]=25;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      temp_b_code[0]|=SV>3?0x1b1b:SV>2?0x1818:SV>1?0x1111:0;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
#endif
    }
  else if(VOrF == 'f')
    {
      temp_b_code[0]=25;
      temp_b_code[0]=temp_b_code[0]<<12;
      temp_b_code[0]|=x_regi;
      temp_b_code[0]=temp_b_code[0]<<16;
#ifdef SET_NOP_NINE
      temp_b_code[0]|=4;
#else
      temp_b_code[0]|=2;
#endif
      temp_b_code[0]=temp_b_code[0]<<16;
      if(SV==4)
	temp_b_code[0]|=0x1b1b;
      else if(SV==3)
       	temp_b_code[0]|=0x1818;
      else if(SV==2)
	temp_b_code[0]|=0x0505;
      else
	temp_b_code[0]|=0x0000;
      temp_b_code[0]=temp_b_code[0]<<1;
      temp_b_code[0]|=1;
      temp_b_code[0]=temp_b_code[0]<<7;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
    }
  else
    perror("GL_All(): VOrF variable is wrong.");

  temp_b_code[1]=10;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<38;
  temp_b_code[1]|=0;
  temp_b_code[1]=temp_b_code[1]<<5;
  temp_b_code[1]|=8;
  temp_b_code[1]=temp_b_code[1]<<3;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

#ifdef SET_NOP_NINE
      insert_nop_nine(2);
      for(i=0;i<2*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=4;
#else
      for(i=0;i<2;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=2;
#endif

  MVR_instr( return_regi , remain_regi, 0X1b1b, 0X1b1b, 0 , 15 , 3 ,0);  //Kaymon add the last mvr instr.
#ifdef SET_NOP_NINE
      insert_nop_nine(1);
      for(i=0;i<1*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
      instr_count+=1;
#else
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
      instr_count+=1;
#endif
#endif
  return instr_count;
}

int GL_Not(int return_regi,
	   int x_regi,
	   int remain_regi,
	   char VOrF,
	   int SV,
	   FILE *output_ass)
{
  GL_Equal(return_regi,x_regi,0,remain_regi,VOrF,SV,output_ass);
}

int loadFromIMemory(int store_regi,int remain_regi,int vector_size,int address,int row_major_to_column_major,int start_address,FILE *output_ass)
{

  //assembly code

  int i,imm_num=0;
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"LD_for_%d(vector%d)_ass_file",store_regi,vector_size);
  ass_file=fopen(temp_string,"w");
#endif

  //sprintf(temp_string,"\tLDI R%d, 0x%x\n",remain_regi,address);//(No this instruction. Need to fix)\n",remain_regi,address);
  //fputs(temp_string,ass_file);
  //bzero(temp_string,sizeof(temp_string));

#ifdef LD_BY_ONE
  for(i=0;i<vector_size;i++)
    {
      if(vector_size==4 && row_major_to_column_major==1)
	sprintf(temp_string,"\tLD R%d, R%d, %d\n",store_regi+i,remain_regi,i+start_address);
      else
	sprintf(temp_string,"\tLD R%d, R%d, %d\n",store_regi+i,remain_regi,imm_num+start_address);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      if(vector_size==4 && row_major_to_column_major==1)
	sprintf(temp_string,"\tLD R%d.0100, R%d, %d\n",store_regi+i,remain_regi,4+i+start_address);
      else
	sprintf(temp_string,"\tLD R%d.0100, R%d, %d\n",store_regi+i,remain_regi,imm_num+1+start_address);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      if(vector_size==4 && row_major_to_column_major==1)
	sprintf(temp_string,"\tLD R%d.0010, R%d, %d\n",store_regi+i,remain_regi,8+i+start_address);
      else
	sprintf(temp_string,"\tLD R%d.0010, R%d, %d\n",store_regi+i,remain_regi,imm_num+2+start_address);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      if(vector_size==4 && row_major_to_column_major==1)
	sprintf(temp_string,"\tLD R%d.0001, R%d, %d\n",store_regi+i,remain_regi,12+i+start_address);
      else
	sprintf(temp_string,"\tLD R%d.0001, R%d, %d\n",store_regi+i,remain_regi,imm_num+3+start_address);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      imm_num+=4;

  fflush(ass_file);

    }
#endif
#ifndef LD_BY_ONE
  for(i=0;i<vector_size;i++)
    {
      if(vector_size==4 && row_major_to_column_major==1)
        sprintf(temp_string,"\tLD R%d, R%d, %d\n",store_regi+i,remain_regi,i*4+start_address);
      else
        sprintf(temp_string,"\tLD R%d, R%d, %d\n",store_regi+i,remain_regi,imm_num+start_address);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      if(vector_size==4 && row_major_to_column_major==1)
        sprintf(temp_string,"\tLD R%d.0100, R%d, %d\n",store_regi+i,remain_regi,16+i*4+start_address);
      else
        sprintf(temp_string,"\tLD R%d.0100, R%d, %d\n",store_regi+i,remain_regi,imm_num+4+start_address);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      if(vector_size==4 && row_major_to_column_major==1)
        sprintf(temp_string,"\tLD R%d.0010, R%d, %d\n",store_regi+i,remain_regi,32+i*4+start_address);
      else
        sprintf(temp_string,"\tLD R%d.0010, R%d, %d\n",store_regi+i,remain_regi,imm_num+8+start_address);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      if(vector_size==4 && row_major_to_column_major==1)
        sprintf(temp_string,"\tLD R%d.0001, R%d, %d\n",store_regi+i,remain_regi,48+i*4+start_address);
      else
        sprintf(temp_string,"\tLD R%d.0001, R%d, %d\n",store_regi+i,remain_regi,imm_num+12+start_address);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      imm_num+=16;
    }

  fflush(ass_file);

#endif
#endif

  //machine code

  //Need to fix

  //temp_b_code[0]=10;
  //temp_b_code[0]=temp_b_code[0]<<6;
  //temp_b_code[0]|=remain_regi;
  //temp_b_code[0]=temp_b_code[0]<<38;
  //temp_b_code[0]|=address;
  //temp_b_code[0]=temp_b_code[0]<<5;
  //temp_b_code[0]|=8;
  //temp_b_code[0]=temp_b_code[0]<<10;
  //fwrite(temp_b_code,sizeof(long long),1,output_ass);

  //temp_b_code[0]=10;
  //temp_b_code[0]=temp_b_code[0]<<6;
  //temp_b_code[0]|=store_regi;
  //temp_b_code[0]=temp_b_code[0]<<38;
  //temp_b_code[0]|=address;
  //temp_b_code[0]=temp_b_code[0]<<5;
  //temp_b_code[0]|=8;
  //temp_b_code[0]=temp_b_code[0]<<10;

  //fwrite(temp_b_code,sizeof(long long),1,output_ass);





#ifdef LD_BY_ONE
  temp_b_code[0]=8;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=store_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<32;
  temp_b_code[0]|=start_address;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<10;
  temp_b_code[1]=temp_b_code[0];
  temp_b_code[1]&=(long long unsigned)0xFFFF8000000043FFLL;
  temp_b_code[2]=temp_b_code[1];
  temp_b_code[3]=temp_b_code[1];
  if(vector_size==4 && row_major_to_column_major==1)
    temp_b_code[1]|=(long long unsigned)((4+start_address)<<15);
  else
    temp_b_code[1]|=(long long unsigned)((1+start_address)<<15);
  temp_b_code[1]|=(long long unsigned)(4<<10);
  if(vector_size==4 && row_major_to_column_major==1)
    temp_b_code[2]|=(long long unsigned)((8+start_address)<<15);
  else
    temp_b_code[2]|=(long long unsigned)((2+start_address)<<15);
  temp_b_code[2]|=(long long unsigned)(2<<10);
  if(vector_size==4 && row_major_to_column_major==1)
    temp_b_code[3]|=(long long unsigned)((12+start_address)<<15);
  else
    temp_b_code[3]|=(long long unsigned)((3+start_address)<<15);
  temp_b_code[3]|=(long long unsigned)(1<<10);
  imm_num=4;

  for(i=1;i<vector_size;i++)
    {
      temp_b_code[i*4]=temp_b_code[0];
      temp_b_code[i*4]&=(long long unsigned)0xF81F800000007FFFLL;
      temp_b_code[i*4]|=(long long unsigned)(store_regi+i)<<53;
      temp_b_code[i*4+1]=temp_b_code[1];
      temp_b_code[i*4+1]&=(long long unsigned)0xF81F800000007FFFLL;
      temp_b_code[i*4+1]|=(long long unsigned)(store_regi+i)<<53;
      temp_b_code[i*4+2]=temp_b_code[2];
      temp_b_code[i*4+2]&=(long long unsigned)0xF81F800000007FFFLL;
      temp_b_code[i*4+2]|=(long long unsigned)(store_regi+i)<<53;
      temp_b_code[i*4+3]=temp_b_code[3];
      temp_b_code[i*4+3]&=(long long unsigned)0xF81F800000007FFFLL;
      temp_b_code[i*4+3]|=(long long unsigned)(store_regi+i)<<53;
      if(vector_size==4 && row_major_to_column_major==1)
	{
	  temp_b_code[i*4]|=(long long unsigned)(i+start_address)<<15;
	  temp_b_code[i*4+1]|=(long long unsigned)(i+4+start_address)<<15;
	  temp_b_code[i*4+2]|=(long long unsigned)(i+8+start_address)<<15;
	  temp_b_code[i*4+3]|=(long long unsigned)(i+12+start_address)<<15;
	}
      else
	{
	  temp_b_code[i*4]|=(long long unsigned)(imm_num+start_address)<<15;
          temp_b_code[i*4+1]|=(long long unsigned)(imm_num+1+start_address)<<15;
          temp_b_code[i*4+2]|=(long long unsigned)(imm_num+2+start_address)<<15;
          temp_b_code[i*4+3]|=(long long unsigned)(imm_num+3+start_address)<<15;
	}
      imm_num+=4;
    }

#endif
#ifndef LD_BY_ONE  
  temp_b_code[0]=8;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=store_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<32;
  temp_b_code[0]|=start_address;
  temp_b_code[0]=temp_b_code[0]<<5;
  temp_b_code[0]|=8;
  temp_b_code[0]=temp_b_code[0]<<10;
  temp_b_code[1]=temp_b_code[0];
  temp_b_code[1]&=(long long unsigned)0xFFFF8000000043FFLL;
  temp_b_code[2]=temp_b_code[1];
  temp_b_code[3]=temp_b_code[1];
  if(vector_size==4 && row_major_to_column_major==1)
    temp_b_code[1]|=(long long unsigned)((start_address+16)<<15);
  else
    temp_b_code[1]|=(long long unsigned)((start_address+4)<<15);
  temp_b_code[1]|=(long long unsigned)(4<<10);
  if(vector_size==4 && row_major_to_column_major==1)
    temp_b_code[2]|=(long long unsigned)((start_address+32)<<15);
  else
    temp_b_code[2]|=(long long unsigned)((start_address+8)<<15);
  temp_b_code[2]|=(long long unsigned)(2<<10);
  if(vector_size==4 && row_major_to_column_major==1)
    temp_b_code[3]|=(long long unsigned)((start_address+48)<<15);
  else
    temp_b_code[3]|=(long long unsigned)((start_address+12)<<15);
  temp_b_code[3]|=(long long unsigned)(1<<10);
  imm_num=16;

  for(i=1;i<vector_size;i++)
    {
      temp_b_code[i*4]=temp_b_code[0];
      temp_b_code[i*4]&=(long long unsigned)0xF81F800000007FFFLL;
      temp_b_code[i*4]|=(long long unsigned)(store_regi+i)<<53;
      temp_b_code[i*4+1]=temp_b_code[1];
      temp_b_code[i*4+1]&=(long long unsigned)0xF81F800000007FFFLL;
      temp_b_code[i*4+1]|=(long long unsigned)(store_regi+i)<<53;
      temp_b_code[i*4+2]=temp_b_code[2];
      temp_b_code[i*4+2]&=(long long unsigned)0xF81F800000007FFFLL;
      temp_b_code[i*4+2]|=(long long unsigned)(store_regi+i)<<53;
      temp_b_code[i*4+3]=temp_b_code[3];
      temp_b_code[i*4+3]&=(long long unsigned)0xF81F800000007FFFLL;
      temp_b_code[i*4+3]|=(long long unsigned)(store_regi+i)<<53;
      if(vector_size==4 && row_major_to_column_major==1)
	{
	  temp_b_code[i*4]|=(long long unsigned)(4*i+start_address)<<15;
	  temp_b_code[i*4+1]|=(long long unsigned)(4*i+16+start_address)<<15;
	  temp_b_code[i*4+2]|=(long long unsigned)(4*i+32+start_address)<<15;
	  temp_b_code[i*4+3]|=(long long unsigned)(4*i+48+start_address)<<15;
	}
      else
	{
	  temp_b_code[i*4]|=(long long unsigned)(imm_num+start_address)<<15;
          temp_b_code[i*4+1]|=(long long unsigned)(imm_num+4+start_address)<<15;
          temp_b_code[i*4+2]|=(long long unsigned)(imm_num+8+start_address)<<15;
          temp_b_code[i*4+3]|=(long long unsigned)(imm_num+12+start_address)<<15;
	}
      imm_num+=16;
    }
  
#endif

  for(i=0;i<(4*vector_size);i++)
      all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4*vector_size,output_ass);
#endif
  return 4*vector_size;
}

int writeToRegister(char *str,int regi,FILE *ass_file,FILE *output_ass)
{
  int i;

  //assembly code

  if(strcmp(str,"gl_ProjectionMatrix")==0)
    {
#ifdef output_ass_code

      sprintf(temp_string,"\tLDI R%d, \n",regi);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0100, \n",regi);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0010, \n",regi);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0001, \n",regi);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d, \n",regi+1);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0100, \n",regi+1);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0010, \n",regi+1);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0001, \n",regi+1);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d, \n",regi+2);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0100, \n",regi+2);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0010, \n",regi+2);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0001, \n",regi+2);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d, \n",regi+3);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0100, \n",regi+3);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0010, \n",regi+3);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0001, \n",regi+3);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      
      fflush(ass_file);

#endif

  //machine code

      temp_b_code[0]=10;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regi;
      temp_b_code[0]=temp_b_code[0]<<38;
      temp_b_code[0]|=0;//value
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=8;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[1]|=(long long unsigned)(0<<15);//value
      temp_b_code[1]|=(4<<10);
      temp_b_code[2]=temp_b_code[0];
      temp_b_code[2]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[2]|=(long long unsigned)(0<<15);//value
      temp_b_code[2]|=(2<<10);
      temp_b_code[3]=temp_b_code[0];
      temp_b_code[3]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[3]|=(long long unsigned)(0<<15);//value
      temp_b_code[3]|=(1<<10);
      temp_b_code[4]=temp_b_code[0];
      temp_b_code[4]&=(long long unsigned)0xF81F800000007FFFLL;
      temp_b_code[4]|=((long long unsigned)(regi+1)<<53);
      temp_b_code[4]|=(long long unsigned)(0<<15);//value
      temp_b_code[5]=temp_b_code[4];
      temp_b_code[5]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[5]|=(long long unsigned)(0<<15);//value
      temp_b_code[5]|=(4<<10);
      temp_b_code[6]=temp_b_code[4];
      temp_b_code[6]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[6]|=(long long unsigned)(0<<15);//value
      temp_b_code[6]|=(2<<10);
      temp_b_code[7]=temp_b_code[4];
      temp_b_code[7]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[7]|=(long long unsigned)(0<<15);//value
      temp_b_code[7]|=(1<<10);
      temp_b_code[8]=temp_b_code[0];
      temp_b_code[8]&=(long long unsigned)0xF81F800000007FFFLL;
      temp_b_code[8]|=((long long unsigned)(regi+2)<<53);
      temp_b_code[8]|=(long long unsigned)(0<<15);//value
      temp_b_code[9]=temp_b_code[8];
      temp_b_code[9]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[9]|=(long long unsigned)(0<<15);//value
      temp_b_code[9]|=(4<<10);
      temp_b_code[10]=temp_b_code[8];
      temp_b_code[10]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[10]|=(long long unsigned)(0<<15);//value
      temp_b_code[10]|=(2<<10);
      temp_b_code[11]=temp_b_code[8];
      temp_b_code[11]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[11]|=(long long unsigned)(0<<15);//value
      temp_b_code[11]|=(1<<10);
      temp_b_code[12]=temp_b_code[0];
      temp_b_code[12]&=(long long unsigned)0xF81F800000007FFFLL;
      temp_b_code[12]|=((long long unsigned)(regi+3)<<53);
      temp_b_code[12]|=(long long unsigned)(0<<15);//value
      temp_b_code[13]=temp_b_code[12];
      temp_b_code[13]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[13]|=(long long unsigned)(0<<15);//value
      temp_b_code[13]|=(4<<10);
      temp_b_code[14]=temp_b_code[12];
      temp_b_code[14]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[14]|=(long long unsigned)(0<<15);//value
      temp_b_code[14]|=(2<<10);
      temp_b_code[15]=temp_b_code[12];
      temp_b_code[15]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[15]|=(long long unsigned)(0<<15);//value
      temp_b_code[15]|=(1<<10);

      for(i=0;i<16;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),16,output_ass);
#endif
      return 16;
    }
  //assembly code
  if(strcmp(str,"gl_ModelViewMatrix")==0)
    {
#ifdef output_ass_code

      sprintf(temp_string,"\tLDI R%d, \n",regi);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0100, \n",regi);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0010, \n",regi);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0001, \n",regi);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d, \n",regi+1);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0100, \n",regi+1);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0010, \n",regi+1);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0001, \n",regi+1);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d, \n",regi+2);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0100, \n",regi+2);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0010, \n",regi+2);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0001, \n",regi+2);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d, \n",regi+3);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0100, \n",regi+3);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0010, \n",regi+3);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      sprintf(temp_string,"\tLDI R%d.0001, \n",regi+3);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));

      fflush(ass_file);

#endif

  //machine code

      temp_b_code[0]=10;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regi;
      temp_b_code[0]=temp_b_code[0]<<38;
      temp_b_code[0]|=0;//value
      temp_b_code[0]=temp_b_code[0]<<5;
      temp_b_code[0]|=8;
      temp_b_code[0]=temp_b_code[0]<<10;
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[1]|=(long long unsigned)(0<<15);//value
      temp_b_code[1]|=(4<<10);
      temp_b_code[2]=temp_b_code[0];
      temp_b_code[2]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[2]|=(long long unsigned)(0<<15);//value
      temp_b_code[2]|=(2<<10);
      temp_b_code[3]=temp_b_code[0];
      temp_b_code[3]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[3]|=(long long unsigned)(0<<15);//value
      temp_b_code[3]|=(1<<10);
      temp_b_code[4]=temp_b_code[0];
      temp_b_code[4]&=(long long unsigned)0xF81F800000007FFFLL;
      temp_b_code[4]|=((long long unsigned)(regi+1)<<53);
      temp_b_code[4]|=(long long unsigned)(0<<15);//value
      temp_b_code[5]=temp_b_code[4];
      temp_b_code[5]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[5]|=(long long unsigned)(0<<15);//value
      temp_b_code[5]|=(4<<10);
      temp_b_code[6]=temp_b_code[4];
      temp_b_code[6]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[6]|=(long long unsigned)(0<<15);//value
      temp_b_code[6]|=(2<<10);
      temp_b_code[7]=temp_b_code[4];
      temp_b_code[7]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[7]|=(long long unsigned)(0<<15);//value
      temp_b_code[7]|=(1<<10);
      temp_b_code[8]=temp_b_code[0];
      temp_b_code[8]&=(long long unsigned)0xF81F800000007FFFLL;
      temp_b_code[8]|=((long long unsigned)(regi+2)<<53);
      temp_b_code[8]|=(long long unsigned)(0<<15);//value
      temp_b_code[9]=temp_b_code[8];
      temp_b_code[9]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[9]|=(long long unsigned)(0<<15);//value
      temp_b_code[9]|=(4<<10);
      temp_b_code[10]=temp_b_code[8];
      temp_b_code[10]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[10]|=(long long unsigned)(0<<15);//value
      temp_b_code[10]|=(2<<10);
      temp_b_code[11]=temp_b_code[8];
      temp_b_code[11]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[11]|=(long long unsigned)(0<<15);//value
      temp_b_code[11]|=(1<<10);
      temp_b_code[12]=temp_b_code[0];
      temp_b_code[12]&=(long long unsigned)0xF81F800000007FFFLL;
      temp_b_code[12]|=((long long unsigned)(regi+3)<<53);
      temp_b_code[12]|=(long long unsigned)(0<<15);//value
      temp_b_code[13]=temp_b_code[12];
      temp_b_code[13]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[13]|=(long long unsigned)(0<<15);//value
      temp_b_code[13]|=(4<<10);
      temp_b_code[14]=temp_b_code[12];
      temp_b_code[14]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[14]|=(long long unsigned)(0<<15);//value
      temp_b_code[14]|=(2<<10);
      temp_b_code[15]=temp_b_code[12];
      temp_b_code[15]&=(long long unsigned)0xFFFF8000000043FFLL;
      temp_b_code[15]|=(long long unsigned)(0<<15);//value
      temp_b_code[15]|=(1<<10);

      for(i=0;i<16;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),16,output_ass);
#endif
      return 16;
 
    }
  perror("writeToRegister error!!");
  
}


int GL_MatrixTimesVector(int return_reg,
			 int matrix_reg,
			 int matrix_reg1,
			 int matrix_reg2,
			 int matrix_reg3,
			 int vector_reg,
			 int remain_regi,
			 int bool_change_matrix_vector,
			 int computative_size,
			 FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"MatTimesVec_R%d_R%d_ass_file",matrix_reg,vector_reg);
      ass_file=fopen(temp_string,"w");
    }


  //writeToRegister(mat_name,matrix_reg,ass_file,output_ass);
  //writeToRegister(vec_name,vector_reg,ass_file,output_ass);
  sprintf(temp_string," ");
  if(bool_change_matrix_vector==1)
    {
      switch(computative_size)
	{
	case 4:
	  sprintf(temp_string,"%s\tDPF R%d.0001, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi,vector_reg,matrix_reg3,computative_size);
	case 3:
	  sprintf(temp_string,"%s\tDPF R%d.0010, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi,vector_reg,matrix_reg2,computative_size);
	case 2:
	  sprintf(temp_string,"%s\tDPF R%d.0100, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi,vector_reg,matrix_reg1,computative_size);
	case 1:
	  sprintf(temp_string,"%s\tDPF R%d.1000, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi,vector_reg,matrix_reg,computative_size);
	}
    }
  else
    {
      switch(computative_size)
	{
	case 4:
	  sprintf(temp_string,"%s\tDPF R%d.0001, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi,matrix_reg3,vector_reg,computative_size);
	case 3:
	  sprintf(temp_string,"%s\tDPF R%d.0010, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi,matrix_reg2,vector_reg,computative_size);
	case 2:
	  sprintf(temp_string,"%s\tDPF R%d.0100, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi,matrix_reg1,vector_reg,computative_size);
	case 1:
	  sprintf(temp_string,"%s\tDPF R%d.1000, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi,matrix_reg,vector_reg,computative_size);
	}
    }
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_reg,remain_regi,computative_size);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //#ifndef output_ass_code

  //writeToRegister(mat_name,matrix_reg,NULL,output_ass);
  //writeToRegister(vec_name,vector_reg,NULL,output_ass);

  //#endif


  //machine code

  temp_b_code[3]=19;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=remain_regi;
  if(bool_change_matrix_vector==1)
    {
      temp_b_code[3]=temp_b_code[3]<<6;
      temp_b_code[3]|=vector_reg;
      temp_b_code[3]=temp_b_code[3]<<6;
      temp_b_code[3]|=matrix_reg;
    }
  else
    {
      temp_b_code[3]=temp_b_code[3]<<6;
      temp_b_code[3]|=matrix_reg;
      temp_b_code[3]=temp_b_code[3]<<6;
      temp_b_code[3]|=vector_reg;
    }
  temp_b_code[3]=temp_b_code[3]<<24;
  temp_b_code[3]|=0x1B1B1B;
  temp_b_code[3]=temp_b_code[3]<<7;
  temp_b_code[3]|=8;
  temp_b_code[3]=temp_b_code[3]<<1;
  temp_b_code[3]|=1;
  temp_b_code[3]=temp_b_code[3]<<2;
  temp_b_code[3]|=3;
  temp_b_code[3]=temp_b_code[3]<<7;
  temp_b_code[2]=temp_b_code[3];
  if(bool_change_matrix_vector==1)
    {
      temp_b_code[2]&=(long long)0xFFFF81FFFFFFC3FFLL;
      temp_b_code[2]|=((long long)(matrix_reg1)<<41);
    }
  else
    {
      temp_b_code[2]&=(long long)0xFFE07FFFFFFFC3FFLL;
      temp_b_code[2]|=((long long)(matrix_reg1)<<47);
    }
  temp_b_code[2]|=(1<<12);
  temp_b_code[1]=temp_b_code[3];
  if(bool_change_matrix_vector==1)
    {
      temp_b_code[1]&=(long long)0xFFFF81FFFFFFC3FFLL;
      temp_b_code[1]|=((long long)(matrix_reg2)<<41);
    }
  else
    {
      temp_b_code[1]&=(long long)0xFFE07FFFFFFFC3FFLL;
      temp_b_code[1]|=((long long)(matrix_reg2)<<47);
    }
  temp_b_code[1]|=(1<<11);
  temp_b_code[0]=temp_b_code[3];
  if(bool_change_matrix_vector==1)
    {
      temp_b_code[0]&=(long long)0xFFFF81FFFFFFC3FFLL;
      temp_b_code[0]|=((long long)(matrix_reg3)<<41);
    }
  else
    {
      temp_b_code[0]&=(long long)0xFFE07FFFFFFFC3FFLL;
      temp_b_code[0]|=((long long)(matrix_reg3)<<47);
    }
  temp_b_code[0]|=(1<<10);

#ifdef SET_NOP_NINE
  switch(computative_size)
    {
    case 4:
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
    case 3:
      temp_b_code[0]=temp_b_code[1];
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
    case 2:
      temp_b_code[0]=temp_b_code[2];
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
    case 1:
      temp_b_code[0]=temp_b_code[3];
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
    }
#else
  switch(computative_size)
    {
    case 4:
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
    case 3:
      temp_b_code[0]=temp_b_code[1];
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
    case 2:
      temp_b_code[0]=temp_b_code[2];
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
    case 1:
      temp_b_code[0]=temp_b_code[3];
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
    }
#endif

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_reg;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=31;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
      BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
      BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef SET_NOP_NINE
  return (computative_size+1)*2-1;
#else
  return computative_size+1;
#endif      


}

int GL_MatrixTimesScalar(int return_reg,
			 int matrix_reg,
			 int matrix_reg1,
			 int matrix_reg2,
			 int matrix_reg3,
			 int vector_reg,
			 int remain_regi,
			 int computative_size,
			 FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"MatTimesScalar_R%d_R%d_ass_file",matrix_reg,vector_reg);
      ass_file=fopen(temp_string,"w");
    }


  //writeToRegister(mat_name,matrix_reg,ass_file,output_ass);
  //writeToRegister(vec_name,vector_reg,ass_file,output_ass);
  sprintf(temp_string," ");
  switch(computative_size)
    {
    case 4:
      sprintf(temp_string,"%s\tMULF R%d.1111, R%d.xxxx, R%d.xyzw, <%d>\n",temp_string,remain_regi,vector_reg,matrix_reg3,computative_size);
    case 3:
      sprintf(temp_string,"%s\tMULF R%d.1111, R%d.xxxx, R%d.xyzw, <%d>\n",temp_string,remain_regi,vector_reg,matrix_reg2,computative_size);
    case 2:
      sprintf(temp_string,"%s\tMULF R%d.1111, R%d.xxxx, R%d.xyzw, <%d>\n",temp_string,remain_regi,vector_reg,matrix_reg1,computative_size);
    case 1:
      sprintf(temp_string,"%s\tMULF R%d.1111, R%d.xxxx, R%d.xyzw, <%d>\n",temp_string,remain_regi,vector_reg,matrix_reg,computative_size);
    }
  
  sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_reg,remain_regi,computative_size);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //#ifndef output_ass_code

  //writeToRegister(mat_name,matrix_reg,NULL,output_ass);
  //writeToRegister(vec_name,vector_reg,NULL,output_ass);

  //#endif


  //machine code


#ifdef SET_NOP_NINE
  switch(computative_size)
    {
    case 4:
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
    case 3:
      temp_b_code[0]=temp_b_code[1];
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
    case 2:
      temp_b_code[0]=temp_b_code[2];
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
    case 1:
      temp_b_code[0]=temp_b_code[3];
      insert_nop_nine(1);
      for(i=0;i<1*2;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
    }
#else
  switch(computative_size)
    {
    case 4:
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
    case 3:
      temp_b_code[0]=temp_b_code[1];
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
    case 2:
      temp_b_code[0]=temp_b_code[2];
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
    case 1:
      temp_b_code[0]=temp_b_code[3];
      for(i=0;i<1;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
    }
#endif

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_reg;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=31;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

#ifdef SET_NOP_NINE
  insert_nop_nine(1);
  for(i=0;i<1*2-1;i++)
      BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<1;i++)
      BinCode[LastBinLine++]=temp_b_code[i];
#endif

#ifdef SET_NOP_NINE
  return (computative_size+1)*2-1;
#else
  return computative_size+1;
#endif      


}

/*
int MatrixTimesMatrix(int return_regi,
		      int return_regi_2,
		      int return_regi_3,
		      int return_regi_4,
		      int matrix_regi,
		      int matrix_regi_2,
		      int matrix_regi_3,
		      int matrix_regi_4,
		      int matrix_regi2,
		      int matrix_regi2_2,
		      int matrix_regi2_3,
		      int matrix_regi2_4,
		      int SV,
		      FILE *output_ass)
{
  int i;
  //assembly code
  FILE *ass_file;

  if(output_ass)
    ass_file=output_ass;
  else
    {
      sprintf(temp_string,"MATRIXTIMESMATRIX_%d_%d_ass_file",matrix_regi,matrix_regi2);
      ass_file=fopen(temp_string,"w");
    }

  //writeToRegister(mat_name1,matrix_reg,ass_file,output_ass);
  //writeToRegister(mat_name2,matrix_reg2,ass_file,output_ass);

  sprintf(temp_string,"");
  sprintf(temp_string,"%s\tDP R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi,matrix_regi,matrix_regi2);
  if(SV>1)
    {  
      sprintf(temp_string,"%s\tDP R%d.0100, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi,matrix_regi,matrix_regi2_2);
      if(SV>2)
	{
	  sprintf(temp_string,"%s\tDP R%d.0010, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi,matrix_regi,matrix_regi2_3);
	  if(SV>3)
	    sprintf(temp_string,"%s\tDP R%d.0001, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi,matrix_regi,matrix_regi2_4);
	}
    }

  sprintf(temp_string,"%s\tDP R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi_2,matrix_regi_2,matrix_regi2);
  if(SV>1)
    {  
      sprintf(temp_string,"%s\tDP R%d.0100, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi_2,matrix_regi_2,matrix_regi2_2);
      if(SV>2)
	{
	  sprintf(temp_string,"%s\tDP R%d.0010, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi_2,matrix_regi_2,matrix_regi2_3);
	  if(SV>3)
	    sprintf(temp_string,"%s\tDP R%d.0001, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi_2,matrix_regi_2,matrix_regi2_4);
	}
    }

  sprintf(temp_string,"%s\tDP R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi_3,matrix_regi_3,matrix_regi2);
  if(SV>1)
    {  
  sprintf(temp_string,"%s\tDP R%d.0100, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi_3,matrix_regi_3,matrix_regi2_2);
      if(SV>2)
	{
	  sprintf(temp_string,"%s\tDP R%d.0010, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi_3,matrix_regi_3,matrix_regi2_3);
	  if(SV>3)
	    sprintf(temp_string,"%s\tDP R%d.0001, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi_3,matrix_regi_3,matrix_regi2_4);
	}
    }

  sprintf(temp_string,"%s\tDP R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi_4,matrix_regi_4,matrix_regi2);
  if(SV>1)
    {    
      sprintf(temp_string,"%s\tDP R%d.0100, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi_4,matrix_regi_4,matrix_regi2_2);
      if(SV>2)
	{
	  sprintf(temp_string,"%s\tDP R%d.0010, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi_4,matrix_regi_4,matrix_regi2_3);
	  if(SV>3)
	    sprintf(temp_string,"%s\tDP R%d.0001, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,return_regi_4,matrix_regi_4,matrix_regi2_4);
	}
    }

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

  if(!output_ass)
    fclose(ass_file);

  //machine code

  temp_b_code[0]=19;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=matrix_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=matrix_regi2;
  temp_b_code[0]=temp_b_code[0]<<31;
  temp_b_code[0]|=0xD8D8D88;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[1]=temp_b_code[0];
  temp_b_code[1]&=(long long)0xFFFF81FFFFFFC3FFLL;
  temp_b_code[1]|=((long long)(matrix_regi2_2)<<41);
  temp_b_code[1]|=(1<<12);
  temp_b_code[2]=temp_b_code[0];
  temp_b_code[2]&=(long long)0xFFFF81FFFFFFC3FFLL;
  temp_b_code[2]|=((long long)(matrix_regi2_3)<<41);
  temp_b_code[2]|=(1<<11);
  temp_b_code[3]=temp_b_code[0];
  temp_b_code[3]&=(long long)0xFFFF81FFFFFFC3FFLL;
  temp_b_code[3]|=((long long)(matrix_regi2_4)<<41);
  temp_b_code[3]|=(1<<10);
  temp_b_code[4]=temp_b_code[0];
  temp_b_code[5]=temp_b_code[1];
  temp_b_code[6]=temp_b_code[2];
  temp_b_code[7]=temp_b_code[3];
  temp_b_code[8]=temp_b_code[0];
  temp_b_code[9]=temp_b_code[1];
  temp_b_code[10]=temp_b_code[2];
  temp_b_code[11]=temp_b_code[3];
  temp_b_code[12]=temp_b_code[0];
  temp_b_code[13]=temp_b_code[1];
  temp_b_code[14]=temp_b_code[2];
  temp_b_code[15]=temp_b_code[3];
  temp_b_code[4]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[4]|=((long long)(return_regi_2)<<53);
  temp_b_code[4]|=((long long)(matrix_regi_2)<<47);
  temp_b_code[5]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[5]|=((long long)(return_regi_2)<<53);
  temp_b_code[5]|=((long long)(matrix_regi_2)<<47);
  temp_b_code[6]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[6]|=((long long)(return_regi_2)<<53);
  temp_b_code[6]|=((long long)(matrix_regi_2)<<47);
  temp_b_code[7]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[7]|=((long long)(return_regi_2)<<53);
  temp_b_code[7]|=((long long)(matrix_regi_2)<<47);
  temp_b_code[8]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[8]|=((long long)(return_regi_3)<<53);
  temp_b_code[8]|=((long long)(matrix_regi_3)<<47);
  temp_b_code[9]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[9]|=((long long)(return_regi_3)<<53);
  temp_b_code[9]|=((long long)(matrix_regi_3)<<47);
  temp_b_code[10]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[10]|=((long long)(return_regi_3)<<53);
  temp_b_code[10]|=((long long)(matrix_regi_3)<<47);
  temp_b_code[11]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[11]|=((long long)(return_regi_3)<<53);
  temp_b_code[11]|=((long long)(matrix_regi_3)<<47);
  temp_b_code[12]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[12]|=((long long)(return_regi_4)<<53);
  temp_b_code[12]|=((long long)(matrix_regi_4)<<47);
  temp_b_code[13]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[13]|=((long long)(return_regi_4)<<53);
  temp_b_code[13]|=((long long)(matrix_regi_4)<<47);
  temp_b_code[14]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[14]|=((long long)(return_regi_4)<<53);
  temp_b_code[14]|=((long long)(matrix_regi_4)<<47);
  temp_b_code[15]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[15]|=((long long)(return_regi_4)<<53);
  temp_b_code[15]|=((long long)(matrix_regi_4)<<47);

#ifdef SET_NOP_NINE
  insert_nop_nine(SV);
  for(i=0;i<SV*2-1;i++)
      BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
  for(i=0;i<SV;i++)
      BinCode[LastBinLine++]=temp_b_code[i];
#endif

  if(SV>1)
    {
      temp_b_code[0]=temp_b_code[0+4];
      temp_b_code[1]=temp_b_code[1+4];
      temp_b_code[2]=temp_b_code[2+4];
      temp_b_code[3]=temp_b_code[3+4];

#ifdef SET_NOP_NINE
      BinCode[LastBinLine++]=nop_nine_instruction;
      insert_nop_nine(SV);
      for(i=0;i<SV*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<SV;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif
    }

  if(SV>2)
    {
      temp_b_code[0]=temp_b_code[0+4*2];
      temp_b_code[1]=temp_b_code[1+4*2];
      temp_b_code[2]=temp_b_code[2+4*2];
      temp_b_code[3]=temp_b_code[3+4*2];

#ifdef SET_NOP_NINE
      BinCode[LastBinLine++]=nop_nine_instruction;
      insert_nop_nine(SV);
      for(i=0;i<SV*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<SV;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif
    }

  if(SV>3)
    {
      temp_b_code[0]=temp_b_code[0+4*3];
      temp_b_code[1]=temp_b_code[1+4*3];
      temp_b_code[2]=temp_b_code[2+4*3];
      temp_b_code[3]=temp_b_code[3+4*3];

#ifdef SET_NOP_NINE
      BinCode[LastBinLine++]=nop_nine_instruction;
      insert_nop_nine(SV);
      for(i=0;i<SV*2-1;i++)
	BinCode[LastBinLine++]=temp_b_code_NOP[i];
#else
      for(i=0;i<SV;i++)
	BinCode[LastBinLine++]=temp_b_code[i];
#endif
    }

#ifdef SET_NOP_NINE
  return (SV*SV)*2-1; 
#else
  return SV*SV;
#endif
}
*/

#ifdef inline_standalone
int main()
{
  FILE *out=fopen("inline_ass","w");

  char assembly_string[ASSEMBLY_STRING_SIZE];

  sprintf(assembly_string," ");

  //GL_Radians(4,4,OUR_TEMP_REGI,4,out);            //OK
  //GL_Degrees(7,7,OUR_TEMP_REGI,4,out);             //OK
  //GL_Sin(6,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,ARGU_REGI+3,RETU_REGI,4,assembly_string,out);                     //Need to run simulator OK but the sets of register can't be the same.
  //GL_Cos(6,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,ARGU_REGI+3,ARGU_REGI+4,RETU_REGI,4,assembly_string,out);         //Need to run simulator
  //GL_Tan();
  //GL_ASin();
  //GL_ACos();
  //GL_ATan();
  //GL_ATan();



  //GL_Pow(9,9,9,OUR_TEMP_REGI,4,out);              //OK
  //GL_Exp(16,16,6,22,4,out);                       //OK
  //GL_Log(27,27,OUR_TEMP_REGI,18,4,out);           //OK
  //GL_Exp2(19,19,OUR_TEMP_REGI,4,out);             //OK
  //GL_Log2(20,20,OUR_TEMP_REGI,4,out);             //OK
  //GL_Sqrt(6,6,OUR_TEMP_REGI,18,4,out);            //OK
  //GL_InverseSqrt(29,29,OUR_TEMP_REGI,4,out);      //OK


  //GL_Abs(6,6,'v',4,out);                          //OK
  //GL_Sign(12,12,'v',4,out);                       //OK
  //GL_Floor(4,4,OUR_TEMP_REGI,'v',4,out);          //OK
  //GL_Ceil(29,29,'v',4,out);                       //OK
  //GL_Fract(16,15,17,18,'f',3,out);                //OK
  //GL_Mod(16,15,16,17,18,19,'v',3,1,out);          //OK
  //GL_Min(23,23,23,ARGU_REGI,4,1,out);             //OK
  //GL_Min(23,23,23,ARGU_REGI,4,0,out);             //OK
  //GL_Max(23,23,23,ARGU_REGI,4,1,out);             //OK
  //GL_Max(23,23,23,ARGU_REGI,4,0,out);             //OK
  //GL_Clamp(5,18,24,10,17,3,0,out);                //Ok
  //GL_Clamp(5,18,24,10,17,3,1,out);                //OK
  //GL_Clamp(5,18,24,10,17,3,0,out);                //OK
  //GL_Clamp(5,18,24,10,17,3,1,out);                //OK
  //GL_Mix(17,17,17,17,18,0,4,out);                 //OK
  //GL_Step(17,17,17,ARGU_REGI,ARGU_REGI+1,1,'v',4,out); //OK
  //GL_SmoothStep(16,15,16,17,18,19,20,0,'f',2,out);//OK



  //GL_Length(18,18,29,30,4,out);                   //OK
  //GL_Distance(17,17,18,19,20,4,out);              //OK
  //GL_Dot(13,13,13,2,out);                         //OK
  //GL_Cross(14,15,16,17,out);                      //OK
  //GL_Normalize(15,16,17,18,4,out);                //OK
  //GL_FaceForward(16,16,16,16,18,'v',4,out);       //Need to run simulator OK
  //GL_Reflect(10,10,10,14,4,out);                  //Need to run simulator Ok, Code has been changed.  Using LDIF to load 0.5 value.
  //GL_Refract(15,15,15,15,18,19,20,21,'v',4,out);  //Need to run simulator Ok, Code has been changed.  Using two instructions of LDIF to load 1.0 and 0.5 value.
  //GL_MatrixCompMult();



  //GL_LessThan(7,7,7,ARGU_REGI,'v',4,out);         //OK
  //GL_LessThanEqual(7,7,7,ARGU_REGI,'v',4,out);    //OK
  //GL_GreaterThan(7,7,8,ARGU_REGI,'v',4,out);      //OK
  //GL_GreaterThanEqual(7,7,7,ARGU_REGI,'v',4,out); //OK
  //GL_Equal(7,7,7,ARGU_REGI,'v',4,out);            //OK
  //GL_NotEqual(7,7,7,ARGU_REGI,'v',4,out);         //OK
  //GL_Any(7,7,ARGU_REGI,'v',4,out);                //OK
  //GL_All(7,7,ARGU_REGI,'v',4,out);                //OK
  //GL_Not(7,7,ARGU_REGI,'v',4,out);                //OK



  //GL_Texture2D();
  //GL_Texture2D();
  //GL_Texture2DProj();
  //GL_Texture2DProj();
  //GL_Texture2DProj();
  //GL_Texture2DProj();
  //GL_Texture2DLod();
  //GL_Texture2DProjLod();
  //GL_Texture2DProjLod();
  //GL_TextureCube();
  //GL_TextureCube();
  //GL_TextureCubeLod();


  //GL_ChangeMatrix(19,15,16,17,18,23,1,4,out);
  //GL_ChangeMatrix(20,15,16,17,18,23,2,4,out);
  //GL_ChangeMatrix(21,15,16,17,18,23,3,4,out);
  //GL_ChangeMatrix(22,15,16,17,18,23,4,4,out);


  //VectorTimesScalar(8,26,7,1,3,out);

  //VectorNegate(6,11,1,2,out);
  //VectorNegate(6,11,0,4,out);


  //printf("%s\n",assembly_string);

  putoutthefile((char *)"inline_binary");
  fclose(out);

  return 0;
}
#endif
