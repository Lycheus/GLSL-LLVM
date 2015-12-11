#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define streq(A,B) (strcmp(A,B)==0) 
#define badCOMMAND(A)  ( (A[0]=='\t') && ((A[1]>='a')&&(A[1]<='z')) )
#define goodCOMMAND(A) ( (A[0]=='\t') && ((A[1]>='A')&&(A[1]<='Z')) )
#define ADDI 1
#define SHL  2
#define STV  3
char Window[128][128], *CurLn=Window[127];
char Wops[128]={0};
long unsigned int regs[128]={0};

void quit()
{ int i;
  for (i=0;i<127;i++)
  if (! streq(Window[i],"\tNOP") )
  {
    printf("%s\n", Window[i]);
    if (goodCOMMAND(Window[i]))
      printf("\tNOP 9\n");
  }
  exit (1);
}

void shiftupline()
{ int i;
  if (! streq(Window[0],"\tNOP") )
  {
    printf("%s\n", Window[0]);
    if (goodCOMMAND(Window[0]) || ((Window[0][1]=='v')&&(Window[0][2]=='m')&&(Window[0][3]=='r')&&(Window[0][4]=='g')&&(Window[0][5]=='h')&&(Window[0][6]=='w')) || ((Window[0][1]=='v')&&(Window[0][2]=='s')&&(Window[0][3]=='l')&&(Window[0][4]=='d')&&(Window[0][5]=='o')&&(Window[0][6]=='i')))
      printf("\tNOP 9\n");
  }
  for (i=0; i<127; i++)
  { strcpy(Window[i], Window[i+1]);
    Wops[i]=Wops[i+1];
    regs[i]=regs[i+1];
  }
}

void GetNextCommand()
{ while (1)
  { if (! badCOMMAND(CurLn) )
       shiftupline();
    if(!gets(CurLn))
       quit();
    //printf("CurLn : %s\n",CurLn);
    if (badCOMMAND(CurLn))
    { if ((CurLn[1]=='b')&&(CurLn[2]=='l')&&(CurLn[3]=='r'))
      { CurLn[1]='R'; CurLn[2]='E'; CurLn[3]='T'; }
      if ((CurLn[1]=='s')&&(CurLn[2]=='t')&&(CurLn[3]=='v')&&(CurLn[4]=='x'))
      { CurLn[1]='S'; CurLn[2]='T'; CurLn[3]='V'; CurLn[4]=' '; }
      //added by sheng-chih
      if ((CurLn[1]=='m')&&(CurLn[2]=='f')&&(CurLn[3]=='s')&&(CurLn[4]=='p')&&(CurLn[5]=='r'))
	{shiftupline();return;}
      if ((CurLn[1]=='m')&&(CurLn[2]=='t')&&(CurLn[3]=='s')&&(CurLn[4]=='p')&&(CurLn[5]=='r'))
	{shiftupline();return;}
      if ((CurLn[1]=='m')&&(CurLn[2]=='t')&&(CurLn[3]=='l')&&(CurLn[4]=='r'))
	{shiftupline();return;}
      if ((CurLn[1]=='f')&&(CurLn[2]=='n')&&(CurLn[3]=='e')&&(CurLn[4]=='g')) //Kaymon added
	{ CurLn[1]='F'; CurLn[2]='N'; CurLn[3]='E'; CurLn[4]='G'; }
      if ((CurLn[1]=='v')&&(CurLn[2]=='m')&&(CurLn[3]=='r')&&(CurLn[4]=='g')&&(CurLn[5]=='h')&&(CurLn[6]=='w'))
	{shiftupline();return;}
      if ((CurLn[1]=='v')&&(CurLn[2]=='s')&&(CurLn[3]=='l')&&(CurLn[4]=='d')&&(CurLn[5]=='o')&&(CurLn[6]=='i'))
	{shiftupline();return;}
      if ((CurLn[1]=='m')&&(CurLn[2]=='r')&&(CurLn[3]==' '))
	{
	  int i;
	  int mr_size = strlen(CurLn);
	  for(i = mr_size ; i>=2;i--)
	    {
	      CurLn[i+1] = CurLn[i];
	    }

	  CurLn[1]='M'; CurLn[2]='V'; CurLn[3]='R'; CurLn[4]=' '; }
      //end added
      if ((CurLn[1]=='g')&&(CurLn[2]=='p')&&(CurLn[3]=='u')&&(CurLn[4]=='_'))
      {
	int i;
	char *tmp1, tmp2[128];
	for(i=0;i<128;i++)
	{
	  if (CurLn[i]==' ') break;
	  CurLn[i] = toupper(CurLn[i]);
	}
	tmp1 = CurLn + 5;
	strcpy(tmp2, "\t");
	strcat(tmp2, tmp1);
	strcpy(CurLn, tmp2);
      }
    }
    //printf("revision of CurLn : %s\n",CurLn);
    if (goodCOMMAND(CurLn))
      return;  
  }
}

int Uses(long unsigned int U, long unsigned int W)
{  long unsigned int G, WR = W & 31;
  if (((U>>8)&31)==WR)
    return 1;
  if (((U>>16)&31)==WR)
    return 1;
  return 0;
}

void Categorize(char *L)
{ char *a, *b;
  if (strstr(L,"\tADDI ")==L)
     Wops[127]=ADDI;
  else if (strstr(L,"\tSHL ")==L)
     Wops[127]=SHL;
  else
     Wops[127]=0;
  regs[127]=0;

  a = strstr(L, " ");
  if (a==NULL)
    return;
  b=a++;
  if (*b =='r')
    regs[127]+=atoi(b+1);

  a = strstr(b, " ");
  if (a==NULL)
    return;
  b=a++;
  if (*b =='r')
    regs[127]+=((atoi(b+1))<<8);

  a = strstr(b, " ");
  if (a==NULL)
    return;
  b=a++;
  if (*b =='r')
    regs[127]+=((atoi(b+1))<<16);
}

void LookFor32BitLoad()
{ char *a, L[80];
  int i,j;

  if (Wops[127]!=ADDI) return;
  a = strstr(CurLn,"@l");
  if (!a) return;

  for (; a>CurLn; a--) if (*a==' ') break;

  strcpy(L,a+1);
  a = strstr(L,"@l");
  *(a+1)='h';
  *(a+2)='a';
  *(a+3)='\0';

  for (i=126; i>=0; i--)
  { if((Wops[i]==SHL)&&(strstr(Window[i],", 16"))) break;
  }
  if (i==-1) return;
  for (j=i-1; j>=0; j--)
  { if (Uses(regs[i], regs[j]))
    { if ((Wops[j]!=ADDI) || (!strstr(Window[j],L)))
        return;
      strcpy(Window[i],"\tNOP");
      strcpy(Window[j],"\tNOP");
      a = strstr(CurLn,"@l");
      *a='\0';
      return;
    }
  }
}

void ChangestvxToSTV(char *L)
{
  char *a, *b, *c, *mark, tmp[128];
  int size_b;
  //added by sheng-chih
  int size_c;
  //end added
  if (strstr(L,"\tSTV ")==L) Wops[127]=STV;
  if (Wops[127]!=STV) return;

  // STV (r)a, b, c => STV (r)a, rc, b
  // or
  // STV (r)a, b, c set_mark => STV (r)a, rc, b set_mark
  // or
  // STV (r)a, b, <SV> => STV (
  a = strstr(L,"r");
  if (!a) return;
  b = strstr(a,", ");
  if (!b) return;
  c = b + 2;
  c = strstr(c,", ");
  size_b = strlen(b) - strlen(c);

  //added by sheng-chih
  mark = c + 2;
  mark = strstr(c,"set_mark");
  //end added

  c = strtok(c,", ");
  if (!c) return;

  strcpy(tmp, "\tSTV ");
  a = strtok(a,", ");
  strcat(tmp, a);
  strcat(tmp, ", r");
  strcat(tmp, c);
  strcat(tmp, ", ");
  strncat(tmp, b+2, size_b-2);

  //added by sheng-chih
  if (mark)
    {
      strcat(tmp," ");
      strcat(tmp,mark);
    }
  //end added

  strcpy(L, tmp);
  return;
}

int main()
{ int i;
  for (i=0;i<128;i++){
    strcpy(Window[i], "\tNOP");
    //printf("Window[%d]: %s\n",i,Window[i]);
  }
  while (1)
  { GetNextCommand();
    //printf("[ %s ]\n",CurLn);
    Categorize(CurLn);
    LookFor32BitLoad();
    //ChangestvxToSTV(CurLn); // Goda 2011-01-04
  }
}


/*
ADDI rx, r0, Y@ha  
([^ ]* [^rx] *$)*
SHL  rx, rx, 16
([^ ]* [^rx] *$)*
ADDI rz, r0, Y@l		ADDI rz, r0, Y@l
blr
mtlr
mtspr
mfspr

#define BEG  11000
#define JR   11000
#define BNE  11001
#define BLT  11010
#define BGT  11011
#define CAL  11100
#define RET  11100
#deline JMP  11100
#define B    11100
#define LD   01000
#define LDI  01010
#define LDIF 01010
#define ADDI 01010
#define ADDIF 01010
#define ST   01001
#define MVR  00111
#define RCP  00010
#define RSQ  00011
#define LOG2 00100
#define POW2 00101
#define DIV  00110
#define DSQ  00001
#define ADD  10000
#define ADDF 10000
#define SUB  10000
#define SUBF 10000
#define RED  10000
#define GREEN 10000
#define BLUE  10000
#define ALPHA 10000
#define GETX  10000
#define GETY  10000
#define GETZ  10000
#define GETW  10000
#define MULL  01101
#define MULH  10001
#define MULF  10001
#define MAD   10010
#define MADF  10010
#define DP    10011
#define DPF   10011
#define AND   10101
#define OR    10110
#define XOR   10111
#define INV   10100
#define MIN   01011
#define MINF  01011

#define MAX   01100
#define MAXF  01100
#define ITOF  11100
#define FTOI  11100
#define SHR   11110
#define SHL   11110
*/
