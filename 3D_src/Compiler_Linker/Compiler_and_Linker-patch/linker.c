#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <sys/mman.h>
#include "inlineFunction.h"
//#include "sct_table.h"
#ifdef COMPILER_STANDALONE
#include "APIContext.h"
#else
#include "../context.h"
#endif
#include "link_var.h"
#include "linker.h"
#include "instructions.h"

#define MaxVarSize 40000
#define MaxProgSize 65536
#define VERT_SPACE 64*1024
#define FRAG_SPACE 64*1024

#define detect_error
//#define debug_information
#define standalone
#define compiler_standalone
//#define linker_debug_flag
#ifdef unified_shader_register
#define uniform_for_unified_shader
#define VS_DMEM_SET  //billy VertexAddress change 0 to 1024
#define attribute_varying_beginning_address 24
#else
#define attribute_varying_beginning_address 32
#endif
//#define IMM_FOR_LD

#define VAR_INFO
//#define VAR_INFO_2
//#define LoadAttributeFromSCT
#define test_mmap

#ifdef test_mmap
#include <sys/types.h>
#include <unistd.h>
#endif

#define printable
//#define readable
#ifdef readable
#define emit(X) printinst(X)
#else
#define emit(X) {input_buffer(X); if(LastBinLine==MaxProgSize) {printf("Error. Increase MaxProgSize\n"); po->LinkerStatus = GL_FALSE;exit (1);}}
#endif
#define N(X) ((X==0)?"":"-")
#define InTy(X) ((X==0)?'I':'F')
#define perr(X) {printf("%s error: %s\n",decode_line[0],X);po->LinkerStatus = GL_FALSE;exit(1);}

#ifdef assembler_standalone
#define PRINT_INSTRUCTION_GUILD { printf("assembler [flag] [input file] [output file]\n\n\nflag:\n-simple: Test the simple assembly file.\n\ninput file:\nInput file name.\n\noutput file:\nOutput file name.\n\n");}
#endif

//#define getSamplerIndex //now drop it. just use Opt_getSamplerIndex
#ifdef getSamplerIndex //discard this function
int GetSamplerIndex(char input_type);
char sampler_id[128][128]; //Kaymon add to record the sampler id.
#endif
#define Opt_getSamplerIndex
#define edward_work

void print_table();
void print_variable();
void print_variable2();
int used_uniform(int VOrF);
int used_varying();
int used_attribute();
void input_buffer(long long unsigned int x);

void type_1(int input_class,int argu_number,int F);
void type_2(int input_class,int argu_number,int F);
void type_3(int input_class,int argu_number,int F);
void type_4(int input_class,int argu_number,int F,char VOrF);
void type_5(int input_class,int argu_number,int F);

long long unsigned int BinCode[MaxProgSize];
int LastBinLine=0;
long long unsigned int nowBin=0;
char VarCode[MaxVarSize],nowVar=0;
int LastVarLine=0,buffer_count=0,buffer_remained_bit=64,end_buffer=0,end_bigger_than_54=0;
int float_of_imm=0,PC=0;
int CurrentAlignmentNumber=0;
char nowRead[200],t_read[200],decode_line[9][512];
char table[300][6][60];
char now_file[30];
int table_count=0,address=0;
FILE *pWBinary;
int regular_variable_address=-1;
int uniform_number=-1;

struct programobject *po;
#ifdef assembler_standalone
struct varying_form v_form;
#else
struct varying_form *v_form;
#endif
struct uniform_form *u_form;
struct attribute_form *a_form;
int varying_count=1; //Because r32 should be gl_position.

struct variable_form r_form[256]; //global variable
struct variable_form r_form2[256]; //noninitialize_global variable
int variable_count=0;
struct variable_form *p_first_r_form=&r_form[0];
struct variable_form *p_first_r_form2=&r_form2[0];


#ifdef uniform_for_unified_shader  //Jimmy modify     
struct uniform_form temp_u_form;   //In order to modify uniform_for_unified_shader, hence variable change to global
#endif

struct assembly_element
{
  int argu_num;
  int bool_mark_case;
  int regi[4];
  int reg_num;
  int swizzle[3];
  int wm;
  int scc;
  int imm;
  int ide_imm;
  int imm_searched;
  int imm_sign;
  int neg;
  int neg_change;
  int sv;
  int shift_amount;
}temp_assembly;

void print_element(){
  
  printf("================element===============\n");
  printf("argu_num:%d\n",temp_assembly.argu_num);
  printf("bool_mark_case:%d\n",temp_assembly.bool_mark_case);
  printf("regi[0]:%d\n",temp_assembly.regi[0]);
  printf("regi[1]:%d\n",temp_assembly.regi[1]);
  printf("regi[2]:%d\n",temp_assembly.regi[2]);
  printf("regi[3]:%d\n",temp_assembly.regi[3]);
  printf("reg_num:%d\n",temp_assembly.reg_num);
  printf("swizzle[0]:%d\n",temp_assembly.swizzle[0]);
  printf("swizzle[1]:%d\n",temp_assembly.swizzle[1]);
  printf("swizzle[2]:%d\n",temp_assembly.swizzle[2]);
  printf("swizzle[3]:%d\n",temp_assembly.swizzle[3]);
  printf("wm:%d\n",temp_assembly.wm);
  printf("scc:%d\n",temp_assembly.scc);
  printf("imm:%d\n",temp_assembly.imm);
  printf("ide_imm:%d\n",temp_assembly.ide_imm);
  printf("imm_searched:%d\n",temp_assembly.imm_searched);
  printf("imm_sign:%d\n",temp_assembly.imm_sign);
  printf("neg:%d\n",temp_assembly.neg);
  printf("neg_change:%d\n",temp_assembly.neg_change);
  printf("sv:%d\n",temp_assembly.sv);
  printf("shift_amount:%d\n",temp_assembly.shift_amount);

}

void print_uniform();
void print_varying();
/*
Fill long into the machine code file.
*/
void input_long(int input)
{
  int temp_input=input;
  char temp_var;
  memcpy(&temp_var,&temp_input,1);
  VarCode[LastVarLine]=temp_input;
  LastVarLine++;
  temp_input=(input<<16)>>24;
  memcpy(&temp_var,&temp_input,1);
  VarCode[LastVarLine]=temp_input;
  LastVarLine++;
  temp_input=(input<<8)>>24;
  memcpy(&temp_var,&temp_input,1);
  VarCode[LastVarLine]=temp_input;
  LastVarLine++;
  temp_input=input>>24;
  memcpy(&temp_var,&temp_input,1);
  VarCode[LastVarLine]=temp_input;
  LastVarLine++;
#ifdef printable
  FILE *pfile=fopen("opcode_and_binary","a");
  fprintf(pfile,"PC:%6d\t%08x",PC,input);
  fprintf(pfile,"%s",nowRead);
  fclose(pfile);
#endif
  PC+=4;
}

/*
Fill short into machine code file.
*/
void input_short(short input)
{
  short temp_input=input;
  char temp_var;
  memcpy(&temp_var,&temp_input,1);
  VarCode[LastVarLine]=temp_input;
  LastVarLine++;
  temp_input=input>>8;
  memcpy(&temp_var,&temp_input,1);
  VarCode[LastVarLine]=temp_input;
  LastVarLine++;

#ifdef printable
  FILE *pfile=fopen("opcode_and_binary","a");
  fprintf(pfile,"PC:%6d\t%04x\t",PC,input);
  fprintf(pfile,"%s",nowRead);
  fclose(pfile);
#endif
  PC+=2;
}

void input_byte(char input)
{
  char temp_input=input;
  char temp_var;
  memcpy(&temp_var,&temp_input,1);
  VarCode[LastVarLine]=temp_input;
  LastVarLine++;

#ifdef printable
  FILE *pfile=fopen("opcode_and_binary","a");
  fprintf(pfile,"PC:%6d\t%04x\t",PC,input);
  fprintf(pfile,"%s",nowRead);
  fclose(pfile);
#endif
  PC+=1;
}
void BackToMainFunction(char type){
  //Load from the address of PC_count. PC_count save the the next one instrunction of caller.
  nowBin=0;
  nowBin=8;
  nowBin=nowBin<<6;
  nowBin|=3;
  nowBin=nowBin<<38;
  if(type == 'v')
    nowBin|=uniform_number;
  else
    nowBin|=FS_DMEM_START+1; //1024 => gl_coord
  nowBin=nowBin<<5;
  nowBin|=15;
  nowBin=nowBin<<3;
  nowBin|=3;
  nowBin=nowBin<<7;
  emit(nowBin);
  //Ret instrustio. e.g. RET R3.
  nowBin=0;
  nowBin=29;
  nowBin=nowBin<<12;
  nowBin|=3;
  nowBin=nowBin<<47;
  emit(nowBin);
}
void SavePCCountandJumpToUserDefiningFunction(char type){
  nowBin = 0;
  nowBin=10; //load pc_count to temp register. LDI instructions.
  nowBin=nowBin<<6;
  nowBin|=3;
  nowBin=nowBin<<38;
  nowBin|=(PC+24)/8; //translate to real PC count.
  nowBin=nowBin<<5;
  nowBin|=8;    
  nowBin=nowBin<<10;
  emit(nowBin);
    
  nowBin = 0;
  nowBin=9; //store the pc to address of PC_count in the variable table.
  nowBin=nowBin<<6;
  nowBin|=0;
  nowBin=nowBin<<6;
  nowBin|=3;
  nowBin=nowBin<<32;
  if(type == 'v')
    nowBin|=uniform_number;
  else
    nowBin|=FS_DMEM_START+1; //1024 => gl_coord //1025 => PC_count
  nowBin=nowBin<<5;
  nowBin|=15;
  nowBin=nowBin<<3;
  nowBin|=3;
  nowBin=nowBin<<7;
  emit(nowBin);

  nowBin = 0;
  //nowBin=24; //jr function name.
  nowBin=29; //jmp function name.
  nowBin=nowBin<<OPCODE_BIT;
  type_4(6,temp_assembly.argu_num,0,type);

}

void callinlinefunction(char type,struct programobject *input_object)
{
  //printf("decode_line: %s\n",decode_line[1]);
  FILE *out=fopen("binary","w");

  int i;
  char *p_tok,fun_name[64],temp_fun_name[64];
  strcpy(fun_name,decode_line[1]);
  p_tok=strtok(fun_name,".");
  //printf("fist_tok: %s\n",p_tok);
  strcpy(temp_fun_name,fun_name);
  //printf("temp_tok: %s\n",temp_fun_name);
  if(strcmp(p_tok,"OpenGLES")==0)
    {
      p_tok=strtok(NULL,".");
      //printf("p_tok: %s\n",p_tok);
      strcpy(temp_fun_name,p_tok);
      //printf("temp_tok: %s\n",temp_fun_name);
      if(strncmp(temp_fun_name,"LoadAttribute",13)==0)
	{
	  p_tok=strtok(NULL,".");
	  //printf("p_tok: %s\n",p_tok);

	  //if(strcmp(p_tok,"rm_Vertex")==0)
	  //GL_LoadAttribute(32,4,NULL);
	  //else
	    {
	      int b_search=0;
	      for(i=0;i<8;i++)
		{
		  //printf("%s\t%s\n",a_form->att_name[i],p_tok);
		  if(a_form->index[i]==-1)
		    {
		      /*
		      strcpy(a_form->att_name[i],p_tok);
		      a_form->index[i]=i;
		      GL_LoadAttribute(32+i,atoi(&temp_fun_name[13]),NULL);
		      */
		      //perr("No this attribute!!");
		      continue;
		    }
		  else if(strcmp(a_form->att_name[i],p_tok)==0)
		    {
		      b_search=1;
      		      GL_LoadAttribute(attribute_varying_beginning_address+i,atoi(&temp_fun_name[13]),NULL);
		      break;
		    }
		  //else if(a_form->index[i]==7)
		  //{
		  //perr("The number of attribute is more than 8");
		  //}
		}
	      if(!b_search && i>=8)
		{
		  int b_add=0;
		  for(i=0;i<8;i++)
		    {
		      if(a_form->index[i]==-1)
			{
			  strcpy(a_form->att_name[i],p_tok);
			  a_form->index[i]=i;
			  char *new_name=(char*)malloc(sizeof(strlen(p_tok)+1));
			  strcpy(new_name,p_tok);
			  input_object->attribute_name[i]=new_name;
			  //printf("a_form.att_name[%d]: %s\n",i,a_form->att_name[i]);
			  GL_LoadAttribute(attribute_varying_beginning_address+i,atoi(&temp_fun_name[13]),NULL);
			  b_add=1;
			  break;
			}
		      else
			continue;
		    }
		  if(!b_add && i>=8)
		    perr("More than 8 attribute");
		}
	    }
	}
      else if(strcmp(temp_fun_name,"Start")==0)
	{
	  //print_variable();
	  GL_Start(NULL,type);
	}
      else if(strcmp(temp_fun_name,"CallRegister")==0 || strcmp(temp_fun_name,"CallFloatRegister")==0 )
	{
	  GL_CallRegister(NULL);
	}
      else if(strncmp(temp_fun_name,"ChangeMatrix",12)==0)
	{
	  p_tok=strtok(NULL,".");
	  //p_tok=strtok(NULL,".");
	  //printf("hi: %d\n",*p_tok);
	  switch(atoi(&temp_fun_name[12]))
	    {
	    case 4:
	      GL_ChangeMatrix(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,ARGU_REGI+3,OUR_TEMP_REGI,*p_tok-48,atoi(&temp_fun_name[12]),NULL);
	      break;
	    case 3:
	      GL_ChangeMatrix(RETU_REGI,0,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,OUR_TEMP_REGI,*p_tok-48,atoi(&temp_fun_name[12]),NULL);
	      //GL_ChangeMatrix(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,0,OUR_TEMP_REGI,*p_tok-48,atoi(&temp_fun_name[12]),NULL);
	      break;
	    case 2:
	      GL_ChangeMatrix(RETU_REGI,0,0,ARGU_REGI,ARGU_REGI+1,OUR_TEMP_REGI,*p_tok-48,atoi(&temp_fun_name[12]),NULL);
	      //GL_ChangeMatrix(RETU_REGI,ARGU_REGI,ARGU_REGI+1,0,0,OUR_TEMP_REGI,*p_tok-48,atoi(&temp_fun_name[12]),NULL);
	      break;
	    case 1:
	      GL_ChangeMatrix(RETU_REGI,0,0,0,ARGU_REGI,OUR_TEMP_REGI,*p_tok-48,atoi(&temp_fun_name[12]),NULL);
	      //GL_ChangeMatrix(RETU_REGI,ARGU_REGI,0,0,0,OUR_TEMP_REGI,*p_tok-48,atoi(&temp_fun_name[12]),NULL);
	      break;
	    default:
	      perr("ChangeMatrix function name is error.");
	      break;
	    }
	}
      else if(strncmp(temp_fun_name,"VectorTimesMatrix",17)==0)
	{
	  p_tok=strtok(NULL,".");
	  //printf("VTM: %s\n",p_tok);
	  //GL_MatrixTimesVector(6,5,4,3,2,7,2,1,atoi(&temp_fun_name[17]),out);
	  if(atoi(&temp_fun_name[17])==4)
	    GL_MatrixTimesVector(RETU_REGI,ARGU_REGI+4,ARGU_REGI+3,ARGU_REGI+2,ARGU_REGI+1,ARGU_REGI,ARGU_REGI+5,1,atoi(&temp_fun_name[17]),out);
	  else if(atoi(&temp_fun_name[17])==3)
	    GL_MatrixTimesVector(RETU_REGI,ARGU_REGI+3,ARGU_REGI+2,ARGU_REGI+1,0,ARGU_REGI,ARGU_REGI+4,1,atoi(&temp_fun_name[17]),out);
	  else if(atoi(&temp_fun_name[17])==2)
	    GL_MatrixTimesVector(RETU_REGI,ARGU_REGI+2,ARGU_REGI+1,0,0,ARGU_REGI,ARGU_REGI+3,1,atoi(&temp_fun_name[17]),out);
	  else if(atoi(&temp_fun_name[17])==1)
	    GL_MatrixTimesVector(RETU_REGI,ARGU_REGI+1,0,0,0,ARGU_REGI,ARGU_REGI+2,1,atoi(&temp_fun_name[17]),out);
	  

	}
      else if(strncmp(temp_fun_name,"MatrixTimesVector",17)==0)
	{
	  p_tok=strtok(NULL,".");
	  //printf("VTM: %s\n",p_tok);
	  //GL_MatrixTimesVector(6,5,4,3,2,7,2,0,atoi(&temp_fun_name[17]),out);
	  if(atoi(&temp_fun_name[17])==4)
	    GL_MatrixTimesVector(RETU_REGI,ARGU_REGI+3,ARGU_REGI+2,ARGU_REGI+1,ARGU_REGI,ARGU_REGI+4,OUR_TEMP_REGI,0,atoi(&temp_fun_name[17]),out);
	  else if(atoi(&temp_fun_name[17])==3)
	    GL_MatrixTimesVector(RETU_REGI,ARGU_REGI+2,ARGU_REGI+1,ARGU_REGI,0,ARGU_REGI+3,OUR_TEMP_REGI,0,atoi(&temp_fun_name[17]),out);
	  else if(atoi(&temp_fun_name[17])==2)
	    GL_MatrixTimesVector(RETU_REGI,ARGU_REGI+1,ARGU_REGI,0,0,ARGU_REGI+2,OUR_TEMP_REGI,0,atoi(&temp_fun_name[17]),out);
	  else if(atoi(&temp_fun_name[17])==1)
	    GL_MatrixTimesVector(RETU_REGI,ARGU_REGI,0,0,0,ARGU_REGI+1,OUR_TEMP_REGI,0,atoi(&temp_fun_name[17]),out);
	  //GL_MatrixTimesVector(RETU_REGI,ARGU_REGI+4,ARGU_REGI+3,ARGU_REGI+2,ARGU_REGI+1,ARGU_REGI,ARGU_REGI+5,0,atoi(&temp_fun_name[17]),out);
	}
      else if(strcmp(temp_fun_name,"END")==0)
	{
	  if(varying_count<=max_number_of_varying_variables) //The max varying variables are 8. But gl_position is the first one. Maybe we should extense the numbers of varying variables.
	    {
#ifdef assembler_standalone
	      strcpy(v_form.vary_name[0],"gl_Position\0");
	      v_form.index[0]=0;
#else
	      strcpy(v_form->vary_name[0],"gl_Position\0");
	      v_form->index[0]=0;
#endif
	      //printf("hi END!!\n");
	      GL_END(attribute_varying_beginning_address,NULL);
	      //varying_count++;//Initialize 1 to varying_count. So don't count again.
	    }
	  else 
	    {
	      perr("The number of varying is more than 8");
	    }
	}
      else if(strncmp(temp_fun_name,"ENDFrag",7)==0)
	{
	  GL_ENDFrag(NULL);
	}
      else if(strncmp(temp_fun_name,"VaryingPut",10)==0)
	{
	  p_tok=strtok(NULL,".");
	  for(i=1;i<max_number_of_varying_variables ;i++)
	    {
#ifdef assembler_standalone	      
	      if(strcmp(v_form.vary_name[i],p_tok)==0)
		{
		  GL_VaryingPut(attribute_varying_beginning_address+i,atoi(&temp_fun_name[10]),NULL);
		  break;
		}
	      if(v_form.index[i]==-1)
		{
		  varying_count++;
		  strcpy(v_form.vary_name[i],p_tok);
		  v_form.index[i]=i;
		  //printf("temp_fun_name[10]: %d\n",atoi(&temp_fun_name[10]));
		  GL_VaryingPut(attribute_varying_beginning_address+i,atoi(&temp_fun_name[10]),NULL);
		  break;
		}
	      else if(v_form.index[i]==max_number_of_varying_variables-1)
		{
		  perr("The number of varying is more than 8");
		}
#else
	      if(strcmp(v_form->vary_name[i],p_tok)==0)
		{
		  GL_VaryingPut(attribute_varying_beginning_address+i,atoi(&temp_fun_name[10]),NULL);
		  break;
		}
	      if(v_form->index[i]==-1)
		{
		  varying_count++;
		  //printf("p_tok: %s  i:%d %d\n",p_tok,i,attribute_varying_beginning_address+i);
		  strcpy(v_form->vary_name[i],p_tok);
		  v_form->index[i]=i;
		  //printf("i: %d\ttemp_fun_name[10]: %d\n",i,atoi(&temp_fun_name[10]));
		  GL_VaryingPut(attribute_varying_beginning_address+i,atoi(&temp_fun_name[10]),NULL);
		  break;
		}
	      else if(v_form->index[i]==max_number_of_varying_variables-1)
		{
		  perr("The number of varying is more than 8");
		}
#endif
	    }
	}
      else if(strncmp(temp_fun_name,"LoadVarying",11)==0)
	{
	  p_tok=strtok(NULL,".");
	  for(i=0;i<max_number_of_varying_variables;i++)
	    {
#ifdef assembler_standalone
	      if(strcmp(v_form.vary_name[i],p_tok)==0)
#else
	      if(strcmp(v_form->vary_name[i],p_tok)==0)
#endif
		break;
	      if(i==max_number_of_varying_variables-1)
		perr("No this varying");
	    }
	  GL_LoadVarying(attribute_varying_beginning_address+i);
	}
      else if(strncmp(temp_fun_name,"VectorTimesScalar",17)==0)
	{
	  GL_VectorTimesScalar(ARGU_REGI,ARGU_REGI+1,1,atoi(&decode_line[3][5]),NULL);
	}
      else if(strncmp(temp_fun_name,"ScalarTimesVector",17)==0)
	{
	  GL_VectorTimesScalar(ARGU_REGI+1,ARGU_REGI,1,atoi(&decode_line[3][5]),NULL);
	}
      else if(strncmp(temp_fun_name,"FloatDiv",8)==0)
	{
	  GL_FloatDiv(ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,ARGU_REGI+3,atoi(&temp_fun_name[8]),NULL);
	}
      else if(strncmp(temp_fun_name,"Normalize",8)==0)
	{
	  p_tok=strtok(NULL,".");
	  int vector=4; 
	  if(p_tok)
	    {
	      if(strncmp(p_tok,"f",1)==0)
		vector=1;
	      else if(strncmp(p_tok,"vec",3)==0)
		{
		  int temp_vector=atoi(&p_tok[3]);
		  switch(temp_vector)
		    {
		    case 2:
		    case 3:
		    case 4:
		      vector=temp_vector;
		      break;
		    default:
		      perr("Normalize has bad arguments.");
		    }
		}
	    }
	  GL_Normalize(ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,RETU_REGI,vector,NULL);
	}
      else if(strncmp(temp_fun_name,"Max",3)==0)
	{
	  p_tok=strtok(NULL,".");
	  int vector=4; 
	  int type=0;
	  if(p_tok)
	    {
	      if(strncmp(p_tok,"f",1)==0)
		vector=1;
	      else if(strncmp(p_tok,"vec",3)==0)
		{
		  int temp_vector=atoi(&p_tok[3]);
		  switch(temp_vector)
		    {
		    case 2:
		    case 3:
		    case 4:
		      vector=temp_vector;
		      break;
		    default:
		      perr("Max has bad arguments.");
		    }
		  p_tok=strtok(NULL,".");

		  if(!p_tok)
		    perr("Max has bad arguments.");

		  if(strncmp(p_tok,"f",1)==0)
		    {
		      if(vector!=1)
			type=1;
		    }
		  else if(strncmp(p_tok,"vec",3)==0)
		    {
		      temp_vector=atoi(&p_tok[3]);

		      switch(temp_vector)
			{
			case 2:
			case 3:
			case 4:
			  if(vector==temp_vector)
			    break;
			default:
			  perr("Max has bad arguments.");
			}
		    }
		  else
		    perr("Max has bad arguments.");
		}
	    }
	  //printf("max type:%d\n",type);
	  //GL_Max(RETU_REGI,ARGU_REGI,ARGU_REGI+1,vector,type,NULL);
	  GL_Max(RETU_REGI,ARGU_REGI,ARGU_REGI+1,OUR_TEMP_REGI,vector,type,NULL); //Kaymon fix it
	}
      else if(strncmp(temp_fun_name,"Dot",3)==0)
	{
	  p_tok=strtok(NULL,".");
	  int vector=4; 
	  if(p_tok)
	    {
	      if(strncmp(p_tok,"f",1)==0)
		vector=1;
	      else if(strncmp(p_tok,"vec",3)==0)
		{
		  int temp_vector=atoi(&p_tok[3]);
		  switch(temp_vector)
		    {
		    case 2:
		    case 3:
		    case 4:
		      vector=temp_vector;
		      break;
		    default:
		      perr("Dot has bad arguments.");
		    }
		  p_tok=strtok(NULL,".");

		  if(!p_tok)
		    perr("Dot has bad arguments.");

		  if(strncmp(p_tok,"f",1)==0)
		    {
		      if(vector!=1)
			perr("Dot has bad arguments.");
		    }
		  else if(strncmp(p_tok,"vec",3)==0)
		    {
		      temp_vector=atoi(&p_tok[3]);

		      switch(temp_vector)
			{
			case 2:
			case 3:
			case 4:
			  if(vector==temp_vector)
			    break;
			default:
			  perr("Dot has bad arguments.");
			}
		    }
		  else
		    perr("Dot has bad arguments.");
		}
	    }
	  GL_Dot(RETU_REGI,ARGU_REGI,ARGU_REGI+1,vector,NULL);
	}
      else if(strncmp(temp_fun_name,"Pow",3)==0)
	{
	  p_tok=strtok(NULL,".");
	  int vector=4; 
	  if(p_tok)
	    {
	      if(strncmp(p_tok,"f",1)==0)
		vector=1;
	      else if(strncmp(p_tok,"vec",3)==0)
		{
		  int temp_vector=atoi(&p_tok[3]);
		  switch(temp_vector)
		    {
		    case 2:
		    case 3:
		    case 4:
		      vector=temp_vector;
		      break;
		    default:
		      perr("Pow has bad arguments.");
		    }
		  p_tok=strtok(NULL,".");

		  if(!p_tok)
		    perr("Pow has bad arguments.");

		  if(strncmp(p_tok,"f",1)==0)
		    {
		      if(vector!=1)
			perr("Pow has bad arguments.");
		    }
		  else if(strncmp(p_tok,"vec",3)==0)
		    {
		      temp_vector=atoi(&p_tok[3]);

		      switch(temp_vector)
			{
			case 2:
			case 3:
			case 4:
			  if(vector==temp_vector)
			    break;
			default:
			  perr("Pow has bad arguments.");
			}
		    }
		  else
		    perr("Pow has bad arguments.");
		}
	    }
	  GL_Pow(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,vector,NULL);
	}
      else if(strncmp(temp_fun_name,"ConstructVec4",13)==0)
	{
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"f",1)==0)
	    {
	      p_tok = strtok(NULL,".");
	      if(strncmp(p_tok,"f",1)==0)
		{
		  p_tok = strtok(NULL,".");
		  if(strncmp(p_tok,"f",1)==0)
		    {
		      p_tok = strtok(NULL,".");
		      if(strncmp(p_tok,"f",1)==0)
			{
			  link_ConVec4(ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,ARGU_REGI+3,RETU_REGI,type,NULL);
			}
		    }
		}
	    }
	   //unfinished job
	  else if(strncmp(p_tok,"vec3",4)==0){ //Kaymon add
	    //printf("+++++++:%s\n",decode_line[0]);
	    //printf("+++++++:%s\n",decode_line[1]);
	    //printf("+++++++:%s\n",decode_line[2]);
	    int j=0;
	    p_tok = strtok(NULL,".");
	    if(strncmp( p_tok,"f", 1 ) == 0 ){
	      link_ConVec4_Vec3_F(ARGU_REGI,ARGU_REGI+1,RETU_REGI,NULL);
	    }
	  }
	  else if(strncmp(p_tok,"vec2",4)==0){
	    p_tok = strtok(NULL,".");
	    if(strncmp( p_tok,"vec2", 4 ) == 0 ){
	      link_ConVec4_Vec2_Vec2(ARGU_REGI,ARGU_REGI+1,RETU_REGI,NULL);
	    }
	    else if(strncmp( p_tok,"f", 1 ) == 0 ){
	      p_tok = strtok(NULL,".");
	      if(strncmp( p_tok,"f", 1 ) == 0 )
		link_ConVec4_Vec2_F_F(ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,RETU_REGI,NULL);
	    }
	  }
	}

      else if(strncmp(temp_fun_name,"ConstructVec3",13)==0)
	{
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec4",4)==0)
	    link_Vec4ToVec3(ARGU_REGI,RETU_REGI,NULL);
	  else if(strncmp(p_tok,"vec2",4)==0){
	    p_tok = strtok(NULL,".");
	    if(strncmp(p_tok,"f",1)==0){
	      link_ConVec3_Vec2_F(ARGU_REGI,ARGU_REGI+1,RETU_REGI,NULL);
	    }
	    else
	      perr("ConstructVec3 failed!!\n");
	  }
	  else if(strncmp(p_tok,"f",4)==0){
	    p_tok = strtok(NULL,".");
	    if(!p_tok)
	      link_FToVec3(ARGU_REGI,RETU_REGI,NULL);
	    else if(strncmp(p_tok,"f",4)==0){
	      p_tok = strtok(NULL,".");
	      if(strncmp(p_tok,"f",4)==0)
		link_ConVec3_F_F_F(ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,RETU_REGI,NULL);;
	    }
	    else if(strncmp(p_tok,"vec2",4)==0)
	      link_ConVec3_F_Vec2(ARGU_REGI,ARGU_REGI+1,RETU_REGI,NULL);
	    else
	      perr("ConstructVec3 failed!!\n");
	  }
	}
      else if(strncmp(temp_fun_name,"ConstructIVec4",14)==0)
	{
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"i",1)==0){
	    p_tok = strtok(NULL,".");
	    if(!p_tok){
	      perr("Didn't implement ConstrucIVec4 with four integer\n");
	    }
	    else if(strncmp(p_tok,"i",1)==0){
	      p_tok = strtok(NULL,".");
	      if(strncmp(p_tok,"i",1)==0){
		p_tok = strtok(NULL,".");
		if(strncmp(p_tok,"i",1)==0)
		  link_ConVec4(ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,ARGU_REGI+3,RETU_REGI,type,NULL);
		else 
		perr("COnstrctIVec4 failed with arguments.\n");
	      }
	      else
		perr("Didn't implement ConstrucIVec4 with two integer and other types\n");
	    }
	  }
	}
      else if(strncmp(temp_fun_name,"ConstructIVec3",14)==0)
	{
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"ivec4",5)==0)
	    link_Vec4ToVec3(ARGU_REGI,RETU_REGI,NULL);
	  if(strncmp(p_tok,"i",1)==0){
	    p_tok = strtok(NULL,".");
	    if(!p_tok)
	      link_FToVec3(ARGU_REGI,RETU_REGI,NULL);
	    else if(strncmp(p_tok,"i",1)==0){
	      p_tok = strtok(NULL,".");
	      if(strncmp(p_tok,"i",1)==0)
		link_ConVec3_F_F_F(ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,RETU_REGI,NULL);;
	    }
	  }
	}
      else if(strncmp(temp_fun_name,"ConstructBVec4",14)==0)
	{
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"b",1)==0){
	    p_tok = strtok(NULL,".");
	    if(!p_tok)
	      link_FToVec3(ARGU_REGI,RETU_REGI,NULL);
	    else if(strncmp(p_tok,"b",1)==0){
	      p_tok = strtok(NULL,".");
	      if(strncmp(p_tok,"b",1)==0){
		p_tok = strtok(NULL,".");
		if(strncmp(p_tok,"b",1)==0)
		  link_ConVec4(ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,ARGU_REGI+3,RETU_REGI,type,NULL);
	      }
	      else
		perr("ConstructBVec4 failed\n");
	    }
	  }
	}
      else if(strncmp(temp_fun_name,"ConstructBVec3",14)==0)
	{
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"bvec4",5)==0)
	    link_Vec4ToVec3(ARGU_REGI,RETU_REGI,NULL);
	  if(strncmp(p_tok,"b",1)==0){
	    p_tok = strtok(NULL,".");
	    if(!p_tok)
	      link_FToVec3(ARGU_REGI,RETU_REGI,NULL);
	    else if(strncmp(p_tok,"b",1)==0){
	      p_tok = strtok(NULL,".");
	      if(strncmp(p_tok,"b",1)==0)
		link_ConVec3_F_F_F(ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,RETU_REGI,NULL);;
	    }
	  }
	}
      else if(strncmp(temp_fun_name,"ConstructVec2",13)==0) //Kaymon
	{
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec4",4)==0)
	    {
	      char error_msg[128];
	      sprintf(error_msg,"New Label function: %s",temp_fun_name);
	      perr(error_msg);
	    }
	  if(strncmp(p_tok,"vec3",4)==0)
	    link_Vec3ToVec2(ARGU_REGI,RETU_REGI,NULL);
	  else if(strncmp(p_tok,"vec2",4)==0)
	    link_Vec2ToVec2(ARGU_REGI,RETU_REGI,NULL);
	  else if(strncmp( p_tok,"f", 1 ) == 0 ){
	    p_tok = strtok(NULL,".");
	    if(strncmp( p_tok,"f", 1 ) == 0 )
	      link_ConVec2_F_F(ARGU_REGI,ARGU_REGI+1,RETU_REGI,NULL);
	  }
	  
	}
      else if(strncmp(temp_fun_name,"ConstructIVec2",14)==0) //Kaymon
	{
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"ivec4",5)==0)
	    {
	      char error_msg[128];
	      sprintf(error_msg,"New Label function: %s",temp_fun_name);
	      perr(error_msg);
	    }
	  if(strncmp(p_tok,"ivec3",5)==0)
	    link_Vec3ToVec2(ARGU_REGI,RETU_REGI,NULL);
	  else if(strncmp( p_tok,"i", 1 ) == 0 ){
	    p_tok = strtok(NULL,".");
	    if(strncmp( p_tok,"i", 1 ) == 0 )
	      link_ConVec2_F_F(ARGU_REGI,ARGU_REGI+1,RETU_REGI,NULL);
	  }
	  
	}
      else if(strncmp(temp_fun_name,"ConstructBVec2",14)==0) //Kaymon
	{
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"bvec4",5)==0)
	    {
	      char error_msg[128];
	      sprintf(error_msg,"New Label function: %s",temp_fun_name);
	      perr(error_msg);
	    }
	  if(strncmp(p_tok,"bvec3",5)==0)
	    link_Vec3ToVec2(ARGU_REGI,RETU_REGI,NULL);
	  else if(strncmp( p_tok,"b", 1 ) == 0 ){
	    p_tok = strtok(NULL,".");
	    if(strncmp( p_tok,"b", 1 ) == 0 )
	      link_ConVec2_F_F(ARGU_REGI,ARGU_REGI+1,RETU_REGI,NULL);
	  }
	  
	}
      else if(strncmp(temp_fun_name,"Cos",3)==0)
	{
	  char assembly_string[4096];
	  p_tok = strtok(NULL,".");
	  int input_vector=0;
	  if(strncmp(p_tok,"f",1)==0)
	    input_vector=1;
	  if(strncmp(p_tok,"vec2",4)==0)
	    input_vector=2;
	  if(strncmp(p_tok,"vec3",4)==0)
	    input_vector=3;
	  if(strncmp(p_tok,"vec4",4)==0)
	    input_vector=4; 
	  GL_Cos(ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,ARGU_REGI+3,ARGU_REGI+4,ARGU_REGI+5,RETU_REGI,input_vector,assembly_string,NULL);
	}
      else if(strncmp(temp_fun_name,"Tan",3)==0)
	{
	  char assembly_string[4096];
	  p_tok = strtok(NULL,".");
	  int input_vector=0;
	  if(strncmp(p_tok,"f",1)==0)
	    input_vector=1;
	  if(strncmp(p_tok,"vec2",4)==0)
	    input_vector=2;
	  if(strncmp(p_tok,"vec3",4)==0)
	    input_vector=3;
	  if(strncmp(p_tok,"vec4",4)==0)
	    input_vector=4; 
	  GL_Tan(ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,ARGU_REGI+3,ARGU_REGI+4,ARGU_REGI+5,RETU_REGI,input_vector,assembly_string,NULL);
	}
      else if(strncmp(temp_fun_name,"Sin",3)==0)
	{
	  char assembly_string[8192];
	  memset(assembly_string,0,8192);
	  p_tok = strtok(NULL,".");
	  int input_vector=0;
	  if(strncmp(p_tok,"f",1)==0)
	    input_vector=1;
	  if(strncmp(p_tok,"vec2",4)==0)
	    input_vector=2;
	  if(strncmp(p_tok,"vec3",4)==0)
	    input_vector=3;
	  if(strncmp(p_tok,"vec4",4)==0)
	    input_vector=4; 
	  GL_Sin(ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,ARGU_REGI+3,ARGU_REGI+4,RETU_REGI,input_vector,assembly_string,NULL);
	  }
      else if(strncmp(temp_fun_name,"VectorNegate",12)==0) //Didn't add the specified vector type.
	{
	  link_VectorNegate(ARGU_REGI,RETU_REGI,NULL);
	}
      else if(strncmp(temp_fun_name,"Clamp",5)==0)
	{
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"f",1)==0)
	    {
	      p_tok = strtok(NULL,".");
	      if(strncmp(p_tok,"f",1)==0)
		{
		  p_tok = strtok(NULL,".");
		  if(strncmp(p_tok,"f",1)==0)
		    {
		      //GL_Clamp(RETU_REGI, ARGU_REGI, ARGU_REGI+1, ARGU_REGI+2, ARGU_REGI+3, 1, 1,NULL);
		      GL_Clamp(RETU_REGI, ARGU_REGI, ARGU_REGI+1, ARGU_REGI+2, OUR_TEMP_REGI, 1, 1,NULL); //Kaymon
		    }
		  else
		    perr("Clamp");
		}
	      else
		perr("Clamp");
	    }
	  else if(strncmp(p_tok,"vec2",4)==0)
	    {
	      p_tok = strtok(NULL,".");
	      if(strncmp(p_tok,"f",1)==0)
		{
		  p_tok = strtok(NULL,".");
		  if(strncmp(p_tok,"f",1)==0)
		    {
		      //GL_Clamp(RETU_REGI, ARGU_REGI, ARGU_REGI+1, ARGU_REGI+2, ARGU_REGI+3, 2, 1,NULL);
		      GL_Clamp(RETU_REGI, ARGU_REGI, ARGU_REGI+1, ARGU_REGI+2, OUR_TEMP_REGI, 2, 1,NULL); //Kaymon
		    }
		  else
		    perr("Clamp");
		}
	      else if(strncmp(p_tok,"vec2",4)==0)
		{
		  p_tok = strtok(NULL,".");
		  if(strncmp(p_tok,"vec2",2)==0)
		    {
		      //GL_Clamp(RETU_REGI, ARGU_REGI, ARGU_REGI+1, ARGU_REGI+2, ARGU_REGI+3, 2, 0,NULL);
		      GL_Clamp(RETU_REGI, ARGU_REGI, ARGU_REGI+1, ARGU_REGI+2, OUR_TEMP_REGI, 2, 0,NULL); //Kaymon
		    }
		  else
		    perr("Clamp");
		}
	      else
		perr("Clamp")
	    }
	  else if(strncmp(p_tok,"vec3",4)==0)
	    {
	      p_tok = strtok(NULL,".");
	      if(strncmp(p_tok,"f",1)==0)
		{
		  p_tok = strtok(NULL,".");
		  if(strncmp(p_tok,"f",1)==0)
		    {
		      //GL_Clamp(RETU_REGI, ARGU_REGI, ARGU_REGI+1, ARGU_REGI+2, ARGU_REGI+3, 3, 1,NULL);
		      GL_Clamp(RETU_REGI, ARGU_REGI, ARGU_REGI+1, ARGU_REGI+2, OUR_TEMP_REGI, 3, 1,NULL); //Kaymon
		    }
		  else
		    perr("Clamp");
		}
	      else if(strncmp(p_tok,"vec3",4)==0)
		{
		  p_tok = strtok(NULL,".");
		  if(strncmp(p_tok,"vec3",4)==0)
		    {
		      //GL_Clamp(RETU_REGI, ARGU_REGI, ARGU_REGI+1, ARGU_REGI+2, ARGU_REGI+3, 3, 0,NULL);
		      GL_Clamp(RETU_REGI, ARGU_REGI, ARGU_REGI+1, ARGU_REGI+2, OUR_TEMP_REGI, 3, 0,NULL); //Kaymon
		    }
		  else
		    perr("Clamp");

		}
	      else
		perr("Clamp")
	    }
	  else if(strncmp(p_tok,"vec4",4)==0)
	    {
	      p_tok = strtok(NULL,".");
	      if(strncmp(p_tok,"f",1)==0)
		{
		  p_tok = strtok(NULL,".");
		  if(strncmp(p_tok,"f",1)==0)
		    {
		      //GL_Clamp(RETU_REGI, ARGU_REGI, ARGU_REGI+1, ARGU_REGI+2, ARGU_REGI+3, 4, 1,NULL);
		      GL_Clamp(RETU_REGI, ARGU_REGI, ARGU_REGI+1, ARGU_REGI+2, OUR_TEMP_REGI, 4, 1,NULL); //Kaymon
		    }
		  else
		    perr("Clamp");
		}
	      else if(strncmp(p_tok,"vec4",4)==0)
		{
		  p_tok = strtok(NULL,".");
		  if(strncmp(p_tok,"vec4",4)==0)
		    {
		      //GL_Clamp(RETU_REGI, ARGU_REGI, ARGU_REGI+1, ARGU_REGI+2, ARGU_REGI+3, 4, 0,NULL);
		      GL_Clamp(RETU_REGI, ARGU_REGI, ARGU_REGI+1, ARGU_REGI+2, OUR_TEMP_REGI, 4, 0,NULL); //Kaymon
		    }
		  else
		    perr("Clamp");
		}
	      else
		perr("Clamp")
	    }
	  else
	    perr("Clamp");
	    }
      else if(strncmp(temp_fun_name,"Abs",3)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4; 	
	GL_Abs(RETU_REGI,ARGU_REGI,type,input_vector,NULL);
      }
      else if(strncmp(temp_fun_name,"Degrees",7)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4; 	
	GL_Degrees(RETU_REGI,ARGU_REGI,OUR_TEMP_REGI,input_vector,NULL); //probably call callregister(), not to use the OUR_TEMP_REGI
      }
      else if(strncmp(temp_fun_name,"Radians",7)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4; 	
	GL_Radians(RETU_REGI,ARGU_REGI,OUR_TEMP_REGI,input_vector,NULL);//probably call callregister(), not to use the OUR_TEMP_REGI
      }
      else if(strncmp(temp_fun_name,"Sign",4)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4; 	
	GL_Sign(RETU_REGI,ARGU_REGI,type,input_vector,NULL);
      }
      else if(strncmp(temp_fun_name,"Exp2",4)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4; 	
	GL_Exp2(RETU_REGI,ARGU_REGI,OUR_TEMP_REGI,input_vector,NULL);//probably call callregister(), not to use the OUR_TEMP_REGI
      }
      else if(strncmp(temp_fun_name,"Sqrt",4)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4;
	GL_Sqrt(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,input_vector,NULL);
      }
      else if(strncmp(temp_fun_name,"InverseSqrt",11)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4;
	GL_InverseSqrt(RETU_REGI,ARGU_REGI,OUR_TEMP_REGI,input_vector,NULL);//probably call callregister(), not to use the OUR_TEMP_REGI
      }
      else if(strncmp(temp_fun_name,"Exp",3)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4;
	GL_Exp(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,input_vector,NULL);
      }
      else if(strncmp(temp_fun_name,"Log2",4)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4;
	GL_Log2(RETU_REGI,ARGU_REGI,OUR_TEMP_REGI,input_vector,NULL);//probably call callregister(), not to use the OUR_TEMP_REGI
      }
      else if(strncmp(temp_fun_name,"Log",3)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4;
	GL_Log(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,input_vector,NULL);
      }
      else if(strncmp(temp_fun_name,"Convert",7)==0){ //Kaymon //convert int to float.
	p_tok = strtok(NULL,".");
	if(strncmp(p_tok,"sitofp",6)==0){
	  link_Convert_ItoF(RETU_REGI,ARGU_REGI,NULL);
	}
	else if(strncmp(p_tok,"uitofp",6)==0){
	  link_Convert_ItoF(RETU_REGI,ARGU_REGI,NULL);
	}
	else if(strncmp(p_tok,"fptosi",6)==0){
	  link_Convert_FtoI(RETU_REGI,ARGU_REGI,NULL);
	}
	else if(strncmp(p_tok,"fptoui",6)==0){
	  link_Convert_FtoI(RETU_REGI,ARGU_REGI,NULL);
	}
	else if(strncmp(p_tok,"ItoB",6)==0){
	  link_Convert_ItoB(RETU_REGI,ARGU_REGI,NULL);
	}
	else if(strncmp(p_tok,"BtoI",6)==0){
	  link_Convert_ItoB(RETU_REGI,ARGU_REGI,NULL);
	}
	else
	  perr("Not impelemete this.");

      }
      else if(strncmp(temp_fun_name,"Length",6)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4;
	GL_Length(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"Distance",8)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4;
	GL_Distance(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,ARGU_REGI+3,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"Floor",5)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4;
	GL_Floor(RETU_REGI,ARGU_REGI,OUR_TEMP_REGI,type,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"Ceil",4)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4;
	GL_Ceil(RETU_REGI,ARGU_REGI,type,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"Fract",5)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0)
	  input_vector=1;
	if(strncmp(p_tok,"vec2",4)==0)
	  input_vector=2;
	if(strncmp(p_tok,"vec3",4)==0)
	  input_vector=3;
	if(strncmp(p_tok,"vec4",4)==0)
	  input_vector=4;
	GL_Fract(RETU_REGI,ARGU_REGI,ARGU_REGI+1,OUR_TEMP_REGI,type,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"Mod",3)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int mode = 0;
	if(strncmp(p_tok,"f",1)==0){
	  input_vector=1;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"f",1)==0){
	    mode = 1;
	  }
	  else
	    perr("second argument has wrong type in Mod function");
	}
	else if(strncmp(p_tok,"vec2",4)==0){
	  input_vector=2;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"f",1)==0){
	    mode = 1;
	  }
	  else if(strncmp(p_tok,"vec2",4)==0){
	  }
	  else
	    perr("second argument has wrong type in Mod function");

	}
	else if(strncmp(p_tok,"vec3",4)==0){
	  input_vector=3;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"f",1)==0){
	    mode = 1;
	  }
	  else if(strncmp(p_tok,"vec3",4)==0){
	  }
	  else
	    perr("second argument has wrong type in Mod function");

	}
	else if(strncmp(p_tok,"vec4",4)==0){
	  input_vector=4;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"f",1)==0){
	    mode = 1;
	  }
	  else if(strncmp(p_tok,"vec3",4)==0){
	  }
	  else
	    perr("second argument has wrong type in Mod function");

	}
	
	GL_Mod(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,ARGU_REGI+3,OUR_TEMP_REGI,type,input_vector,mode,NULL);

      }
      else if(strncmp(temp_fun_name,"Reflect",7)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0){
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"f",1)==0)
	    input_vector=1;
	}
	else if(strncmp(p_tok,"vec2",4)==0){
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec2",4)==0)
	    input_vector=2;
	}
	else if(strncmp(p_tok,"vec3",4)==0){
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec3",4)==0)
	    input_vector=3;
	}
	else if(strncmp(p_tok,"vec4",4)==0){
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec4",4)==0)
	    input_vector=4;
	}
	else{
	  perr("Arguments has wrong type in Reflect function");
	}
	GL_Reflect(RETU_REGI,ARGU_REGI,ARGU_REGI+1,OUR_TEMP_REGI,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"Min",3)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int mode=0;
	if(strncmp(p_tok,"f",1)==0){
	  input_vector=1;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"f",1)==0){
	    mode=1;
	  }
	  else
	    perr("second argument has wrong type in min function");
	}
	else if(strncmp(p_tok,"vec2",4)==0){
	  input_vector=2;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec2",4)==0){
	  }
	  else if(strncmp(p_tok,"f",1)==0){
	    mode=1;
	  }
	  else
	    perr("second argument has wrong type in min function");
	}
	else if(strncmp(p_tok,"vec3",4)==0){
	  input_vector=3;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec3",4)==0){
	  }
	  else if(strncmp(p_tok,"f",1)==0){
	    mode=1;
	  }
	  else
	    perr("second argument has wrong type in min function");
	}
	else if(strncmp(p_tok,"vec4",4)==0){
	  input_vector=4;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec4",4)==0){
	  }
	  else if(strncmp(p_tok,"f",1)==0){
	    mode=1;
	  }
	  else
	    perr("second argument has wrong type in min function");
	}
	else
	  perr("first argument has wrong type in min function");
	GL_Min(RETU_REGI,ARGU_REGI,ARGU_REGI+1,OUR_TEMP_REGI,input_vector,mode,NULL);

      }
      else if(strncmp(temp_fun_name,"Mix",3)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int mode=0;
	if(strncmp(p_tok,"f",1)==0){
	  input_vector=1;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"f",1)==0){
	    p_tok = strtok(NULL,".");
	    if(strncmp(p_tok,"f",1)==0){
	      mode=1;
	    }
	    else
	      perr("third argument has wrong type in mix function");
	  }
	  else
	    perr("second argument has wrong type in mix function");
	}
	else if(strncmp(p_tok,"vec2",4)==0){
	  input_vector=2;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec2",4)==0){
	    p_tok = strtok(NULL,".");
	    if(strncmp(p_tok,"vec2",4)==0){
	    }
	    else if(strncmp(p_tok,"f",1)==0){
	      mode=1;
	    }
	    else
	      perr("third argument has wrong type in mix function");
	  }
	  else
	    perr("second argument has wrong type in mix function");
	}
	else if(strncmp(p_tok,"vec3",4)==0){
	  input_vector=3;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec3",4)==0){
	    p_tok = strtok(NULL,".");
	    if(strncmp(p_tok,"vec3",4)==0){
	    }
	    else if(strncmp(p_tok,"f",1)==0){
	      mode=1;
	    }
	    else
	      perr("third argument has wrong type in mix function");
	  }
	  else
	    perr("second argument has wrong type in mix function");
	}
	else if(strncmp(p_tok,"vec4",4)==0){
	  input_vector=4;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec4",4)==0){
	    p_tok = strtok(NULL,".");
	    if(strncmp(p_tok,"vec4",4)==0){
	    }
	    else if(strncmp(p_tok,"f",1)==0){
	      mode=1;
	    }
	    else
	      perr("third argument has wrong type in mix function");
	  }
	  else
	    perr("second argument has wrong type in mix function");
	}
	else
	  perr("first argument has wrong type in mix function");
	GL_Mix(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,OUR_TEMP_REGI,mode,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"Step",4)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int mode=0;
	if(strncmp(p_tok,"f",1)==0){
	  mode=1;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"f",1)==0)
	    input_vector=1;
	  else if(strncmp(p_tok,"vec2",4)==0)
	    input_vector=2;
	  else if(strncmp(p_tok,"vec3",4)==0)
	    input_vector=3;
	  else if(strncmp(p_tok,"vec4",4)==0)
	    input_vector=4;
	  else
	    perr("second argument has wrong type in step function");
	}
	else if(strncmp(p_tok,"vec2",4)==0){
	  input_vector=2;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec2",4)==0){
	  }
	  else
	    perr("Wrong arguments in step function");
	}
	else if(strncmp(p_tok,"vec3",4)==0){
	  input_vector=3;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec3",4)==0){
	  }
	  else
	    perr("Wrong arguments in step function");
	}
	else if(strncmp(p_tok,"vec4",4)==0){
	  input_vector=4;
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec4",4)==0){
	  }
	  else
	    perr("Wrong arguments in step function");
	}
	else
	  perr("first argument has wrong type in min function");
	GL_Step(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,OUR_TEMP_REGI,mode,type,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"SmoothStep",7)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int mode =0;
	if(strncmp(p_tok,"f",1)==0){
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"f",1)==0){
	    p_tok = strtok(NULL,".");
	    if(strncmp(p_tok,"f",1)==0)
	      input_vector=1;
	    else if(strncmp(p_tok,"vec2",4)==0)
	      input_vector=2;
	    else if(strncmp(p_tok,"vec3",4)==0)
	      input_vector=3;
	    else if(strncmp(p_tok,"vec4",4)==0)
	      input_vector=4;
	    else
	      perr("Third argument has wrong type in smoothstep function");
	    mode = 1;
	  }
	}
	else{
	  char buffer[3][50];
	  strcpy(buffer[0],p_tok);
	  p_tok = strtok(NULL,".");
	  strcpy(buffer[1],p_tok);
	  p_tok = strtok(NULL,".");
	  strcpy(buffer[2],p_tok);
	  if(!strcmp(buffer[0],buffer[1])){
	    if(!strcmp(buffer[1],buffer[2]))
	      input_vector = buffer[1][3]-48;
	  }
	  else
	      perr("First argument is different than second argument in smoothstep function");

	}
	GL_SmoothStep(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,ARGU_REGI+3,ARGU_REGI+4,OUR_TEMP_REGI,mode,type,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"LessThanEqual",13)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int mode=0;
	if(strncmp(p_tok,"vec",3)==0){
	  input_vector=p_tok[3]-48;
	}
	else if(strncmp(p_tok,"ivec",4)==0){
	  input_vector=p_tok[4]-48;
	  mode = 1;
	}
	else
	  perr("First argument is different than second argument in LessThanEqual function");

	GL_LessThanEqual(RETU_REGI,ARGU_REGI,ARGU_REGI+1,OUR_TEMP_REGI,type,mode,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"LessThan",8)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int mode=0;
	if(strncmp(p_tok,"vec",3)==0){
	  input_vector=p_tok[3]-48;
	}
	else if(strncmp(p_tok,"ivec",4)==0){
	  input_vector=p_tok[4]-48;
	  mode = 1;
	}
	else
	  perr("First argument is different than second argument in LessThan function");

	GL_LessThan(RETU_REGI,ARGU_REGI,ARGU_REGI+1,OUR_TEMP_REGI,type,mode,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"GreaterThanEqual",16)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int mode=0;
	if(strncmp(p_tok,"vec",3)==0){
	  input_vector=p_tok[3]-48;
	}
	else if(strncmp(p_tok,"ivec",4)==0){
	  input_vector=p_tok[4]-48;
	  mode = 1;
	}
	else
	  perr("First argument is different than second argument in GreaterThanEqual function");

	GL_GreaterThanEqual(RETU_REGI,ARGU_REGI,ARGU_REGI+1,OUR_TEMP_REGI,type,mode,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"GreaterThan",11)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int mode=0;
	if(strncmp(p_tok,"vec",3)==0){
	  input_vector=p_tok[3]-48;
	}
	else if(strncmp(p_tok,"ivec",4)==0){
	  input_vector=p_tok[4]-48;
	  mode = 1;
	}
	else
	  perr("First argument is different than second argument in GreaterThan function");

	GL_GreaterThan(RETU_REGI,ARGU_REGI,ARGU_REGI+1,OUR_TEMP_REGI,type,mode,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"VectorEqual",11)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int offset=0;
	if(strncmp(p_tok,"vec",3)==0){
	  input_vector=p_tok[3]-48;
	}
	else if(strncmp(p_tok,"ivec",4)==0){
	  input_vector=p_tok[4]-48;
	}
	else if(strncmp(p_tok,"bvec",4)==0){
	  input_vector=p_tok[4]-48;
	  offset=3;
	}
	else
	  perr("First argument is different than second argument in VectorEqual function");

	GL_Equal(RETU_REGI,ARGU_REGI,ARGU_REGI+1+offset,OUR_TEMP_REGI,type,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"VectorNotEqual",14)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int offset=0;
	if(strncmp(p_tok,"vec",3)==0){
	  input_vector=p_tok[3]-48;
	}
	else if(strncmp(p_tok,"ivec",4)==0){
	  input_vector=p_tok[4]-48;
	}
	else if(strncmp(p_tok,"bvec",4)==0){ //bvec will generate 4 returned register. r16~r19. But we just need first one.
	  input_vector=p_tok[4]-48;
	  offset=3;
	}
	else
	  perr("First argument is different than second argument in VectorNotEqual function");

	GL_NotEqual(RETU_REGI,ARGU_REGI,ARGU_REGI+1+offset,OUR_TEMP_REGI,type,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"Any",3)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int offset=0;
	if(strncmp(p_tok,"bvec",4)==0){ //bvec will generate 4 returned register. r16~r19. But we just need first one.
	  input_vector=p_tok[4]-48;
	}
	else
	  perr("First argument is different than second argument in Any function");

	GL_Any(RETU_REGI,ARGU_REGI,OUR_TEMP_REGI,type,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"All",3)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int offset=0;
	if(strncmp(p_tok,"bvec",4)==0){ //bvec will generate 4 returned register. r16~r19. But we just need first one.
	  input_vector=p_tok[4]-48;
	}
	else
	  perr("First argument is different than second argument in All function");

	GL_All(RETU_REGI,ARGU_REGI,OUR_TEMP_REGI,type,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"Cross",5)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int offset=0;
	if(strncmp(p_tok,"vec3",4)==0){ 
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec3",4)==0) 
	    GL_Cross(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,OUR_TEMP_REGI,NULL);
	}
	else
	  perr("First argument is different than second argument in Cross function");
      }
      else if(strncmp(temp_fun_name,"FaceForward",11)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0){ 
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"f",1)==0){
	    p_tok = strtok(NULL,".");
	    if(strncmp(p_tok,"f",1)==0){
	      input_vector=1;
	    }
	  }
	}
	else if(strncmp(p_tok,"vec2",4)==0){ 
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec2",4)==0){
	    p_tok = strtok(NULL,".");
	    if(strncmp(p_tok,"vec2",4)==0){
	      input_vector=2;
	    }
	  }
	}
	else if(strncmp(p_tok,"vec3",4)==0){ 
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec3",4)==0){
	    p_tok = strtok(NULL,".");
	    if(strncmp(p_tok,"vec3",4)==0){
	      input_vector=3;
	    }
	  }
	}
	else
	  perr("Arguments has wrong tpe in Faceforward function");
	GL_FaceForward(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,OUR_TEMP_REGI,type,input_vector,NULL);
      }
      else if(strncmp(temp_fun_name,"Refract",7)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	if(strncmp(p_tok,"f",1)==0){ 
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"f",1)==0){
	      input_vector=1;
	  }
	}
	else if(strncmp(p_tok,"vec2",4)==0){ 
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec2",4)==0){
	    input_vector=2;
	  }
	}
	else if(strncmp(p_tok,"vec3",4)==0){ 
	  p_tok = strtok(NULL,".");
	  if(strncmp(p_tok,"vec3",4)==0){
	    input_vector=3;
	  }
	}
	else
	  perr("Arguments has wrong tpe in Faceforward function");

	p_tok = strtok(NULL,".");
	if(strncmp(p_tok,"f",1)==0) 
	  GL_Refract(RETU_REGI,ARGU_REGI,ARGU_REGI+1,ARGU_REGI+2,ARGU_REGI+3,ARGU_REGI+4,ARGU_REGI+5,OUR_TEMP_REGI,type,input_vector,NULL);
	else
	  perr("Third argument has wrong tpe in refract function");

      }
      else if(strncmp(temp_fun_name,"VectorLogicalNot",16)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int input_vector=0;
	int offset=0;
	if(strncmp(p_tok,"bvec",4)==0){ //bvec will generate 4 returned register. r16~r19. But we just need first one.
	  input_vector=p_tok[4]-48;
	}
	else
	  perr("First argument is different than second argument in Equal function");

	GL_Not(RETU_REGI,ARGU_REGI,OUR_TEMP_REGI,type,input_vector,NULL);

      }
      else if(strncmp(temp_fun_name,"Extractelement",14)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int SV = p_tok[0] - 48;
	p_tok = strtok(NULL,".");
	unsigned short swi = 0;
	swi = atoi( p_tok );
	inline_extractelement(RETU_REGI,ARGU_REGI,swi,SV,NULL);
      }
      else if(strncmp(temp_fun_name,"Insertelement",14)==0){ //Kaymon
	p_tok = strtok(NULL,".");
	int SV = p_tok[0] - 48;
	p_tok = strtok(NULL,".");
	unsigned short swi = 0;
	swi = atoi( p_tok );
	inline_insertelement(RETU_REGI,ARGU_REGI,ARGU_REGI+1,swi,SV,NULL);
      }
      else if(strncmp(temp_fun_name,"BackToMainFunction",18)==0){ //Kaymon
	BackToMainFunction(type);
      }
      else
	{
	  char error_msg[128];
	  sprintf(error_msg,"New Label function: %s",temp_fun_name);
	  perr(error_msg);
	}
    }
  else if(strncmp(decode_line[1],"texture2D",9)==0)
    {
      GL_texture2D();
    }
  else if(strncmp(decode_line[1],"textureCube",11)==0)
    {
      GL_textureCube();
    }
  else{//For user defining function.
    SavePCCountandJumpToUserDefiningFunction(type);
  }

  
  fclose(out);

  /*
  nowBin=29;
  nowBin=nowBin<<OPCODE_BIT;
  type_4(6,temp_assembly.argu_num,0);
  */
}
void input_var(char input)
{
  VarCode[LastVarLine]=input;
  LastVarLine++;
#ifdef printable
  FILE *pfile=fopen("opcode_and_binary","a");
  fprintf(pfile,"PC:%6d\t%02x\t\t",PC,input);
  fprintf(pfile,"%s",nowRead);
  fclose(pfile);
#endif
  PC++;
}

int pTwoAlignAddress(int addr, int algn)
{
  while(1)
    {
      //printf("%d\t%d\n",addr,((addr>>algn)<<algn));
      if (((addr>>algn)<<algn)==addr)
	return addr;
      input_var(0);
      addr++;
    }
}

int AlignAddress(int addr, int algn)
{ 
  while(1)
  { 
    if (((addr>>algn)<<algn)==addr)
       return addr;
    addr++;
  }

}


void temp_assembly_init()
{
  int i;
  bzero(&temp_assembly,sizeof(temp_assembly));
  //temp_imm=0;
  temp_assembly.imm_sign=0;
  temp_assembly.imm_searched=0;
  float_of_imm=0;
  temp_assembly.argu_num=0;
  temp_assembly.bool_mark_case=0;
  temp_assembly.wm=-1;
  temp_assembly.reg_num=0;
  temp_assembly.neg=0;
  temp_assembly.neg_change=0;
  temp_assembly.ide_imm=0;
  temp_assembly.sv=-1;
  for(i=0;i<4;i++)
    {
      temp_assembly.regi[i]=-1;
      temp_assembly.swizzle[i]=-1;
    }
  for(i=0;i<9;i++)
    {
      decode_line[i][0]='\0';
    }
  
}


void input_buffer(long long unsigned int x)
{
  BinCode[LastBinLine]=x;
  LastBinLine++;
#ifdef printable
  FILE *pfile=fopen("opcode_and_binary","a");
  fprintf(pfile,"PC:%6d\t%llx\t",PC,x);
  fprintf(pfile,"%s",nowRead);
  fclose(pfile);
#endif
}


void printinst(long long unsigned int X)
{  
  //if(Has3Regs(X>>50) 
  //  printf("%s%c %sr%d, %sr%d, %sr%d, ",opcode(X>>50),InTy(1&(X>>2)),N((X>>9)&1),((X>>45)&31),N((X>>8)&1), ((X>>40)&31),N((X>>7)&1), ((X>>35)&31))


}

void putoutmemory(char type,int fd)
{

  void *map_memory;

  if(type=='v')
    {
      map_memory=mmap(0,VERT_SPACE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x3000);
#ifdef test_mmap
      lseek(fd,LastBinLine*sizeof(long long)+0x3000+1,SEEK_SET);
      write(fd,"",1);
      lseek(fd,0,SEEK_SET);
#endif  
    }
  else if(type=='f')
    {
      map_memory=mmap(0,FRAG_SPACE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x4000);
#ifdef test_mmap
      lseek(fd,LastBinLine*sizeof(long long)+0x4000+1,SEEK_SET);
      write(fd,"",1);
      lseek(fd,0,SEEK_SET);
#endif
    }
  else
    perr("putoutmemory: type");

  memcpy(map_memory,BinCode,LastBinLine*sizeof(long long));

}

void putoutthefile(char *S)
{  
  FILE *F=fopen(S,"w");
  fwrite(BinCode,sizeof (long long unsigned int), LastBinLine, F);
  //fwrite(VarCode,sizeof (char),LastVarLine,F);
  fclose(F);
}




void no_opcode()
{
  if(!strcmp(decode_line[0],"mtspr")) //kaymon for user defining functon
    printf("Warning: Check mtspr\n");
  else
    printf("%s is not defined.\n",decode_line[0]);
  //exit (1);
}

int search_table(char * target)
{
  int i;
  int match=0,just_match=0;
  //printf("search_table target: %s\n",target);
  for(i=0;i<300;i++)
    {
      //printf("table: %s\ttarget: %s\n",table[i][0],target);
      if(strcmp(table[i][0],"\0")==0)
	break;
      else if(strcmp(table[i][0],target)==0)
	{
	  match++;
	  just_match=atoi(table[i][2]);
	}

      //else if(table[i][0][0]=='.')
      //{
      //if(strcmp(&table[i][0][1],target)==0)
      //{
      //match++;
      //just_match=atoi(table[i][2]);
      //printf("just_match: %d\n",just_match);
      //}
      //}

      if(match>1)
	{
	  if(strcmp(table[i][5],now_file)==0 && strcmp(table[i][0],target)==0)
	    return atoi(table[i][2]);
	}
    }
  if(match==0)
    {
      char *p_tok,temp_target[256],temp_target2[256],temp_cond[64],*p_tok2;
      //printf("target: %s\n",target);
      strcpy(temp_target,target);
      strcpy(temp_target2,target);
      p_tok=strtok(temp_target,".");
      //printf("search p_tok: %s\n",p_tok);
      
      p_tok2=strtok(temp_target2,".");
      p_tok2=strtok(NULL,".");

      //check target's type
      //Type: variable
      if(p_tok2==NULL)
	{
	  strcpy(temp_cond,"r");
	}
      //Type: uniform ,attribute ,varying
      else
	strcpy(temp_cond,p_tok2);

      //printf("search table point 2\n");
      if(strcmp(temp_cond,"u")==0)
	{
	  //printf("p_tok 2: %s\ntemp_target: %s\n",p_tok,temp_target);
	  //printf("uniform_number: %d\n",uniform_number);
	  for(i=0;i<max_number_of_uniform_variables;i++)
	    {
	      //printf("u_form->uniform_name[%d]: ",i);
	      //printf("%s\n",u_form->uniform_name[i]);
	      if(strcmp(u_form->uniform_name[i],p_tok)==0)
		{
		  p_tok=strtok(NULL,".");
		  if(p_tok==NULL)
		    {
		      //printf("return IMM: %d\n",i*16);
#ifdef IMM_FOR_LD
		      return i*16;
#endif

#ifdef SET_UNIFORM_MATRIX_UNIT
		      return i*4;
#else
		      return i;
#endif
		    }
		  else
		    {
		      //printf("return IMM: %d\n",i*16+4*atoi(p_tok));
#ifdef IMM_FOR_LD
		      return i*16+4*atoi(p_tok);
#endif

#ifdef SET_UNIFORM_MATRIX_UNIT
		      return i*4+atoi(p_tok);
#else
		      return i+atoi(p_tok);
#endif
		    }
		}
	    }
	}
      else if(strcmp(temp_cond,"a")==0)
	{
	  //printf("p_tok 2: %s\ntemp_target: %s\n",p_tok,temp_target);
	  for(i=0;i<8;i++)
	    {
	      if(strcmp(a_form->att_name[i],p_tok)==0)
		{
		  p_tok=strtok(NULL,".");
		  //printf("return: %d\n",i*16+4*atoi(p_tok));
		  return i*16+4*atoi(p_tok);
		}
	    }	  
	}
      else if(strcmp(temp_cond,"v")==0)
	{
	}
      else if(strcmp(temp_cond,"r")==0)
	{
	  struct variable_form *p_cur_r_form=p_first_r_form;
	  int bool_variable=1; //This is for variable table, not IR variable table

	  //printf("p_tok 2: %s\ntemp_target: %s\n",p_tok,temp_target);

	  while(1)
	    {
	      //check variable table
	      //printf("p_cur_r_form->name: %s\np_tok: %s\n",&p_cur_r_form->name[1],p_tok);
	      if(strcmp(&p_cur_r_form->name[1],p_tok)==0) //For reserved variable. e.g. .LCPIO_0.
		{
		  return p_cur_r_form->address;
		}
	      else if(strcmp(&p_cur_r_form->name[0],p_tok)==0) //For reserved variable. e.g. gl_FragCoord
		{
		  return p_cur_r_form->address;
		}
	      else
		{
		  if(p_cur_r_form->next==NULL)
		    {
		      bool_variable=0;
		      break;
		    }
		  p_cur_r_form=p_cur_r_form->next;
		}
	    }
	  if(bool_variable==0)
	    {
	      //check IR variable table
	      
	      p_cur_r_form=p_first_r_form2;
	      while(1)
		{
		  //printf("p_cur_r_form->name: %s\np_tok: %s\n",&p_cur_r_form->name[0],p_tok);
		  if(strcmp(&p_cur_r_form->name[0],p_tok)==0)
		    {
		      return p_cur_r_form->address;
		    }
		  else
		    {
		      if(p_cur_r_form->next==NULL)
			{
			  return -1;
			}
		      p_cur_r_form=p_cur_r_form->next;
		    } 
		}
	    }
	  else
	    {
	      return -1;
	    }
	  //print_variable();
	}
      else
	{
#ifdef linker_debug_flag
	  perr("No this function or variable");    
#endif
	  return -1;
	}
    }
  else if (match==1)
    return just_match;
  else  
    {
      for(i=0;i<300;i++)
	{
	  if(strcmp(table[i][0],"\0")==0)
	    {
#ifdef linker_debug_flag
	      perr("No this function or variable");
#endif
	      return -1;
	    }
	  else if(strcmp(table[i][0],target)==0 && strcmp(table[i][5],now_file)==0)
	    return atoi(table[i][2]);
	}
    }
  return -1;
}

void decode()
{
  char *pRead;
  int line_number=0;
  pRead=strtok(t_read," ,\n\t");
  while(pRead !=NULL)
    {
      if((strcmp(decode_line[0],"MAD")==0 || strcmp(decode_line[0],"MADF")==0) && pRead[0]!='<')
	temp_assembly.reg_num++;
      strcpy(decode_line[line_number],pRead);
      //printf("decode_line[%d]: %s\n",line_number,decode_line[line_number]);
      pRead=strtok(NULL," ,\n\t");
      temp_assembly.argu_num=line_number;
      line_number++;
    }

  if(strncmp(decode_line[temp_assembly.argu_num],"set_mark",8)==0 || strncmp(decode_line[temp_assembly.argu_num-2],"set_mark",8)==0)
    //minus 2 for the metadata: # size:4
      temp_assembly.bool_mark_case=1;
  //printf("argu_num=%d\n",temp_assembly.argu_num);
  //printf("//////////////////////////////////////\n");
}

void shift_amount(int input)
{
  printf("shift_amount:%d\n",input);
  if(input>=0 && input <32)
    {
      nowBin=nowBin>>SA_BIT;
      nowBin=nowBin|input;
      nowBin=nowBin<<SA_BIT;
    }
  else
    {
      printf("shift_amount ERROR\n");
      po->LinkerStatus = GL_FALSE;
      exit (1);
    }
}


void FXOrFL(char *input)
{
  //printf("%s\n",input);

  if(strcmp(input,"FL")==0)
    {
      nowBin=nowBin>>F_BIT;
      nowBin=nowBin|1;
      nowBin=nowBin<<F_BIT;
    }
  else if(strcmp(input,"FX")!=0)
    {
      perr("FX\\FL");
    }
}
void write_sv(int input)
{
  //printf("write_sv:%d\n",input);
#ifdef detect_error
  if(input>3 || input <0)
    perr("SV");
#endif

  nowBin=nowBin >> SV_BIT;
  nowBin=nowBin|input;
  nowBin=nowBin << SV_BIT;
}

void SOrV(int decode_line_num)
{
  //printf("S/V: %d\n",atoi(&decode_line[decode_line_num][1]));
  
#ifdef detect_error
  if(atoi(&decode_line[decode_line_num][1]) >4 || atoi(&decode_line[decode_line_num][1]) <1)
      perr("S/V");
#endif
  nowBin=nowBin >> SV_BIT;
  nowBin=nowBin|(atoi(&decode_line[decode_line_num][1])-1);
  nowBin=nowBin << SV_BIT;
}

void ROrL (char input)
{
  if(input == 'L')
    {
      nowBin=nowBin>>RL_BIT;
      nowBin=nowBin|1;
      nowBin=nowBin<<RL_BIT;
    }
  else if(input ==0);
  else 
    {
      printf("R/L ERROR\n");
      po->LinkerStatus = GL_FALSE;
      exit (1);
    }
}


void write_register(int regi_num,int status)
{
  int i;
  //for(i=0;i<4;i++){printf("register %d: %d\n",i,temp_assembly.regi[i]);}
  if(status==BRANCH_WRITE_FRONT_REGISTER)
    {
      //int temp_regi = temp_assembly.regi[0];      
      //temp_assembly.regi[0]=temp_assembly.regi[1];
      //temp_assembly.regi[1]=temp_regi;
      temp_assembly.regi[2]=temp_assembly.regi[0];
      temp_assembly.regi[3]=temp_assembly.regi[1];
    }

  if(status!=BRANCH_WRITE_LATER_REGISTER)
    {
      nowBin=nowBin>>RD_BIT;
      for(i=0;i<regi_num;i++)
	{
	  if(temp_assembly.regi[i]==-1)
	    perr("register");
	  if((strcmp(decode_line[0],"MAD")==0 || strcmp(decode_line[0],"MADF")==0 || strcmp(decode_line[0],"DIV")==0 || strcmp(decode_line[0],"DSQ")==0) && i>=1)
	    nowBin=nowBin|temp_assembly.regi[i+1];
	  else
	    nowBin=nowBin|temp_assembly.regi[i];
	  nowBin=nowBin<<REGISTER_BIT;
	}
    
      nowBin=nowBin<<(RD_BIT-(REGISTER_BIT*regi_num));
    }
  if(status==BRANCH_WRITE_LATER_REGISTER)
    {
      //for(i=0;i<4;i++){printf("register %d: %d\n",i,temp_assembly.regi[i]);}
      if(temp_assembly.regi[2]==-1 || temp_assembly.regi[3]==-1)
	perr("register");
      nowBin=nowBin>>R3_BIT;
      nowBin=nowBin|temp_assembly.regi[2];
      nowBin=nowBin<<(REGISTER_BIT-1);
      nowBin=nowBin|temp_assembly.regi[3];
      nowBin=nowBin<<(R3_BIT-(REGISTER_BIT-1));
    }
  

}


void write_neg(int remained_bits)
{
  //printf("neg: %d\tchange:%d\n",temp_assembly.neg,temp_assembly.neg_change);
  nowBin=nowBin>>remained_bits;
  if(temp_assembly.neg_change==1)
    {
      if(strcmp(decode_line[0],"MAD")==0 || strcmp(decode_line[0],"MADF")==0)
	temp_assembly.neg=7-temp_assembly.neg;
      else
	temp_assembly.neg=3-temp_assembly.neg;
    }
  //printf("neg: %d\tchange:%d\n",temp_assembly.neg,temp_assembly.neg_change);
  nowBin=nowBin|temp_assembly.neg;
  nowBin=nowBin<<remained_bits;
  
}

int ahtoi(char * in_addr)
{
  int re_value=0;
  while(* in_addr !='\0')
    {
      re_value = re_value << 4;
      if(*in_addr >= 48 && *in_addr <=57)
	re_value=re_value | (*in_addr-48);
      else if(*in_addr >= 65 && *in_addr <=70)
	re_value=re_value | (*in_addr-55);
      else if(*in_addr >= 97 && *in_addr <=102)
        re_value=re_value | (*in_addr-87);
      else
	  perr("IMM");
      
      in_addr++;
    }
  return re_value;
}

float myatof(char * s)
{
  if(s[0]=='-')
    {
      float none_neg_result=myatof(&s[1]);
      return -none_neg_result;
    }
  float wp=0,fp=0;
  float re_num;
  for(;(*s!='.')&&(*s!='\0');s++)
    wp=wp*10+(*s-'0');
  int denom=1;
  if(*s=='.')
    {
      for(s=s+1;*s!='\0';s++)
	{
	  fp=fp*10+(*s-'0');
	  denom*=10;
	}
    }
  else
    fp=0;

  re_num=wp+(fp/(double)denom);
  return re_num;
}

void imm(int decode_line_num,int status,int type)
{
  int i_imm=0,i,search_var=0;
  float f_imm=0;
  char *pTok,copy_imm[64],var[64];

  //printf("IMM: %s\t%s\t%s\n",decode_line[0],decode_line[2],decode_line[decode_line_num]);
  i=0;
  while(decode_line[decode_line_num][i]!='\0')
    {
      if(decode_line[decode_line_num][i]=='@')
	search_var=1;
      i++;
    }
  if(search_var==1)
    {
      //printf("Original:%s %s %s %s\n",decode_line[0],decode_line[1],decode_line[2],decode_line[decode_line_num]);
      strcpy(copy_imm,decode_line[decode_line_num]);
      pTok=strtok(copy_imm,"@");
      //printf("label: %s\n",pTok);
      strcpy(var,pTok);
      pTok=strtok(NULL,"@");
      //printf("var: %s\n",pTok);
      i_imm=search_table(var);
      if(pTok[0]=='l')
	{
	  i_imm=(i_imm<<16)>>16;
	}
      else if(pTok[0]=='h' && pTok[1]=='a')
	{
	  i_imm=i_imm>>16;
	}
      else
	perr("IMM variable");

    }
  else if(temp_assembly.ide_imm==0)
    {
      if(status==FLOAT)
	{  
	  //minus #
	  f_imm=myatof(&decode_line[decode_line_num][0]);
	  memcpy(&i_imm,&f_imm,sizeof(f_imm));
	  //printf("f_imm: int: %d\tfloat: %f\n",f_imm,f_imm);
	  //printf("i_imm: int: %d\tfloat: %f\n",i_imm,i_imm);
	}
      else if(decode_line[decode_line_num][1]=='0' && decode_line[decode_line_num][2]=='x')
	//minus #
	i_imm=ahtoi(&decode_line[decode_line_num][2]);
      else 
	//minus #
	i_imm=atoi(&decode_line[decode_line_num][0]);

    }
  else
    {
      i_imm=temp_assembly.imm;
    }

  /*
  if(status == BRANCH_IMM)
    {
#ifdef SET_NOP_NINE	
      i_imm=i_imm*2-1;
#else
      ;
#endif
    }
  */

  //printf("i_imm: %d\t",i_imm);
  
  //if(status!=FLOAT && (decode_line[2][0]=='-' || decode_line[decode_line_num][0]=='-'))
  //i_imm=0-i_imm;
  
  //printf("%f\n",f_imm);
  //printf("i_imm: %d\n",i_imm);

  if(type==2)
    {
      nowBin=nowBin>>IMM_BIT;
      memcpy((void*) &nowBin,(void*) &i_imm,4);
      //nowBin=nowBin|i_imm;
      nowBin=nowBin<<IMM_BIT;
    }
  else if(type==4)
    {
      nowBin=nowBin>>OFFSET_BIT;
      nowBin=nowBin|(unsigned short)i_imm;
      nowBin=nowBin<<OFFSET_BIT;
    }
  return;

}

void register_mark_case()
{
  //printf("reg_mark_0: %s\n",decode_line[0]);
  //printf("reg_mark_1: %s\n",decode_line[1]);
  //printf("reg_mark_2: %s\n",decode_line[2]);
  //printf("reg_mark_3: %s\n",decode_line[3]);
  //printf("reg_mark_4: %s\n\n",decode_line[4]);
  if(strcmp(decode_line[0],"ADDI")==0)
    {
      if( !strcmp("r0",decode_line[1]) && !strcmp("r1",decode_line[2]) ){ //temporary solution Kaymon for ADDI, 28, 28, 2 set_mark because it would be changed to ADDI 3, 2,set_mark
	temp_assembly.regi[0]=3;
	temp_assembly.regi[1]=2;
      }
    }
  else if(strcmp(decode_line[0],"ADD")==0)
    {
      if(strcmp(decode_line[temp_assembly.argu_num],"set_mark_2")==0 || strcmp(decode_line[temp_assembly.argu_num-2],"set_mark_2")==0)
	{    //minus 2 for the matadata: # size:4
	  temp_assembly.regi[1]=3;
	}
      else
	{
	  perr("register_mark_case: ADD mark case");
	}
    }
  else if(strcmp(decode_line[0],"STV")==0 || strcmp(decode_line[0],"ST")==0)
    {
      if(strcmp(decode_line[temp_assembly.argu_num],"set_mark_4")==0 || strcmp(decode_line[temp_assembly.argu_num-2],"set_mark_4")==0)
	{
	  if(temp_assembly.regi[1]==1)
	    temp_assembly.regi[1]=2;
	  int temp=temp_assembly.regi[0];
	  temp_assembly.regi[0]=temp_assembly.regi[1];
	  temp_assembly.regi[1]=temp;
	}
      else
	{
	  temp_assembly.regi[1]=temp_assembly.regi[0];
	  temp_assembly.regi[0]=3;
	}
    }
  else if(strcmp(decode_line[0],"LDV")==0 || strcmp(decode_line[0],"LD")==0)
    {
      if(strcmp(decode_line[temp_assembly.argu_num],"set_mark_4")==0 || strcmp(decode_line[temp_assembly.argu_num-2],"set_mark_4")==0)
	{
	  if(temp_assembly.regi[1]==1)
	    temp_assembly.regi[1]=2;
	}
      else
	{
	  temp_assembly.regi[0]=3;
	  temp_assembly.regi[1]=2;
	}
    }
  else
    perr("register_mark_case: mark case");
}

void register_decode(int reg_num, int input_class) 
{
  int regi_num;
  int ide_imm=0,i;
  //printf("__________nowRead: %s",nowRead);
  //printf("reg_num: %d\n",reg_num);

  //if((strcmp(decode_line[0],"MAD")==0 || strcmp(decode_line[0],"MADF")==0) && temp_reg_num>3)
  //redo_decode(temp_reg_num);
  
  //for(i=0;i<=temp_assembly.argu_num;i++){printf("decode_line[%d]: %s\n",i,decode_line[i]);}
  
  //printf("ide_imm: %d\n",ide_imm);
  for(regi_num=1;regi_num<=reg_num;regi_num++)
    {
      int ide_dot=0;
      //printf("Register: %d\t%s\n",regi_num,decode_line[regi_num]);
      for(i=0;i<50;i++) //check the imm and dot '.'.
	{
	  if(decode_line[regi_num][i]=='\0')
	    break;
	  if(decode_line[regi_num][i]=='(')
	    {
	      ide_imm=1;
	      ide_dot=0;
	      temp_assembly.ide_imm=1;
	      break;
	    }
	  if(decode_line[regi_num][i]=='.' && i!=0)
	    ide_dot=1;
	}
      if(decode_line[regi_num][0]=='-' && ide_imm==0)
	{
	  if(reg_num<=3 && regi_num!=1)
	    {
	      if(regi_num==2)
		temp_assembly.neg=temp_assembly.neg|4;
	      if(regi_num==3)
		temp_assembly.neg=temp_assembly.neg|2;
	    }
	  else 
	    {
	      if(regi_num==1)
		temp_assembly.neg_change=1;
	      else
		temp_assembly.neg=temp_assembly.neg|1;
	    }
	}
      else if (decode_line[regi_num][0]=='-' && ide_imm==1)
	temp_assembly.imm_sign=1;

      if(regi_num!=reg_num && ((reg_num>3 && reg_num!=1)))
	temp_assembly.neg=temp_assembly.neg<<1;

      if(ide_imm==1 && regi_num==1)
	{
	  printf("Register 1 has imm.\nFormat ERROR!!\n");
	  po->LinkerStatus = GL_FALSE;
	  exit (1);
	}
      char *pRegister,temp_regi[50],temp_buffer[2][50]; 
      //printf("decode_line: %s\n",decode_line[regi_num]);
      int backup_regi_num = regi_num;

      strcpy(temp_buffer[0],"");
      strcpy(temp_buffer[1],"");

      if(decode_line[regi_num][0] != '\0')
	strcpy(temp_regi,decode_line[regi_num]);

      regi_num = backup_regi_num;

      if(ide_imm==1)
	pRegister=strtok(temp_regi,"-()");
      else
	pRegister=strtok(temp_regi,".-()rR");

      //printf("pRegister: %s\n",pRegister);
      strcpy(temp_buffer[0],pRegister);
      if(ide_imm==1)
	pRegister=strtok(NULL,".-()rR");
      else
	pRegister=strtok(NULL,".-()rR");
      //printf("pRegister2: %s\n",pRegister);
      if(pRegister==NULL)
	temp_buffer[1][0]='\0';
      else
	strcpy(temp_buffer[1],pRegister);

      if (ide_imm==1)
	{
	  if((temp_buffer[0][0]>=48 && temp_buffer[0][0]<=57) || temp_buffer[0][0]=='-')
	    {
	      temp_assembly.imm=atoi(temp_buffer[0]);
	      temp_assembly.imm_searched = 2;
	      if(temp_assembly.imm_sign)//Kaymon add this neg imm.
		temp_assembly.imm*=-1;
	      //printf("+++++imm_neg:%d\n",temp_assembly.imm_sign);
	      //printf("++++++temp_assembly.imm: %d\n",temp_assembly.imm);
	    }
	  else
	    {
	      char *pToken,var[128];
	      //printf("Original buffer: %s\n",temp_buffer);
	      pToken=strtok(temp_buffer[0],"@");
	      strcpy(var,pToken);
	      //printf("var: %s\n",var);
	      pToken=strtok(NULL,"@");
	      //printf("ptoken: %s\n",pToken);
	      temp_assembly.imm=search_table(var);
	      temp_assembly.imm_searched = 1;
	      //printf("temp_assembly.imm: %d\n",temp_assembly.imm);

	      if(pToken[0]=='l')
		{
		  temp_assembly.imm=(temp_assembly.imm<<16)>>16;
		}
	      else if(pToken[0]=='h' && pToken[1]=='a')
		{
		  temp_assembly.imm=temp_assembly.imm>>16;
		}
	      else
		perr("IMM variable");

	    }
	  temp_assembly.regi[regi_num-1]=atoi(temp_buffer[1]);
	  //printf("temp_assembly[1]:%d\n",temp_assembly.regi[1]);
	  if(regi_num==2 && strcmp(decode_line[0],"ST")==0)
	    {
	      int temp_regi=0;
	      temp_regi=temp_assembly.regi[0];
	      temp_assembly.regi[0]=temp_assembly.regi[1];
	      temp_assembly.regi[1]=temp_regi;
	    }
	}
      else 
	{

	  if(strcmp(temp_buffer[0],"ZE")==0)
	    {
	      temp_assembly.regi[regi_num-1]=0;
	      //printf("temp_buffer[0]:%s\ttemp_assembly.regi[%d]: %d\n",temp_buffer[0],regi_num-1,temp_assembly.regi[regi_num-1]);
	    }
	  else if(temp_buffer[0][0]>=48 && temp_buffer[0][0]<=57)
	    {
	      //printf("%d\n",atoi(temp_buffer[0]));
	      temp_assembly.regi[regi_num-1]=atoi(temp_buffer[0]);
	    }
	  else 
	    {
	      if((strcmp(decode_line[0],"MAD")==0 || strcmp(decode_line[0],"MADF")==0 || strcmp(decode_line[0],"DIV")==0 || strcmp(decode_line[0],"DSQ")==0) && regi_num==2)
		{
		  temp_assembly.regi[1]=temp_assembly.regi[0];
		}
	    }

	}
      //printf("regi %d: %d\n",regi_num-1,temp_assembly.regi[regi_num-1]);
      if(ide_dot==1)
	{
	  strcpy(temp_regi,decode_line[regi_num]);
	  pRegister=strtok(temp_regi,".");
	  if(decode_line[regi_num][0]!='.')
	    pRegister=strtok(NULL,".");
	  
	  //printf("regi_num: %d\n",regi_num);
	  //printf("dot_tok: %s   atoi:%d    tok[1]:%c\n",pRegister,atoi(pRegister),pRegister[1]);
	  if(atoi(pRegister)==0 && (pRegister[0]<'0' || pRegister[0]>'9'))
	    {
	      int swizzle_num;
	      if(strcmp(decode_line[0],"MAD")==0 || strcmp(decode_line[0],"MADF")==0)
		swizzle_num=regi_num-2;
	      else
		swizzle_num=regi_num-1;
	      //printf("%d\n",swizzle_num );
	      temp_assembly.swizzle[swizzle_num]=0;
	      for(i=0;i<4;i++)
		{
		  //printf("%c\n",pRegister[i]);
		  switch (pRegister[i])
		    {
		    case 'x':
		    case 'X':
		    case 'r':
		    case 'R':
		    case '_':
		      temp_assembly.swizzle[swizzle_num]=temp_assembly.swizzle[swizzle_num]|0;
		      break;
		    case 'y':
		    case 'Y':
		    case 'g':
		    case 'G':
		      temp_assembly.swizzle[swizzle_num]=temp_assembly.swizzle[swizzle_num]|1;
		      break;
                    case 'z':
		    case 'Z':
		    case 'b':
		    case 'B':
                      temp_assembly.swizzle[swizzle_num]=temp_assembly.swizzle[swizzle_num]|2;
		      break;
                    case 'w':
		    case 'W':
		    case 'a':
		    case 'A':
                      temp_assembly.swizzle[swizzle_num]=temp_assembly.swizzle[swizzle_num]|3;
		      break;
		    default:
		      #ifdef detect_error
		      perr("Swizzle input error (valid: xXyYzZwWrRgGbBaA_)");
		      #endif
		      break;
		    }
		  if(i!=3)
		    temp_assembly.swizzle[swizzle_num]=temp_assembly.swizzle[swizzle_num]<<2;
		}
	      //printf("tok:%s\tswizzle(%d):%x\n",pRegister,regi_num-2,temp_assembly.swizzle[regi_num-2]);
	    }
	  else if((atoi(pRegister)!=0 || (atoi(pRegister)==0 && pRegister[0]>='0' && pRegister[0]<='9'))&& regi_num==1)
	    {
	      temp_assembly.wm=0;
	      for(i=0;i<4;i++)
		{
		  if(pRegister[i]=='1')
		    temp_assembly.wm=temp_assembly.wm|1;
		  if(i!=3)
		    temp_assembly.wm=temp_assembly.wm<<1;		  
		}
	      //printf("wm:%x\n",temp_assembly.wm);
	    }
	  else
	    {
	      printf("Register %d format error!!\n",regi_num);
	      //printf("decode_line[%d]: %s\n",regi_num,decode_line[regi_num]);
	      //printf("atoi(pRegister): %d\tregi_num: %d\t,pRegister[0]: %c\n",atoi(pRegister),regi_num,pRegister[0]);
	    }


	}
      //printf("register %d:%d\n",regi_num-1,temp_assembly.regi[regi_num-1]);
    }

  //Change link register R1 to our link register R2
  if(strcmp(decode_line[0],"ADDI")==0)
    {
      if(temp_assembly.regi[1]==1)
	temp_assembly.regi[1]=2;
    }

  //Set-mark help us use our link register R2 and temp register R3
  //handle the set-mark case
  if(temp_assembly.bool_mark_case==1)
      register_mark_case();


  //printf("change_neg:%d\tneg: %x\n",temp_assembly.neg_change,temp_assembly.neg);
#ifdef detect_error
  if(temp_assembly.regi[0]!=temp_assembly.regi[1] && reg_num==4 && temp_assembly.regi[1]!=-1 && (strcmp(decode_line[0],"MAD")==0 || strcmp(decode_line[0],"MADF")==0))
    perr("temp Register 1 and Register 2 must be the same!!");
  if(temp_assembly.regi[0]!=temp_assembly.regi[1] && (strcmp(decode_line[0],"DIV")==0 || strcmp(decode_line[0],"DSQ")==0))
    perr("temp Register 1 and Register 2 must be the same!!");

#endif

  /*all register plus 2*/

  //printf("decode_line: %s  %s  %s  %s  %s\n",decode_line[0],decode_line[1],decode_line[2],decode_line[3],decode_line[4]);

  
  //for(i=0;i<reg_num;i++){temp_assembly.regi[i]+=2;}

}
void altivec_shift_decode(int assign_argu)
{
  temp_assembly.shift_amount=atoi(decode_line[assign_argu]);
}

void write_wm()
{
  nowBin=nowBin>>WM_BIT;
  nowBin=nowBin|temp_assembly.wm;
  nowBin=nowBin<<WM_BIT;
}

void write_scc()
{
  nowBin=nowBin>>SCC_BIT;
  nowBin=nowBin|temp_assembly.scc;
  nowBin=nowBin<<SCC_BIT;

}

void write_swizzle(int input)
{
  int i;
  //for(i=0;i<input;i++){printf("swizzle[%d]: %x\n",i,temp_assembly.swizzle[i]);}

  nowBin=nowBin>>SW_RD_BIT;  

  for(i=0;i<input;i++)
    {
      nowBin=nowBin|temp_assembly.swizzle[i];
      nowBin=nowBin<<SWIZZLE_BIT;
    }

  nowBin=nowBin<<(SW_RD_BIT-(input*SWIZZLE_BIT));

}

void write_branch_swizzle()
{
  int i;
  //for(i=0;i<input;i++){printf("swizzle: %x\n",temp_assembly.swizzle[i]);}

  nowBin=nowBin>>BR_SW_R1_BIT;  

  for(i=0;i<2;i++)
    {
      if(temp_assembly.swizzle[i]!=-1)
	nowBin=nowBin|temp_assembly.swizzle[i];
      nowBin=nowBin<<SWIZZLE_BIT;
    }

  nowBin=nowBin<<(BR_SW_R1_BIT-(2*SWIZZLE_BIT));
}

void write_vector_scalar(int input_vector_scalar)
{
  nowBin=nowBin>>VECTOR_SCALAR_BIT;  
  nowBin|=input_vector_scalar;
  nowBin=nowBin<<VECTOR_SCALAR_BIT;  
}

void write_bits(int input,int former_bit, int later_bit)
{
  int dis=former_bit-later_bit+1;
  if((input>>dis)!=0)
    perr("write_bits");
  nowBin=nowBin>>later_bit;
  nowBin=nowBin|input;
  nowBin=nowBin<<later_bit;
}

void write_nop_number(int nop_number)
{
  if(nop_number<0 || nop_number>15)
    perr("Nop number");
  nowBin=nowBin>>NOP_BIT;
  nowBin=nowBin|nop_number;
  nowBin=nowBin<<NOP_BIT;
}

unsigned int catch_value(int former,int later)
{

#ifdef debug_information
  long long unsigned temp_now;
  unsigned int result;
  
  temp_now=nowBin<<(63-former);
  //printf("%lld\t%lld\n",nowBin,temp_now);
  temp_now=temp_now>>(63-former+later);
  memcpy((void*) &result,(void*) &temp_now,4);
  return result;

#endif

  return 0;

}

void debug_info(int type)
{
#ifdef debug_information
  int i;

  for(i=0;;i++)
    {
      if(decode_line[i][0]=='\0')
	break;
      //printf("%s\t",decode_line[i]);
    }
  
  switch(type)
    {
    case 1:
      printf("\nopcode\tRd\tRs\tRt\tSw_Rd\tSw_Rs\tSw_Rt\tNeg\tWM\tFX/FL\tS/V\n");

      printf(" 0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\n\n",catch_value(63,59),catch_value(58,53),catch_value(52,47),catch_value(46,41),catch_value(40,33),catch_value(32,25),catch_value(24,17),catch_value(16,14),catch_value(13,10),catch_value(9,9),catch_value(8,7));

      break;
      
    case 2:
      printf("\nopcode\tRd\tRs\tIMM\tWM\tFX/FL\tS/V\n");

      printf(" 0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\n\n",catch_value(63,59),catch_value(58,53),catch_value(52,47),catch_value(46,15),catch_value(13,10),catch_value(9,9),catch_value(8,7));

      break;
      
    case 3:
      printf("\nopcode\tRd\tRs\tSw_Rd\tSw_Rs\tNeg\tWM\tFX/FL\tS/V\n");

      printf(" 0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\n\n",catch_value(63,59),catch_value(58,53),catch_value(52,47),catch_value(40,33),catch_value(32,25),catch_value(16,14),catch_value(13,10),catch_value(9,9),catch_value(8,7));

      break;

    case 4:
      printf("\nopcode\tRd\tRs\tAddress\tR3\tR4\tFX/FL\tS/V\n");

      printf(" 0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\n\n",catch_value(63,59),catch_value(58,53),catch_value(52,47),catch_value(46,31),catch_value(30,25),catch_value(24,20),catch_value(9,9),catch_value(8,7));
      break;

    case 5:
      break;
    case 6: //shift
      printf("\nopcode\tRd\tRs\tSA\tSw_Rd\tSw_Rs\tR/L\tWM\tFX/FL\tS/V\n");

      printf(" 0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\n\n",catch_value(63,59),catch_value(58,53),catch_value(52,47),catch_value(46,42),catch_value(40,33),catch_value(32,25),catch_value(14,14),catch_value(13,10),catch_value(9,9),catch_value(8,7));


      break;

    default:
      perr("debug_info");
      break;

    }

#endif
}

void type_nop()
{
  write_nop_number(atoi(decode_line[1]));
  emit(nowBin);
}

void type_altivec (int input_class,int argu_number)
{
  register_decode(3,input_class);
  if(strcmp(decode_line[0],"vsldoi")==0)
    altivec_shift_decode(4);


  if(strcmp(decode_line[0],"vmrghw")==0)
    {
      inline_vmrghw(temp_assembly.regi[0],temp_assembly.regi[1],temp_assembly.regi[2],NULL);
    }

  if(strcmp(decode_line[0],"vsldoi")==0)
    {
      inline_vsldoi(temp_assembly.regi[0],temp_assembly.regi[1],temp_assembly.regi[2],temp_assembly.shift_amount,NULL);
    }
 

}

void type_branch_less_equal_or_greater_equal(int argu_number, char VOrF)
{
  //printf("number:%d\n",argu_number);
  if(strcmp(decode_line[0],"BLE")==0 || strcmp(decode_line[0],"BLEF")==0 || strcmp(decode_line[0],"ble")==0 || strcmp(decode_line[0],"blef")==0 || strcmp(decode_line[0],"BLES")==0 || strcmp(decode_line[0],"BLESF")==0 || strcmp(decode_line[0],"BLEV")==0 || strcmp(decode_line[0],"BLEVF")==0 )
    {
      if(decode_line[argu_number-1][0]!='.')
	{
	  //printf("---- +1:%s\n",decode_line[argu_number-1]);
	  sprintf(decode_line[argu_number-1],"%d",atoi(decode_line[argu_number-1])+1);
	}
      nowBin=26; //BLT
      nowBin=nowBin<<OPCODE_BIT;
      if(!strcmp(decode_line[0],"BLEF") || !strcmp(decode_line[0],"blef") || !strcmp(decode_line[0],"BLESF") || !strcmp(decode_line[0],"BLEVF"))
	type_4(6,argu_number,1,VOrF);
      else
	type_4(6,argu_number,0,VOrF);
      if(decode_line[argu_number-1][0]!='.')
	{
	  //printf("---- -1:%s\n",decode_line[argu_number-1]);
	  sprintf(decode_line[argu_number-1],"%d",atoi(decode_line[argu_number-1])-1);
	}

      PC+=8;
      nowBin=24; //BEQ
      nowBin=nowBin<<OPCODE_BIT;
      type_4(6,argu_number,0,VOrF);
      PC-=8;
    }
  if(strcmp(decode_line[0],"BGE")==0 || strcmp(decode_line[0],"BGEF")==0 || strcmp(decode_line[0],"BGES")==0 || strcmp(decode_line[0],"BGESF")==0 || strcmp(decode_line[0],"BGEV")==0 || strcmp(decode_line[0],"BGEVF")==0 || strcmp(decode_line[0],"bge")==0 || strcmp(decode_line[0],"bgef")==0)
    {
      if(decode_line[argu_number-1][0]!='.')
	{
	  sprintf(decode_line[argu_number-1],"%d",atoi(decode_line[argu_number-1])+1);
	}
      nowBin=27; //BGT
      nowBin=nowBin<<OPCODE_BIT;
      if(!strcmp(decode_line[0],"BGEF") || !strcmp(decode_line[0],"BGESF") || !strcmp(decode_line[0],"BGEVF") || !strcmp(decode_line[0],"bgef"))
	type_4(6,argu_number,1,VOrF);
      else
	type_4(6,argu_number,0,VOrF);
      if(decode_line[argu_number-1][0]!='.')
	{
	  sprintf(decode_line[argu_number-1],"%d",atoi(decode_line[argu_number-1])-1);
	}

      PC+=8;
      nowBin=24; //BEQ
      nowBin=nowBin<<OPCODE_BIT;
      type_4(6,argu_number,0,VOrF);
      PC-=8;
    }
}

void type_1(int input_class,int argu_number,int F)
{
  //printf("type_1: %s\targu_num: %d\n",decode_line[0],argu_number);
  int i;

  if (input_class == 2 )
    {
      if(argu_number==2)
	{
	  register_decode(2,input_class);
	  write_register(2,NO_MEANING);
          if(temp_assembly.wm==-1)
            {
              //printf("swizzle[0]: %d\n",temp_assembly.swizzle[0]);
                temp_assembly.wm=8;
            }
	  if(strcmp(decode_line[0],"RED")==0 || strcmp(decode_line[0],"GETX")==0)
	    {
	      temp_assembly.swizzle[0]=0x1B;
	      temp_assembly.swizzle[1]=0;
	      temp_assembly.swizzle[2]=27;
	    }
          if(strcmp(decode_line[0],"GREEN")==0 || strcmp(decode_line[0],"GETY")==0)
            {
              temp_assembly.swizzle[0]=0x1B;
              temp_assembly.swizzle[1]=85;
              temp_assembly.swizzle[2]=27;
            }
	  if(strcmp(decode_line[0],"BLUE")==0 || strcmp(decode_line[0],"GETZ")==0)
            {
              temp_assembly.swizzle[0]=0x1B;
              temp_assembly.swizzle[1]=170;
              temp_assembly.swizzle[2]=27;
            }
          if(strcmp(decode_line[0],"ALPHA")==0 || strcmp(decode_line[0],"GETW")==0)
            {
              temp_assembly.swizzle[0]=0x1B;
              temp_assembly.swizzle[1]=255;
              temp_assembly.swizzle[2]=27;
            }
	  write_swizzle(3);
	  if(temp_assembly.neg==1)
	    {
	      temp_assembly.neg=3;
	    }
	  write_neg(14);
	  write_wm();
	  write_sv(3);
	}
      else
	{
	  if(strcmp(decode_line[0],"MAD")==0 || strcmp(decode_line[0],"MADF")==0 )
	    register_decode(4,input_class);
	  else
	    register_decode(3,input_class);
	  write_register(3,NO_MEANING);
	  //write_neg(14);
	  if(temp_assembly.wm==-1)
	    {
	      //printf("swizzle[0]: %d\n",temp_assembly.swizzle[0]);
	      if(temp_assembly.swizzle[1]!=-1 && strcmp(decode_line[0],"MAD")!=0 && strcmp(decode_line[0],"MADF")!=0)
		temp_assembly.wm=15;
	      else
		temp_assembly.wm=8;
	    }
	  if(decode_line[0][0]=='M' && decode_line[0][1]=='U' && decode_line[0][2]=='L' && temp_assembly.swizzle[1]!=-1 && temp_assembly.swizzle[2]!=-1 )
	    temp_assembly.sv=3;
	  
	  for(i=0;i<3;i++)
	    {
	      //printf("swizzle[%d]: %d\n",i,temp_assembly.swizzle[i]);//swizzle for registers.
	      if(temp_assembly.swizzle[i]==-1)
		{
		  temp_assembly.swizzle[i]=0;
#ifdef edward_work
		  temp_assembly.swizzle[0]=0x1b;//Suit the spec. Kaymon. It's for the swizzle of R1. It will be always 0x1b.
		  temp_assembly.sv=3;           //set 3 to sv. It's fine. Now just temporarily serve for edward.
#endif
		}
	      else if(i!=2 && temp_assembly.swizzle[i]!=-1)
		{
		  temp_assembly.sv=3;
		}
	    }
	  
	  write_swizzle(3);
	  
	  if(strcmp(decode_line[0],"SUB")==0 || strcmp(decode_line[0],"SUBF")==0)
	    {
#ifdef detect_error
	      if(temp_assembly.neg!=0 || temp_assembly.neg_change!=0)
		perr("Neg");
#endif	  
	      temp_assembly.neg=2;
	    }
	  
	  write_neg(14);
	  write_wm();
	  if(F==1)
	    FXOrFL((char *)"FL");
	  
	  if(decode_line[argu_number][0]=='<')
	    SOrV(argu_number);
	  //printf("decode: %s\n",decode_line[0]);
	  else if(temp_assembly.sv!=-1)
	    write_sv(temp_assembly.sv);
	}

      /*
      if(F==1 && (temp_assembly.neg!=0 || temp_assembly.neg_change!=0) && strcmp(decode_line[0],"ADD")!=0 && strcmp(decode_line[0],"ADDF")!=0 && strcmp(decode_line[0],"SUBF")!=0)
	  perr("If FL set, no Neg!!");

      if(strcmp(decode_line[0],"SUBF")==0)
	{
	  //printf("LastBinLine: %d\n",LastBinLine);
	  inline_SUBF(temp_assembly.regi[0],temp_assembly.regi[1],temp_assembly.regi[2],temp_assembly.wm,temp_assembly.sv+1);
	  //printf("LastBinLine: %d\n",LastBinLine);
	}
      else
	emit(nowBin);
      */
      emit(nowBin);
    }
  else if (input_class == 3)
    {
      //printf("opcode: %s\tarug:%d\n",decode_line[0],temp_assembly.argu_num);

      if(temp_assembly.argu_num==3 ||temp_assembly.argu_num==4 ||temp_assembly.argu_num==5 || temp_assembly.argu_num==6)
	{
	  register_decode(3,input_class);

	  write_register(3,NO_MEANING);
	  for(i=0;i<3;i++)
	    temp_assembly.swizzle[i]=27;
	  write_swizzle(3);
	  write_neg(14);
	  if(temp_assembly.wm==-1)
	    temp_assembly.wm=8;
	  //printf("wm: %d\n",temp_assembly.wm);
	  write_wm();
	  if(temp_assembly.argu_num==4)
	    SOrV(argu_number);
	  if(temp_assembly.argu_num==6)
	    SOrV(argu_number-2);
	}
      else if(temp_assembly.argu_num==2)
	{
	  register_decode(2,input_class);
	  write_register(2,NO_MEANING);
	  for(i=0;i<3;i++)
            temp_assembly.swizzle[i]=27;
          write_swizzle(3);
          if(temp_assembly.wm==-1)
            temp_assembly.wm=8;
          write_wm();

	}

      //ari_register(3);
      //FX/FL
      if(decode_line[argu_number][0]=='<')
	SOrV(argu_number);

      if(F==1 && (temp_assembly.neg!=0 || temp_assembly.neg_change!=0))
          perr("If FL set, no Neg!!");

      emit(nowBin);
    }
  else if (input_class == 4)
    {
      //printf("opcode: %s\n",decode_line[0]);
      register_decode(3,input_class);
      write_register(3,NO_MEANING);
      //printf("decode_line[3]: %s\t atoi: %d\n",decode_line[3],atoi(&decode_line[3][1]));

      //minus #
      //if(argu_number!=2)
      //minus #
      //shift_amount(atoi(&decode_line[3][0])-1);

      for(i=0;i<2;i++)
	{
	  if(temp_assembly.swizzle[i]==-1)
	    temp_assembly.swizzle[i]=27;
	}
      write_swizzle(2);

      if(decode_line[0][2]=='L')
	ROrL('L');
      //imm(2);
      //write_imm(temp_imm);
      //ROrL(atoi(decode_line[4]));
      if(argu_number!=4)
	temp_assembly.wm=8;
      write_wm();
      if(decode_line[argu_number][0]=='<')
        SOrV(argu_number);
      if(F==1 && (temp_assembly.neg!=0 || temp_assembly.neg_change!=0))
          perr("If FL set, no Neg!!");

      emit(nowBin);
    }
  else if(input_class==7)
    {
      register_decode(3,input_class);
      write_register(3,NO_MEANING);
      for(i=0;i<3;i++)
	{
	  if(temp_assembly.swizzle[i]==-1)
	    temp_assembly.swizzle[i]=27;
	}
      write_swizzle(3);
      if(temp_assembly.wm==-1)
	{
	  if(argu_number==3)
	    temp_assembly.wm=8;
	  else if(argu_number==4)
	    temp_assembly.wm=15;
	      
	}
      write_neg(14);
      write_wm();
      if(F==1)
	FXOrFL((char *)"FL");
      if(decode_line[argu_number][0]=='<')
	SOrV(argu_number);

      emit(nowBin);
    }
  else if(input_class==9)
    {
      register_decode(2,input_class);
      write_register(2,NO_MEANING);
#ifdef edward_work
      temp_assembly.swizzle[0]=27; //just for FS. because FS care these bits.
      temp_assembly.swizzle[1]=27; //just for FS. because FS care these bits.
      temp_assembly.swizzle[2]=27; //just for FS. because FS care these bits.
      write_swizzle(3);
#endif
      write_wm();
      if(decode_line[0][0]=='F')
	write_bits(1,9,9);
      if(decode_line[argu_number][0]=='<')
        SOrV(argu_number);
      emit(nowBin);
    }

  if(input_class!=4)
    debug_info(1);
  else 
    debug_info(6);

}

void type_2(int input_class,int argu_number,int F)
{
  //printf("type_2: %s %s\targu_num: %d\tF:%d\n",decode_line[0],decode_line[1],argu_number,F);  
  //printf("input_class:%d %s",input_class,nowRead);
  long long unsigned temp_instruction;
  int insert_instruction_mark = 0;
  if(strcmp(decode_line[0],"LD")==0 || strcmp(decode_line[0],"ADDI")==0 || strcmp(decode_line[0],"ADDIF")==0 || strcmp(decode_line[0],"ST")==0 ||strcmp(decode_line[0],"LDV")==0 || strcmp(decode_line[0],"STV")==0)
    register_decode(2,1);
  else
    register_decode(1,1);
  /*
  if(temp_assembly.imm%16 != 0 && (strncmp(decode_line[0],"LDV",3) ==0 || strncmp(decode_line[0],"STV",3) ==0))
    {
      if(temp_assembly.imm%4 == 0)
	{
	  int assigned_component = temp_assembly.imm%16/4;
	  int swizzle_count = 0;

	  switch(assigned_component)
	    {
	    case 1:
	      swizzle_count = 7003;
	      break;
	    case 2:
	      swizzle_count = 7067;
	      break;
	    case 3:
	      swizzle_count = 7131;
	      break;
	    default:
	      perr("imm condition");
	    }

	  if(strncmp(decode_line[0],"LDV",3) ==0 && temp_assembly.regi[1]==2 )
	    {
	      int ld_rd=temp_assembly.regi[0];

	      insert_instruction_mark = 1;
	      temp_assembly.regi[0] = 3;
	      temp_assembly.imm = temp_assembly.imm/16 * 16;
	      temp_assembly.wm=15;

	      temp_instruction = 7;
	      temp_instruction = temp_instruction << 6;
	      temp_instruction |= ld_rd;
	      temp_instruction = temp_instruction << 6;
	      temp_instruction |= 3;
	      temp_instruction = temp_instruction << 22;
	      temp_instruction |= swizzle_count;
	      temp_instruction = temp_instruction << 15;
	      temp_instruction |= 8;
	      temp_instruction = temp_instruction << 3;
	      temp_instruction |= 3;
	      temp_instruction = temp_instruction << 7;

	      //printf("%s\tFix.\n",nowRead);
	    }
	  else if(strncmp(decode_line[0],"STV",3) == 0 && temp_assembly.regi[0]==2)
	    {
	      insert_instruction_mark = 2;
	      //temp_assembly.imm = temp_assembly.imm/16 * 16;

	      switch(assigned_component)
		{
	      case 1:
		temp_assembly.wm = 4;
		break;
	      case 2:
		temp_assembly.wm = 2;
		break;
	      case 3:
		temp_assembly.wm = 1;
		break;
		}

	      //printf("%s\tFix.\n",nowRead);
	    }
	}
      else if(!temp_assembly.imm_searched)
	{
	  printf("LDV or STV imm need to fix\n");
	}
    }
*/
  //printf("now Read:%s",nowRead);
  //printf("temp[%d]:%d\ttemp[%d]:%d\ttemp[%d]:%d\n\n",0,temp_assembly.regi[0],1,temp_assembly.regi[1],2,temp_assembly.regi[2]);
  if(!strcmp(decode_line[0],"LDV")&&temp_assembly.regi[1]==1){ //LDV instruction indicate  the wrong stack register.
    //printf("come here\n");
    temp_assembly.regi[1]=2;
    //printf("temp[%d]:%d\ttemp[%d]:%d\ttemp[%d]:%d\n\n",0,temp_assembly.regi[0],1,temp_assembly.regi[1],2,temp_assembly.regi[2]);
  }

  if(temp_assembly.regi[1]!=-1)
    {
      write_register(2,NO_MEANING);
      if(F==1)
	imm(3,FLOAT,2);
      else
	imm(3,DEFAULT,2);

    }
  else
    {
      write_register(1,NO_MEANING);
      if(F==1)
	imm(2,FLOAT,2);
      else
	imm(2,DEFAULT,2);
    }

  if(temp_assembly.wm==-1)
    {
      if(strcmp(decode_line[0],"LDV")==0 ||strcmp(decode_line[0],"STV")==0)
	temp_assembly.wm=15;
      else
	temp_assembly.wm=8;
    }
  write_wm();

  if(strcmp(decode_line[0],"LDV")==0 ||strcmp(decode_line[0],"STV")==0)
    {
      //printf("%s",nowRead);
      //printf("Rd: %d\tRs: %d\n",temp_assembly.regi[0],temp_assembly.regi[1]);
    }

  if(F==1)
    FXOrFL((char *)"FL");
  if(strcmp(decode_line[0],"ADDI")==0 || strcmp(decode_line[0],"ADDIF")==0 || strcmp(decode_line[0],"ST")==0 )
    {
      if(decode_line[argu_number-2][0]=='<')
	SOrV(argu_number-2);
    }
  
  if( strcmp(decode_line[0],"STV")==0 || strcmp(decode_line[0],"LDV")==0 || strcmp(decode_line[0],"LDIF")==0 || strcmp(decode_line[0],"MVIF")==0 )
    {
      write_sv(3);
    }

  if(insert_instruction_mark == 1)
    {
      emit(nowBin);
      emit(temp_instruction);
    }
  else if(insert_instruction_mark == 2)
    {
      emit(nowBin);
    }
  else
    emit(nowBin);

  debug_info(2);
}

void type_3(int input_class,int argu_number,int F)
{
  //printf("type_3: %s\targu_num: %d\n",decode_line[0],argu_number);
  int i,exist_wm_sv=0;
  if((strcmp(decode_line[0],"DIV")==0 || strcmp(decode_line[0],"DSQ")==0) && argu_number == 3)
    register_decode(3,input_class);
  else
    register_decode(2,input_class);
  if(strcmp(decode_line[0],"MVR")==0 || strcmp(decode_line[0],"mr")==0 )
    {
      write_register(2,DEFAULT);
#ifdef edward_work
      for(i=0;i<3;i++)  //kaymon change i<2 to i<3. This work is for edward.
#else
      for(i=0;i<2;i++)  //old version.
#endif
	{
	  //printf("temp_assembly.swizzle[i]: %d\n",temp_assembly.swizzle[i]);
	  if(temp_assembly.swizzle[i]==-1)
	    temp_assembly.swizzle[i]=27;
	  else if(i==1)
	    exist_wm_sv=1;
	}
#ifdef edward_work
      write_swizzle(3); //For edward's work
#else
      write_swizzle(2);
#endif
      if(temp_assembly.neg_change==1)
	{
	  temp_assembly.neg=temp_assembly.neg|2;
	  temp_assembly.neg_change=0;
	}
      //write_neg(15);
      write_neg(14); //I thought the argument of write_neg is 14; Kaymon. old version is above.
      if(exist_wm_sv==1)
	{
	  write_wm();
	  SOrV(3);
	}
      else
	{
	  if(temp_assembly.wm==-1)
	    temp_assembly.wm=8;
	  write_wm();
	}
#ifdef edward_work
      FXOrFL("FL"); //set 1 to MVR's Fbit. For edward's work. 
#endif
    }
  else 
    {
      /*
      if(strcmp(decode_line[0],"DIV")==0 || strcmp(decode_line[0],"DSQ")==0 )
	write_register(3,DEFAULT);
      else
      */
      write_register(2,DEFAULT);
#ifdef edward_work
      for(i=0;i<3;i++)  //kaymon change i<2 to i<3. This work is for edward.
#else
      for(i=0;i<2;i++)
#endif
        {
#ifdef edward_work
	  if(temp_assembly.swizzle[i]==-1){
	    if(i==1)
	      temp_assembly.swizzle[i]=0x55;
	    else
	      temp_assembly.swizzle[i]=27;
	  }
#else
          if(temp_assembly.swizzle[i]==-1)
            temp_assembly.swizzle[i]=27;
#endif
          else if(i==1)
            exist_wm_sv=1;
        }
#ifdef edward_work
      write_swizzle(3); //For edward's work
#else
      write_swizzle(2);
#endif
      if(temp_assembly.wm==-1)
	  temp_assembly.wm=8;
      write_wm();
      FXOrFL((char *)"FL");
    }
#ifdef edward_work
  strcpy(decode_line[3],"<4>"); //basicly for RCP. set 3 to RCP's SV.
  SOrV(3);//set 3 to sv
#endif
  if( !strcmp("Fragment",decode_line[5])&& !strcmp("interface",decode_line[6]) && !strcmp("register)",decode_line[7]))//Add this condition for like MVR R0.1111,R18.xyzw,<4> (Rd(R0): Fragment interface register)
    nowBin = nowBin|0x40;

  emit(nowBin);

  debug_info(3);

}

void type_4(int input_class,int argu_number,int F, char VOrF = 'v')
{
  //printf("type_4: %s\targu_num: %d\n",decode_line[0],argu_number);
  //printf("type_4: %s\targu_num: %d\n",decode_line[1],argu_number);
  //printf("type_4: %s\targu_num: %d\n",decode_line[2],argu_number);
  //printf("type_4: %s\targu_num: %d\n",decode_line[3],argu_number);
  //printf("type_4: %s\targu_num: %d\n",decode_line[4],argu_number);

  if(strcmp(decode_line[0],"BL")==0)
    strcpy(decode_line[0],"B");
  int i;
  if(strcmp(decode_line[0],"B")!=0)
    register_decode(argu_number-2,input_class);

  for(i=0;i<512;i++)
    {
      if(decode_line[0][i]=='\0')
	break;
      if(i==2 && decode_line[0][2]=='E')
	{
	  if( !strcmp("BLE",decode_line[0]) ){ //Kaymon
	    continue;
	  }
	  /* //comment first. Why should we exchange these two registers? 
	  int temp=temp_assembly.regi[0];
	  temp_assembly.regi[0]=temp_assembly.regi[1];
	  temp_assembly.regi[1]=temp;
	  temp=temp_assembly.regi[2];
	  temp_assembly.regi[2]=temp_assembly.regi[3];
	  temp_assembly.regi[3]=temp;
	  */
	}
    }

  if(strcmp(decode_line[0],"JR")==0)
    {
      for(i=0;i<4;i++)
	temp_assembly.regi[i]=0;
      write_register(2,DEFAULT);
      //minus #
      /*
#ifdef detect_error
      if(decode_line[1][0]!='#')
	perr("IMM");
#endif
      */
      imm(1,BRANCH_IMM,4);
      if(VOrF == 'v')
	write_register(2,BRANCH_WRITE_LATER_REGISTER);
      else
	write_vector_scalar(1);
    }
  else if(strcmp(decode_line[0],"JMP")==0 || strcmp(decode_line[0],"B")==0)
    {
      for(i=0;i<2;i++)
        temp_assembly.regi[i]=0;
      write_register(2,DEFAULT);
      //minus #
      /*
#ifdef detect_error
      if(decode_line[1][0]!='#' && strcmp(decode_line[0],"JMP")==0)
        perr("IMM");
#endif
      */
      if(strcmp(decode_line[0],"JMP")==0)
	imm(1,BRANCH_IMM,4);
      else if(strcmp(decode_line[0],"B")==0)
	{
	  //temp_assembly.imm=search_table(decode_line[1]);
	  //printf("imm:%d\n",search_table(decode_line[1]));
	  //printf("@@@@@@@@@@@@@ %s %s\n",decode_line[0],decode_line[1]);
	  fflush(stdout);
	  if(search_table(decode_line[1])%8)
	    perr("Function pc value");
	  temp_assembly.imm=search_table(decode_line[1])/8;
	  if(temp_assembly.imm == address/8)
	    temp_assembly.imm-=1;
	  temp_assembly.ide_imm=1;
	  imm(1,DEFAULT,4);
	}

    }
  else if(strcmp(decode_line[0],"RET")==0)
    {
      temp_assembly.regi[0]=0;
      temp_assembly.regi[1]=atoi(&decode_line[1][1]);
      write_register(2,DEFAULT);

    }
  else if(argu_number==4)
    {
      //For unified shader, all branch instructions just have the only one type.
      VOrF = 'f';
      write_register(2,BRANCH_WRITE_FRONT_REGISTER);
      //minus #
      /*
#ifdef detect_error
      if(decode_line[3][0]!='#')
        perr("IMM");
#endif
      */
      if(decode_line[3][0]=='.')
	{
	  //printf("decode_line[3]: %s: %d\tPC: %d\n",decode_line[3],search_table(decode_line[3]),PC);
	  temp_assembly.imm=(search_table(decode_line[3])-PC)/8;
	  //printf("imm: %d\n",temp_assembly.imm);
	  temp_assembly.ide_imm=1;
	}
      imm(3,BRANCH_IMM,4);
      
      if(VOrF=='v')
	{
	  write_register(2,BRANCH_WRITE_LATER_REGISTER);
	  temp_assembly.scc=8;
	  write_scc();
	}
      else
	{
	  write_branch_swizzle();
	  write_vector_scalar(1);
	}

      if(F==1)
	FXOrFL((char *)"FL");

      SOrV(4);
    }
  else if(argu_number==7)
    {
      //For unified shader, all branch instructions just have the only one type.
      VOrF = 'f';

      printf("type_4(): decode_line[0]: %s\n",decode_line[0]);
      write_register(2,DEFAULT);
      //minus #
      /*
#ifdef detect_error
      if(decode_line[5][0]!='#')
        perr("IMM");
#endif
      */
      imm(5,BRANCH_IMM,4);
      write_register(2,BRANCH_WRITE_LATER_REGISTER);

      printf("decode_line[6]: %s\n",decode_line[6]);
      temp_assembly.scc=0;
      if(decode_line[6][0]=='1')
	temp_assembly.scc|=8;
      if(decode_line[6][1]=='1')
	temp_assembly.scc|=4;
      if(decode_line[6][2]=='1')
	temp_assembly.scc|=2;
      if(decode_line[6][3]=='1')
	temp_assembly.scc|=1;
      
      write_scc();

      if(F==1)
        FXOrFL((char *)"FL");

      SOrV(7);
    }
  emit(nowBin);

  debug_info(4);
}

void type_5(int input_class,int argu_number)
{

  //printf("type_5: %s\targu_num: %d\n",decode_line[0],argu_number);
  //  Myfprintf(pWBinary,"(%s)\n",decode_line[0]);
  register_decode(2,input_class);
  write_register(2,DEFAULT);
  temp_assembly.wm=15;
  write_wm();

  emit(nowBin);
  debug_info(5);
}


int deopcode(char type,struct programobject *input_object)
{
  int i,FXorFL=0;
  //printf("int :%d\tchar: %c\tstring: %s\n",*(decode_line[0]+1),*(decode_line[0]+1),&decode_line[0]);
  //printf("%s %s\n",decode_line[0],decode_line[1]);
  fflush(stdout);
  if(strncmp(decode_line[0],"gpu_",4)==0)
    {
      char temp_string[512];
      strcpy(temp_string,&decode_line[0][4]);
      sprintf(decode_line[0],"%s",temp_string);
      //printf("%s\n",temp_string);
      if(strcmp(decode_line[0],"beqf")==0 || strcmp(decode_line[0],"bnef")==0){
	decode_line[0][3]='\0';
	FXorFL=1;
      }
      for(i=0;i<512;i++)//make op code to upcast.
	{
	  if(decode_line[0][i]!='\0')
	    decode_line[0][i]-=32;
	  else
	    break;
	}
    }
  int b_float=0;
  switch(*decode_line[0])
    {
    case 'A':
      switch(*(decode_line[0]+1))
	{  
	case 'D':
	  if(*(decode_line[0]+2)=='D')
	  {
	    switch(*(decode_line[0]+3))
	      {
	      case '\0':
		nowBin=16;
		nowBin=nowBin<<OPCODE_BIT;
		type_1(2,temp_assembly.argu_num,0);
		break;
	      case 'I':
		if(*(decode_line[0]+4)=='F' || *(decode_line[0]+4)=='\0')
		  {
		    nowBin=10;
		    nowBin=nowBin<<OPCODE_BIT;
		    if(*(decode_line[0]+4)=='\0')
		      {
			type_2(1,temp_assembly.argu_num,0);
			break;
		      }
		    else
		      {
			if(*(decode_line[0]+5)!='\0')
			  {
			    no_opcode();
			    break;
			  }
			else
			  {
			    type_2(1,temp_assembly.argu_num,1);
			    break;
			  }
		      }
		  }
		break;
	      case 'F':
		if(*(decode_line[0]+4)!='\0')
		  no_opcode();
		else
		  {
		    nowBin=16;
		    nowBin=nowBin<<OPCODE_BIT;
		    type_1(2,temp_assembly.argu_num,1);
		  }
		break;
	      default:
		no_opcode();
		break;
	      }
	  }
	  else
	    {
	      no_opcode();
	      break;
	    }
	  break;
	case 'L':
	  if(*(decode_line[0]+2)=='P' && *(decode_line[0]+3)=='H' && *(decode_line[0]+4)=='A' && *(decode_line[0]+5)=='\0')
	    {
	      nowBin=16;
	      nowBin=nowBin<<OPCODE_BIT;
	      type_1(2,temp_assembly.argu_num,0);
	      break;
	    }
	  no_opcode();
	  break;
	case 'N':
	  if(*(decode_line[0]+2)!='D' || *(decode_line[0]+3)!='\0')
	  {
	    no_opcode();
	    break;
	  }
	  nowBin=21;
	  nowBin=nowBin<<OPCODE_BIT;
	  type_1(3,temp_assembly.argu_num,0);
	  break;
	default:
	  no_opcode();
	  break;
    
	}
      break;
    case 'B':
      switch(*(decode_line[0]+1))
	{
	case 'E':
	  if(*(decode_line[0]+2) != 'Q')
	    {
	      no_opcode();
	      break;
	    }
	  else if(*(decode_line[0]+2) == 'Q' && *(decode_line[0]+3)== 'F' && *(decode_line[0]+4)!= 'S' && *(decode_line[0]+4) != 'V' )
	    {
	      if(*(decode_line[0]+4)!='\0')
		{
		  no_opcode();
		  break;
		}
	    }
	  else if(*(decode_line[0]+3) != 'S' && *(decode_line[0]+3) != 'V' && *(decode_line[0]+3) != '\0')
	    {
	      no_opcode();
	      break;
	    }
	  else if(*(decode_line[0]+3) == 'S' || *(decode_line[0]+3) == 'V')
	    if(*(decode_line[0]+4)!='\0')
	      {
		no_opcode();
		break;
	      }

	  nowBin=24;
	  nowBin=nowBin<<OPCODE_BIT;
	  if(type == 'a')
	    {
	      if(*(decode_line[0]+3)!='S')
		type_4(6,temp_assembly.argu_num,0,'v');
	      else
		type_4(6,temp_assembly.argu_num,0,'f');
	    }
	  else if(type == 'v')
	    {
	      if(*(decode_line[0]+3)=='S')
		{
		  perr("Vector shader doesn't have branch scalar instructions");
		}
	      else
		type_4(6,temp_assembly.argu_num,0,type);
	    }
	  else if(type == 'f')
	    {
	      if(*(decode_line[0]+3)=='V')
		{
		  perr("Fragment shader doesn't have branch vector instructions");
		}
	      else
		//type_4(6,temp_assembly.argu_num,0,type); 
		type_4(6,temp_assembly.argu_num,FXorFL,type);
	    }
	  else
	    perr("deopcode(): type is wrong.");
	  break;
	case 'G':
	  if(*(decode_line[0]+2)!='T' && *(decode_line[0]+2)!='E')
	  {
            no_opcode();
	    break;
	  }
	  b_float=0;
	  if(*(decode_line[0]+3)=='V' || *(decode_line[0]+3)=='S' || *(decode_line[0]+3)=='F')
	    {
	      if(*(decode_line[0]+3) == 'F')
		{
		  if(*(decode_line[0]+4) != '\0')
		    {
		      no_opcode();
		      break;
		    }
		  b_float=1;
		}
	      else
		{
		  if(*(decode_line[0]+4) != 'F' && *(decode_line[0]+4) != '\0')
		    {
		      no_opcode();
		      break;
		    }
		  else
		    {
		      if(*(decode_line[0]+4) == 'F')
			{
			  if(*(decode_line[0]+5) != '\0')
			    {
			      no_opcode();
			      break;
			    }
			  b_float=1;
			}
		    }
		}
	    }
	  
	  if(*(decode_line[0]+2)=='T')
	    nowBin=27;
	  else
	    {
	      type_branch_less_equal_or_greater_equal(temp_assembly.argu_num,type);
	      break;
	    }
          nowBin=nowBin<<OPCODE_BIT;

	  if(type == 'a')
	    {
	      if(*(decode_line[0]+3)!='S')
		type_4(6,temp_assembly.argu_num,b_float,'v');
	      else
		type_4(6,temp_assembly.argu_num,b_float,'f');
	    }
	  else if(type == 'v')
	    {
	      if(*(decode_line[0]+3)=='S')
		{
		  perr("Vector shader doesn't have branch scalar instructions");
		}
	      else
		type_4(6,temp_assembly.argu_num,b_float,type);
	    }
	  else if(type == 'f')
	    {
	      if(*(decode_line[0]+3)=='V')
		{
		  perr("Fragment shader doesn't have branch vector instructions");
		}
	      else
		type_4(6,temp_assembly.argu_num,b_float,type);
	    }
	  else
	    {
	      perr("deopcode(): type is wrong.");
	    }
	  break;

        case 'L':
	  if(*(decode_line[0]+2)=='\0')
	    {
	      callinlinefunction(type,input_object);
	      break;
	    }
          if(*(decode_line[0]+2)=='U' && *(decode_line[0]+3)=='E' &&*(decode_line[0]+4)=='\0')
	    {
	      nowBin=16;
	      nowBin=nowBin<<OPCODE_BIT;
	      type_1(2,temp_assembly.argu_num,0);
	      break;
	    }
	  if(*(decode_line[0]+2)!='T' && *(decode_line[0]+2)!='E')
	  {
            no_opcode();
	    break;
	  }
	  b_float=0;
	  if(*(decode_line[0]+3)=='V' || *(decode_line[0]+3)=='S' || *(decode_line[0]+3)=='F')
	    {
	      if(*(decode_line[0]+3) == 'F')
		{
		  if(*(decode_line[0]+4) != '\0')
		    {
		      no_opcode();
		      break;
		    }
		  b_float=1;
		}
	      else
		{
		  if(*(decode_line[0]+4) != 'F' && *(decode_line[0]+4) != '\0')
		    {
		      no_opcode();
		      break;
		    }
		  else
		    {
		      if(*(decode_line[0]+4) == 'F')
			{
			  if(*(decode_line[0]+5) != '\0')
			    {
			      no_opcode();
			      break;
			    }
			  b_float=1;
			}
		    }
		}
	    }

	  if(*(decode_line[0]+2)=='T')
	    nowBin=26;
	  else
	    {
	      type_branch_less_equal_or_greater_equal(temp_assembly.argu_num,type);
	      break;
	    }
          nowBin=nowBin<<OPCODE_BIT;

	  if(type == 'a')
	    {
	      if(*(decode_line[0]+3)!='S')
		type_4(6,temp_assembly.argu_num,b_float,'v');
	      else
		type_4(6,temp_assembly.argu_num,b_float,'f');
	    }
	  else if(type == 'v')
	    {
	      if(*(decode_line[0]+3)=='S')
		{
		  perr("Vector shader doesn't have branch scalar instructions");
		}
	      else
		type_4(6,temp_assembly.argu_num,b_float,type);
	    }
	  else if(type == 'f')
	    {
	      if(*(decode_line[0]+3)=='V')
		{
		  perr("Fragment shader doesn't have branch vector instructions");
		}
	      else
		type_4(6,temp_assembly.argu_num,b_float,type);
	    }
	  else
	    {
	      perr("deopcode(): type is wrong.");
	    }
	  break;

	case 'N':
	  if(*(decode_line[0]+2)!='E')
	    {
	      no_opcode();
	      break;
	    }
	  if(*(decode_line[0]+3) != 'S' && *(decode_line[0]+3) != 'V' &&*(decode_line[0]+3) != 'F' && *(decode_line[0]+3) != '\0')
	    {
	      no_opcode();
	      break;
	    }
	  if(*(decode_line[0]+3) == 'S' || *(decode_line[0]+3) == 'V'|| *(decode_line[0]+3) == 'F')
	    if(*(decode_line[0]+4)!='\0')
	      {	     
		no_opcode();
		break;
	      }
	  nowBin=25;
          nowBin=nowBin<<OPCODE_BIT;
	  if(type == 'a')
	    {
	      if(*(decode_line[0]+3)!='S')
		type_4(6,temp_assembly.argu_num,0,'v');
	      else
		type_4(6,temp_assembly.argu_num,0,'f');
	    }
	  else if(type == 'v')
	    {
	      if(*(decode_line[0]+3)=='S')
		{
		  perr("Vector shader doesn't have branch scalar instructions");
		}
	      else
		type_4(6,temp_assembly.argu_num,0,type);
	    }
	  else if(type == 'f')
	    {
	      if(*(decode_line[0]+3)=='V')
		{
		  perr("Fragment shader doesn't have branch vector instructions");
		}
	      else
		type_4(6,temp_assembly.argu_num,0,type);
	    }
	  else
	    {
	      perr("deopcode(): type is wrong.");
	    }
	  break;	  
	case '\0':
	  nowBin=29;
	  nowBin=nowBin<<OPCODE_BIT;
	  type_4(6,temp_assembly.argu_num,0,type);
	  break;
	default:
	  no_opcode();
	  break;
	}
      break;
    case 'b':
      switch(*(decode_line[0]+1))
        {
	case 'e':
	  if(*(decode_line[0]+2)!='q' || (*(decode_line[0]+3)!='\0' && (*(decode_line[0]+3)!='f' || *(decode_line[0]+4)!='\0')))
	  {
	    no_opcode();
	    break;
	  }
	  nowBin=24;
	  nowBin=nowBin<<OPCODE_BIT;
	  type_4(6,temp_assembly.argu_num,0,type);
	  break;
	case 'g':
	  if((*(decode_line[0]+2)!='t' && *(decode_line[0]+2)!='e') || (*(decode_line[0]+3)!='\0' && (*(decode_line[0]+3)!='f' || *(decode_line[0]+4)!='\0')))
	  {
            no_opcode();
	    break;
	  }
	  if(*(decode_line[0]+2)=='t')
	    nowBin=27;
	  else
	    {
	      type_branch_less_equal_or_greater_equal(temp_assembly.argu_num,type);
	      break;
	    }
          nowBin=nowBin<<OPCODE_BIT;
	  if(*(decode_line[0]+3)=='f')
	    type_4(6,temp_assembly.argu_num,1,type);
	  else 
	    type_4(6,temp_assembly.argu_num,0,type);
	  break;
        case 'l':
	  if(*(decode_line[0]+2)=='\0')
	    {
	      callinlinefunction(type,input_object);
	      break;
	    }
          if(*(decode_line[0]+2)=='u' && *(decode_line[0]+3)=='e' &&*(decode_line[0]+4)=='\0')
	    {
	      nowBin=16;
	      nowBin=nowBin<<OPCODE_BIT;
	      type_1(2,temp_assembly.argu_num,0);
	      break;
	    }
          if((*(decode_line[0]+2)!='t' && *(decode_line[0]+2)!='e') || (*(decode_line[0]+3)!='\0' && (*(decode_line[0]+3)!='f' || *(decode_line[0]+4)!='\0')))
	  {
            no_opcode();
	    break;
	  }
	  if(*(decode_line[0]+2)=='t')
	    nowBin=26;
	  else
	    {
	      type_branch_less_equal_or_greater_equal(temp_assembly.argu_num,type);
	      break;
	    }
          nowBin=nowBin<<OPCODE_BIT;
	  if(*(decode_line[0]+3)=='f')
	    type_4(6,temp_assembly.argu_num,1,type);
	  else
	    type_4(6,temp_assembly.argu_num,0,type);
	  break;
	case 'n':
          if(*(decode_line[0]+2)!='e' || (*(decode_line[0]+3)!='\0' && *(decode_line[0]+3)!='f' && *(decode_line[0]+4)!='\0'))
	  {
            no_opcode();
	    break;
	  }
	  nowBin=25;
          nowBin=nowBin<<OPCODE_BIT;
	  type_4(6,temp_assembly.argu_num,0,type);
	  break;
	case '\0':
	  nowBin=29;
	  nowBin=nowBin<<OPCODE_BIT;
	  type_4(6,temp_assembly.argu_num,0,type);
	  break;
	default:
	  no_opcode();
	  break;
        }
      break;

      /*
    case 'b':
      {
	if(*(decode_line[0]+1)!='l' || *(decode_line[0]+2)=='r')
	{
	  no_opcode();
	  break;
	}
	nowBin=28;
	nowBin=nowBin<<OPCODE_BIT;
	type_4(6,argu_num);
	break;
      }
      */
    case 'C':
      {
	if(*(decode_line[0]+1)!='A' || *(decode_line[0]+2)!='L' || (*(decode_line[0]+3)!='\0' && (*(decode_line[0]+3)!='F' || *(decode_line[0]+4)!='\0')))
	{
	  no_opcode();
	  break;
	}
	nowBin=29;
	nowBin=nowBin<<OPCODE_BIT;
	if(*(decode_line[0]+3)=='F')
	  type_4(6,temp_assembly.argu_num,1,type);
	else
	  type_4(6,temp_assembly.argu_num,0,type);
      }
      break;
    case 'D':
      {
	switch(*(decode_line[0]+1))
	  {
	  case 'I':
	    if(*(decode_line[0]+2)!='V' || *(decode_line[0]+3)!='\0')
	    {
	      no_opcode();
	      break;
	    }
	    nowBin=6;
	    nowBin=nowBin<<OPCODE_BIT;
	    type_3(5,temp_assembly.argu_num,0);
	    break;
	  case 'P':
	    if(*(decode_line[0]+2)!='\0' && (*(decode_line[0]+2)!='F' || *(decode_line[0]+3)!='\0'))
	      {
		no_opcode();
		break;
	      }
            nowBin=19;
            nowBin=nowBin<<OPCODE_BIT;
	    if(*(decode_line[0]+2)=='F')
	      type_1(2,temp_assembly.argu_num,1);
	    else
	      type_1(2,temp_assembly.argu_num,0);
            break;
	  case 'S':
	    if(*(decode_line[0]+2)!='Q' || *(decode_line[0]+3)!='\0')
	    {
	      no_opcode();
	      break;
	    }
	    nowBin=1;
	    nowBin=nowBin<<OPCODE_BIT;
	    type_3(5,temp_assembly.argu_num,0);
	    break;
	  default:
	    no_opcode();
	    break;
	  }
      }
      break;
    case 'F':
      if(*(decode_line[0]+1)=='I' && *(decode_line[0]+2)=='N' && *(decode_line[0]+3)=='\0')
	{
	  nowBin=14;
	  nowBin=nowBin<<OPCODE_BIT;
	  if( !strcmp("Start!!",decode_line[2]) && !strcmp("Fragment",decode_line[1]) ) //add this condition for FIN Fragment Start!!
	    nowBin=nowBin|0x40;
	  emit(nowBin);
	  break;
	}
      if(strcmp(decode_line[0],"FNEG")==0){ //kaymon
	//no_opcode();
	//printf("%s\n",decode_line[1]);
	//printf("%s\n",decode_line[2]);
	int reg1,reg2;
	reg1 = decode_line[1][0]-48;
	reg2 = decode_line[2][0]-48;
	inline_fneg(reg1,reg2,NULL);
	break;
      }
      
      if(*(decode_line[0]+1)!='T' || *(decode_line[0]+2)!='O' || *(decode_line[0]+3)!='I' || *(decode_line[0]+4)!='\0')
	{

	  no_opcode();
	  break;
	}

      //comment it temporary.
      //nowBin=28;// Kaymon think it's 13
      nowBin=13;
      nowBin=nowBin<<OPCODE_BIT;
      type_1(9,temp_assembly.argu_num,0);
      
      break;
    case 'f':
      if(strcmp(decode_line[0],"fneg")==0){ //kaymon
	//no_opcode();
	//printf("%s\n",decode_line[1]);
	//printf("%s\n",decode_line[2]);
	int reg1,reg2;
	reg1 = decode_line[1][0]-48;
	reg2 = decode_line[2][0]-48;
	inline_fneg(reg1,reg2,NULL);
	break;
      }
      break;
    case 'G':
      {
	switch (*(decode_line[0]+1))
	  {
	  case 'E':
	    if(*(decode_line[0]+2)!='T' || *(decode_line[0]+3)<'W' || *(decode_line[0]+3)>'Z' || *(decode_line[0]+4) !='\0')
	      {
		no_opcode();
		break;
	      }
	    nowBin=16;
	    nowBin=nowBin<<OPCODE_BIT;
	    type_1(2,temp_assembly.argu_num,0);
	    break;
	  case 'R':
	    if(*(decode_line[0]+2)!='E' || *(decode_line[0]+3)!='E'|| *(decode_line[0]+4)!='N'|| *(decode_line[0]+5)!='\0')
	      {
		no_opcode();
		break;
	      }
	    nowBin=16;
	    nowBin=nowBin<<OPCODE_BIT;
	    type_1(2,temp_assembly.argu_num,0);
	    break;
	  }
	break;
      }

    case 'I':
      switch (*(decode_line[0]+1))
	{
	case 'N':
	  if(*(decode_line[0]+2)!='V' || *(decode_line[0]+3)!='\0')
	    {
	      no_opcode();
	      break;
	    }
	  nowBin=20;
	  nowBin=nowBin<<OPCODE_BIT;
	  type_1(3,temp_assembly.argu_num,0);
	  break;
	case 'T':
	  if(*(decode_line[0]+2)!='O' || *(decode_line[0]+3)!='F' || *(decode_line[0]+4)!='\0' ) 
	    {
	      no_opcode();
	      break;
	    }
	  //nowBin=28; //Kaymon think it's 13
	  nowBin=13; //Kaymon think it's 13
	  nowBin=nowBin<<OPCODE_BIT;
          type_1(9,temp_assembly.argu_num,0);
	  break;
	default:
	  no_opcode();
        break;

	}
      break;
    case 'J':
      switch (*(decode_line[0]+1))
	{
	case 'M':
	  if(*(decode_line[0]+2)!='P' || *(decode_line[0]+3)!='\0')
	    {
	      no_opcode();
	      break;
	    }
	  nowBin=29;
	  nowBin=nowBin<<OPCODE_BIT;
	  type_4(6,temp_assembly.argu_num,0,type);
	  break;
	case 'R':
	  if(*(decode_line[0]+2)!='\0')
	    {
	      no_opcode();
	      break;
	    }
	  nowBin=24;
	  nowBin=nowBin<<OPCODE_BIT;
	  type_4(6,temp_assembly.argu_num,0,type);
	  break;
	default:
	  no_opcode();
	  break;
	}
      break;
    case 'L':
      switch (*(decode_line[0]+1))
	{
	case 'D':
	  if(*(decode_line[0]+2)=='\0' )
	    {
	      no_opcode();

	      //nowBin=8;
	      //nowBin=nowBin<<OPCODE_BIT;
	      //type_2(1,temp_assembly.argu_num,0);

	      break;
	    }
	  else if(*(decode_line[0]+2)=='I')
	    {
	      if(*(decode_line[0]+3)=='\0' || *(decode_line[0]+3)=='F')
		{
		  nowBin=10;
		  nowBin=nowBin<<OPCODE_BIT;
		  if(*(decode_line[0]+3)=='\0')
		    {
		      type_2(1,temp_assembly.argu_num,0);
		      break;
		    }
		  else
		    {
		      if(*(decode_line[0]+4)!='\0')
			{
			  no_opcode();
			  break;
			}
		      else
			{
			  type_2(1,temp_assembly.argu_num,1);
			  break;
			}
		    }
		}
	    }
	  else if(*(decode_line[0]+2)=='V' && *(decode_line[0]+3)=='\0')
	    {
	      //nowBin=13;
	      nowBin=8;
	      nowBin=nowBin<<OPCODE_BIT;
	      type_2(1,temp_assembly.argu_num,0);
	      break;
	    }
	  else
	    no_opcode();
	  break;
	case 'O':
	  if(*(decode_line[0]+2)!='G' || *(decode_line[0]+3)!='2')
	  {
	    no_opcode();
	    break;
	  }
	  nowBin=4;
	  nowBin=nowBin<<OPCODE_BIT;
	  type_3(5,temp_assembly.argu_num,0);
	  break;
	default:
	  no_opcode();
	  break;
	}
      break;
    case 'M':
      switch (*(decode_line[0]+1))
	{
	case 'A':
	  switch(*(decode_line[0]+2))
	    {
	    case 'D':
	      if(*(decode_line[0]+3)!='\0' && *(decode_line[0]+3)!='F')
		{
		  no_opcode();
		  break;
		}	      
	      nowBin=18;
	      nowBin=nowBin<<OPCODE_BIT;
	      if(*(decode_line[0]+3)=='F')
		{
		  if(*(decode_line[0]+4)=='\0')
		    type_1(2,temp_assembly.argu_num,1);
		  else
		    no_opcode();
		  break;
		}
	      type_1(2,temp_assembly.argu_num,0);
	      break;
	    case 'X':
	      if(*(decode_line[0]+3)!='\0' && (*(decode_line[0]+3)!='F' || *(decode_line[0]+4)!='\0'))
		{
		  no_opcode();
		  break;
		}
	      nowBin=12;
	      nowBin=nowBin<<OPCODE_BIT;
	      if(*(decode_line[0]+3)=='F')
		type_1(7,temp_assembly.argu_num,1);
	      else
		type_1(7,temp_assembly.argu_num,0);
	      //printf("%s type not define.\n",decode_line[0]);
	      break;
	    default:
	      no_opcode();
	      break;
	    }
	  break;
	case 'I':
	  if(*(decode_line[0]+2)!='N' || (*(decode_line[0]+3)!='\0' && (*(decode_line[0]+3)!='F' || *(decode_line[0]+4)!='\0')))
	  {
	    no_opcode();
	    break;
	  }
	  nowBin=11;
	  nowBin=nowBin<<OPCODE_BIT;
	  if(*(decode_line[0]+3)=='F')
	    type_1(7,temp_assembly.argu_num,1);
	  else
	    type_1(7,temp_assembly.argu_num,0);
	  break;
	case 'V':
	  if(*(decode_line[0]+2)=='I' && (*(decode_line[0]+3)=='\0' || (*(decode_line[0]+3)=='F' && *(decode_line[0]+4)=='\0')))
	    {
	      nowBin=10;
	      nowBin=nowBin<<OPCODE_BIT;
	      if(*(decode_line[0]+3)=='F')
		type_2(1,temp_assembly.argu_num,1);
	      else
		type_2(1,temp_assembly.argu_num,0);
	      break;
	    }
	  else if(*(decode_line[0]+2)=='R' && (*(decode_line[0]+3)=='\0' || *(decode_line[0]+3)=='F'))
	    {
	      nowBin=7;
	      nowBin=nowBin<<OPCODE_BIT;
	      if(*(decode_line[0]+3)=='\0')
		{
		  type_3(1,temp_assembly.argu_num,0);
		  break;
		}
	      else
		{
		  if(*(decode_line[0]+4)!='\0')
		    {
		      no_opcode();
		      break;
		    }
		  else
		    {
		      type_3(1,temp_assembly.argu_num,1);
		      break;
		    }
		}
	    }
	  else
	    no_opcode();
	  break;
	case 'U':
	  if(*(decode_line[0]+2)!='L' || (*(decode_line[0]+3)!='X' && *(decode_line[0]+3)!='H' && *(decode_line[0]+3)!='F') || *(decode_line[0]+4)!='\0')
	  {
	    no_opcode();
	    break;
	  }
	  if(*(decode_line[0]+3)=='X')
	    nowBin=28;
	  else
	    nowBin=17;
	  nowBin=nowBin<<OPCODE_BIT;
	  if(*(decode_line[0]+3)=='F')
	    type_1(2,temp_assembly.argu_num,1);
	  else
	    type_1(2,temp_assembly.argu_num,0);
	  break;
	default:
	  no_opcode();
	  break;
   	}
      break;
    case 'm':
      if(*(decode_line[0]+1)!='r' || *(decode_line[0]+2)!='\0' )
	{
	  no_opcode();
	  break;
	}
      nowBin=7;
      nowBin=nowBin<<OPCODE_BIT;
      type_3(1,temp_assembly.argu_num,0);

      break;
    case 'N':
      if(*(decode_line[0]+1)!='O' || *(decode_line[0]+2)!='P' || *(decode_line[0]+3)!='\0')
      {
	no_opcode();
	break;
      }
      nowBin=31;
      nowBin=nowBin<<OPCODE_BIT;
      type_nop();
      break;
    case 'O':
      if(*(decode_line[0]+1)=='R' || *(decode_line[0]+2)!='\0')
	{
	  nowBin=22;
	  nowBin=nowBin<<OPCODE_BIT;
	  type_1(3,temp_assembly.argu_num,0);
	  break;
	}
      else
	no_opcode();
      break;
    case 'P':
      if(*(decode_line[0]+1)!='O' || *(decode_line[0]+2)!='W' || *(decode_line[0]+3)!='2' || *(decode_line[0]+4)!='\0')
      {
	no_opcode();
	break;
      }
      nowBin=5;
      nowBin=nowBin<<OPCODE_BIT;
      type_3(5,temp_assembly.argu_num,0);
      break;
    case 'R':
      switch (*(decode_line[0]+1))
	{
	case 'C':
	  if(*(decode_line[0]+2)!='P' || *(decode_line[0]+3)!='\0')
	  {
	    no_opcode();
	    break;
	  }
	  nowBin=2;
	  nowBin=nowBin<<OPCODE_BIT;
	  type_3(5,temp_assembly.argu_num,0);
	  break;
	case 'E':
	  if(*(decode_line[0]+2)=='T' && *(decode_line[0]+3)=='\0')
	  {
	    nowBin=29;
	    nowBin=nowBin<<OPCODE_BIT;
	    type_4(6,temp_assembly.argu_num,0,type);
	    
	    break;
	  }
	  else if (*(decode_line[0]+2)=='D' && *(decode_line[0]+3)=='\0')
	    {
	      nowBin=16;
	      nowBin=nowBin<<OPCODE_BIT;
	      type_1(2,temp_assembly.argu_num,0);
	      break;
	    }
	  else
	    {
	      no_opcode();
	      break;
	    }
          break;
	case 'S':
	  if(*(decode_line[0]+2)!='Q' || *(decode_line[0]+3)!='\0')
	  {
            no_opcode();
	    break;
	  }
          nowBin=3;
          nowBin=nowBin<<OPCODE_BIT;
          type_3(5,temp_assembly.argu_num,0);
          break;
	default:
	  no_opcode();
	  break;
	}
      break;
    case 'S':
      switch (*(decode_line[0]+1))
	{
	case 'T':
	  //no STF
	  //if(*(decode_line[0]+2)=='\0' || *(decode_line[0]+2)=='F')
	  if(*(decode_line[0]+2)=='\0')
	    {
	      no_opcode();
	      
	      
	      //nowBin=9;
	      //nowBin=nowBin<<OPCODE_BIT;
	      //if (*(decode_line[0]+2)=='\0')
	      //{
	      //type_2(1,temp_assembly.argu_num,0);
	      //break;
	      //}

	      break;
	      //else
	      //{
	      //if(*(decode_line[0]+3)!='\0')
	      //{
	      //no_opcode();
	      //break;
	      //}
	      //else
	      //{
	      //type_2(1,temp_assembly.argu_num,1);
	      //break;
	      //}
	      //}
	    }
	  if(*(decode_line[0]+2)=='V' && *(decode_line[0]+3)=='\0')
	    {
	      //nowBin=14;
	      nowBin=9;
	      nowBin=nowBin<<OPCODE_BIT;
	      type_2(1,temp_assembly.argu_num,0);
	      break;
	    }
	  no_opcode();
	  break;
	case 'H':
	  //no_opcode();
	  
	  if(*(decode_line[0]+2)!='R' && *(decode_line[0]+2)!='L')
	  {
	    no_opcode();
	    break;
	  }
	  nowBin=30;
	  nowBin=nowBin<<OPCODE_BIT;
	  type_1(4,temp_assembly.argu_num,0);
	  break;

	case 'U':
	  if(*(decode_line[0]+2)!='B' || (*(decode_line[0]+3)!='\0' && (*(decode_line[0]+3)!='F' || *(decode_line[0]+4)!='\0')))
	    {
	      no_opcode();
	      break;
	    }
	  nowBin=16;
	  nowBin=nowBin<<OPCODE_BIT;
	  if(*(decode_line[0]+3)=='F')
	    type_1(2,temp_assembly.argu_num,1);
	  else
	    type_1(2,temp_assembly.argu_num,0);
	  break;

	default:
	  no_opcode();
	  break;
	}
      break;
    case 'T':
      //TEX need to be added
      if(*(decode_line[0]+1)=='E' || *(decode_line[0]+2)=='X')
      {
	
	nowBin=15;
	nowBin=nowBin<<OPCODE_BIT;
	type_5(8,temp_assembly.argu_num);
	break;
      }
      else
	  no_opcode();
      break;
      
    case 'v':
      if(strcmp(decode_line[0],"vmrghw")==0)
	{
	  type_altivec(0,temp_assembly.argu_num);
	}
      if(strcmp(decode_line[0],"vsldoi")==0)
	{
	  type_altivec(0,temp_assembly.argu_num);
	}
      break;
    case 'X':
      if(*(decode_line[0]+1)!='O' || *(decode_line[0]+2)!='R' || *(decode_line[0]+3)!='\0')
      {
	no_opcode();
	break;
      }
      nowBin=23;
      nowBin=nowBin<<OPCODE_BIT;
      type_1(3,temp_assembly.argu_num,0);
      break;
      //special case: we just skip it
    case '#':
      break;
    default:
      printf("%s\n",decode_line[0]);
      no_opcode();
      break;
    }
  
  //printf("%s\targu_num: %d\n",decode_line[0],temp_assembly.argu_num);

  return 0;
}

void fill_binary(int size,int value)
{
  int temp_size;
  if(value==0)
    {
      temp_size=size/8;
      
    }
}

void delete_table_line(int delete_line)
{
  int i=delete_line;
  
  for(;i<table_count;i++)
    {
      int j;
      for(j=0;j<6;j++)
	strcpy(table[i][j],table[i+1][j]);
    }

  bzero(table[table_count],sizeof(&table[table_count]));
}


void check_table(char input_type)
{
  int i=0;

  //remove table line that is uniform, varying, and varying

  for(i=table_count-1;i>=0;i--)
    {
      int j;
      int count_uniform_used;
      int count_varying_used=used_varying();
      int count_attribute_used=used_attribute();

      if(input_type=='v')
	count_uniform_used=used_uniform(0);
      else if(input_type=='f')
	count_uniform_used=used_uniform(1);
      else
	{
	  perr("make_table: input_type");
	}

      for(j=0;j<count_uniform_used;j++)
	{
	  if(u_form->index[j]!=-1)
	    {
	      if(!strcmp((char *)&table[i][0],u_form->uniform_name[j]))
		{
		  //printf("name: %s\tline: %d\n",u_form->uniform_name[j],i);
		  delete_table_line(i);
		  break;
		}
	    }
	}

      for(j=0;j<count_varying_used;j++)
	{
#ifdef assembler_standalone
	  if(v_form.index[j]!=-1)
	    {
	      if(!strcmp((char *)&table[i][0],v_form.vary_name[j]))
		{
		  //printf("name: %s\tline: %d\n",v_form.vary_name[j],i);
		  delete_table_line(i);
		  break;
		}
	    }
#else
	  if(v_form->index[j]!=-1)
	    {
	      if(!strcmp((char *)&table[i][0],v_form->vary_name[j]))
		{
		  //printf("name: %s\tline: %d\n",v_form->vary_name[j],i);
		  delete_table_line(i);
		  break;
		}
	    }
#endif
	}
      if(input_type=='v')
	{
	  for(j=0;j<count_attribute_used;j++)
	    {
	      if(a_form->index[j]!=-1)
		{
		  if(!strcmp((char *)&table[i][0],a_form->att_name[j]))
		    {
		      //printf("name: %s\tline: %d\n",a_form->att_name[j],i);
		      delete_table_line(i);
		      break;
		    }
		}	      
	    }
	}
    }
}

void move_table_variable_to_noninitialize_global_variable_table()
{
  int i=0;
  //print_table();
  for(i=0;i<table_count;i++)
    {
      if(!strcmp(table[i][4],"var") && table[i][0][0]!='.')
	{
	  struct variable_form *p_cur_r_form=p_first_r_form2;

	  if(variable_count>=256)
	    {
	      perr("variable over 256.");	  
	    }

	  while(1)
	    {
	      if(p_cur_r_form->next==NULL)	  
		{
		  sprintf(table[i][2],"%d",variable_count);
		  strcpy(p_cur_r_form->name,table[i][0]);
		  p_cur_r_form->address=variable_count;		  
		  p_cur_r_form->next=&r_form2[++variable_count];
		  p_cur_r_form=p_cur_r_form->next; 
		  delete_table_line(i);
		  i--;
		  break;
		}
	      else
		{
		  if(!strcmp(table[i][0],p_cur_r_form->name))
		    {
		      perr("Table's variable and r table's variable are the same.");
		    }
		  p_cur_r_form=p_cur_r_form->next; 
		}
	    }
	}
    }
}

void make_table(char *filename,char input_type,struct programobject *input_object)
{
  FILE *read_file;
  char *nEOF=t_read;
  int i,j;
  int data_address,first_address=0,have_colon;
  int lock_address=0,mtspr_count=0,inline_mark=0;
  int endofmain = 0;
  int first_instruction=1;
#ifdef getSamplerIndex
  int num_of_sampler = GetSamplerIndex(input_type);//Kaymon add it for recording sampler_id.
#endif
  if(!(read_file=fopen(filename,"r")))
    {
      char err_msg[64];
      sprintf(err_msg,"open file: %s",filename);
      perr(err_msg);
    }

  bzero(table,sizeof(table));
  table_count=0;

  //fixed by sheng-chih
  //if(input_type=='f')
  //address=8;
  //else
  //end fixed
    address=0;

  while(nEOF)
    {
      nEOF=fgets(nowRead,200,read_file);
      strcpy(t_read,nowRead);
      have_colon=0;
      if(!nEOF)
	{
	  //printf("End make table\n");
	  break;
	}
      temp_assembly_init();

      decode();
      //if(decode_line[0][0]!='.')
      //printf("1. PC: %d\tdecode_line:[0]:%s [1]:%s [2]:%s [0][0]:%d\n",address,decode_line[0],decode_line[1],decode_line[2],decode_line[0][0]);

      for(i=0;i<50;i++)
	{
	  if(decode_line[0][i]=='\0')
	    break;
	  if(decode_line[0][i]==':')
	    {
	      //check variable
	      //variable
	      if(strcmp(decode_line[1],"#")==0 && strcmp(decode_line[2],"constant")==0 && strcmp(decode_line[3],"pool")==0)// && strcmp(decode_line[4],"float")==0)
		{
		  struct variable_form *p_cur_r_form=p_first_r_form;
		  
		  if(variable_count>256)
		    {
		      perr("variable over 256.");
		    }

		  while(1)
		    {
		      //printf("var %d %s  %p\n",variable_count,p_cur_r_form->name,p_cur_r_form->next);
		      if(p_cur_r_form->next==NULL)
			{
			  char variable_name[36];
			  float f_value;
			  long long ll_value;
			  
			  strcpy(variable_name,decode_line[0]);
			  variable_name[i]='\0';
			  strcpy(p_cur_r_form->name,variable_name);
			  p_cur_r_form->address=variable_count;
			  if(strcmp(decode_line[4],"float")==0)
			    {
			      nEOF=fgets(nowRead,200,read_file);
			      strcpy(t_read,nowRead);
			      decode();
			      ll_value=atoll(decode_line[1]);
			      memcpy(&f_value,&ll_value,4);
			      p_cur_r_form->value=f_value;
			      p_cur_r_form->vector_size=1;
			    }
			  if(strcmp(decode_line[4],"<4")==0 && strcmp(decode_line[5],"x")==0 && strcmp(decode_line[6],"float>")==0)
			    {
			      nEOF=fgets(nowRead,200,read_file);
			      strcpy(t_read,nowRead);
			      decode();
			      ll_value=atoi(decode_line[1]);
			      memcpy(&f_value,&ll_value,4);
			      p_cur_r_form->value=f_value;

			      nEOF=fgets(nowRead,200,read_file);
			      strcpy(t_read,nowRead);
			      decode();
			      ll_value=atoi(decode_line[1]);
			      memcpy(&f_value,&ll_value,4);
			      p_cur_r_form->value1=f_value;

			      nEOF=fgets(nowRead,200,read_file);
			      strcpy(t_read,nowRead);
			      decode();
			      ll_value=atoi(decode_line[1]);
			      memcpy(&f_value,&ll_value,4);
			      p_cur_r_form->value2=f_value;

			      nEOF=fgets(nowRead,200,read_file);
			      strcpy(t_read,nowRead);
			      decode();
			      ll_value=atoi(decode_line[1]);
			      memcpy(&f_value,&ll_value,4);
			      p_cur_r_form->value3=f_value;

			      p_cur_r_form->vector_size=4;
			    }


			  p_cur_r_form->next=&r_form[++variable_count];
			  p_cur_r_form=p_cur_r_form->next;

			  //General ".long" will add address 4

			  if(lock_address==1)
			    address+=4;
			  break;
			}
		      else{
			  p_cur_r_form=p_cur_r_form->next;
		      }
		    }
		  
		}
	      //Label, not variable
	      else
		{
		  decode_line[0][i]='\0';
		  i--;
		  
		  //printf("cmp: %s\t%s\n",table[table_count-1][0],decode_line[0]);
		  //printf("address:%d \n",address/8);
		  if(strcmp(table[table_count-1][0],decode_line[0])!=0)
		    {
		      sprintf(table[table_count][0],"%s",decode_line[0]);
		      sprintf(table[table_count][1],"0");

		      if(lock_address==1)
			address=AlignAddress(address,CurrentAlignmentNumber);

		      sprintf(table[table_count][2],"%d",address);
		      sprintf(table[table_count][3],"%d",CurrentAlignmentNumber);
		      sprintf(table[table_count][4],"%s","func");
		      sprintf(table[table_count][5],"%s",now_file);
		      //printf("decode_line[0]: %s\tdecode_line[1]: %s\tdecode_line[2]: %s\n",decode_line[0],decode_line[1],decode_line[2]);
		      table_count++;
		    }
		  else
		    { //for .type XXXX,@function
		      if(lock_address==1)
			address=AlignAddress(address,CurrentAlignmentNumber);
		      sprintf(table[table_count-1][2],"%d",address);
		      sprintf(table[table_count-1][3],"%d",CurrentAlignmentNumber);
		      sprintf(table[table_count-1][4],"%s","func");
		    }
		}
	      have_colon=1;
	      continue;
	    }
	}
      if(have_colon==1)
	continue;
      if(strcmp(decode_line[0],"")==0){
	continue;
      }
      if(strcmp(decode_line[0],".align")==0)
	{
	  CurrentAlignmentNumber=atoi(decode_line[1]);
	  //printf("CurrentAlignmentNumber: %d\n",CurrentAlignmentNumber);
	  /*
	  if(strcmp(table[table_count-1][4],"func")==0)
	    {
	      address=AlignAddress(address,CurrentAlignmentNumber);
	      sprintf(table[table_count-1][2],"%d",address);
	    }
	  */
	  continue;
	}

      if(strcmp(decode_line[0],".comm")==0)
	{
	  if(first_address==0)
	    {
	      data_address=address;
	      first_address=1;
	    }
	  //printf("address: %d\tCurrentAlignmentNumber: %d\n",address,CurrentAlignmentNumber);	  
	  sprintf(table[table_count][0],"%s",decode_line[1]);
	  sprintf(table[table_count][1],"%s",decode_line[2]);

	  if(lock_address==1)
	    address=AlignAddress(address,CurrentAlignmentNumber);

	  sprintf(table[table_count][2],"%d",address);
	  sprintf(table[table_count][3],"%d",CurrentAlignmentNumber);
	  sprintf(table[table_count][4],"%s","var");
	  sprintf(table[table_count][5],"%s",now_file);	      
	  //printf("%s\n",variable_table[v_table_count][2]);
	  //printf("%s\t%s\t%s\n",decode_line[1],decode_line[2],decode_line[3]);
	  //printf("decode_line[1]: %s\n",decode_line[1]);
	  table_count++;

	  if(lock_address==1)
	    address+=atoi(decode_line[2]);
	  /*
	  if((atoi(decode_line[2])%8)!=0)
	    address+=((atoi(decode_line[2])/8)+1);
	  else
	    address+=(atoi(decode_line[2])/8);
	  */

	  continue;
	}
      if(strcmp(decode_line[0],".section")==0)
	continue;
      if(strcmp(decode_line[0],".data")==0)
        continue;
      if(strcmp(decode_line[0],".file")==0)
	{
	  char *pTok;
	  char temp_file[30];
	  strcpy(temp_file,decode_line[1]);
	  pTok=strtok(temp_file,"\"");
	  strcpy(now_file,pTok);
	  continue;
	}
      if(strcmp(decode_line[0],".text")==0)
	continue;
      if(strcmp(decode_line[0],".global")==0)
        continue;
      if(strcmp(decode_line[0],".globl")==0)
	continue;
      if(strcmp(decode_line[0],".type")==0)
	{
	  
	  sprintf(table[table_count][0],"%s",decode_line[1]);
	  sprintf(table[table_count][1],"%d",0);

	  if(lock_address==1)
	    address=AlignAddress(address,CurrentAlignmentNumber);

	  sprintf(table[table_count][2],"%d",address);
	  sprintf(table[table_count][3],"%d",CurrentAlignmentNumber);
	  if(strcmp(decode_line[2],"@function")==0)
	    sprintf(table[table_count][4],"%s","func");
	  if(strcmp(decode_line[2],"@object")==0)
	    sprintf(table[table_count][4],"%s","var");
	  sprintf(table[table_count][5],"%s",now_file);
	  table_count++;
	  continue;
	}
      if(strcmp(decode_line[0],".size")==0)
        continue;
      if(strcmp(decode_line[0],".ascii")==0)
	{
	  char new_Tok[100];
          char *pTok;
          strcpy(new_Tok,nowRead);
          pTok=strtok(new_Tok,"\"");
          pTok=strtok(NULL,"\"");

	  for(i=0;pTok[i]!='\0';i++);
	  //printf("%s\t%d\n",pTok,i);
	  sprintf(table[table_count-1][1],"%d",atoi(table[table_count-1][1])+i);
          sprintf(table[table_count-1][4],"%s","var");

	  if(lock_address==1)
	    address+=i;

	  continue;
	}
      if(strcmp(decode_line[0],".asciz")==0)
	{
	  char new_Tok[100];
	  char *pTok;
	  strcpy(new_Tok,nowRead);
	  pTok=strtok(new_Tok,"\"");
	  pTok=strtok(NULL,"\"");

	  j=0;
	  
	  for(i=0;pTok[i]!='\0';i++)
          {
	    if(pTok[i]=='\\' && pTok[i+1]=='0' && pTok[i+2]=='0' && pTok[i+3]=='0')
	      i+=3;
	    j++;
	  }
	  j++;
	  //printf("%s\t%d\n",pTok,j);
          sprintf(table[table_count-1][1],"%d",atoi(table[table_count-1][1])+j);
          sprintf(table[table_count-1][4],"%s","var");

	  if(lock_address==1)
	    address+=j;
	  continue;
	}
      if(strcmp(decode_line[0],".space")==0)
	{
	  //printf("%d\t%s\n",address,decode_line[1]);
	  //printf("%d\t%s\n",address,table[table_count-1][1]);
	  //printf("%d\t%s\n",address,table[table_count-1][0]);
	  //printf("%d\t%s\n",address,table[table_count-2][1]);
	  //printf("%d\t%s\n",address,table[table_count-2][0]);
	  sprintf(table[table_count-1][1],"%d",atoi(table[table_count-1][1])+atoi(decode_line[1]));
          sprintf(table[table_count-1][4],"%s","var");

	  if(lock_address==1)
	    address+=atoi(decode_line[1]);

	  continue;
	}
      if(strcmp(decode_line[0],".long")==0)
	{
	  sprintf(table[table_count-1][1],"%d",atoi(table[table_count-1][1])+4);
          sprintf(table[table_count-1][4],"%s","var");

	  //printf("decode: %s\n",decode_line[1]);
	  //printf("test!!\n");

	  if(lock_address==1)
	    address+=4;

	  continue;
	}
      if(strcmp(decode_line[0],".short")==0)
	{
          sprintf(table[table_count-1][1],"%d",atoi(table[table_count-1][1])+2);
          sprintf(table[table_count-1][4],"%s","var");

	  if(lock_address==1)
	    address+=2;

	  continue;
	}
      if(strcmp(decode_line[0],".byte")==0)
	{
          sprintf(table[table_count-1][1],"%d",atoi(table[table_count-1][1])+1);
          sprintf(table[table_count-1][4],"%s","var");

	  if(lock_address==1)
	    address+=1;

	  continue;
	}
      char var_tok[5][128];
      //printf("decode_line[0]:%s\n",decode_line[0]);
      //printf("\t%s\n",decode_line[1]);
#ifdef Opt_getSamplerIndex
      memset(var_tok,0,640);
#endif
      int count_var_tok=0;
      if(strcmp(decode_line[0],".uleb128")==0)
	continue;
      if(strcmp(decode_line[0],".sleb128")==0)
	continue;
      if(strncmp(decode_line[0],".Lset",5)==0)
	continue;
      if(strcmp(decode_line[0],".weak")==0)
	{

	  char temp_str[256];
	  char *p_tok1;
	  strcpy(temp_str,decode_line[1]);
	  p_tok1=strtok(temp_str,".");
	  while(p_tok1!=NULL)
	    {
	      strcpy(var_tok[count_var_tok],p_tok1);
	      count_var_tok++;
	      p_tok1=strtok(NULL,".");
	    }
	  if(strcmp(var_tok[1],"u")==0)
	    {
	      for(i=0;i<max_number_of_uniform_variables;i++)
		{
		  if(u_form->index[i]!=-1)
		    {
		      if(strcmp(u_form->uniform_name[i],var_tok[0])==0)
			break;
		      continue;
		    }
		  else
		    {
		      int temp_size,j;
		      strcpy(u_form->uniform_name[i],var_tok[0]);
		      //printf("Uniform name:%s\n",var_tok[0]);
		      temp_size=atoi(var_tok[3]);
		      //printf("\n\n%d\n\n",u_form->size[i]);
		      //printf("\n\n%d\n\n",temp_size);


#ifdef SET_UNIFORM_MATRIX_UNIT
		      temp_size/=4;
#endif

#ifdef getSamplerIndex
		      int loop_index;  //******Record sampler_index_of_uniform******** Kaymon
		      for(loop_index=0;loop_index<num_of_sampler;loop_index++){
			//printf("1.%s\n",var_tok);
			//printf("2.%s\n",sampler_id[loop_index]);
			if(!strcmp(var_tok[0],sampler_id[loop_index])){
			  u_form->IsSampler2D[i]=1;
			  //printf("+++++++++match string: %s\n",var_tok[0]);
			}
		      }  //**************end**************
#endif
#ifdef Opt_getSamplerIndex
		      //printf("%s\n",var_tok[4]);

		      if(var_tok[4][0]=='s')
			u_form->IsSampler2D[i]=1;
		      else if(var_tok[4][0]=='c')
			u_form->IsSamplerCube[i]=1;
		      else
			u_form->IsNormal[i]=1;
#endif
		      if(temp_size==0)
			temp_size=1;
		      for(j=1;j<temp_size;j++)
			  u_form->index[i+j]=-2;
		      u_form->index[i]=i;
		      u_form->size[i]=temp_size;
		      //printf("u_form->index[%d]      %d\n", i, u_form->index[i]);
		      //printf("\n\n%d\n\n",u_form->size[i]);
		      //printf("u_form.uniform_name[%d]: %s\n",i,u_form->uniform_name[i]);
		      break;
		    }
		  perr("uniform variables are more than 128");
		}
	    }
	  else if(strcmp(var_tok[1],"a")==0)
	    {
	      int b_search=0;
	      for(i=0;i<8;i++)
		{
		  if(a_form->index[i]!=-1)
		    {
		      if(strcmp(a_form->att_name[i],var_tok[0])==0)
			{
			  b_search=1;
			  break;
			}
		      continue;
		    }
		  else
		    {
		      continue;
		    }
		}
	      //if not searched, add it
	      if(!b_search && i>=8)
		{
		  int b_add=0;
		  for(i=0;i<8;i++)
		    {
		      if(a_form->index[i]==-1)
			{
			  strcpy(a_form->att_name[i],var_tok[0]);
			  a_form->index[i]=i;
			  char *new_name=(char*)malloc(sizeof(strlen(var_tok[0])+1));
			  strcpy(new_name,var_tok[0]);
			  input_object->attribute_name[i]=new_name;
			  //printf("a_form.att_name[%d]: %s\n",i,a_form->att_name[i]);
			  b_add=1;
			  break;
			}
		      else
			{
			  continue;
			}
		    }
		  if(!b_add && i>=8)
		    perr("attribute variables are more than 8");
		}
	    }
	  else if(strcmp(var_tok[0],"v")==0)
	    {

	    }
	  else
	    continue;

          continue;
	}

      if(lock_address==0)
	{
	  /* 0716 comment
	  if(strncmp(&nowRead[1],"mtspr",5)==0 && mtspr_count==0)
	    {
	      if(endofmain) //Kaymon add this flag to control how to decode with user defining function.
		endofmain=0;
	      else{
		lock_address=1;
		mtspr_count++;	
	      }
	      }*/
	  
	  if(decode_line[0][0]>='A' && decode_line[0][0]<='Z'){
	    lock_address=1;
#ifndef assembler_standalone	    
	    if(input_type=='f' && first_instruction)
	      {
		first_instruction = 0;
		address+=8;
	      }
	    if(strcmp(decode_line[0],"BL")==0)
	    {
	      //PC=address; //Kaymon. For the user defining function.
	      callinlinefunction(input_type,input_object);
	      //printf("%s\ttable: LastBinLine: %d %s\n",nowRead,LastBinLine,decode_line[1]);
	      if(LastBinLine!=1)
		{
		  address+=(LastBinLine-1)*8;
		}
	      LastBinLine=0;
	      inline_mark=0;
	    }
#endif
	  }
	  //continue;
	}
      else
	{
	  //deopcode();
	  //record the number of inline function instructions

	  //printf("address: %d\tdecode_line[0]: %s\n",address,decode_line[0]);
	  if(strncmp(decode_line[0],"LDV",3) == 0 || strncmp(decode_line[0],"STV",3) == 0 )
	    {
	      //printf("++++++++:%s %d\n",decode_line[0],temp_assembly.argu_num);
	      //printf("address: %d\tdecode_line[0]: %s\n",address,decode_line[0]);
	      type_2(1,temp_assembly.argu_num,0);
	      inline_mark = 1;
	    }
	  if(strcmp(decode_line[0],"BL")==0)
	    {
	      //PC=address; //Kaymon. For the user-defined function.
	      if(!strncmp(decode_line[1],"OpenGLES.",9))
		callinlinefunction(input_type,input_object);
	      else
		LastBinLine+=3; //User-defined function always cost three instructions, so skip it.
	      //printf("%s\ttable: LastBinLine: %d %s\n",nowRead,LastBinLine,decode_line[1]);
	      inline_mark=1;
	    }
	  if(strcmp(decode_line[0],"vmrghw")==0)
	    {
	      type_altivec(0,temp_assembly.argu_num);
	      //printf("table: LastBinLine: %d\n",LastBinLine);
	      inline_mark=1;
	    }
	  if(strcmp(decode_line[0],"vsldoi")==0)
	    {
	      type_altivec(0,temp_assembly.argu_num);
	      //printf("table: LastBinLine: %d\n",LastBinLine);
	      inline_mark=1;
	    }
	  if(strcmp(decode_line[0],"SUBF")==0)
	    {
	      nowBin=16;
	      nowBin=nowBin<<OPCODE_BIT;
	      if(*(decode_line[0]+3)=='F')
		type_1(2,temp_assembly.argu_num,1);
	      else
		type_1(2,temp_assembly.argu_num,0);
	      //printf("table: SUBF: LastBinLine: %d\n",LastBinLine);
	      inline_mark=1;
	    }
	  if(strcmp(decode_line[0],"BLE")==0 || strcmp(decode_line[0],"gpu_ble")==0 || strcmp(decode_line[0],"BLEF")==0 || strcmp(decode_line[0],"gpu_blef")==0 ||
	     strcmp(decode_line[0],"BGE")==0 || strcmp(decode_line[0],"gpu_bge")==0 || strcmp(decode_line[0],"BGEF")==0 || strcmp(decode_line[0],"gpu_bgef")==0)
	    {
	      if(strncmp(decode_line[0],"gpu_",4)==0)
		{
		  char temp_string[512];
		  strcpy(temp_string,&decode_line[0][4]);
		  sprintf(decode_line[0],"%s",temp_string);

		  for(i=0;i<512;i++)
		    {
		      if(decode_line[0][i]!='\0')
			decode_line[0][i]-=32;
		      else
			break;
		    }
		}
	      type_branch_less_equal_or_greater_equal(temp_assembly.argu_num,input_type);
	      inline_mark=1;

	    }
	  /*
#ifndef assembler_standalone	    
	  if(input_type=='f' && first_instruction)
	    {
	      first_instruction = 0;
#ifdef SET_NOP_NINE
	      address+=8;
	      address+=8;
#else
	      address+=8;
#endif		
	      }
#endif
	  */
	  if(inline_mark==1)
	    {
	      if(LastBinLine!=1)
		{
		  address+=(LastBinLine-1)*8;
		}
	      LastBinLine=0;
	      inline_mark=0;
	    }
	  //decode();
	  
	  //if(strncmp(&nowRead[1],"BL OpenGLES.END.2",17)==0 || strncmp(&nowRead[1],"BL OpenGLES.ENDFrag.1",21)==0)

	  //if(strncmp(&nowRead[1],"BL OpenGLES.ENDFrag.1",21)==0)
	  //{
	  //lock_address=0;
	  //}
	  //else if (strncmp(&nowRead[1],"mtspr",5)==0 && mtspr_count!=0)
	  //if (strncmp(&nowRead[1],"mtspr",5)==0 && mtspr_count!=0)

	  /*
	  if(strncmp(&nowRead[1],"BL OpenGLES.END.3",17)==0 || strncmp(&nowRead[1],"BL OpenGLES.ENDFrag.1",21)==0)
	    {
	      lock_address = 0;
	    }
	  else if (strncmp(&nowRead[1],"mtspr",5)==0 && mtspr_count!=0)
	  */
	  /* 0716 comment
	  if(strncmp(&nowRead[1],"BL OpenGLES.END.3",17)==0 || strncmp(&nowRead[1],"BL OpenGLES.ENDFrag.1",21)==0)
	    {
	      endofmain = 1;  //Kaymon add this flag to control how to decode with user defining function.
	      lock_address = 0;
	      mtspr_count = 0; //Kaymon
	    }
	  else if (strncmp(&nowRead[1],"mtspr",5)==0 && mtspr_count!=0)
	    {
	      lock_address=0;
	      mtspr_count=0;
	    }
	  */
	  if(strncmp(&nowRead[1],"BL OpenGLES.END.3",17)==0||strncmp(&nowRead[1],"BL OpenGLES.ENDFrag.1",21)==0||strncmp(&nowRead[1],"BL OpenGLES.BackToMainFunction",30)==0)
	    lock_address = 0;
	}
      
      //printf("address: %d %d\n",address,lock_address);
      address=AlignAddress(address,3);
      //printf("see decode_line[0]: %s\n",decode_line[0]);
      if(strncmp(decode_line[0],"#",1)==0||strncmp(decode_line[0],"\.L",2)==0)//skip string beginning with .L .e.g. .Lgth.eh = 0 .
	continue;
      address+=8;
      //printf("after adding ddress: %d\n",address);
      
    }

  //check table part
  //If the variable records in table and other variable group, remove it
  //print_table();
  check_table(input_type);
  //print_table();
  sprintf(table[table_count][0]," ");
  printf("final address:%d\n",address);

  fclose(read_file);  
}
#ifdef getSamplerIndex
int GetSamplerIndex( char input_type ){

  FILE* fp;
  int VorF; //V: 0 ,F: 1
  if(input_type == 'v'){
    fp = fopen("Compiler_and_Linker-patch/StandAlone/sampler_id_of_uniform_v","r");
  }
  else{
    fp = fopen("Compiler_and_Linker-patch/StandAlone/sampler_id_of_uniform_f","r");
  }

  char buffer[1024];
  int index=0;  
  if( fp==NULL ){
    ;
    }
  else{
    while( fgets(buffer,1024,fp) != NULL ){
      strncpy(sampler_id[index++],buffer,strlen(buffer)-1);
      //printf("=================------------------------------%s\n",buffer);
    }
    /*
    int i = 0;
    for( ;i<128;i++){
      if( sampler_id[i][0] != 0 )
	printf("%s\n",sampler_id[i]);
      else{
	printf("num:%d\n",i);
	break;
      }
    }
    */
  }
  return index;
  
}
#endif
void fill_regular_variable_start_address(char type)
{
  struct variable_form *p_cur_r_form=p_first_r_form;

#ifdef SET_UNIFORM_MATRIX_UNIT
  int record_used_address=uniform_number*4;
#else
  int record_used_address=uniform_number;
#endif

#ifdef FS_DMEM_SET

#ifdef VS_DMEM_SET
 // if(type == 'f')
 #else
  if(type == 'f')
 #endif
    record_used_address = FS_DMEM_START; //reserve a memory space to save the built-in variable from the beginning address. (now enable gl_FragCoord)
#endif

  regular_variable_address=record_used_address;
  //printf("regular variable address: %d\n",regular_variable_address);
  while(1)
    {
		  //set the start_address  need fixing
		  //p_cur_r_form->address+=VARIABLE_START_ADDRESS;
		  if(p_cur_r_form->next!=NULL)
		{
		  p_cur_r_form->address+=record_used_address;
		  regular_variable_address=(p_cur_r_form->address)+1;
		}
		  else
		break;
		  p_cur_r_form=p_cur_r_form->next;
    }

  p_cur_r_form=p_first_r_form2;

  while(1)
    {
      if(p_cur_r_form->next!=NULL)
	{
	  p_cur_r_form->address+=record_used_address;
	  regular_variable_address=(p_cur_r_form->address)+1;
	}
      else
	break;
      p_cur_r_form=p_cur_r_form->next;
    }


  printf("regular variable address: %d\n",regular_variable_address);
}

void print_table()
{
  int i;
  printf("==================Table=================\n");
  printf("                         name\t  size\t address\talign\tfunc/var\t               file\n");
  for(i=0;i<300;i++)
    {
      if(strcmp(table[i][0],"\0")!=0)
	{
	  printf("%30s\t%6s\t%8s\t%5s\t%8s\t%20s\n",table[i][0],table[i][1],table[i][2],table[i][3],table[i][4],table[i][5]);
	}
      else
	break;
    }
  
  printf("==================END Table=================\n");

}
void print_uniform()
{
  int i;
  printf("==================Uniform=================\n");
  printf("Index\t                name\t size\tstore_regi\n");
  for (i=0;i<max_number_of_uniform_variables;i++)
    {
      if((u_form->index[i]!=-1)&&(u_form->index[i]!=-2))
	{
	  printf("%5d\t%20s\t%5d\t",u_form->index[i],u_form->uniform_name[i],u_form->size[i]);
	  if(u_form->store_regi[i]==-1)
	    printf("    (NULL)\n");
	  else
	    printf("%10d\n",u_form->store_regi[i]);
	}
    }
  printf("==================END Uniform=================\n");
}

void print_varying()
{
  int i;
  printf("==================Varying=================\n");
  printf("     Index\t                name\n");
  for (i=0;i<varying_count;i++)
    {
#ifdef assembler_standalone
      if(v_form.index[i]==-1||v_form.index[i]==-2);
      else
	printf("%10d\t%20s\n",v_form.index[i],v_form.vary_name[i]);
#else
      if(v_form->index[i]==-1||v_form->index[i]==-2);
      else
	printf("%10d\t%20s\n",v_form->index[i],v_form->vary_name[i]);
#endif
    }
  printf("==================END Varying=================\n");
}

void print_variable()
{
  printf("==================Variable=================\n");
  printf("      name\t     value\t address\n");
  struct variable_form *p_cur_r_form=p_first_r_form;
  while(1)
    {
      if(p_cur_r_form->next!=NULL)
	printf("%10s\t%10f\t%8d\n",p_cur_r_form->name,p_cur_r_form->value,p_cur_r_form->address);
      else
	break;
      p_cur_r_form=p_cur_r_form->next;
    }
  printf("================END Variable===============\n");
}

void print_variable2()
{
  printf("==================Variable2=================\n");
  printf("      name\t     value\t address\n");
  struct variable_form *p_cur_r_form=p_first_r_form2;
  while(1)
    {
      if(p_cur_r_form->next!=NULL)
	printf("%10s\t%10f\t%8d\n",p_cur_r_form->name,p_cur_r_form->value,p_cur_r_form->address);
      else
	break;
      p_cur_r_form=p_cur_r_form->next;
    }
  printf("================END Variable===============\n");
}

void print_attribute()
{
  int i;
  printf("==================Attribute=================\n");
  printf("      name\t     \n");
  for(i=0;i<8;i++)
    {
      if(a_form->index[i]!=-1)
	printf("%d %10s\n",i,a_form->att_name[i]);
    }
  printf("================END Attribute===============\n");
}

int used_uniform(int VOrF)
{
  int i=0;
  for(i=0;i<max_number_of_uniform_variables;i++)
    {
      if(u_form->index[i]==-1)
	{
	  if(VOrF==0||VOrF==1)
	    return i;
	  else
	    perr("used_uniform: VOrF");
	}
    }
  return max_number_of_uniform_variables;
}

int used_varying()
{
  int i=0;
  for(i=0;i<max_number_of_varying_variables;i++)
    {
#ifdef assembler_standalone
      if(v_form.index[i]==-1)
#else
      if(v_form->index[i]==-1)
#endif
	{
	  return i;
	}
    }
  return max_number_of_varying_variables;
}

int used_attribute()
{
  int i=0;
  for(i=0;i<8;i++)
    {
      if(a_form->index[i]==-1)
	{
	  return i;
	}
    }
  return 8;
}

#ifndef assembler_standalone
void pass_data(struct programobject *input_object,int VOrF) //for API
{
  printf("==================Pass_data=================\n");
  int i;
  int uni_index=0;
  if(VOrF==1)
    uni_index=max_number_of_uniform_variables;
#ifdef compiler_standalone
  input_object->shaders[VOrF]=(struct shaderobject*)malloc(sizeof(struct shaderobject));
#endif
  //input_object->shaders[VOrF]->shader_length=PC/8+1;
  input_object->shaders[VOrF]->shader_length=LastBinLine;
  printf("shader[%d]_length: %d\n",VOrF,input_object->shaders[VOrF]->shader_length);

  for(i=0;i<max_number_of_uniform_variables;i++)
    {
      if(u_form->index[i]==-1)
        {
          if(VOrF==0)
            input_object->vertex_uniform_number=i;
          else if(VOrF==1)
            input_object->fragment_uniform_number=i;
          else
            perr("pass_data: VOrF");
          //printf("uniform_number: %d   %d\tline: %d\n",input_object->vertex_uniform_number,input_object->fragment_uniform_number,i);
          break;
        }
      else if(u_form->index[i]==-2)
        {
          input_object->uniform[uni_index+i].name=(char *)malloc(32*sizeof(char));
          strcpy(input_object->uniform[uni_index+i].name,"(null)");
          input_object->uniform[uni_index+i].sizei=0;
          input_object->uniform[uni_index+i].location=-1;
        }
      else if(u_form->index[i]==i)
        {
#ifdef Opt_getSamplerIndex
	  if(u_form->IsSampler2D[i]) //Kaymon, sampler_id
	    input_object->uniform[uni_index+i].IsSampler2D=1;
	  else if(u_form->IsSamplerCube[i]) 
	    input_object->uniform[uni_index+i].IsSamplerCube=1;
	  else if(u_form->IsNormal[i]) 
	    input_object->uniform[uni_index+i].IsNormal=1;
#endif
          input_object->uniform[uni_index+i].name=(char *)malloc(32*sizeof(char));
          strcpy(input_object->uniform[uni_index+i].name,u_form->uniform_name[i]);
          input_object->uniform[uni_index+i].sizei=u_form->size[i];
          input_object->uniform[uni_index+i].location=uni_index+i;;
        }
      else
        perr("u_form->index error");
      //printf("name: %s\tsize: %d\tlocation: %d\n",input_object->uniform[uni_index+i].name,input_object->uniform[uni_index+i].sizei,input_object->uniform[uni_index+i].location);

    }
  /*
  for(int g=0;g<256;g++){ //debug for sampler_id
    if(input_object->uniform[g].IsSampler2D)
      printf("Sampler2D_Name[%d]:%s\n",g,input_object->uniform[g].name);
    else if(input_object->uniform[g].IsSamplerCube)
      printf("SamplerCube_Name[%d]:%s\n",g,input_object->uniform[g].name);
    else if(input_object->uniform[g].IsNormal)
      printf("NormalType_Name[%d]:%s\n",g,input_object->uniform[g].name);
  }
*/
  printf("==================END Pass_data=================\n");
}
#endif

int detect_uniform_number()
{
  int i;
  for (i=0;i<max_number_of_uniform_variables;i++)
    {
      if((u_form->index[i]==-1))
	break;
    }  
  return i;
}

#ifdef LoadAttributeFromSCT
int x;
int y;
int width;
int height;
#endif

void output_the_user_defining_function(char* filename,char type){
  FILE* fp;
  FILE* fp1;
  if(type == 'v')
    system("cp vertex.s v.txt");
  else
    system("cp fragment.s f.txt");

  if(type == 'v')
    fp = fopen("main_v.txt","w");
  else
    fp = fopen("main_f.txt","w");
  if(type == 'v')
    fp1  = fopen("userfunction_v.txt","w");
  else
    fp1  = fopen("userfunction_f.txt","w");

  if(!fp1 && !fp)
    printf("Failed to creat main or userfunction file.\n");

  FILE* read_file = fopen(filename,"r");
  char *pch;
  char tempRead[200];
  char *nEOF = tempRead;
  while(nEOF){
    nEOF = fgets(nowRead,200,read_file);
    //printf("nowRead from %s",nowRead);
    fflush(stdout);
    if(nowRead[0] != '.' && nowRead[0] != '\t' && nowRead[0] != '#'){//ascii code: '\t'=>9 ' '=>32
      strcpy(tempRead,nowRead);
      pch = strtok(nowRead," #");
      pch = strtok(NULL," #");  //Get the second token to check if the string is equal to "main". 
      if(pch!=NULL){
	if(strncmp(pch,"@main",5)&&pch[0]=='@'){ //If nowRead != "main" then ok. It will cut user defining function down and then paste below the main function.
	  //printf("pch:%s\n",pch);
	  fputs(tempRead,fp1);
	  nEOF = fgets(nowRead,200,read_file);
	  if(strncmp(nowRead,".Leh_func_begin",15)==0)  //Beginning string:
	    fputs(nowRead,fp1);
	  nEOF = fgets(nowRead,200,read_file);
	  while(nEOF){
	    if(strncmp(nowRead,".Leh_func_end",13)==0){ //Ending string.
	      fputs(nowRead,fp1);
	      break;
	    }
	    /*
	    else if(strncmp(nowRead,"\tBL OpenGLES.Start",18)==0){ //Delete this funciotn in user defined function.
	      nEOF = fgets(nowRead,200,read_file);
	      continue;
	    }
	    else if(strncmp(nowRead,"\tmtspr",6)==0){
	      fputs("\tBL OpenGLES.BackToMainFunction\n",fp1); //add one more instrunction to jump to main function.
	      fputs(nowRead,fp1);
	      }*/
	    else
	      fputs(nowRead,fp1);
	    nEOF = fgets(nowRead,200,read_file);
	  }
	}
	else{ //put main function into main_? file.
	  fputs(tempRead,fp);
	  continue;
	}
      }
      else{     //puts variable name into main_? file. e.g. Position_temp:
	fputs(tempRead,fp);
	continue;
      }
    }
    else{
      fputs(nowRead,fp);
    }
  }
  fclose(fp);
  fclose(fp1);
  fclose(read_file);
}
void merge_main_and_user_defining_function(char type){
  FILE* fp;
  FILE* fp1;
  if(type == 'v')
    system("cp main_v.txt main_v.txt2");
  else
    system("cp main_f.txt main_f.txt2");

  if(type == 'v')
    fp = fopen("main_v.txt","a+");
  else
    fp = fopen("main_f.txt","a+");
  if(type == 'v')
    fp1  = fopen("userfunction_v.txt","r");
  else
    fp1  = fopen("userfunction_f.txt","r");
  char tempRead[200];
  char *nEOF =  tempRead;
  while(nEOF){
    nEOF = fgets(tempRead,200,fp1);
    if(nEOF)
      fputs(tempRead,fp);
  }
  fclose(fp);
  fclose(fp1);
}
#ifdef assembler_standalone
int main(int argc,char *argv[])
#else
int linker(int file_number, char *filename[], struct varying_form *input_varying, char type, char *output_filename,struct programobject *input_object,int input_fd)
#endif
{
#ifndef assembler_standalone
  po = input_object;
#else
  struct programobject *input_object=NULL;
#endif
#ifdef printable
  //FILE *pfile1=fopen("opcode_and_binary","w");
#endif

#ifndef assembler_standalone
  int first_instruction=1;
#endif

  int i,j;
  int endofmain=0;
#ifndef uniform_for_unified_shader // Jimmy modify
  struct uniform_form temp_u_form; //In order to modify uniform_for_unified_shader, hence variable change to global
#endif
  u_form=&temp_u_form;
  struct attribute_form temp_a_form; 
  a_form=&temp_a_form;
#ifndef assembler_standalone
  v_form=input_varying;
#endif

#ifdef assembler_standalone
  for(i=0;i<argc;i++)
    {
      if(!strcmp("-h",argv[i]))
	{
	  PRINT_INSTRUCTION_GUILD;
	  exit(0);
	}
    }
#endif
  LastBinLine=0;
  uniform_number = -1;
  PC=0;
  bzero(&r_form,sizeof(r_form));
  bzero(&r_form2,sizeof(r_form2));
  variable_count=0;
#ifndef assembler_standalone
#ifdef uniform_for_unified_shader    // Jimmy modify
  if(type == 'v')   // To judge fragment shader will after vertex shader continue execute not to initial value 
    {
#endif
#endif
     for(i=0;i<max_number_of_uniform_variables;i++)
       {
	 u_form->index[i]=-1;
	 u_form->store_regi[i]=-1;
	 u_form->IsSampler2D[i]=0; //Kaymon add it.
	 u_form->IsSamplerCube[i]=0; //Kaymon add it.
	 u_form->IsNormal[i]=0; //Kaymon add it.
#ifdef getSamplerIndex
	 u_form->IsSampler2D[i]=0; //Kaymon add it.
#ifdef getSamplerIndex
	 memset(sampler_id[i],0,128);//Kaymon add it.
#endif
#endif
	 strcpy(u_form->uniform_name[i],"");
	 if(i<max_number_of_varying_variables)
	   {
	     a_form->index[i]=-1;
#ifdef assembler_standalone
	     v_form.index[i]=-1;
	     strcpy(v_form.vary_name[i],"");
#endif	 
	   }
       }
#ifndef assembler_standalone
#ifdef uniform_for_unified_shader    // Jimmy modify                                                 
    }              // To judge fragment shader will after vertex shader continue execute not to initial value 
#endif
#endif

#ifndef assembler_standalone
  if(type=='v')
    {
#ifdef LoadAttributeFromSCT
      /*
      a_form->index[0]=0;
      strcpy(a_form->att_name[0],input_object->attribute_name[0]);
      a_form->index[1]=0;
      strcpy(a_form->att_name[1],input_object->attribute_name[1]);
      a_form->index[2]=0;
      strcpy(a_form->att_name[2],input_object->attribute_name[2]);
      */
      x = input_object->x;
      y = input_object->y;
      width = input_object->width;
      height = input_object->height;
      //printf("width:%d\n",width);
      int index;
      for(index=0;index<max_number_of_varying_variables;index++){
	if( input_object->attribute_name[index] ){ //make here
	  a_form->index[index]=index;
	  strcpy(a_form->att_name[index],input_object->attribute_name[index]);
	}
      }
#else
      a_form->index[5]=5;
      strcpy(a_form->att_name[5],"rm_Vertex");
      //a_form->index[0]=0;
      //strcpy(a_form->att_name[0],"rm_Normal");
      a_form->index[2]=2;
      strcpy(a_form->att_name[2],"texcoord");

      //for simulator
      //v_form->index[1]=1;
      //strcpy(v_form->vary_name[1],"texcoord0");
#endif
    }
#endif
#ifndef assembler_standalone
  char tempRead[200];
  for(int index=0;index<1;index++){
    output_the_user_defining_function(filename[0],type);
    merge_main_and_user_defining_function(type);
    if(type == 'v')
      sprintf(tempRead,"cp main_v.txt %s",filename[0]);
    else
      sprintf(tempRead,"cp main_f.txt %s",filename[0]);
    system(tempRead);
  }

  //Add variables I need to variable table Kaymon

  if(variable_count==0){
    struct variable_form *p_cur_r_form=p_first_r_form;
#ifdef FS_DMEM_SET
    //If glsl do not use the constant value, it won't set the PC_count and gl_FragCoord. (Should be fixed.)
    if(type == 'f'){
      strcpy(p_cur_r_form->name,"gl_FragCoord");
      p_cur_r_form->address=variable_count; //indicate 1024 to this variable's address 
      p_cur_r_form->next = &r_form[++variable_count];
      p_cur_r_form=p_cur_r_form->next;
    }
#endif

    strcpy(p_cur_r_form->name,"PC_count"); //VS: used_record_address. FS: 1025(1024+1)
    p_cur_r_form->address=variable_count; 
    p_cur_r_form->next = &r_form[++variable_count];
    p_cur_r_form=p_cur_r_form->next;

  }
#endif

  //}
#ifdef assembler_standalone
  if(argc<=2)
    {
      PRINT_INSTRUCTION_GUILD;
      perr("Arguments are too few.");
    }
  for(j=0;j<argc-2;j++)
    {
      if(argv[j+1][0]=='-')
	continue;
      
      make_table(argv[j+1],'v',input_object);
    }
#else
  for(j=0;j<file_number;j++){
    make_table(filename[j],type,input_object);
  }
#endif
  printf("LastBinLine: %d\n",LastBinLine);

  uniform_number=detect_uniform_number();
  move_table_variable_to_noninitialize_global_variable_table();
#ifdef assembler_standalone
  fill_regular_variable_start_address('v');
#else
  fill_regular_variable_start_address(type);
#endif 

#ifdef VAR_INFO
  print_uniform();
  print_variable();
  print_variable2();
  print_table();
#endif

  FILE *read_file;
  //FILE *read_file1;
#ifdef assembler_standalone
  for(j=0;j<argc-2;j++)
  {
    if(argv[j+1][0]=='-')
      continue;

    read_file=fopen(argv[j+1],"r");
    //read_file1=fopen(argv[j+1],"r");
#else
  for(j=0;j<file_number;j++)
  {
    read_file=fopen(filename[j],"r");
    //read_file1=fopen(filename[j],"r");
#endif
    char *nEOF=t_read;//,*read_op;
    int have_func;
    int mtspr_count=0;
    int lock_decode=0;

#ifdef assembler_standalone
    for(i=0;i<argc;i++)
      {
	if(!strcmp("-simple",argv[i]))
	  {
	    lock_decode=1;
	  }
      }
#endif

    //pWBinary=fopen("temp_binary","w");
    //bzero(&buffer,sizeof(buffer));
    //printf("Before encoding: PC %d ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n",PC);
    while(nEOF)
      {
	int i,id_dot_skip=0;
	nEOF=fgets(nowRead,200,read_file);
	have_func=0;
	strcpy(t_read,nowRead);
	if(!nEOF)
	  {
	    //printf("End decode.\n");
	    break;
	  }
	//read_op=strtok(nowRead," ");
	temp_assembly_init();

	decode();
	//printf("NowRead: %s",nowRead);
	//printf("2. PC: %d\tdecode_line:%s %s\n",PC,decode_line[0],decode_line[1]);

	for(i=0;i<50;i++)
	  {
	    if(decode_line[0][i]==':'|| decode_line[0][0]=='.'|| decode_line[0][0]=='\0')
	      {
		if((strcmp(decode_line[1],"#")!=0 || strcmp(decode_line[2],"constant")!=0 || strcmp(decode_line[3],"pool")!=0 || strcmp(decode_line[4],"float")!=0) && decode_line[0][i]==':')
		  have_func=1;
		//printf("i:%d\tbe continue\n",i);
		id_dot_skip=1;
	      }
	    if(decode_line[0][i]=='\0')
	      break;
	  }
	if(have_func==1)
	  {
	    char temp_label[20];
	    for(i=0;decode_line[0][i]!='\0';i++)
	      {
		if(decode_line[0][i]==':')
		  {
		    temp_label[i]='\0';
		    break;
		  }
		temp_label[i]=decode_line[0][i];
		
	      }

	    if(lock_decode==1)
	      PC=pTwoAlignAddress(PC,CurrentAlignmentNumber);

	    //printf("CurrentAlignmentNumber: %d\n",CurrentAlignmentNumber);

	    //printf("temp_label: %s\n",temp_label);
	    //printf("PC: %d\ttable: %d\n",PC,search_table(temp_label));
	    //printf("decode_line[0]: %s\n",decode_line[0]);
   
	    if(lock_decode==1 && PC!=search_table(temp_label))
	      {
		printf("decode_line[0]: %s\n",decode_line[0]);
		printf("%s\n",temp_label);
		printf("%s: %d\n",now_file,search_table(temp_label));
		printf("PC:%d\n",PC);
		printf("\n\n\nNotice label!!\n\n\n");
		perr("label address");
	      }
	    
	    continue;
	  }
	if(strcmp(decode_line[0],"")==0)
	  continue;
	if(strcmp(decode_line[0],".align")==0)
	  {
	    CurrentAlignmentNumber=atoi(decode_line[1]);
	    //printf("CurrentAlignmentNumber: %d\n",CurrentAlignmentNumber);
	    continue;
	  }
	if(id_dot_skip==1)
	  {
	    if(strcmp(decode_line[0],".comm")==0)
	      {
		if(lock_decode==1)
		  PC=pTwoAlignAddress(PC,CurrentAlignmentNumber);

		for(i=0;i<atoi(decode_line[2]);i++)
		  {
		    if(lock_decode==1)
		      input_var(0);
		  }
		/*      
			if((atoi(decode_line[2])%8)!=0)
			PC+=((atoi(decode_line[2])/8)+1);
			else
			PC+=(atoi(decode_line[2])/8);
		*/
		continue;
	      }
	    if(strcmp(decode_line[0],".section")==0)
	      continue;
	    if(strcmp(decode_line[0],".data")==0)
	      continue;
	    if(strcmp(decode_line[0],".file")==0)
	      {
		char *pTok;
		char temp_file[30];
		strcpy(temp_file,decode_line[1]);
		pTok=strtok(temp_file,"\"");
		strcpy(now_file,pTok);
		
		continue;
	      }
	    if(strcmp(decode_line[0],".text")==0)
	      continue;
	    if(strcmp(decode_line[0],".global")==0)
	      continue;
	    if(strcmp(decode_line[0],".globl")==0)
	      continue;
	    if(strcmp(decode_line[0],".type")==0)
	      {
		if(lock_decode==1)
		  PC=pTwoAlignAddress(PC,CurrentAlignmentNumber);
		continue;
	      }
	    if(strcmp(decode_line[0],".size")==0)
	      continue;
	    if(strcmp(decode_line[0],".ascii")==0)
	      {
		char temp_asc[50],*p_Tok;
		strcpy(temp_asc,nowRead);
		p_Tok=strtok(temp_asc,"\"");
		p_Tok=strtok(NULL,"\"");
		for(i=0;p_Tok[i]!='\0';i++)
		  {
		    if(lock_decode==1)
		      input_var(p_Tok[i]);
		  }
		continue;
	      }
	    if(strcmp(decode_line[0],".asciz")==0)
	      {
		char temp_asc[50],*p_Tok;
		strcpy(temp_asc,nowRead);
		p_Tok=strtok(temp_asc,"\"");
		p_Tok=strtok(NULL,"\"");
		for(i=0;p_Tok[i]!='\0';i++)
		  {
		    if(p_Tok[i]=='\\' && p_Tok[i+1]=='0' && p_Tok[i+2]=='0' && p_Tok[i+3]=='0')
		      {
			if(lock_decode==1)
			  input_var(0);
			i+=3;
			continue;
		      }
		    if(lock_decode==1)
		      input_var(p_Tok[i]);
		  }
		if(lock_decode==1)
		  input_var('\0');
		continue;
	      }
	    if(strcmp(decode_line[0],".space")==0)
	      {
		if(lock_decode==1)
		  for(i=0;i<atoi(decode_line[1]);i++)
		    input_var(0);
		continue;
	      }
	    if(strcmp(decode_line[0],".long")==0)
	      {
		if(lock_decode==1)
		  input_long(atoi(decode_line[1]));
		continue;
	      }
	    if(strcmp(decode_line[0],".short")==0)
	      {
		if(lock_decode==1)
		  input_short((short)atoi(decode_line[1]));
		continue;
	      }
	    if(strcmp(decode_line[0],".byte")==0)
	      {
		if(lock_decode==1)
		  input_byte(atoi(decode_line[1]));
		continue;
	      }
	    if(strcmp(decode_line[0],".uleb128")==0)
	      continue;
	    if(strcmp(decode_line[0],".sleb128")==0)
	      continue;	    
	    if(strncmp(decode_line[0],".Lset",5)==0)
	      continue;
	    //PC=AllignAddress(PC+(1<<CurrentAlingmentNumber),CurrentAlingmentNumber);
	    //PC=AlignAddress(PC,3);
	    //PC+=8;
	    continue;
	  }
	//printf("------------------------------\n");

	if(lock_decode==0)
	  {/*
	    if(strncmp(&nowRead[1],"mtspr",5)==0 && mtspr_count==0)
	      {
		if(endofmain)
		  endofmain=0;
		else{
		  lock_decode=1;
		  mtspr_count++;	
		}
		}*/
	    if(decode_line[0][0]>='A'&&decode_line[0][0]<='Z'){//&&decode_line[0][strlen(decode_line[0])-1]!=':'){
	      lock_decode=1;
#ifndef assembler_standalone	    
	      if(type=='f' && first_instruction) //duplicate the last instruction after calling opengl.start function.
		{
		  first_instruction = 0;
		  BinCode[LastBinLine++]=0xfc80000000000000LL;
		}
#endif
#ifndef assembler_standalone	    
	      deopcode(type,input_object);
#else
	      deopcode('a',input_object);
#endif
	      //continue;
	    }
	  }
	else
	  {
#ifdef assembler_standalone	    
	    deopcode('a',input_object);
#else
	    deopcode(type,input_object);
#endif
	    //decode();
	    /*
#ifndef assembler_standalone	    
	    if(type=='f' && first_instruction) //duplicate the last instruction after calling opengl.start function.
	      {
		first_instruction = 0;

#ifdef SET_NOP_NINE
		BinCode[LastBinLine++]=0xfc80000000000000LL;
		BinCode[LastBinLine++]=BinCode[LastBinLine-2];
#else
		BinCode[LastBinLine]=BinCode[LastBinLine-1];
		LastBinLine++;
#endif		
	      }
#endif
	    */
	    //if(strncmp(&nowRead[1],"BL OpenGLES.END.2",17)==0 || strncmp(&nowRead[1],"BL OpenGLES.ENDFrag.1",21)==0)
	    //if(strncmp(&nowRead[1],"BL OpenGLES.ENDFrag.1",21)==0)
	    //{
	    //lock_decode=0;
	    //}
	    //else if (strncmp(&nowRead[1],"mtspr",5)==0 && mtspr_count!=0)

	    //if (strncmp(&nowRead[1],"mtspr",5)==0 && mtspr_count!=0)
	  }

	//printf("%s\n",read_op);
	
	//      printf("main(%s):\t%s\n",decode_line[0],itob_54(nowBin));
	//emit(nowBin);
	
	//PC=AlignAddress(PC,3);
	//if(tempBinLine!=LastBinLine)
	//PC+=8;
	//printf("PC: %d\tLastBinLine: %d\n",PC,LastBinLine);

	if(lock_decode==1)
	  {
	    PC=LastBinLine*8;
	  }
	if(strncmp(&nowRead[1],"BL OpenGLES.END.3",17)==0||strncmp(&nowRead[1],"BL OpenGLES.ENDFrag.1",21)==0||strncmp(&nowRead[1],"BL OpenGLES.BackToMainFunction",30)==0)
	  lock_decode = 0;
	/*
	if(strncmp(&nowRead[1],"BL OpenGLES.END.3",17)==0 || strncmp(&nowRead[1],"BL OpenGLES.ENDFrag.1",21)==0)
	  {
	    endofmain = 1;
	    mtspr_count=0;
	    lock_decode = 0;
	  }
	else if (strncmp(&nowRead[1],"mtspr",5)==0 && mtspr_count!=0)
	  {
	    lock_decode=0;
	    mtspr_count=0;
	    //remove useless code
#ifndef assembler_standalone
	    if(type=='v')
	      {
#ifdef SET_NOP_NINE
		LastBinLine-=3;
#else
		//LastBinLine-=1; //Kaymonmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm!!!!!!!!!!!!!!!!!
#endif
	      }
#endif
}*/

      }
    //force_buffer_input();
    //fclose(pWBinary);

    printf("final PC:%d\n",PC);
    
    fclose(read_file);
    //fclose(read_file1);
#ifdef printable
    //fclose(pfile1);
#endif

  }

#ifdef VAR_INFO
  print_varying();
  print_attribute();
#endif

#ifndef assembler_standalone
  if(type=='v')
    pass_data(input_object,0);
  else if(type=='f')
    pass_data(input_object,1);
  else
    perr("type error!!");
#endif


#ifndef readable
#ifdef assembler_standalone
  printf("argv[argc-1]: %s\n",argv[argc-1]);
  printf("LastBinLine: %d\n",LastBinLine);
  putoutthefile(argv[argc-1]);
#else
  putoutthefile(output_filename);
#ifndef COMPILER_STANDALONE
  putoutmemory(type,input_fd);
#endif

#endif
  /*FILE *F=fopen("binary","w");
  fwrite(BinCode,sizeof (long long unsigned int), LastBinLine, F);
  fclose(F);
  */
#endif


  u_form=NULL;
  a_form=NULL;
#ifndef assembler_standalone
  v_form=NULL;
#endif

#ifndef assembler_standalone
  po->LinkerStatus = GL_TRUE; //Assembler didn't need this variable. The address of this variable point to null address.
#endif
  return 0;

}
