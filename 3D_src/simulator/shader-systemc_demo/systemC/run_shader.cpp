#include <stdio.h>
#include <stdlib.h>
int main()
{
  char run_command[100];
  int Uniform_index=0;
  char get_ch='0';

  do
    {
      switch(get_ch)
	{
	case 'a':
	  if(Uniform_index>0)
	    Uniform_index--;
	  break;
	case 'd':
	  if(Uniform_index<9)
	    Uniform_index++;
	  break;
	case '0':
	  break;
	default:
	  get_ch='1';
	  break;
	}
      if(get_ch=='1')
	{
	  printf("Wrong input!!Please input again.\n");
	  printf("Input the rotate direction(w a s d)(q: quit): ");
	  get_ch=getchar();
	  continue;
	}

      sprintf(run_command,"cp file/backup/1_cube/Uniform%d.txt file/Uniform.txt",Uniform_index);
      //printf("%s\n",run_command);
      system(run_command);


      printf("running...\n");
      sprintf(run_command,"./test_shader");
      system(run_command);


      printf("Input the rotate direction(w a s d)(q: quit): ");
      get_ch=getchar();
      if(get_ch=='0')
	get_ch='1';
    }while(get_ch!='q');
  
  return 0;
}
