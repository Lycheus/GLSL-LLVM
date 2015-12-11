#include <stdio.h>
#include <stdlib.h>

unsigned long long temp_b[2];

#define perror(X){printf("%s is error!!\n",X);exit(1);}
int main(int argc,char* argv[])
{
  if(argc!=3)
    perror("argc");

  FILE *load_file=fopen(argv[1],"r");
  FILE *store_file=fopen(argv[2],"w");
  char load_str[128];


  while(fgets(load_str,128,load_file)!=NULL)
    {
      printf("load_str: %s",load_str);
      int i;
      temp_b[0]=0;
      for(i=0;i<64;i++)
	{
	  temp_b[0]=temp_b[0]<<1;
	  temp_b[0]|=(load_str[i]==48)?0:1;
	}
      fwrite(temp_b,sizeof(long long),1,store_file);
    }
  return 0;
}
