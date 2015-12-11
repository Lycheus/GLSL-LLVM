#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){


  FILE* fp = fopen("rm_Vertex.txt","r");
  FILE* fp1 = fopen("texcoord.txt","r");
  FILE* fp2 = fopen("new.txt","w");


  int size = 2880;

  int index = 0;
  char temp[100];
  while( index++ < size){


    fgets(temp,100,fp);
    fputs(temp,fp2);
    fputs("0.000000 0.000000 0.000000 0.000000\n",fp2);
    fgets(temp,100,fp1);
    fputs(temp,fp2);








  }















}
