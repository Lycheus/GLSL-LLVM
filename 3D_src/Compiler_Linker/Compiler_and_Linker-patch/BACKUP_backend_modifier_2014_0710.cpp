#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linker.h"
using namespace std;
#define test1 
#define print_all
//#define enable_callbycaller
//#define show_decode
#define Move_OpenGLES_Start_to_beginning
#define NOP(x)     sprintf(x,"%s\tNOP 9\n",x);

char decode_line[10][50];
int set_mark = 0;
int arguments = 0;
int vsldoi = 0;
int vmrghw = 0;
int enable_record_register=0;
char type;
char fnname[128];
char tt[128];
char gg[128];

int  SaveandLoadRegisterForUserDefinedFunction=0;
//map<char *,map<int,int> > register_list; //wrong. This method record the address of string. So if the address of char * was changed, you can not access the right context.
map<string,map<int,int> > register_list; //Record the string's context.
void STVandLDVForUserDefindedFunction(int itype,FILE* fw){
  //printf("==========          %c\n",type);
  if(SaveandLoadRegisterForUserDefinedFunction){
    string temp_fnname=decode_line[1];
    temp_fnname+=':';
    int instr_count=1;
    for(int ii=0;ii<register_list[fnname].size();ii++){
      //printf("[%s][%d]=%d\n",fnname,ii,register_list[fnname][ii]);
      if(register_list[fnname][ii])
	if(register_list[fnname][ii]==register_list[temp_fnname][ii]){
	  char STVandLOAD[128];
	  if(ii==3 || ii==15 || ii==16|| ii==17||ii==18||ii==19||ii==20||ii==0) //Delete some register like temporary, zero register.
	    continue;
	  if(itype)
	    sprintf(STVandLOAD,"\tSTV R%d, -%d(R2)\n",ii,instr_count*4);
	  else
	    sprintf(STVandLOAD,"\tLDV R%d, -%d(R2)\n",ii,instr_count*4);
	  fputs(STVandLOAD,fw);
	  instr_count++;
	}
    }
    if(itype)
      fprintf(fw,"\tADDI R2, R2, 256\n");
    else
      fprintf(fw,"\tADDI R2, R2, -256\n"); 
  }
  if(!itype)//LDV parts, type ==0
    SaveandLoadRegisterForUserDefinedFunction=0;
}
void Record_register(){
  if(!strncmp(decode_line[0],"STV",3))
    ;
  else if(!strncmp(decode_line[0],"BL",2)){
    if(strncmp(decode_line[1],"OpenGLES",8))
      if(strncmp(decode_line[1],"texture2D",9))
	if(strncmp(decode_line[1],"textureCube",11))
	  SaveandLoadRegisterForUserDefinedFunction=1;
  }
  else if(!strncmp(decode_line[0],"NOP",3)){
    //printf("Detect NOP\n");
  }
  else if(decode_line[0][0]=='.'){
  }
  else{
    //Record the register number.
    int offset = 0;
    if(decode_line[1][0]=='r' || decode_line[1][0] =='R')
      offset = 1;
    int register_number = atoi((decode_line[1]+offset));

    //printf("offset:%d\n",offset);
    //printf("offset:%c\n",decode_line[1][offset]);
    //printf("++++++++++++++++++++++++register_number: %d\n",register_number);

    if(register_number <= 32) //make sure the number is less than or equal to 32.
      register_list[fnname][register_number]=1;
    offset = 0;
    //printf(" register[%s][%d]:  %d\n",fnname,register_number,register_list[fnname][register_number]);
    enable_record_register = -1; //if number of mtspr is changed, it might be wrong;
    /*
    if(!strncmp(decode_line[0],"mtspr",5)){
      for(int index=0;index<33;index++)
	printf(" register[%s][%d]:  %d\n",fnname,index,register_list[fnname][index]);
    }
    */
  }
}
void decode( char* string_buffer ){

  char temp_buffer[300];
  //printf("string_buffer:%s",string_buffer);
  strcpy(  temp_buffer , string_buffer );
  char* pch;
  int i = 0;
  pch = strtok( temp_buffer ," ,\n\t");
  //printf("pch:%s\n",pch);
  
  while( pch != NULL ){
    strcpy( decode_line[i++] , pch );
    pch = strtok(NULL," ,\n\t");
    //printf("DDDD:%s\n",decode_line[i]);
  }
  arguments = i-1;
  if( strncmp( decode_line[ i-1 ] , "set_mark" , 8 ) == 0 ||  strncmp( decode_line[ i-3 ] , "set_mark" , 8 ) == 0 ){
    set_mark = 1;
  }

  if( strncmp( decode_line[0], "vsldoi" , 6 ) == 0 )
    vsldoi = 1;
  if( strncmp( decode_line[0], "vmrghw" , 6 ) == 0  )
    vmrghw = 1;

  if(decode_line[0][0]!='.' && decode_line[1][0]=='#'&&decode_line[2][0]=='@')
    strcpy(fnname,decode_line[0]);
#ifdef enable_callbycaller
  if(enable_record_register)
    Record_register();
#endif
  //printf("total:%d\n",i);
  //printf("set_mark:%d\n",set_mark);
  //printf("vmrghw:%d\n",vmrghw);
  //printf("vsldoi:%d\n",vsldoi);
  //printf("register[]:%d\n",register_list[0]);
#ifdef show_decode
  printf("string_buffer:%s",string_buffer);
  printf("==========\n");  
  for( int j = 0; j < i ; j++){
    printf("decode_line[%d]:%s\n",j,decode_line[j]);
  
  }
  printf("==========\n");
#endif
}
void adjust_assembly( char* string_buffer , FILE* fw ){ //for set_mark

  char fixed_buffer[300];
  string_buffer[0] = ' ';
  //case: ADDI r0, r1, 15 set_mark ====> ADDI R3, R2, 15
  if( !strcmp(decode_line[0],"ADDI" ) ){ //Correct the register and attach the original instruction
    if( !strcmp(decode_line[1],"r0")&&!strcmp(decode_line[2],"r1")){
      strcpy(fixed_buffer,"\tADDI R3, R2, ");
      strcat(fixed_buffer,decode_line[3]);
      strcat(fixed_buffer,"\t\t#");
      strcat(fixed_buffer,string_buffer);
      }
    else{ //ADDI r29,r29,2 set_mark ====> ADDI r29, r29, 2
      strcpy(fixed_buffer,"\tADDI ");
      strcat(fixed_buffer,decode_line[1]);
      strcat(fixed_buffer,", ");
      strcat(fixed_buffer,decode_line[2]);
      strcat(fixed_buffer,", ");
      strcat(fixed_buffer,decode_line[3]);
      strcat(fixed_buffer,"\t\t#");
      strcat(fixed_buffer,string_buffer);
    }
  }
  else if( !strcmp(decode_line[0],"ADD" ) ){ //Correct the register and attach the original instruction
    if(strcmp(decode_line[arguments],"set_mark_2")==0 || strcmp(decode_line[arguments-2],"set_mark_2") == 0 ){

      sprintf(fixed_buffer,"\tADD %s, R%d, %s\t\t#%s",decode_line[1],3,decode_line[3],string_buffer);
      
    }
    else 
      perror("do not match the set_mark case.\n");
  }
  else if(strcmp(decode_line[0],"STV")==0 || strcmp(decode_line[0],"ST")==0){
#ifndef test1
    string_buffer[0] = '\t';
    strcpy(fixed_buffer,string_buffer);
#else
    sprintf(fixed_buffer,"\tSTV %s, %s, %s\t\t#%s",decode_line[1],decode_line[2],decode_line[3],string_buffer);
#endif
  }
  else if(strcmp(decode_line[0],"LDV")==0 || strcmp(decode_line[0],"LD")==0){
#ifndef test1
    string_buffer[0] = '\t';
    strcpy(fixed_buffer,string_buffer);
#else
    sprintf(fixed_buffer,"\tLDV %s, %s \t\t#%s",decode_line[1],decode_line[2],string_buffer);
#endif
  }
  fputs(fixed_buffer,fw );
}
void expand_vsldoi( char* string_buffer , FILE* fw ){

  char fixed_buffer[512];
  int  size = atoi(decode_line[4]);
  //printf("Size:%d\n",size);
  sprintf(fixed_buffer," ");
  string_buffer[0]=' ';

  if( size == 4 ){ 
    sprintf(fixed_buffer,"%s\tMVR R%d.1110, R%s.yzww, <4>",fixed_buffer, 3, decode_line[2]);
    sprintf(fixed_buffer,"%s\t#%s",fixed_buffer,string_buffer);
    //sprintf(fixed_buffer,"%s\tNOP 9\n",fixed_buffer);
    //if(type=='v')  //only for VS.
      NOP(fixed_buffer);
    sprintf(fixed_buffer,"%s\tMVR R%d.0001, R%s.xyzx, <4>\n",fixed_buffer, 3, decode_line[3]);
    //if(type=='v')
      NOP(fixed_buffer);
    sprintf(fixed_buffer,"%s\tMVR R%s.1111, R%d.xyzw, <4>",fixed_buffer, decode_line[1], 3);
    sprintf(fixed_buffer,"%s\t#end of %s",fixed_buffer,string_buffer);
  }
  else if( size == 8 ){
    sprintf(fixed_buffer,"%s\tMVR R%d.1100, R%s.zwzw, <4>",fixed_buffer, 3, decode_line[2]);
    sprintf(fixed_buffer,"%s\t#%s",fixed_buffer,string_buffer);
    //if(type=='v')
      NOP(fixed_buffer);
    sprintf(fixed_buffer,"%s\tMVR R%d.0011, R%s.xyxy, <4>\n",fixed_buffer, 3, decode_line[3]);
    //if(type=='v')
      NOP(fixed_buffer);
    sprintf(fixed_buffer,"%s\tMVR R%s.1111, R%d.xyzw, <4>",fixed_buffer, decode_line[1], 3);
    sprintf(fixed_buffer,"%s\t#end of %s",fixed_buffer,string_buffer);
  }
  else if( size == 12 ){
    sprintf(fixed_buffer,"%s\tMVR R%d.1000, R%s.wyzw, <4>",fixed_buffer, 3, decode_line[2]);
    sprintf(fixed_buffer,"%s\t#%s",fixed_buffer,string_buffer);
    //if(type=='v')
      NOP(fixed_buffer);
    sprintf(fixed_buffer,"%s\tMVR R%d.0111, R%s.xxyz, <4>\n",fixed_buffer, 3, decode_line[3]);
    //if(type=='v')
      NOP(fixed_buffer);
    sprintf(fixed_buffer,"%s\tMVR R%s.1111, R%d.xyzw, <4>",fixed_buffer, decode_line[1], 3);
    sprintf(fixed_buffer,"%s\t#end of %s",fixed_buffer,string_buffer);
  }
  else{
    perror("inline_vsldoi: input_sa");
  }
  //printf("%s\n",fixed_buffer);
  fputs( fixed_buffer , fw );
}
void expand_vmrghw( char* string_buffer , FILE* fw )
{
  char fixed_buffer[512];
  sprintf(fixed_buffer," ");
  string_buffer[0]=' ';
  sprintf(fixed_buffer,"%s\tMVR R%d.1010, R%s.xxyy, <2>",fixed_buffer, 3, decode_line[2]);
  sprintf(fixed_buffer,"%s\t#%s",fixed_buffer,string_buffer);
  //if(type=='v')  //only for VS.
    NOP(fixed_buffer);
  sprintf(fixed_buffer,"%s\tMVR R%d.0101, R%s.xxyy, <2>\n",fixed_buffer, 3, decode_line[3]);
  //if(type=='v')  //only for VS.
    NOP(fixed_buffer);
  sprintf(fixed_buffer,"%s\tMVR R%s.1111, R%d.xyzw, <4>",fixed_buffer, decode_line[1], 3);
  sprintf(fixed_buffer,"%s\t#end of %s",fixed_buffer,string_buffer);
  fputs( fixed_buffer , fw );
}

int main(int argc,char** argv){

  //printf("+++:file_name:%s\n",argv[1]);
  FILE *fp = fopen(argv[1],"r");
  char output_file[200];
  strcpy(output_file,"new_");
  strcat(output_file,argv[1]);
  FILE *fw = fopen(output_file,"w");
  
  type = argv[1][0];
  //printf("type:%c\n",type);
  if( fp == NULL)
    perror("file read doesn't exist\n");

  if( fw == NULL)
    perror("file written doesn't exist\n");

  char string_buffer[300];
#ifndef print_all
  int unlock = 0;
#else
  int unlock = 1;
#endif
  while ( fgets( string_buffer , 300 , fp ) ){
    decode( string_buffer );
    if( unlock ){

      if( set_mark ){
	//fputs( string_buffer, fw ); //debug message.
	adjust_assembly( string_buffer , fw );
	set_mark = 0;
      }
      else if ( vsldoi ){
	//fputs( string_buffer, fw ); //debug message.
	//fputs( string_buffer , fw );
	expand_vsldoi( string_buffer , fw );
	vsldoi = 0;
      }
      else if ( vmrghw ){
	//fputs( string_buffer, fw ); //debug message.
	expand_vmrghw( string_buffer , fw );	
	vmrghw = 0;
      }
      else{
#ifdef Move_OpenGLES_Start_to_beginning
#ifdef SET_NOP_NINE
	if( strncmp( "mtspr" , decode_line[0] , 5 ) == 0  ){
	  fputs( string_buffer , fw );
	  fputs( "\tBL OpenGLES.Start\t# size:4\n\tNOP 9\n" , fw );
#ifdef enable_callbycaller
	  enable_record_register++;
#endif
	}
	else if( strncmp( "OpenGLES.Start" , decode_line[1] , 14 ) == 0  ){
	  if( type == 'f' )
	    continue;
	  fgets( string_buffer , 100 , fp );//remove next instruction of OpenGLES.Start. it might be nop 9.
	  continue;
	}
	else 
#else
	if( strncmp( "mtspr" , decode_line[0] , 5 ) == 0  ){
	  fputs( string_buffer , fw );
	  if(!strcmp(fnname,"main:"))
	    fputs( "\tBL OpenGLES.Start\t# size:4\n" , fw );
#ifdef enable_callbycaller
	  enable_record_register++;
#endif
	}
	else if( strncmp( "OpenGLES.Start" , decode_line[1] , 14 ) == 0  ){
	  continue;
	}
	else 
#endif
#endif
	  {
	    STVandLDVForUserDefindedFunction(1,fw);
	    fputs( string_buffer , fw );
	    STVandLDVForUserDefindedFunction(0,fw);
	  }
      }

    }
    else if( strncmp( "mtspr" , decode_line[0] , 5 ) == 0  ){
#ifndef print_all
      unlock = 1;
#endif
      set_mark = 0; //initail this flag because it might be changed.
    }
  }
  fclose(fp);
  fclose(fw);
}
/********************************note**************************************
string_buffer is the original instruction from vertex.s
***************************************************************************/
/*****

remove the nop instruction in FS assembly code.














*****/
