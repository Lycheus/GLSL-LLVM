#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "instructions.h"

long long unsigned int decode_value;
int PC=0;
#define perr(X) {printf("%llx error: %s\n",decode_value,X);exit(1);}
#define perror(X) {printf("%s\n",X);exit(1);}

struct all_element temp_code;

void init()
{
  int i;
  for(i=0;i<4;i++)
    temp_code.regi[i]=-1;
  temp_code.address=-1;
  temp_code.change_imm=-1;
  temp_code.sa=-1;
  temp_code.interface_register=0;
  temp_code.neg=0;
}

void input_error()
{
  printf("disassembler <input file>\n");
  exit(1);
}

void deopcode(int opcode)
{
  temp_code.opcode_value=opcode;
  switch (opcode)
    {
    case 1:
      strcpy(temp_code.opcode,"DSQ");
      break;
    case 2:
      strcpy(temp_code.opcode,"RCP");
      break;
    case 3:
      strcpy(temp_code.opcode,"RSQ");
      break;
    case 4:
      strcpy(temp_code.opcode,"LOG2");
      break;
    case 5:
      strcpy(temp_code.opcode,"POW2");
      break;
    case 6:
      strcpy(temp_code.opcode,"DIV");
      break;
    case 7:
      strcpy(temp_code.opcode,"MVR");
      break;
    case 8:
      strcpy(temp_code.opcode,"LDV");
      break;
    case 9:
      strcpy(temp_code.opcode,"STV");
      break;
    case 10:
      strcpy(temp_code.opcode,"10");//MVI  MVIF  LDI  LDIF  ADDI  ADDIF
      break;
    case 11:
      strcpy(temp_code.opcode,"11");//MIN MINF
      break;
    case 12:
      strcpy(temp_code.opcode,"12");//MAX MAXF
      break;
    case 13:
      strcpy(temp_code.opcode,"13");//FTOI  ITOF
      break;
    case 14:
      strcpy(temp_code.opcode,"FIN");
      break;
    case 15:
      strcpy(temp_code.opcode,"15");
      break;
    case 16:
      strcpy(temp_code.opcode,"16");//ADD  ADDF  SUB  SUBF  RED  GREEN  BLUE  ALPHA  GETX  GETY  GETZ  GETW
      break;
    case 17:
      strcpy(temp_code.opcode,"MULF");
      break;
    case 18:
      strcpy(temp_code.opcode,"18");//MAD  MADF
      break;
    case 19:
      strcpy(temp_code.opcode,"19");//DP  DPF
      break;
    case 20:
      strcpy(temp_code.opcode,"INV");
      break;
    case 21:
      strcpy(temp_code.opcode,"AND");
      break;
    case 22:
      strcpy(temp_code.opcode,"OR");
      break;
    case 23:
      strcpy(temp_code.opcode,"XOR");
      break;
    case 24:
      strcpy(temp_code.opcode,"24");//BEQV BEQS  JR
      break;
    case 25:
      strcpy(temp_code.opcode,"25");//BNEV BNES
      break;
    case 26:
      strcpy(temp_code.opcode,"26");//BLTV BLTVF BLTS BLTSF
      break;
    case 27:
      strcpy(temp_code.opcode,"27");//BGTV BGTVF BGTS BGTSF
      break;
    case 28:
      //strcpy(temp_code.opcode,"28");//FTOI  ITOF
      strcpy(temp_code.opcode,"MULX");
      break;
    case 29:
      strcpy(temp_code.opcode,"29");//CAL  RET  JMP  B
      break;
    case 30:
      strcpy(temp_code.opcode,"30");//SHL  SHR
      break;
    case 31://NOP
	strcpy(temp_code.opcode,"NOP");
      break;
    default:
      perr("No this opcode");
      break;
    }
}

void deregister(int register_number,int input_register)
{
  temp_code.regi[register_number]=input_register;
}

void desampler(int input_sampler)
{
  temp_code.sampler=input_sampler;
}

void deswizzle(int swizzle_number,int input_swizzle)
{
  temp_code.swizzle[swizzle_number]=input_swizzle;
}

void deneg(int input_neg)
{
  temp_code.neg=input_neg;
}

void dewm(int input_wm)
{
  temp_code.wm=input_wm;
}

void descc(int input_scc)
{
  temp_code.scc=input_scc;
}

void defxfl(int input_fxfl)
{
  temp_code.fxfl=input_fxfl;
}

void deb_vs(int input_b_vs)
{
  temp_code.b_vs=input_b_vs;
}

void desv(int input_sv)
{
  temp_code.sv=input_sv;
}

void desa(int input_sa)
{
  temp_code.sa=input_sa;
}

void derl(int input_rl)
{
  temp_code.rl=input_rl;
}

void deimm(int input_imm)
{
  temp_code.change_imm=0;
  temp_code.imm=input_imm;
}

void denop_number(int input_nop_number)
{
  temp_code.nop_number=input_nop_number;
}
void deaddress(int input_address)
{
  temp_code.address=input_address;
}

void deinte_regi(int input_address)
{
  temp_code.interface_register=input_address;
}

void decode()
{
  unsigned int catch_value,catch_value2;
  
  catch_value=decode_value>>59;
  deopcode(catch_value);
  switch (catch_value)
    {
    //type 2
    case 8:
    case 9:
    case 10:
      //case 13:
      //case 14:
    //type 1 SHR SHL
    case 30:
      catch_value2=(decode_value<<5)>>58;
      deregister(0,catch_value2);
      catch_value2=(decode_value<<11)>>58;
      deregister(1,catch_value2);
      if(catch_value==30)
	{
	  //catch_value2=(decode_value<<17)>>59;
	  catch_value2=(decode_value<<17)>>58;
	  desa(catch_value2);
	  catch_value2=(decode_value<<23)>>56;
	  deswizzle(0,catch_value2);
	  catch_value2=(decode_value<<31)>>56;
	  deswizzle(1,catch_value2);
	  catch_value2=(decode_value<<49)>>63;
	  derl(catch_value2);
	}
      else
	{
	  catch_value2=(decode_value<<17)>>32;
          deimm(catch_value2);

	}
      catch_value2=(decode_value<<50)>>60;
      dewm(catch_value2);

    
      break;
    //type 3
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    // type 1
    case 11:
    case 12:
    case 13:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 28:
      catch_value2=(decode_value<<5)>>58;
      deregister(0,catch_value2);
      catch_value2=(decode_value<<11)>>58;
      deregister(1,catch_value2);
      catch_value2=(decode_value<<17)>>58;
      deregister(2,catch_value2);
      catch_value2=(decode_value<<23)>>56;
      deswizzle(0,catch_value2);
      catch_value2=(decode_value<<31)>>56;
      deswizzle(1,catch_value2);
      catch_value2=(decode_value<<39)>>56;
      deswizzle(2,catch_value2);
      catch_value2=(decode_value<<47)>>61;
      deneg(catch_value2);
      catch_value2=(decode_value<<50)>>60;
      dewm(catch_value2);
      if(catch_value==18)
	{
	  temp_code.regi[3]=temp_code.regi[2];
	  temp_code.regi[2]=temp_code.regi[1];
	  temp_code.regi[1]=temp_code.regi[0];
	}
      break;

    //type 4
    case 24:
    case 25:
    case 26:
    case 27:
      catch_value2=(decode_value<<33)>>56;
      deswizzle(0,catch_value2);
      catch_value2=(decode_value<<41)>>56;
      deswizzle(1,catch_value2);
    case 29:
      catch_value2=(decode_value<<5)>>58;
      deregister(0,catch_value2);
      catch_value2=(decode_value<<11)>>58;
      deregister(1,catch_value2);
      catch_value2=(decode_value<<33)>>58;
      deregister(2,catch_value2);
      catch_value2=(decode_value<<39)>>59;
      deregister(3,catch_value2);
      catch_value2=(decode_value<<17)>>48;
      deaddress(catch_value2);
      catch_value2=(decode_value<<50)>>60;
      descc(catch_value2);
      break;
    //type nop
    case 31:
      catch_value2=(decode_value<<5)>>60;
      denop_number(catch_value2);
      break;
    //type TEX
    case 15:
      catch_value2=(decode_value<<5)>>58;
      deregister(0,catch_value2);
      catch_value2=(decode_value<<11)>>58;
      deregister(1,catch_value2);
      //catch_value2=(decode_value<<19)>>58;
      catch_value2=(decode_value<<17)>>58;
      desampler(catch_value2);
      break;
    case 14:
      break;
    default:
      perr("No this opcode");
      break;
    }
  
  catch_value2=(decode_value<<49)>>63;
  deb_vs(catch_value2);
  catch_value2=(decode_value<<54)>>63;
  defxfl(catch_value2);
  catch_value2=(decode_value<<55)>>62;
  desv(catch_value2);

  catch_value2=(decode_value<<57)>>63;
  deinte_regi(catch_value2);
  
}

void output_wm()
{
  printf(".%d%d%d%d",temp_code.wm>>3,(temp_code.wm&4)>>2,(temp_code.wm&2)>>1,(temp_code.wm&1));
}

void output_swizzle(int sw_number)
{
  char output_sw[4];
  int i,sw_value=temp_code.swizzle[sw_number];
  for (i=0;i<4;i++)
    {
      if(sw_value&1)
	{
	  if(sw_value&2)
	    output_sw[i]='w';
	  else
	    output_sw[i]='y';
	}
      else if(sw_value&2)
	output_sw[i]='z';
      else
	output_sw[i]='x';
      sw_value=sw_value>>2;
    }
  printf(".%c%c%c%c",output_sw[3],output_sw[2],output_sw[1],output_sw[0]);
}

void output()
{
  int i;

  //Set remained opcode name
  switch(atoi(temp_code.opcode))
    {
    case 0:
      break;
    case 10:
      if(temp_code.regi[1]==0)
	{
	  if(temp_code.fxfl==0)
	    strcpy(temp_code.opcode,"LDI");
	  else
	    strcpy(temp_code.opcode,"LDIF");
	}
      else
	{
	  if(temp_code.fxfl==0)
            strcpy(temp_code.opcode,"ADDI");
          else
            strcpy(temp_code.opcode,"ADDIF");
	}

      break;
    case 11:
      if(temp_code.fxfl==0)
	strcpy(temp_code.opcode,"MIN");
      else
        strcpy(temp_code.opcode,"MINF");
      break;
    case 12:
      if(temp_code.fxfl==0)
	strcpy(temp_code.opcode,"MAX");
      else
        strcpy(temp_code.opcode,"MAXF");
      break;
    case 13:
      if(temp_code.fxfl==0)
	strcpy(temp_code.opcode,"ITOF");
      else
        strcpy(temp_code.opcode,"FTOI");
      break;
    case 15:
      if(temp_code.interface_register){
	strcpy(temp_code.opcode,"TEXCUBE");
	temp_code.interface_register=0;
      }
      else
	strcpy(temp_code.opcode,"TEX");
      break;
    case 16:
      if(temp_code.regi[2]==0)
	{
	  if(temp_code.swizzle[1]==0)
	    strcpy(temp_code.opcode,"GETX");
	  else if(temp_code.swizzle[1]==85)
	    strcpy(temp_code.opcode,"GETY");
          else if(temp_code.swizzle[1]==170)
            strcpy(temp_code.opcode,"GETZ");
          else if(temp_code.swizzle[1]==255)
            strcpy(temp_code.opcode,"GETW");
	}
      if(temp_code.neg==1 && temp_code.swizzle[1]==0 && temp_code.swizzle[2]==0 && temp_code.wm==8)
	{
	  if(temp_code.fxfl==0)
	    strcpy(temp_code.opcode,"SUB");
	  else
	    strcpy(temp_code.opcode,"SUBF");
	  
	}
      else
	{
	  if(temp_code.fxfl==0)
	    strcpy(temp_code.opcode,"ADD");
	  else
	    strcpy(temp_code.opcode,"ADDF");
	}

      break;
      /*
    case 17:
      if(temp_code.fxfl==0)
	strcpy(temp_code.opcode,"MULH");
      else
	strcpy(temp_code.opcode,"MULF");
      break;
      */
    case 18:
      if(temp_code.fxfl==0)
        strcpy(temp_code.opcode,"MAD");
      else
        strcpy(temp_code.opcode,"MADF");      
      break;
    case 19:
      if(temp_code.fxfl==0)
        strcpy(temp_code.opcode,"DP");
      else
        strcpy(temp_code.opcode,"DPF");
      break;
    case 24:
      if(temp_code.regi[0]==0 && temp_code.regi[1]==0 && temp_code.regi[2]==0 && temp_code.regi[3]==0)
	strcpy(temp_code.opcode,"JR");
      else if(temp_code.b_vs)
	strcpy(temp_code.opcode,"BEQS");
      else
	strcpy(temp_code.opcode,"BEQV");
      break;
    case 25:
      if(temp_code.b_vs)
	strcpy(temp_code.opcode,"BNES");
      else
	strcpy(temp_code.opcode,"BNEV");
      break;
    case 26:
      if(temp_code.b_vs)
	{
	  if(temp_code.fxfl)
	    strcpy(temp_code.opcode,"BLTSF");
	  else
	    strcpy(temp_code.opcode,"BLTS");
	}
      else
	{
	  if(temp_code.fxfl)
	    strcpy(temp_code.opcode,"BLTVF");
	  else
	    strcpy(temp_code.opcode,"BLTV");
	}
      break;
    case 27:
      if(temp_code.b_vs)
	{
	  if(temp_code.fxfl)
	    strcpy(temp_code.opcode,"BGTSF");
	  else
	    strcpy(temp_code.opcode,"BGTS");
	}
      else
	{
	  if(temp_code.fxfl)
	    strcpy(temp_code.opcode,"BGTVF");
	  else
	    strcpy(temp_code.opcode,"BGTV");
	}

      break;
      /*
    case 28:
      if(temp_code.fxfl==0)
        strcpy(temp_code.opcode,"ITOF");
      else
        strcpy(temp_code.opcode,"FTOI");
      break;
      */
    case 29:
      if(temp_code.regi[0]==0)
	{
	  if(temp_code.regi[1]==0)
	    strcpy(temp_code.opcode,"JMP");
	  else
	    strcpy(temp_code.opcode,"RET");
	}
      else
	strcpy(temp_code.opcode,"CAL");
      break;
    case 30:
      if(temp_code.rl==0)
        strcpy(temp_code.opcode,"SHR");
      else
        strcpy(temp_code.opcode,"SHL");
      break;
    case 31:
      strcpy(temp_code.opcode,"NOP");
    default:
      perr("opcode number error");
      break;
    }

  //Output opcode
  printf("%s\t",temp_code.opcode);

  for(i=0;i<4;i++)
    {
      //branch scalar: break after printing two register.
      if(i>1)
	{
	  int b_break=0;
	  switch(temp_code.opcode_value)
	    {
	    case 24:
	    case 25:
	    case 26:
	    case 27:
	      if(temp_code.b_vs)
		b_break=1;
	      break;
	    default:
	      break;
	    }	  
	  if(b_break)
	    break;
	}

      //Output neg
      if(temp_code.opcode_value==7)
	{
	  if(i==1 && temp_code.neg&4)
	    printf("-");
	  /*
	  else if(i==1 && temp_code.neg&2)
	    printf("-");
	  */
	}
      else if (temp_code.opcode_value==18 && temp_code.fxfl==0)
	{
	  if(i==1 && temp_code.neg&1)
            printf("-");
          else if(i==2 && temp_code.neg&4)
            printf("-");
	  else if(i==3 && temp_code.neg&2)
	    printf("-");
	}
      else
	{
	  if(i==1 && temp_code.neg&4)
            printf("-");
          else if(i==2 && temp_code.neg&2)
            printf("-");
	}
      //Output register and write mask
      if(i==0)
	{
	  if(strcmp(temp_code.opcode,"JR")!=0 && strcmp(temp_code.opcode,"JMP")!=0 && strcmp(temp_code.opcode,"RET")!=0 && temp_code.opcode_value!=31 && strcmp(temp_code.opcode,"FIN")!=0)
	    printf("R%d",temp_code.regi[i]);

	  //write mask

	  switch(temp_code.opcode_value)
	    {
	    case 1:
	    case 2:
	    case 3:
	    case 4:
	    case 5:
	    case 6:
	    case 7:
	    case 8:
	    case 9:
            case 10:
	    case 11:
	    case 12:
	    case 13:
	      //case 14:
	    case 16:
	    case 17:
	    case 18:
	    case 19:
            case 20:
            case 21:
            case 22:
            case 23:
	    case 28:
            case 30:
	      output_wm();
	      break;
	    }

	  //output branch scalar swizzle
	  switch(temp_code.opcode_value)
	    {
	    case 24:
	      if(strcmp(temp_code.opcode,"JR")==0)
		break;
	    case 25:
	    case 26:
	    case 27:
	      if(temp_code.b_vs)
		output_swizzle(i);
	      break;
	    }
	}
      else
	{
	  if(strcmp(temp_code.opcode,"LDI")==0)
	    continue;

	  switch(temp_code.opcode_value)
	    {
	    case 24:
	      if(strcmp(temp_code.opcode,"JR")!=0)
		printf("R%d",temp_code.regi[i]);
	      break;
	    case 29:
	      if(strcmp(temp_code.opcode,"RET")==0 && i==1)
		printf("R%d",temp_code.regi[i]);
	      else if(strcmp(temp_code.opcode,"JMP")!=0 && strcmp(temp_code.opcode,"RET")!=0)
                printf("R%d",temp_code.regi[i]);
	      break;
	    case 1:
	    case 2:
	    case 3:
	    case 4:
	    case 5:
	    case 6:
	    case 7:
	    case 8:
	    case 9:
	    case 10:
	    case 13:
	    case 15:
	      //case 28:
	    case 30:
	      if(i<2)
		printf("R%d",temp_code.regi[i]);
	      break;
	    case 14:
	      break;
	    default:
	      printf("R%d",temp_code.regi[i]);
	      break;
	    }
	  //Output swizzle
	  switch(temp_code.opcode_value)
	    {
	    case 7:
	      if(i==1 && (temp_code.sv!=0 || temp_code.wm!=8))
		output_swizzle(1);
	      break;
	    case 11:
	    case 12:
	    case 17:
	    case 19:
	    case 28:
	      if(temp_code.sv!=0 || temp_code.wm!=8 || temp_code.swizzle[1]!=0 || temp_code.swizzle[2]!=0)
		output_swizzle(i);
	      break;
	    case 16:
	      if(strcmp(temp_code.opcode,"ADD")==0 || strcmp(temp_code.opcode,"ADDF")==0)
		{
		  if(temp_code.swizzle[1]!=0 || temp_code.swizzle[2]!=0)
		    output_swizzle(i);
		}
	      break;
	    case 18:
	      //printf("\nsv:%d  wm:%d  sw1:%d  sw2:%d  sw3:%d\n",temp_code.sv,temp_code.wm,temp_code.swizzle[0],temp_code.swizzle[1],temp_code.swizzle[2]);
	      if(temp_code.sv!=0 || temp_code.wm!=8 || temp_code.swizzle[0]!=0 || temp_code.swizzle[1]!=0 || temp_code.swizzle[2]!=0 )
                output_swizzle(i-1);
	      break;
	    case 24:
	      if(strcmp(temp_code.opcode,"JR")==0)
		break;
	    case 25:
	    case 26:
	    case 27:
	      if(temp_code.b_vs)
		output_swizzle(i);
	      break;
	    }
	}
      if(i<3)
	{
	  if(strcmp(temp_code.opcode,"LDI")==0)
	    continue;

	  if(temp_code.regi[i+1]==-1)
	      break;
	  if(strcmp(temp_code.opcode,"JR")!=0 && strcmp(temp_code.opcode,"JMP")!=0 && strcmp(temp_code.opcode,"RET")!=0)
	    {
	      if(i>0)
		{
		  switch(temp_code.opcode_value)	      
		    {
		    case 1:
		    case 2:
		    case 3:
		    case 4:
		    case 5:
		    case 6:
		    case 7:
		    case 8:
		    case 9:
		    case 10:
		    case 13:
		      //case 14:
		      //case 15:
		      //case 28:
		    case 30:
		      break;
		    case 24:
		    case 25:
		    case 26:
		    case 27:
		      if(temp_code.b_vs)
			break;
		    default:
		      printf(", ");
		      break;
		    }
		}
	      else
		printf(", ");
	    }
	}
    }
  //Output address, imm, SCC, shift amount, sampler, or nop_number
  if(temp_code.address!=-1)
    {
      if(strcmp(temp_code.opcode,"JR")!=0 && strcmp(temp_code.opcode,"JMP")!=0)
	printf(", ");
      printf("%d",(signed short)temp_code.address);
    }
  else if(temp_code.change_imm!=-1)
    {
      float temp_change;
      memcpy(&temp_change,&temp_code.imm,4);
      if(temp_code.fxfl==1)
	printf(", %f (%d)",temp_change,temp_code.imm);
      else
	printf(", %d (%f)",temp_code.imm,temp_change);
    }
  else if(temp_code.sa!=-1){
    if(!strcmp("SHL",temp_code.opcode))
      printf(", R%d",temp_code.sa);
    else
      printf(", %d",temp_code.sa);
  }

  //SCC
  switch(temp_code.opcode_value)
    {
    case 24:
    case 25:
    case 26:
    case 27:
      if(!temp_code.b_vs)
	{
	  if(strcmp(temp_code.opcode,"JR"))
	    printf(", %d%d%d%d",temp_code.scc&8?1:0,temp_code.scc&4?1:0,temp_code.scc&2?1:0,temp_code.scc&1?1:0);
	}
      break;
    }

  if(temp_code.opcode_value==15)
    printf("\tSampler: %d",temp_code.sampler);
  if(temp_code.opcode_value==31)
    printf("%d",temp_code.nop_number);
 
  //Output sv
  switch(temp_code.opcode_value)
    {
    case 7:
    case 20:
    case 21:
    case 22:
    case 23:
    case 30:
      if(temp_code.wm!=8 || temp_code.sv!=0)
	printf(", <%d>",temp_code.sv+1);
      break;
    case 9:
      if(temp_code.sv!=0)
	printf(", <%d>",temp_code.sv+1);
      break;
    case 10:
      if(strcmp(temp_code.opcode,"ADDI")==0 || strcmp(temp_code.opcode,"ADDIF")==0)
	{
	  if(temp_code.wm!=8 || temp_code.sv!=0)
	    printf(", <%d>",temp_code.sv+1);
	}
      break;
    case 11:
    case 12:
    case 13:
    case 17:
    case 19:
    case 28:
      //if((temp_code.wm!=8 || temp_code.sv!=0) && (temp_code.wm!=15 || temp_code.sv!=3))
	printf(", <%d>",temp_code.sv+1);

      break;
    case 16:
      if(strcmp(temp_code.opcode,"ADD")==0 || strcmp(temp_code.opcode,"ADDF")==0)
	{
	  if(temp_code.swizzle[1]!=0 || temp_code.swizzle[2]!=0)
	    {
	      if(temp_code.wm!=15 || temp_code.sv!=3)
		printf(", <%d>",temp_code.sv+1);
	    }
	}
      break;
    case 18:
      if(temp_code.swizzle[0]!=0 || temp_code.swizzle[1]!=0 || temp_code.swizzle[2]!=0)
	printf(", <%d>",temp_code.sv+1);
      break;
    case 24:
    case 25:
    case 26:
    case 27:
      //case 28:
      if(strcmp(temp_code.opcode,"JR")!=0)
	printf(", <%d>",temp_code.sv+1);
      break;
      
    }
  if(temp_code.interface_register)
    {
      if(temp_code.opcode_value!=14)
	printf("\t(Rd(R%d): Fragment interface register)",temp_code.regi[0]);
      else
	printf("Fragment Start!!");
    }
  printf("\n");

}


int main(int argc, char *argv[])
{
  char catch[8];
  int i;
  if(argc != 2)
    input_error();
  
  FILE *p_read=fopen(argv[1],"r");
  
  if(!p_read)
    {
      char temp_string[100];
      sprintf(temp_string,"%s can't be opened.",argv[1]);
      perror(temp_string);
    }
  //if(open(argv[2],O_RDONLY)!=-1)
  //{
  //  char over_write[32]="n";
  //  printf("The file ""%s"" exists\nDo you want to overwrite?[y/N] ==>",argv[2]);
  //  scanf("%s",over_write);
  //  if(strcmp(over_write,"y")!=0 && strcmp(over_write,"Y")!=0)
  //	exit(1);
  //}
  
  //FILE *p_write=fopen(argv[2],"w");

  while(1)
    {
      init();
      for(i=0;i<8;i++)
	  catch[i]=getc(p_read);

      if(catch[1]==EOF)
	{
	  printf("End code.\n");
	  break;
	}

      memcpy(&decode_value,catch,8);
      printf("PC: %d (%d)\t%16llx:\t",PC/8,PC,decode_value);
      decode();
      output();
      PC=PC+8;
      
    }

  return 0;
}
