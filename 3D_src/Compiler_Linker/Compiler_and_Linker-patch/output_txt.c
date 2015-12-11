#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int main(int argc,char* argv[])
{
  if(open("c_model_output",O_RDONLY)!=-1)
    {
      char over_write[32]="n";
      printf("The file \"c_model_output\" exists\nDo you want to overwrite?[y/N] ==>");
      scanf("%s",over_write);
      if(strcmp(over_write,"y")!=0 && strcmp(over_write,"Y")!=0)
        exit(1);
    }
  FILE *p_ofile=fopen("c_model_output","w");
  FILE *p_ifile=fopen(argv[1],"r");
  int i;
  char catch[1024];

  while( fgets(catch,sizeof(catch),p_ifile))
    {
      for(i=0;i<1024;i++)
	{
	  if(catch[i]=='\0')
	    break;
	  if(catch[i]==' ')
	    {
	      int j;
	      for(j=i;j<1024;j++)
		{
		  if(catch[j]=='\0')
		    break;
		  catch[j]=catch[j+1];
		}
	    }
	}
//printf("%s",catch);
      fprintf(p_ofile,"%s",catch);


    }
  fclose(p_ofile);
  fclose(p_ifile);


  return 0;
}
