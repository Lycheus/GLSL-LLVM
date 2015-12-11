#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#define combine_ass_code
#define output_ass_code
//#define test_only //Standalone. It can output binary code
#define combine_test
#define LD_BY_ONE 
unsigned long long temp_b_code[64];
#ifndef combine_test
int num_temp_b;
unsigned long long all_temp_b[65536];
#endif
#ifdef combine_test

extern long long unsigned int BinCode[65536];
extern int LastBinLine;

int num_temp_b;
unsigned long long all_temp_b[65536];
#endif
char temp_string[4096];
char combine_ass_name[25]={'a','s','s','F','i','l','e','\0'};

#define perror(X) {printf("\"%s\" is Error",X);exit(1);}

int GL_LoadVarying(int varying_regi)
{
  int i;

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=varying_regi;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1B1B;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=31;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

  return 1;
}

int GL_ENDFrag()
{
  int i;

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=32;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1B1B;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=31;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

  return 1;
}

int GL_END(int return_regi)
{
#define view_w 320.0
#define view_h 320.0
#define view_x 0.0
#define view_y 0.0
#define temp_z 1.0
#define temp_w 1.0

  int i;
  float temp_float=1.0;
  unsigned temp_int;

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1BDB;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=2;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=3;
  temp_b_code[1]=temp_b_code[1]<<22;
  temp_b_code[1]|=6939;
  temp_b_code[1]=temp_b_code[1]<<16;
  temp_b_code[1]|=31;
  temp_b_code[1]=temp_b_code[1]<<9;  

  temp_b_code[2]=17;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=2;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=2;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=3;
  temp_b_code[2]=temp_b_code[2]<<24;
  temp_b_code[2]|=0x1B1B;
  temp_b_code[2]=temp_b_code[2]<<7;
  temp_b_code[2]|=15;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=3;
  temp_b_code[2]=temp_b_code[2]<<7;

  temp_b_code[3]=10;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=2;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=2;
  temp_b_code[3]=temp_b_code[3]<<32;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[3]|=temp_int;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=25;
  //temp_b_code[3]|=24;
  temp_b_code[3]=temp_b_code[3]<<2;
  temp_b_code[3]|=2;
  temp_b_code[3]=temp_b_code[3]<<7;

  temp_b_code[4]=temp_b_code[3];
  temp_b_code[4]&=(long long unsigned)0xF80000000000427FLL;
  temp_b_code[4]|=(long long unsigned)3<<53;
  temp_float=view_w/2;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[4]|=(long long unsigned)temp_int<<15;
  temp_b_code[4]|=(long long unsigned)8<<10;

  temp_b_code[5]=temp_b_code[4];
  temp_b_code[5]&=(long long unsigned)0xFFF700000000427FLL;
  temp_float=view_h/2;
  memcpy(&temp_int,&temp_float,4);
//temp_int=240/2;
  temp_b_code[5]|=(long long unsigned)temp_int<<15;
  temp_b_code[5]|=(long long unsigned)4<<10;
  temp_b_code[5]|=(long long unsigned)1<<7;

  temp_b_code[6]=17;
  temp_b_code[6]=temp_b_code[6]<<6;
  temp_b_code[6]|=2;
  temp_b_code[6]=temp_b_code[6]<<6;
  temp_b_code[6]|=2;
  temp_b_code[6]=temp_b_code[6]<<6;
  temp_b_code[6]|=3;
  temp_b_code[6]=temp_b_code[6]<<24;
  temp_b_code[6]|=0x1B1B;
  temp_b_code[6]=temp_b_code[6]<<7;
  temp_b_code[6]|=12;
  temp_b_code[6]=temp_b_code[6]<<1;
  temp_b_code[6]|=1;
  temp_b_code[6]=temp_b_code[6]<<2;
  temp_b_code[6]|=1;
  temp_b_code[6]=temp_b_code[6]<<7;

  temp_b_code[7]=temp_b_code[4];
  temp_b_code[7]&=(long long unsigned)0xFFF700000000427FLL;
  temp_float=view_x;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[7]|=(long long unsigned)temp_int<<15;
  temp_b_code[7]|=(long long unsigned)8<<10;

  temp_b_code[8]=temp_b_code[4];
  temp_b_code[8]&=(long long unsigned)0xFFF700000000427FLL;
  temp_float=view_y;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[8]|=(long long unsigned)temp_int<<15;
  temp_b_code[8]|=(long long unsigned)4<<10;
  temp_b_code[8]|=(long long unsigned)1<<7;

  temp_b_code[9]=16;
  temp_b_code[9]=temp_b_code[9]<<6;
  temp_b_code[9]|=2;
  temp_b_code[9]=temp_b_code[9]<<6;
  temp_b_code[9]|=2;
  temp_b_code[9]=temp_b_code[9]<<6;
  temp_b_code[9]|=3;
  temp_b_code[9]=temp_b_code[9]<<24;
  temp_b_code[9]|=6939;
  temp_b_code[9]=temp_b_code[9]<<7;
  temp_b_code[9]|=12;
  temp_b_code[9]=temp_b_code[9]<<1;
  temp_b_code[9]|=1;
  temp_b_code[9]=temp_b_code[9]<<2;
  temp_b_code[9]|=1;
  temp_b_code[9]=temp_b_code[9]<<7;

  temp_b_code[10]=temp_b_code[4];
  temp_b_code[10]&=(long long unsigned)0xFFF700000000427FLL;
  temp_float=temp_z;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[10]|=(long long unsigned)temp_int<<15;
  temp_b_code[10]|=(long long unsigned)2<<10;
  temp_b_code[10]|=(long long unsigned)3<<7;

  temp_b_code[11]=temp_b_code[4];
  temp_b_code[11]&=(long long unsigned)0xFFF700000000427FLL;
  temp_float=temp_w;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[11]|=(long long unsigned)temp_int<<15;
  temp_b_code[11]|=(long long unsigned)1<<10;
  temp_b_code[11]|=(long long unsigned)3<<7;

  temp_b_code[12]=17;
  temp_b_code[12]=temp_b_code[12]<<6;
  temp_b_code[12]|=3;
  temp_b_code[12]=temp_b_code[12]<<6;
  temp_b_code[12]|=2;
  temp_b_code[12]=temp_b_code[12]<<6;
  temp_b_code[12]|=3;
  temp_b_code[12]=temp_b_code[12]<<24;
  temp_b_code[12]|=0x1B1B;
  temp_b_code[12]=temp_b_code[12]<<7;
  temp_b_code[12]|=3;
  temp_b_code[12]=temp_b_code[12]<<1;
  temp_b_code[12]|=1;
  temp_b_code[12]=temp_b_code[12]<<2;
  temp_b_code[12]|=3;
  temp_b_code[12]=temp_b_code[12]<<7;

  temp_b_code[13]=16;
  temp_b_code[13]=temp_b_code[13]<<6;
  temp_b_code[13]|=2;
  temp_b_code[13]=temp_b_code[13]<<6;
  temp_b_code[13]|=3;
  temp_b_code[13]=temp_b_code[13]<<6;
  temp_b_code[13]|=3;
  temp_b_code[13]=temp_b_code[13]<<24;
  temp_b_code[13]|=0x1B1F;
  temp_b_code[13]=temp_b_code[13]<<7;
  temp_b_code[13]|=2;
  temp_b_code[13]=temp_b_code[13]<<1;
  temp_b_code[13]|=1;
  temp_b_code[13]=temp_b_code[13]<<2;
  temp_b_code[13]|=3;
  temp_b_code[13]=temp_b_code[13]<<7;

  temp_b_code[14]=temp_b_code[4];
  temp_b_code[14]&=(long long unsigned)0xFFF700000000427FLL;
  temp_float=0.5;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[14]|=(long long unsigned)temp_int<<15;
  temp_b_code[14]|=(long long unsigned)8<<10;
  temp_b_code[14]|=(long long unsigned)1<<7;

  temp_b_code[15]=17;
  temp_b_code[15]=temp_b_code[15]<<6;
  temp_b_code[15]|=2;
  temp_b_code[15]=temp_b_code[15]<<6;
  temp_b_code[15]|=2;
  temp_b_code[15]=temp_b_code[15]<<6;
  temp_b_code[15]|=3;
  temp_b_code[15]=temp_b_code[15]<<24;
  temp_b_code[15]|=0x1B13;
  temp_b_code[15]=temp_b_code[15]<<7;
  temp_b_code[15]|=2;
  temp_b_code[15]=temp_b_code[15]<<1;
  temp_b_code[15]|=1;
  temp_b_code[15]=temp_b_code[15]<<2;
  temp_b_code[15]|=3;
  temp_b_code[15]=temp_b_code[15]<<7;

  temp_b_code[16]=7;
  temp_b_code[16]=temp_b_code[16]<<6;
  temp_b_code[16]|=return_regi;
  temp_b_code[16]=temp_b_code[16]<<6;
  temp_b_code[16]|=2;
  temp_b_code[16]=temp_b_code[16]<<22;
  temp_b_code[16]|=0x1B1B;
  temp_b_code[16]=temp_b_code[16]<<16;
  temp_b_code[16]|=31;
  temp_b_code[16]=temp_b_code[16]<<2;
  temp_b_code[16]|=3;
  temp_b_code[16]=temp_b_code[16]<<7;

  for(i=0;i<17;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

  return 11;
}
int GL_ChangeMatrix4(int step)
{
  int i;
  printf("step: %d\n",step);
  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=0x1B18;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;

  switch (step)
    {
    case 1:
      temp_b_code[0]&=(long long unsigned)0xFFFFFFFFFFFFFFFFLL;
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[1]|=(long long unsigned)3<<47;
      temp_b_code[1]|=(long long unsigned)0x1B13<<25;
      temp_b_code[1]|=(long long unsigned)2<<10;
      temp_b_code[2]=temp_b_code[0];
      temp_b_code[2]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[2]|=(long long unsigned)4<<47;
      temp_b_code[2]|=(long long unsigned)0x1B0B<<25;
      temp_b_code[2]|=(long long unsigned)4<<10;
      temp_b_code[3]=temp_b_code[0];
      temp_b_code[3]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[3]|=(long long unsigned)5<<47;
      temp_b_code[3]|=(long long unsigned)0x1B1B<<25;
      temp_b_code[3]|=(long long unsigned)8<<10;
      break;
    case 2:
      temp_b_code[0]&=(long long unsigned)0xFFFFFE0001FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x1B19<<25;
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[1]|=(long long unsigned)3<<47;
      temp_b_code[1]|=(long long unsigned)0x1B17<<25;
      temp_b_code[1]|=(long long unsigned)2<<10;
      temp_b_code[2]=temp_b_code[0];
      temp_b_code[2]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[2]|=(long long unsigned)4<<47;
      temp_b_code[2]|=(long long unsigned)0x1B1B<<25;
      temp_b_code[2]|=(long long unsigned)4<<10;
      temp_b_code[3]=temp_b_code[0];
      temp_b_code[3]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[3]|=(long long unsigned)5<<47;
      temp_b_code[3]|=(long long unsigned)0x1B5B<<25;
      temp_b_code[3]|=(long long unsigned)8<<10;
      break;
    case 3:
      temp_b_code[0]&=(long long unsigned)0xFFFFFE0001FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x1B1A<<25;
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[1]|=(long long unsigned)3<<47;
      temp_b_code[1]|=(long long unsigned)0x1B1B<<25;
      temp_b_code[1]|=(long long unsigned)2<<10;
      temp_b_code[2]=temp_b_code[0];
      temp_b_code[2]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[2]|=(long long unsigned)4<<47;
      temp_b_code[2]|=(long long unsigned)0x1B2B<<25;
      temp_b_code[2]|=(long long unsigned)4<<10;
      temp_b_code[3]=temp_b_code[0];
      temp_b_code[3]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[3]|=(long long unsigned)5<<47;
      temp_b_code[3]|=(long long unsigned)0x1B9B<<25;
      temp_b_code[3]|=(long long unsigned)8<<10;
      break;
    case 4:
      temp_b_code[0]&=(long long unsigned)0xFFFFFE0001FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)0x1B1B<<25;
      temp_b_code[1]=temp_b_code[0];
      temp_b_code[1]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[1]|=(long long unsigned)3<<47;
      temp_b_code[1]|=(long long unsigned)0x1B1F<<25;
      temp_b_code[1]|=(long long unsigned)2<<10;
      temp_b_code[2]=temp_b_code[0];
      temp_b_code[2]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[2]|=(long long unsigned)4<<47;
      temp_b_code[2]|=(long long unsigned)0x1B3B<<25;
      temp_b_code[2]|=(long long unsigned)4<<10;
      temp_b_code[3]=temp_b_code[0];
      temp_b_code[3]&=(long long unsigned)0xFFE07E0001FFC3FFLL;
      temp_b_code[3]|=(long long unsigned)5<<47;
      temp_b_code[3]|=(long long unsigned)0x1BDB<<25;
      temp_b_code[3]|=(long long unsigned)8<<10;
      break;
    default:

      //perror("step");
      break;
    }
  for(i=0;i<4;i++)
    BinCode[LastBinLine++]=temp_b_code[i];
#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),4,output_file);
#endif

  return 4;
}

int GL_LoadAttribute(int att_regi,int vector_size)
{
  int i;

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=att_regi;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=((1<<3)|((vector_size>1?1:0)<<2)|((vector_size>2?1:0)<<1)|(vector_size>3?1:0));
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;


  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];

#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

  return 1;
}
int GL_VaryingPut(int att_regi,int vector_size)
{
  int i;

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=att_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=2;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=((1<<3)|((vector_size>1?1:0)<<2)|((vector_size>2?1:0)<<1)|(vector_size>3?1:0));
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;


  for(i=0;i<1;i++)
    BinCode[LastBinLine++]=temp_b_code[i];

#ifdef test_only
  //fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

  return 1;
}
/*
int GL_Pow(int return_regi,int regiX, int regiY,int remain_regi, int SV,FILE *output_file);

int GL_Dot(int return_regi,int regiX,int regiY,int SV,int write_mask,FILE *output_file);

int GL_Ceil(int return_regi,int regiX, int SV, FILE *output_file)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"CEIL_R%d_R%d_ass_file",return_regi,regiX);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"\tFTOI R%d.1111, R%d, <%d>\n",return_regi,regiX,SV+1);
  sprintf(temp_string,"%s\tITOF R%d.1111, R%d, <%d>\n",temp_string,return_regi,return_regi,SV+1);
  sprintf(temp_string,"%s\tBLT R%d, R0, 16\n",temp_string,regiX);
  sprintf(temp_string,"%s\tADDIF R%d.1111, R%d, 1.0, <%d>\n",temp_string,return_regi,return_regi,SV);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
#endif    
  
  //machine code

  float temp_float=1.0;
  unsigned temp_int;

  temp_b_code[0]=28;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<38;
  temp_b_code[0]|=31;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=temp_b_code[0];
  temp_b_code[1]&=(long long unsigned)0xFFE07FFFFFFFFDFFLL;
  temp_b_code[1]|=(long long unsigned)((long long unsigned)return_regi<<47);

  temp_b_code[2]=26;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=regiX;
  temp_b_code[2]=temp_b_code[2]<<22;
  temp_b_code[2]|=16;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=regiX;
  temp_b_code[2]=temp_b_code[2]<<18;
  temp_b_code[2]|=SV-1;
  temp_b_code[2]=temp_b_code[2]<<7;

  temp_b_code[3]=10;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=return_regi;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=return_regi;
  temp_b_code[3]=temp_b_code[3]<<32;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[3]|=temp_int;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=31;
  temp_b_code[3]=temp_b_code[3]<<2;
  temp_b_code[3]|=SV-1;
  temp_b_code[3]=temp_b_code[3]<<7;

  for(i=0;i<4;i++)
    all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4,output_file);
#endif

  return 4;  
}

int GL_Floor(int return_regi,int regiX, int SV, FILE *output_file)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"FLOOR_R%d_R%d_ass_file",return_regi,regiX);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"\tFTOI R%d.1111, R%d, <%d>\n",return_regi,regiX,SV+1);
  sprintf(temp_string,"%s\tITOF R%d.1111, R%d, <%d>\n",temp_string,return_regi,return_regi,SV+1);
  sprintf(temp_string,"%s\tBGT R%d, R0, 16\n",temp_string,regiX);
  sprintf(temp_string,"%s\tADDIF R%d.1111, R%d, -1.0, <%d>\n",temp_string,return_regi,return_regi,SV);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
#endif    
  
  //machine code

  float temp_float=-1.0;
  unsigned temp_int;

  temp_b_code[0]=28;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<38;
  temp_b_code[0]|=31;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=temp_b_code[0];
  temp_b_code[1]&=(long long unsigned)0xFFE07FFFFFFFFDFFLL;
  temp_b_code[1]|=(long long unsigned)((long long unsigned)return_regi<<47);

  temp_b_code[2]=27;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=regiX;
  temp_b_code[2]=temp_b_code[2]<<22;
  temp_b_code[2]|=16;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=regiX;
  temp_b_code[2]=temp_b_code[2]<<18;
  temp_b_code[2]|=SV-1;
  temp_b_code[2]=temp_b_code[2]<<7;

  temp_b_code[3]=10;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=return_regi;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=return_regi;
  temp_b_code[3]=temp_b_code[3]<<32;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[3]|=temp_int;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=31;
  temp_b_code[3]=temp_b_code[3]<<2;
  temp_b_code[3]|=SV-1;
  temp_b_code[3]=temp_b_code[3]<<7;

  for(i=0;i<4;i++)
    all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4,output_file);
#endif

  return 4;  
}

int GL_Sign(int return_regi, int regiX, int SV, FILE *output_file)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"SIGN_R%d_R%d_ass_file",return_regi,regiX);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"\tBGT R%d, R0, 24\n",regiX);
  sprintf(temp_string,"%s\tMVIF R%d, -1.0\n",temp_string,return_regi);
  sprintf(temp_string,"%s\tJR 40\n",temp_string);
  sprintf(temp_string,"%s\tBLT R%d, R0, 24\n",temp_string,regiX);
  sprintf(temp_string,"%s\tMVIF R%d, 1.0\n",temp_string,return_regi);
  sprintf(temp_string,"%s\tJR 16\n",temp_string);
  sprintf(temp_string,"%s\tMVIF R%d, 0.0\n",temp_string,return_regi);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
#endif  

  //machine code

  float temp_float=-1.0;
  unsigned temp_int;

  temp_b_code[0]=27;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=24;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<18;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=10;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=return_regi;
  temp_b_code[1]=temp_b_code[1]<<38;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[1]|=temp_int;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=17;
  temp_b_code[1]=temp_b_code[1]<<9;  

  temp_b_code[2]=24;
  temp_b_code[2]=temp_b_code[2]<<28;
  temp_b_code[2]|=40;
  temp_b_code[2]=temp_b_code[2]<<31;

  temp_b_code[3]=temp_b_code[0];
  temp_b_code[3]&=(long long unsigned)0x07FFFFFFFFFFFFFFLL;
  temp_b_code[3]|=(long long unsigned)((long long unsigned)26<<59);

  temp_float=1.0;
  temp_b_code[4]=temp_b_code[1];
  temp_b_code[4]&=(long long unsigned)0xFFFF800000007FFFLL;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[4]|=(long long unsigned)((long long unsigned)temp_int<<15);

  temp_b_code[5]=temp_b_code[2];
  temp_b_code[5]&=(long long unsigned)0xFFFF80007FFFFFFFLL;
  temp_b_code[5]|=(long long unsigned)((long long unsigned)16<<31);

  temp_float=0.0;
  temp_b_code[6]=temp_b_code[1];
  temp_b_code[6]&=(long long unsigned)0xFFFF800000007FFFLL;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[6]|=(temp_int<<15);

  for(i=0;i<7;i++)
    all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),7,output_file);
#endif

  return 7;
}

int GL_Abs(int return_regi,int regiX,int SV,FILE *output_file)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"ABS_R%d_R%d_ass_file",return_regi,regiX);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"\tMVR R%d.1111, R%d.xyzw, <%d>\n",return_regi,regiX,SV);
  sprintf(temp_string,"%s\tBGT R%d, R0, 16, <%d>\n",temp_string,regiX,SV);
  sprintf(temp_string,"%s\tSUBF R%d, R%d, R%d, <%d>\n",temp_string,return_regi,0,return_regi,SV);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
#endif  

  // machine code

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=SV-1;
  temp_b_code[0]=temp_b_code[0]<<7;

  temp_b_code[1]=27;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=regiX;
  temp_b_code[1]=temp_b_code[1]<<22;
  temp_b_code[1]|=16;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=regiX;
  temp_b_code[1]=temp_b_code[1]<<18;
  temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;

  temp_b_code[2]=16;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=return_regi;
  temp_b_code[2]=temp_b_code[2]<<12;
  temp_b_code[2]|=return_regi;
  temp_b_code[2]=temp_b_code[2]<<24;
  temp_b_code[2]|=6939;
  temp_b_code[2]=temp_b_code[2]<<8;
  temp_b_code[2]|=49;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=SV-1;
  temp_b_code[2]=temp_b_code[2]<<7;


  for(i=0;i<3;i++)
    all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),3,output_file);
#endif

  return 3;

}

int GL_InverseSqrt(int return_regi,int regiX,FILE *output_file)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"INVERSESQRT_R%d_R%d_ass_file",return_regi,regiX);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"\tRSQ R%d, R%d\n",return_regi,regiX);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
#endif  

  // machine code

  temp_b_code[0]=3;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<9;

  for(i=0;i<1;i++)
    all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

  return 1;
}

int GL_Sqrt(int return_regi,int regiX,int remain_regi,int remain_regi2,FILE *output_file)
{
  int i,line_record=0;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"SQRT_R%d_R%d_ass_file",return_regi,regiX);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"\tMVIF R%d.1000, 0.5\n",remain_regi);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
#endif  

  //machine code

  float temp_float=0.5;
  unsigned temp_int;
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<9;  

  for(i=0;i<1;i++)
    all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

  line_record++;

  line_record+=GL_Pow(return_regi,regiX,remain_regi,remain_regi2,8,output_file);
  
  return line_record;
}

int GL_Log2(int return_regi,int regiX,FILE *output_file)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"LOG2_R%d_R%d_ass_file",return_regi,regiX);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"\tLOG2 R%d, R%d\n",return_regi,regiX);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
#endif  

  // machine code

  temp_b_code[0]=4;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<9;

  for(i=0;i<1;i++)
    all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

  return 1;
}

int GL_Exp2(int return_regi,int regiX,FILE *output_file)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"EXP2_R%d_R%d_ass_file",return_regi,regiX);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"\tPOW2 R%d, R%d\n",return_regi,regiX);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
#endif  

  // machine code

  temp_b_code[0]=5;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=regiX;
  temp_b_code[0]=temp_b_code[0]<<22;
  temp_b_code[0]|=6939;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<9;

  for(i=0;i<1;i++)
    all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

  return 1;
}

int GL_Log(int return_regi,int regiX,int remain_regi,int SV,FILE *output_file)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"LOG_R%d_R%d_R%d_ass_file",return_regi,regiX,remain_regi);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"\tMVI R%d.1000, 2.718281\n",return_regi);
  switch (SV)
    {
    case 4:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.wyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,return_regi,return_regi);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      sprintf(temp_string,"%s\tRCP R%d, R%d\n",temp_string,return_regi,return_regi);
      sprintf(temp_string,"%s\tMULF R%d.0001, R%d.xyzx, R%d.xyzx, <%d>\n",temp_string,return_regi,return_regi,remain_regi,SV);
    case 3:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.zyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,return_regi,return_regi);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      sprintf(temp_string,"%s\tRCP R%d, R%d\n",temp_string,return_regi,return_regi);
      sprintf(temp_string,"%s\tMULF R%d.0010, R%d.xyxw, R%d.xyxw, <%d>\n",temp_string,return_regi,return_regi,remain_regi,SV);
    case 2:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.yyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,return_regi,return_regi);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      sprintf(temp_string,"%s\tRCP R%d, R%d\n",temp_string,return_regi,return_regi);
      sprintf(temp_string,"%s\tMULF R%d.0100, R%d.xxzw, R%d.xxzw\n",temp_string,return_regi,return_regi,regiX);
    case 1:
      sprintf(temp_string,"%s\tMVR R%d.1000, R%d.xyzw, <%d>\n",temp_string,remain_regi,regiX,SV);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,return_regi,return_regi);
      sprintf(temp_string,"%s\tLOG2 R%d, R%d\n",temp_string,remain_regi,remain_regi);
      sprintf(temp_string,"%s\tRCP R%d, R%d\n",temp_string,return_regi,return_regi);
      sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,return_regi,return_regi,remain_regi,SV);
      break;
    default:
      perror("LOG: SV error");
      break;
    }
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
#endif  

  //machine code
  float temp_float=2.718281;
  unsigned temp_int;
  long long unsigned temp_regi;

  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<9;

  for(i=0;i<1;i++)
    all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_file);
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

  temp_b_code[1]=4;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=return_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=return_regi;
  temp_b_code[1]=temp_b_code[1]<<22;
  temp_b_code[1]|=6939;
  temp_b_code[1]=temp_b_code[1]<<16;
  temp_b_code[1]|=17;
  temp_b_code[1]=temp_b_code[1]<<9;

  temp_b_code[2]=temp_b_code[1];
  temp_b_code[2]&=(long long unsigned)0xF8007FFFFFFFFFFFLL;
  temp_b_code[2]|=(long long unsigned)remain_regi<<53;
  temp_b_code[2]|=(long long unsigned)remain_regi<<47;

  temp_b_code[3]=temp_b_code[1];
  temp_b_code[3]&=(long long unsigned)0x07FFFFFFFFFFFFFFLL;
  temp_b_code[3]|=(long long unsigned)2<<59;

  temp_b_code[4]=17;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=return_regi;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=return_regi;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=remain_regi;
  temp_b_code[4]=temp_b_code[4]<<24;
  temp_b_code[4]|=6168;
  temp_b_code[4]=temp_b_code[4]<<7;
  temp_b_code[4]|=1;
  temp_b_code[4]=temp_b_code[4]<<1;
  temp_b_code[4]|=1;
  temp_b_code[4]=temp_b_code[4]<<2;
  temp_b_code[4]|=SV-1;
  temp_b_code[4]=temp_b_code[4]<<7;

  switch(SV)
    {
    case 4:
      for(i=0;i<5;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),5,output_file);
#endif
    case 3:
      temp_b_code[0]&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)155<<25;
      temp_b_code[4]&=(long long unsigned)0xFFFFFFFE0001C3FFLL;
      temp_b_code[4]|=(long long unsigned)4883<<17;
      temp_b_code[4]|=(long long unsigned)2<<10;
      for(i=0;i<5;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),5,output_file);
#endif
    case 2:
      temp_b_code[0]&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)91<<25;
      temp_b_code[4]&=(long long unsigned)0xFFFFFFFE0001C3FFLL;
      temp_b_code[4]|=(long long unsigned)2827<<17;
      temp_b_code[4]|=(long long unsigned)4<<10;
      for(i=0;i<5;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),5,output_file);
#endif
    case 1:
      temp_b_code[0]&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
      temp_b_code[0]|=(long long unsigned)27<<25;
      temp_b_code[4]&=(long long unsigned)0xFFFFFFFE0001C3FFLL;
      temp_b_code[4]|=(long long unsigned)6939<<17;
      temp_b_code[4]|=(long long unsigned)8<<10;
      for(i=0;i<5;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),5,output_file);
#endif
    }

  return SV*5+1;
}

int GL_Exp(int return_regi,int regiX,int remain_regi,int remain_regi2,int SV,FILE *output_file)
{
  int i,line_record=0;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"EXP_R%d_R%d_ass_file",return_regi,regiX,remain_regi);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"\tMVI R%d.1%d%d%d, 2.718281\n",remain_regi,SV>=2?1:0,SV>=3?1:0,SV>=4?1:0);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
#endif  

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

  for(i=0;i<1;i++)
    all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif

  line_record++;

  line_record+=GL_Pow(return_regi,remain_regi,regiX,remain_regi2,SV,output_file);

  return line_record;
}

int GL_Degree(int return_regi,int regi1,int remain_regi,int SV,FILE *output_file)
{

  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"DEGREE_R%d_R%d_ass_file",return_regi,regi1,remain_regi);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"\tLDIF R%d.1000, 57.295718482\n",remain_regi);
  sprintf(temp_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xxxx, <%d>\n",temp_string,return_regi,SV>=2?1:0,SV>=3?1:0,SV>=4?1:0,regi1,remain_regi,SV);
  
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
#endif
  
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

  for(i=0;i<2;i++)
    all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),2,output_file);
#endif

  return 2;
}

int GL_Radians(int return_regi,int regi1,int remain_regi,int SV,FILE *output_file)
{

  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"RADIANS_R%d_R%d_ass_file",return_regi,regi1,remain_regi);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"\tLDIF R%d.1000, 0.017453\n",remain_regi);
  sprintf(temp_string,"%s\tMULF R%d.1%d%d%d, R%d.xyzw, R%d.xxxx, <%d>\n",temp_string,return_regi,SV>=2?1:0,SV>=3?1:0,SV>=4?1:0,regi1,remain_regi,SV);
  
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);
  
#endif
  
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

  for(i=0;i<2;i++)
    all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),2,output_file);
#endif

  return 2;
}

int GL_Reflect(int return_regi,int I_regi, int N_regi,int remain_regi,int SV,FILE *output_file)
{
  int i,line_record=0;

  //assembly code

  line_record=GL_Dot(remain_regi,I_regi,N_regi,SV,8,output_file);
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"REFLECT_R%d_R%d_R%d_R%d_ass_file",return_regi,I_regi,N_regi,remain_regi);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"\tLDIF R%d.0100, 2.0\n",remain_regi);
  if(SV==1)
    sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.yyzw, <%d>\n",temp_string,remain_regi,remain_regi,remain_regi,2);
  else
    sprintf(temp_string,"%s\tMULF R%d.1000, R%d.xyzw, R%d.yyzw, <%d>\n",temp_string,remain_regi,remain_regi,remain_regi,SV);
  sprintf(temp_string,"%s\tMULF R%d.1111, R%d.xyzw, R%d.xxxx, <%d>\n",temp_string,remain_regi,N_regi,remain_regi,SV);
  sprintf(temp_string,"%s\tADDF R%d.1111, R%d.xyzw, -R%d.xyzw, <%d>\n",temp_string,return_regi,I_regi,remain_regi,SV);


  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

#endif

  //machine code

  
  float temp_float=2.0;
  unsigned temp_int;

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
  temp_b_code[0]=temp_b_code[0]<<9;
  temp_b_code[1]=17;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
  temp_b_code[1]=temp_b_code[1]<<24;
  temp_b_code[1]|=7003;
  temp_b_code[1]=temp_b_code[1]<<7;
  temp_b_code[1]|=8;
  temp_b_code[1]=temp_b_code[1]<<1;
  temp_b_code[1]|=1;
  temp_b_code[1]=temp_b_code[1]<<2;
  if(SV==1)
    temp_b_code[1]|=1;
  else
    temp_b_code[1]|=SV-1;
  temp_b_code[1]=temp_b_code[1]<<7;
  temp_b_code[2]=17;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=N_regi;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=remain_regi;
  temp_b_code[2]=temp_b_code[2]<<24;
  temp_b_code[2]|=6912;
  temp_b_code[2]=temp_b_code[2]<<7;
  temp_b_code[2]|=15;
  temp_b_code[2]=temp_b_code[2]<<1;
  temp_b_code[2]|=1;
  temp_b_code[2]=temp_b_code[2]<<2;
  temp_b_code[2]|=SV-1;
  temp_b_code[2]=temp_b_code[2]<<7;
  temp_b_code[3]=16;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=return_regi;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=I_regi;
  temp_b_code[3]=temp_b_code[3]<<6;
  temp_b_code[3]|=remain_regi;
  temp_b_code[3]=temp_b_code[3]<<24;
  temp_b_code[3]|=6939;
  temp_b_code[3]=temp_b_code[3]<<3;
  temp_b_code[3]|=1;
  temp_b_code[3]=temp_b_code[3]<<4;
  temp_b_code[3]|=15;
  temp_b_code[3]=temp_b_code[3]<<1;
  temp_b_code[3]|=1;
  temp_b_code[3]=temp_b_code[3]<<2;
  temp_b_code[3]|=SV-1;
  temp_b_code[3]=temp_b_code[3]<<7;

  for(i=0;i<4;i++)
    all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),4,output_file);
#endif

  return line_record+4;
}


int GL_Length(int return_regi,int input_regi,int remain_regi,int remain_regi2,int SV,FILE *output_file)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"LENGTH_R%d_R%d_ass_file",return_regi,input_regi);
  ass_file=fopen(temp_string,"w");
#endif
  if(SV>4 || SV<1)
    perror("GL_Length: SV");
  int line_record=0;
  sprintf(temp_string,"\tLDIF R%d, 0.5\n",return_regi);
  sprintf(temp_string,"%s\tMULF R%d.1111, R%d.xyzw, R%d.xyzw, <%d>\n",temp_string,remain_regi,input_regi,input_regi,SV);
  switch(SV)
    {
    case 4:
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.xyzw, R%d.wyzw, <%d>\n",temp_string,remain_regi,input_regi,input_regi,SV);
    case 3:
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.xyzw, R%d.zyzw, <%d>\n",temp_string,remain_regi,input_regi,input_regi,SV);
    case 2:
      sprintf(temp_string,"%s\tADDF R%d.1000, R%d.xyzw, R%d.yyzw, <%d>\n",temp_string,remain_regi,input_regi,input_regi,SV);
    default:
      break;
    }
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  line_record=2+SV-1;
  fflush(ass_file);
  //call GL_Pow

  //sprintf(temp_string,"\tGL_Pow(%d,%d,%d,%d,output_file)\n",return_regi,remain_regi,return_regi,8);
  //fputs(temp_string,ass_file);
  //bzero(temp_string,sizeof(temp_string));



#endif

  //machine code
  
  temp_b_code[0]=10;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<38;
  float temp_float=0.5;
  unsigned temp_int;
  memcpy(&temp_int,&temp_float,4);
  temp_b_code[0]|=temp_int;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=17;
  temp_b_code[0]=temp_b_code[0]<<9;
  temp_b_code[1]=17;
  temp_b_code[1]=temp_b_code[1]<<6;
  temp_b_code[1]|=remain_regi;
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
  for(i=0;i<2;i++)
      all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),2,output_file);
#endif
  switch(SV)
    {
    case 4:
      temp_b_code[0]=16;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=remain_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<24;
      temp_b_code[0]|=7131;
      temp_b_code[0]=temp_b_code[0]<<8;
      temp_b_code[0]|=17;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
      for(i=0;i<1;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif
    case 3:
      temp_b_code[0]=16;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=remain_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<24;
      temp_b_code[0]|=7067;
      temp_b_code[0]=temp_b_code[0]<<8;
      temp_b_code[0]|=17;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
      for(i=0;i<1;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif
    case 2:
      temp_b_code[0]=16;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=remain_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=input_regi;
      temp_b_code[0]=temp_b_code[0]<<24;
      temp_b_code[0]|=7003;
      temp_b_code[0]=temp_b_code[0]<<8;
      temp_b_code[0]|=17;
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;
      for(i=0;i<1;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif
    default:
      break;
    }

  //call GL_Pow
  line_record+=GL_Pow(return_regi,remain_regi,return_regi,remain_regi2,SV,output_file);

  return line_record;


}

int VectorNegate(int return_regi, int input_regi, int bool_float,int SV, FILE *output_file)
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
  fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif
  return 1;
}

int GL_Clamp(int return_regi, int regi, int min_regi, int max_regi, int remain_regi, int SV, int type, FILE *output_file)
//type 0 : genType clamp (genType x, genType minVal, genType maxVal)
//type 1 : genType clamp (genType x, float minVal, float maxVal)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"CLAMP_R%d_R%d_R%d_R%d_ass_file",return_regi,regi,min_regi,max_regi);
  ass_file=fopen(temp_string,"w");
#endif

  if(type==0)
    {
      sprintf(temp_string,"\tMAXF R%d, R%d, R%d, <%d>\n",return_regi,regi,min_regi,SV);
      sprintf(temp_string,"%s\tMINF R%d, R%d, R%d, <%d>\n",temp_string,return_regi,return_regi,max_regi,SV);
    }
  else if(type==1)
    {
      sprintf(temp_string,"\tMVR R%d.1111, R%d.xxxx, <%d>\n",return_regi,min_regi,SV);
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xxxx, <%d>\n",temp_string,remain_regi,max_regi,SV);
      sprintf(temp_string,"%s\tMAXF R%d, R%d, R%d, <%d>\n",temp_string,return_regi,return_regi,regi,SV);
      sprintf(temp_string,"%s\tMINF R%d, R%d, R%d, <%d>\n",temp_string,return_regi,return_regi,remain_regi,SV);
    }
  else
    perror("GL_Clamp: type");
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  fflush(ass_file);

#endif

  //machine code

  if(type==0)
    {
      temp_b_code[0]=12;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=return_regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=regi;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=min_regi;
      temp_b_code[0]=temp_b_code[0]<<31;
      temp_b_code[0]|=15;
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

      for(i=0;i<2;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),2,output_file);
#endif
      return 2;

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
      temp_b_code[2]|=regi;
      temp_b_code[2]=temp_b_code[2]<<31;
      temp_b_code[2]|=15;
      temp_b_code[2]=temp_b_code[2]<<1;
      temp_b_code[2]|=1;
      temp_b_code[2]=temp_b_code[2]<<2;
      temp_b_code[2]|=SV-1;
      temp_b_code[2]=temp_b_code[2]<<7;
      temp_b_code[3]=temp_b_code[2];
      temp_b_code[3]&=(long long unsigned)0x07FF81FFFFFFFFFFLL;
      temp_b_code[3]|=(long long unsigned)11<<59;
      temp_b_code[3]|=(long long unsigned)remain_regi<<41;

      for(i=0;i<4;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),4,output_file);
#endif
      return 4;

    }

}

int VectorTimesScalar(int return_regi, int Vec_regi, int Sca_regi,int bool_float, int Vec_len, FILE *output_file)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"VectorTimesScalar_R%d_R%d_R%d_ass_file",return_regi,Vec_regi,Sca_regi);
  ass_file=fopen(temp_string,"w");
#endif

  if(Vec_len<1 || Vec_len>4)
    perror("VectorTimesScalar Vec_len");
  if(bool_float==1)
    {
      sprintf(temp_string,"\tMULF R%d.%d%d%d%d, R%d.xyzw, R%d.xxxx, <%d>\n",return_regi,1,Vec_len>1?1:0,Vec_len>2?1:0,Vec_len>3?1:0,Vec_regi,Sca_regi,Vec_len);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
    }
  else
    perror("VectorTimesScalar bool_float");

  fflush(ass_file);

#endif

  //machine code

  temp_b_code[0]=17;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=Vec_regi;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=Sca_regi;
  temp_b_code[0]=temp_b_code[0]<<16;
  temp_b_code[0]|=27;
  temp_b_code[0]=temp_b_code[0]<<15;
  temp_b_code[0]|=(Vec_len>3?15:Vec_len>2?14:Vec_len>1?12:8);
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=(Vec_len-1);
  temp_b_code[0]=temp_b_code[0]<<7;

  for(i=0;i<1;i++)
      all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_file);  
#endif
  return 1;
}

int GL_Pow(int return_regi,int regiX, int regiY, int remain_regi, int SV,FILE *output_file)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"POW_R%d_R%d_R%d_ass_file",return_regi,regiX,regiY);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"");
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

#endif

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
      temp_b_code[3]|=SV-1;
      temp_b_code[3]=temp_b_code[3]<<7;

      temp_b_code[4]=temp_b_code[0];
      temp_b_code[4]&=(long long unsigned)0xF81FFFFE000003FFLL;
      temp_b_code[4]|=(long long unsigned)((long long unsigned)return_regi<<53)|((long long unsigned)((24<<15)+1)<<10);

  switch(SV)
    {
    case 4:
      for(i=0;i<5;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),5,output_file);  
#endif
    case 3:
      temp_b_code[0]&=(long long unsigned)0xFFFFFFFF7FFFFFFFLL;
      temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFF7FFFFFLL;
      temp_b_code[4]&=(long long unsigned)0xFFFFFFFE000003FFLL;
      temp_b_code[4]|=(long long unsigned)((long long unsigned)((19<<15)+2)<<10);
      for(i=0;i<5;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),5,output_file);  
#endif
    case 2:
      temp_b_code[0]&=(long long unsigned)0xFFFFFFFE7FFFFFFFLL;
      temp_b_code[0]|=(long long unsigned)((long long unsigned)1<<31);
      temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFE7FFFFFLL;
      temp_b_code[2]|=(long long unsigned)((long long unsigned)1<<23);
      temp_b_code[4]&=(long long unsigned)0xFFFFFFFE000003FFLL;
      temp_b_code[4]|=(long long unsigned)((long long unsigned)((11<<15)+4)<<10);
      for(i=0;i<5;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),5,output_file);  
#endif
    case 1:
      temp_b_code[0]&=(long long unsigned)0xFFFFFFFE7FFFFFFFLL;
      temp_b_code[2]&=(long long unsigned)0xFFFFFFFFFE7FFFFFLL;
      temp_b_code[3]&=(long long unsigned)0xF81FFFFFFFFFFFFFLL;
      temp_b_code[3]|=(long long unsigned)((long long unsigned)return_regi<<53);
      for(i=0;i<4;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),4,output_file);  
#endif
    }
  return (SV-1)*5+4;
}

int GL_Dot(int return_regi,int regiX,int regiY,int SV,int write_mask,FILE *output_file)
{
  int i;

  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"DOT_R%d_R%d_R%d_ass_file",return_regi,regiX,regiY);
  ass_file=fopen(temp_string,"w");
#endif

  if(SV>=1 && SV <=4)
    sprintf(temp_string,"\tDPF R%d.%d%d%d%d, R%d, R%d, <%d>\n",return_regi,(write_mask&8)?1:0,(write_mask&4)?1:0,(write_mask&2)?1:0,(write_mask&1)?1:0,regiX,regiY,SV);
  else
    perror("GL_Dot component");
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

#endif

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

  for(i=0;i<1;i++)
      all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),1,output_file);  
#endif
  return 1;
}

int GL_Max(int return_regi,int regiX,int regiY,int SV,int type,FILE *output_file)
//type 0 : genType max (genType x, genType y)
//type 1 : genType max (genType x, float y)
{
  int i;

  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"MAX_R%d_R%d_R%d_ass_file",return_regi,regi1,regi2);
  ass_file=fopen(temp_string,"w");
#endif

  sprintf(temp_string,"");
  if(type==1)
    {
      sprintf(temp_string,"\tMVR R%d.1111, R%d.xxxx, <%d>\n",return_regi,regiY,SV);
      sprintf(temp_string,"%s\tMAXF R%d.1%d%d%d, R%d, R%d, <%d>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,regiX,return_regi,SV);
    }
  else if(type==0)
    sprintf(temp_string,"%s\tMAXF R%d.1%d%d%d, R%d, R%d, <%d>\n",temp_string,return_regi,SV>1?1:0,SV>2?1:0,SV>3?1:0,regiX,regiY,SV);
  else
    perror("GL_Max: type");

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

#endif

  //machine code

  if(type==1)
    {
      temp_b_code[0]=7;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=return_regi;
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
      temp_b_code[1]|=return_regi;
      temp_b_code[1]=temp_b_code[1]<<32;
      temp_b_code[1]|=((1<<4)|((SV>1?1:0)<<3)|((SV>2?1:0)<<2)|((SV>3?1:0)<<1)|1);
      temp_b_code[1]=temp_b_code[1]<<2;
      temp_b_code[1]|=SV-1;
      temp_b_code[1]=temp_b_code[1]<<7;

      for(i=0;i<2;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),2,output_file);
#endif
      return 2;
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
      temp_b_code[0]=temp_b_code[0]<<32;
      temp_b_code[0]|=((1<<4)|((SV>1?1:0)<<3)|((SV>2?1:0)<<2)|((SV>3?1:0)<<1)|1);
      temp_b_code[0]=temp_b_code[0]<<2;
      temp_b_code[0]|=SV-1;
      temp_b_code[0]=temp_b_code[0]<<7;

      for(i=0;i<1;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif
      return 1;
    }
}

int loadFromIMemory(int store_regi,int remain_regi,int vector_size,int address,int row_major_to_column_major,int start_address,FILE *output_file)
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
  //fwrite(temp_b_code,sizeof(long long),1,output_file);

  //temp_b_code[0]=10;
  //temp_b_code[0]=temp_b_code[0]<<6;
  //temp_b_code[0]|=store_regi;
  //temp_b_code[0]=temp_b_code[0]<<38;
  //temp_b_code[0]|=address;
  //temp_b_code[0]=temp_b_code[0]<<5;
  //temp_b_code[0]|=8;
  //temp_b_code[0]=temp_b_code[0]<<10;

  //fwrite(temp_b_code,sizeof(long long),1,output_file);





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
  temp_b_code[1]&=(long long unsigned)0xFFFF8000000043FF;
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
      temp_b_code[i*4]&=(long long unsigned)0xF81F800000007FFF;
      temp_b_code[i*4]|=(long long unsigned)(store_regi+i)<<53;
      temp_b_code[i*4+1]=temp_b_code[1];
      temp_b_code[i*4+1]&=(long long unsigned)0xF81F800000007FFF;
      temp_b_code[i*4+1]|=(long long unsigned)(store_regi+i)<<53;
      temp_b_code[i*4+2]=temp_b_code[2];
      temp_b_code[i*4+2]&=(long long unsigned)0xF81F800000007FFF;
      temp_b_code[i*4+2]|=(long long unsigned)(store_regi+i)<<53;
      temp_b_code[i*4+3]=temp_b_code[3];
      temp_b_code[i*4+3]&=(long long unsigned)0xF81F800000007FFF;
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
  fwrite(temp_b_code,sizeof(long long),4*vector_size,output_file);
#endif
  return 4*vector_size;
}

int writeToRegister(char *str,int regi,FILE *ass_file,FILE *output_file)
{
  int i;

  //assembly code

  char temp_regi=regi;
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
      fwrite(temp_b_code,sizeof(long long),16,output_file);
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
      fwrite(temp_b_code,sizeof(long long),16,output_file);
#endif
      return 16;
      
    }
}
*/

int GL_MatrixTimesVector(int matrix_reg,int matrix_reg1,int matrix_reg2,int matrix_reg3,int vector_reg,int remain_regi,int return_reg,int bool_change_matrix_vector,FILE *output_file)
{
  int i;

  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"MatTimesVec_%d_%d_ass_file",matrix_reg,vector_reg);
  ass_file=fopen(temp_string,"w");
#endif


  //writeToRegister(mat_name,matrix_reg,ass_file,output_file);
  //writeToRegister(vec_name,vector_reg,ass_file,output_file);
  sprintf(temp_string,"");
  if(bool_change_matrix_vector==1)
    {
      sprintf(temp_string,"%s\tDPF R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi,vector_reg,matrix_reg);
      sprintf(temp_string,"%s\tDPF R%d.0100, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi,vector_reg,matrix_reg1);
      sprintf(temp_string,"%s\tDPF R%d.0010, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi,vector_reg,matrix_reg2);
      sprintf(temp_string,"%s\tDPF R%d.0001, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi,vector_reg,matrix_reg3);
    }
  else
    {
      sprintf(temp_string,"%s\tDPF R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi,matrix_reg,vector_reg);
      sprintf(temp_string,"%s\tDPF R%d.0100, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi,matrix_reg1,vector_reg);
      sprintf(temp_string,"%s\tDPF R%d.0010, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi,matrix_reg2,vector_reg);
      sprintf(temp_string,"%s\tDPF R%d.0001, R%d.xyzw, R%d.xyzw, <4>\n",temp_string,remain_regi,matrix_reg3,vector_reg);
    }
      sprintf(temp_string,"%s\tMVR R%d.1111, R%d.xyzw, <%d>\n",temp_string,return_reg,remain_regi,4);

  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  fflush(ass_file);

#endif  


  //#ifndef output_ass_code

  //writeToRegister(mat_name,matrix_reg,NULL,output_file);
  //writeToRegister(vec_name,vector_reg,NULL,output_file);

  //#endif


  //machine code

  temp_b_code[0]=19;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=remain_regi;
  if(bool_change_matrix_vector==1)
    {
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=vector_reg;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=matrix_reg;
    }
  else
    {
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=matrix_reg;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=vector_reg;
    }
  temp_b_code[0]=temp_b_code[0]<<31;
  temp_b_code[0]|=0xD8D8D88;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=1;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[1]=temp_b_code[0];
  if(bool_change_matrix_vector==1)
    {
      temp_b_code[1]&=(long long)0xFFFF81FFFFFFC3FFLL;
      temp_b_code[1]|=((long long)(matrix_reg1)<<41);
    }
  else
    {
      temp_b_code[1]&=(long long)0xFFE07FFFFFFFC3FFLL;
      temp_b_code[1]|=((long long)(matrix_reg1)<<47);
    }
  temp_b_code[1]|=(1<<12);
  temp_b_code[2]=temp_b_code[0];
  if(bool_change_matrix_vector==1)
    {
      temp_b_code[2]&=(long long)0xFFFF81FFFFFFC3FFLL;
      temp_b_code[2]|=((long long)(matrix_reg2)<<41);
    }
  else
    {
      temp_b_code[2]&=(long long)0xFFE07FFFFFFFC3FFLL;
      temp_b_code[2]|=((long long)(matrix_reg2)<<47);
    }
  temp_b_code[2]|=(1<<11);
  temp_b_code[3]=temp_b_code[0];
  if(bool_change_matrix_vector==1)
    {
      temp_b_code[3]&=(long long)0xFFFF81FFFFFFC3FFLL;
      temp_b_code[3]|=((long long)(matrix_reg3)<<41);
    }
  else
    {
      temp_b_code[3]&=(long long)0xFFE07FFFFFFFC3FFLL;
      temp_b_code[3]|=((long long)(matrix_reg3)<<47);
    }
  temp_b_code[3]|=(1<<10);
  temp_b_code[4]=7;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=2;
  temp_b_code[4]=temp_b_code[4]<<6;
  temp_b_code[4]|=remain_regi;
  temp_b_code[4]=temp_b_code[4]<<22;
  temp_b_code[4]|=6939;
  temp_b_code[4]=temp_b_code[4]<<16;
  temp_b_code[4]|=31;
  temp_b_code[4]=temp_b_code[4]<<2;
  temp_b_code[4]|=3;
  temp_b_code[4]=temp_b_code[4]<<7;

  for(i=0;i<5;i++)
      BinCode[LastBinLine++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),5,output_file);
#endif
  return 5;
}

/*
int MatrixTimesMatrix(int matrix_reg,int matrix_reg2,int return_reg,int ForI,FILE *output_file)
{
  int i;
  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"MatTimesMat_%d_%d_ass_file",matrix_reg,matrix_reg2);
  ass_file=fopen(temp_string,"w");
#endif

  //writeToRegister(mat_name1,matrix_reg,ass_file,output_file);
  //writeToRegister(mat_name2,matrix_reg2,ass_file,output_file);

  sprintf(temp_string,"\tDP R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",return_reg,matrix_reg,matrix_reg2);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDP R%d.0100, R%d.xyzw, R%d.xyzw, <4>\n",return_reg,matrix_reg,matrix_reg2+1);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDP R%d.0010, R%d.xyzw, R%d.xyzw, <4>\n",return_reg,matrix_reg,matrix_reg2+2);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDP R%d.0001, R%d.xyzw, R%d.xyzw, <4>\n",return_reg,matrix_reg,matrix_reg2+3);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  sprintf(temp_string,"\tDP R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",return_reg+1,matrix_reg+1,matrix_reg2);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDP R%d.0100, R%d.xyzw, R%d.xyzw, <4>\n",return_reg+1,matrix_reg+1,matrix_reg2+1);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDP R%d.0010, R%d.xyzw, R%d.xyzw, <4>\n",return_reg+1,matrix_reg+1,matrix_reg2+2);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDP R%d.0001, R%d.xyzw, R%d.xyzw, <4>\n",return_reg+1,matrix_reg+1,matrix_reg2+3);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  sprintf(temp_string,"\tDP R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",return_reg+2,matrix_reg+2,matrix_reg2);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDP R%d.0100, R%d.xyzw, R%d.xyzw, <4>\n",return_reg+2,matrix_reg+2,matrix_reg2+1);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDP R%d.0010, R%d.xyzw, R%d.xyzw, <4>\n",return_reg+2,matrix_reg+2,matrix_reg2+2);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDP R%d.0001, R%d.xyzw, R%d.xyzw, <4>\n",return_reg+2,matrix_reg+2,matrix_reg2+3);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  sprintf(temp_string,"\tDP R%d.1000, R%d.xyzw, R%d.xyzw, <4>\n",return_reg+3,matrix_reg+3,matrix_reg2);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDP R%d.0100, R%d.xyzw, R%d.xyzw, <4>\n",return_reg+3,matrix_reg+3,matrix_reg2+1);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDP R%d.0010, R%d.xyzw, R%d.xyzw, <4>\n",return_reg+3,matrix_reg+3,matrix_reg2+2);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDP R%d.0001, R%d.xyzw, R%d.xyzw, <4>\n",return_reg+3,matrix_reg+3,matrix_reg2+3);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

#endif  


  //#ifndef output_ass_code

  //writeToRegister(mat_name1,matrix_reg,NULL,output_file);
  //writeToRegister(mat_name2,matrix_reg2,NULL,output_file);

  //#endif


  //machine code

  temp_b_code[0]=19;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_reg;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=matrix_reg;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=matrix_reg2;
  temp_b_code[0]=temp_b_code[0]<<31;
  temp_b_code[0]|=0xD8D8D88;
  temp_b_code[0]=temp_b_code[0]<<1;
  temp_b_code[0]|=ForI;
  temp_b_code[0]=temp_b_code[0]<<2;
  temp_b_code[0]|=3;
  temp_b_code[0]=temp_b_code[0]<<7;
  temp_b_code[1]=temp_b_code[0];
  temp_b_code[1]&=(long long)0xFFFF81FFFFFFC3FFLL;
  temp_b_code[1]|=((long long)(matrix_reg2+1)<<41);
  temp_b_code[1]|=(1<<12);
  temp_b_code[2]=temp_b_code[0];
  temp_b_code[2]&=(long long)0xFFFF81FFFFFFC3FFLL;
  temp_b_code[2]|=((long long)(matrix_reg2+2)<<41);
  temp_b_code[2]|=(1<<11);
  temp_b_code[3]=temp_b_code[0];
  temp_b_code[3]&=(long long)0xFFFF81FFFFFFC3FFLL;
  temp_b_code[3]|=((long long)(matrix_reg2+3)<<41);
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
  temp_b_code[4]|=((long long)(return_reg+1)<<53);
  temp_b_code[4]|=((long long)(matrix_reg+1)<<47);
  temp_b_code[5]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[5]|=((long long)(return_reg+1)<<53);
  temp_b_code[5]|=((long long)(matrix_reg+1)<<47);
  temp_b_code[6]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[6]|=((long long)(return_reg+1)<<53);
  temp_b_code[6]|=((long long)(matrix_reg+1)<<47);
  temp_b_code[7]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[7]|=((long long)(return_reg+1)<<53);
  temp_b_code[7]|=((long long)(matrix_reg+1)<<47);
  temp_b_code[8]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[8]|=((long long)(return_reg+2)<<53);
  temp_b_code[8]|=((long long)(matrix_reg+2)<<47);
  temp_b_code[9]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[9]|=((long long)(return_reg+2)<<53);
  temp_b_code[9]|=((long long)(matrix_reg+2)<<47);
  temp_b_code[10]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[10]|=((long long)(return_reg+2)<<53);
  temp_b_code[10]|=((long long)(matrix_reg+2)<<47);
  temp_b_code[11]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[11]|=((long long)(return_reg+2)<<53);
  temp_b_code[11]|=((long long)(matrix_reg+2)<<47);
  temp_b_code[12]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[12]|=((long long)(return_reg+3)<<53);
  temp_b_code[12]|=((long long)(matrix_reg+3)<<47);
  temp_b_code[13]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[13]|=((long long)(return_reg+3)<<53);
  temp_b_code[13]|=((long long)(matrix_reg+3)<<47);
  temp_b_code[14]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[14]|=((long long)(return_reg+3)<<53);
  temp_b_code[14]|=((long long)(matrix_reg+3)<<47);
  temp_b_code[15]&=(long long)0xF8007FFFFFFFFFFFLL;
  temp_b_code[15]|=((long long)(return_reg+3)<<53);
  temp_b_code[15]|=((long long)(matrix_reg+3)<<47);

  for(i=0;i<16;i++)
      all_temp_b[num_temp_b++]=temp_b_code[i];

#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),16,output_file);
#endif
  return 16; 
}

int Normalize(int vector_reg,int remain_reg,int remain_reg2,int return_reg,char *vec_name,FILE *output_file)
{
  int i;

  //assembly code
  FILE *ass_file;
#ifdef output_ass_code
#ifdef combine_ass_code
  sprintf(temp_string,"%s",combine_ass_name);
  ass_file=fopen(temp_string,"a");
#else
  sprintf(temp_string,"Normalize_%d_%d_ass_file",vector_reg,remain_reg);
  ass_file=fopen(temp_string,"w");
#endif
  int temp_vector=vector_reg;
  //writeToRegister(vec_name,vector_reg,ass_file,output_file);
  if(temp_vector>=32)
    {
      sprintf(temp_string,"\tMVR R%d.1111, R%d.xyzw, <4>\n",remain_reg2,vector_reg);
      fputs(temp_string,ass_file);
      bzero(temp_string,sizeof(temp_string));
      vector_reg=remain_reg2;
    }
  sprintf(temp_string,"\tMVR R%d.1111, R%d.xyzw, <4>\n",return_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tLDI R%d.0100, 2\n",remain_reg);//(MVR now ,need to fix)\n",remain_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tLOG2 R%d.1000, R%d\n",vector_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tMVR R%d.1000, R%d.yyzw, <4>\n",remain_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tLOG2 R%d.0100, R%d\n",vector_reg,remain_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tMVR R%d.1000, R%d.zyzw, <4>\n",remain_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tLOG2 R%d.0010, R%d\n",vector_reg,remain_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tMVR R%d.1000, R%d.wyzw, <4>\n",remain_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tLOG2 R%d.0001, R%d\n",vector_reg,remain_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tMULF R%d, R%d.xyzw, R%d.yyyy\n",vector_reg,vector_reg,remain_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tPOW2 R%d.1000, R%d\n",vector_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tMVR R%d.1000, R%d.yyzw, <4>\n",remain_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tPOW2 R%d.0100, R%d\n",vector_reg,remain_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tMVR R%d.1000, R%d.zyzw, <4>\n",remain_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tPOW2 R%d.0010, R%d\n",vector_reg,remain_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tMVR R%d.1000, R%d.wyzw, <4>\n",remain_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tPOW2 R%d.0001, R%d\n",vector_reg,remain_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tADDF R%d.1000, R%d.xyzw, R%d.yyzw, <4>\n",vector_reg,vector_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tADDF R%d.1000, R%d.xyzw, R%d.zyzw, <4>\n",vector_reg,vector_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tADDF R%d.1000, R%d.xyzw, R%d.wyzw, <4>\n",vector_reg,vector_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tMVR R%d.1111, R%d.xyzw, <4>\n",remain_reg,return_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDSQ R%d.1000, R%d\n",return_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tMVR R%d.1000, R%d.yyzw, <4>\n",remain_reg,return_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDSQ R%d.1000, R%d\n",remain_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tMVR R%d.0100, R%d.xxzw, <4>\n",return_reg,remain_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tMVR R%d.1000, R%d.zyzw, <4>\n",remain_reg,return_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDSQ R%d.1000, R%d\n",remain_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tMVR R%d.0010, R%d.xyxw, <4>\n",return_reg,remain_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tMVR R%d.1000, R%d.wyzw, <4>\n",remain_reg,return_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tDSQ R%d.1000, R%d\n",remain_reg,vector_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));
  sprintf(temp_string,"\tMVR R%d.0001, R%d.xyzx, <4>\n",return_reg,remain_reg);
  fputs(temp_string,ass_file);
  bzero(temp_string,sizeof(temp_string));

  fflush(ass_file);

#endif  

#ifdef output_ass_code

  writeToRegister(vec_name,vector_reg,NULL,output_file);

#endif

  //machine code

  unsigned long long temp;

  if(temp_vector>=32)
    {
      temp_b_code[0]=7;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=remain_reg2;
      temp_b_code[0]=temp_b_code[0]<<6;
      temp_b_code[0]|=temp_vector;
      temp_b_code[0]=temp_b_code[0]<<22;
      temp_b_code[0]|=0x1B1B;
      temp_b_code[0]=temp_b_code[0]<<15;
      temp_b_code[0]|=0xF;
      temp_b_code[0]=temp_b_code[0]<<3;
      temp_b_code[0]|=3;
      temp_b_code[0]=temp_b_code[0]<<7;

      for(i=0;i<1;i++)
	all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
      fwrite(temp_b_code,sizeof(long long),1,output_file);
#endif
    }

  temp_b_code[0]=7;
  temp_b_code[0]=temp_b_code[0]<<6;
  temp_b_code[0]|=return_reg;
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

  temp_b_code[11]=temp_b_code[3];
  temp_b_code[13]=temp_b_code[5];
  temp_b_code[15]=temp_b_code[7];

  temp&=(long long unsigned)0xFFFFFFFE01FFC3FFLL;
  temp|=((long long unsigned)(0x1B)<<25);
  temp|=(15<<10);
  temp_b_code[20]=temp;

  temp=temp_b_code[11];
  temp&=(long long unsigned)0xFFE07FFFFFFFFDFFLL;
  temp|=((long long unsigned)return_reg<<47);
  temp|=((long long unsigned)1<<9);
  temp_b_code[22]=temp;

  temp&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
  temp|=((long long unsigned)(0x9B)<<25);
  temp_b_code[25]=temp;

  temp&=(long long unsigned)0xFFFFFFFE01FFFFFFLL;
  temp|=((long long unsigned)(0xDB)<<25);
  temp_b_code[28]=temp;

  temp&=(long long unsigned)0xF81FFFFE01FFC3FFLL;
  temp|=((long long unsigned)return_reg<<53);
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


  float temp_float=2.0;
  unsigned int temp_int=2;
  //memcpy(&temp_int,&temp_float,4);



  //printf("temp_float:%f\n",temp_float);
  //printf("temp_int:%f\n",temp_int);
  //printf("temp_int:%d\n",temp_int);

  //need to fix

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


  //temp_b_code[1]=10;
  //temp_b_code[1]=temp_b_code[1]<<6;
  //temp_b_code[1]|=remain_reg;
  //temp_b_code[1]=temp_b_code[1]<<38;
  //temp_b_code[1]|=temp_int;
  //temp_b_code[1]=temp_b_code[1]<<6;
  //temp_b_code[1]|=8;
  //temp_b_code[1]=temp_b_code[1]<<9;

  temp_b_code[2]=4;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=vector_reg;
  temp_b_code[2]=temp_b_code[2]<<6;
  temp_b_code[2]|=vector_reg;
  temp_b_code[2]=temp_b_code[2]<<38;
  temp_b_code[2]|=17;
  temp_b_code[2]=temp_b_code[2]<<9;
  temp=temp_b_code[2];

  temp&=(long long unsigned)0xFFE07FFFFFFFC3FFLL;
  temp|=((long long unsigned)5<<47);
  temp|=(4<<10);
  temp_b_code[4]=temp;

  temp&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
  temp|=(2<<10);
  temp_b_code[6]=temp;

  temp&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
  temp|=(1<<10);
  temp_b_code[8]=temp;

  temp_b_code[9]=17;
  temp_b_code[9]=temp_b_code[9]<<6;
  temp_b_code[9]|=vector_reg;
  temp_b_code[9]=temp_b_code[9]<<6;
  temp_b_code[9]|=vector_reg;
  temp_b_code[9]=temp_b_code[9]<<6;
  temp_b_code[9]|=remain_reg;
  temp_b_code[9]=temp_b_code[9]<<24;
  temp_b_code[9]|=0x1B55;
  temp_b_code[9]=temp_b_code[9]<<7;
  temp_b_code[9]|=15;
  temp_b_code[9]=temp_b_code[9]<<3;
  temp_b_code[9]|=7;
  temp_b_code[9]=temp_b_code[9]<<7;

  temp_b_code[10]=5;
  temp_b_code[10]=temp_b_code[10]<<6;
  temp_b_code[10]|=vector_reg;
  temp_b_code[10]=temp_b_code[10]<<6;
  temp_b_code[10]|=vector_reg;
  temp_b_code[10]=temp_b_code[10]<<38;
  temp_b_code[10]|=17;
  temp_b_code[10]=temp_b_code[10]<<9;
  temp=temp_b_code[10];
  
  temp&=(long long unsigned)0xFFE07FFFFFFFC3FFLL;
  temp|=((long long unsigned)5<<47);
  temp|=(4<<10);
  temp_b_code[12]=temp;

  temp&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
  temp|=(2<<10);
  temp_b_code[14]=temp;

  temp&=(long long unsigned)0xFFFFFFFFFFFFC3FFLL;
  temp|=(1<<10);
  temp_b_code[16]=temp;
  
  temp_b_code[17]=16;
  temp_b_code[17]=temp_b_code[17]<<6;
  temp_b_code[17]|=vector_reg;
  temp_b_code[17]=temp_b_code[17]<<6;
  temp_b_code[17]|=vector_reg;
  temp_b_code[17]=temp_b_code[17]<<6;
  temp_b_code[17]|=vector_reg;
  temp_b_code[17]=temp_b_code[17]<<24;
  temp_b_code[17]|=0x1B5B;
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
  temp_b_code[21]|=return_reg;
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
  
  for(i=0;i<31;i++)
      all_temp_b[num_temp_b++]=temp_b_code[i];
#ifdef test_only
  fwrite(temp_b_code,sizeof(long long),31,output_file);
#endif
  return 31;
}
*/
/*
int main()
{
  FILE *out=fopen("binary","w");


  //GL_Max(23,13,5,4,1,out);
  //GL_Max(23,13,5,4,0,out);
  //GL_Dot(23,13,5,3,13,out);
  //GL_Pow(13,14,16,20,4,out);
  //VectorTimesScalar(8,26,7,1,3,out);
  //GL_Clamp(5,18,24,10,17,3,0,out);
  //GL_Clamp(5,18,24,10,17,3,1,out);
  //VectorNegate(6,11,1,2,out);
  //VectorNegate(6,11,0,4,out);
  //GL_Length(18,11,29,30,3,out);
  //GL_Reflect(8,10,5,14,1,out);

  //GL_Radians(6,4,15,3,out);
  //GL_Degree(7,10,21,2,out);
  //GL_Exp(8,16,6,22,2,out);
  //GL_Log(12,27,16,3,out);



  //GL_Exp2(19,31,out);
  //GL_Log2(20,30,out);
  //GL_Sqrt(6,6,18,out);
  //GL_InverseSqrt(8,29,out);
  //GL_Abs(6,11,3,out);
  //GL_Sign(12,10,2,out);
  //GL_Floor(4,28,1,out);
  //GL_Ceil(5,29,4,out);



  return 0;
}
*/
