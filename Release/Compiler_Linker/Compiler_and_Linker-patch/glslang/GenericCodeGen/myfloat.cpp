//dondum 2010-07-26
#include <stdio.h>
#include <string.h>
typedef struct{
    unsigned long significand:52;
    unsigned int exponent:11;
    unsigned int s:1;
}myfloat;

void TransBinBit(unsigned long n,int bitcount,char *str){
    if(!bitcount)
	return;
    TransBinBit(n>>4,bitcount-4,str);
    long temp=n&(unsigned long)15;
    switch(temp)
      {
      case 0:
	sprintf(str,"%s0",str);
	break;
      case 1:
	sprintf(str,"%s1",str);
	break;
      case 2:
	sprintf(str,"%s2",str);
	break;
      case 3:
	sprintf(str,"%s3",str);
	break;
      case 4:
	sprintf(str,"%s4",str);
	break;
      case 5:
	sprintf(str,"%s5",str);
	break;
      case 6:
	sprintf(str,"%s6",str);
	break;
      case 7:
	sprintf(str,"%s7",str);
	break;
      case 8:
	sprintf(str,"%s8",str);
	break;
      case 9:
	sprintf(str,"%s9",str);
	break;
      case 10:
	sprintf(str,"%sA",str);
	break;
      case 11:
	sprintf(str,"%sB",str);
	break;
      case 12:
	sprintf(str,"%sC",str);
	break;
      case 13:
	sprintf(str,"%sD",str);
	break;
      case 14:
	sprintf(str,"%sE",str);
	break;
      case 15:
	sprintf(str,"%sF",str);
	break;
      }
    if(bitcount==64)
      sprintf(str,"%s",str);
    return;
}

void TransBinFloat(double n,char *return_str){
    myfloat m;
    //char return_str[256];
    m=*(myfloat *)(&n);
    sprintf(return_str,"0x\0");
    TransBinBit(*(unsigned long*)(&n),64,return_str);
    /*
    TransBinBit(m.s,1);
    printf(" ");

    TransBinBit(m.exponent,11);
    printf(" ");

    TransBinBit(m.significand,52);
    printf("\n");
    */
    return ;
}




