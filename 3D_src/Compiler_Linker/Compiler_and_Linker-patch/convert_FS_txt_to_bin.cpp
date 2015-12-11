#include <stdio.h>
#include <stdlib.h>
#define perr(X){printf(X);exit(1);}

//#define show_size_of_file
//#define show_without_newline

long long instr=0;

void convert_FS_hex_to_txt(char* head,char* tail){
  char *temp = head;
  int i,j=0;
  for(j=0;j<2;j++){
    for(i=2;i<10;i++){

      //printf("%i: %c\n",i,*(temp+i));
            
      switch( *(temp+i) ){

      case '0':
	instr=instr<<4;
	instr|=0;
	break;
      case '1':
	instr=instr<<4;
	instr|=1;
	break;
      case '2':
	instr=instr<<4;
	instr|=2;
      break;
      case '3':
	instr=instr<<4;
	instr|=3;
	break;
      case '4':
	instr=instr<<4;
	instr|=4;
	break;
      case '5':
	instr=instr<<4;
	instr|=5;
	break;
      case '6':
	instr=instr<<4;
	instr|=6;
	break;
      case '7':
	instr=instr<<4;
	instr|=7;
	break;
      case '8':
	instr=instr<<4;
	instr|=8;
      break;
      case '9':
	instr=instr<<4;
	instr|=9;
	break;
      case 'a':
	instr=instr<<4;
	instr|=10;
	break;
      case 'b':
	instr=instr<<4;
	instr|=11;
	break;
      case 'c':
	instr=instr<<4;
	instr|=12;
	break;
      case 'd':
	instr=instr<<4;
	instr|=13;
	break;
      case 'e':
	instr=instr<<4;
	instr|=14;
	break;
      case 'f':
	instr=instr<<4;
	instr|=15;
	break;

      default:
	break;
      }
    }
    temp = tail;    
  }
    
    







}

int main(){

  printf("input file : FS_txt\n");
  printf("output file : out_txt\n");

  FILE *fp = fopen("FS_txt","r");
  FILE *fp2 = fopen("out_txt","w");


  char buffer[13];
  char buffer1[13];
#ifdef show_size_of_file
  fseek(fp,0,SEEK_END);
  int size = ftell(fp);
  printf("Size of this %s:%d\n","FS_txt",size);
  fseek(fp,0,SEEK_SET);
#endif

#ifdef show_without_newline
  int count =0;
  while( fgets(buffer,32,fp) != NULL ){ 
    if(buffer[10]=='\n')
      count++;
    else{
      printf("no newline:%d\n",count+1);
    }
  }
  printf("%d\n",count);
  fseek(fp,0,SEEK_SET);
#endif

  /*  
  fgets(buffer,12,fp);
  fgets(buffer1,12,fp);
  printf("buffer:%s",buffer);
  printf("buffer1:%s",buffer1);
  convert_FS_hex_to_txt(buffer1,buffer);

  printf("%llx\n",instr);
  printf("%lld\n",instr);
  
  fwrite(&instr,sizeof(long long),1,fp2);
  */
  int index = 0;
  while(fgets(buffer,12,fp)!=NULL && fgets(buffer1,12,fp) != NULL ){
    //if( fgets(buffer1,11,fp) == NULL )
    // perr("fgets fail\n");
    convert_FS_hex_to_txt(buffer1,buffer);
    fwrite(&instr,sizeof(long long),1,fp2);
    printf("index:%d\n",++index);
  }
 

  fclose(fp);
  fclose(fp2);


}
