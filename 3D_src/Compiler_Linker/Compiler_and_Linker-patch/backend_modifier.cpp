#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linker.h"
using namespace std;
#define ADD_ORIGINAL_INSTRUCTION_BEHIND_FIXED_INSTRUCTION
//#define enable_callbycaller
//#define show_decode
#define NOP(x)     sprintf(x,"%s\tNOP 9\n",x);
#define disable_NOp
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

#ifdef enable_callbycaller
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
#endif
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
    if(decode_line[1][1] == '0') //Avoid to save the data to zero register.
      return;
      
#ifndef ADD_ORIGINAL_INSTRUCTION_BEHIND_FIXED_INSTRUCTION
    string_buffer[0] = '\t';
    strcpy(fixed_buffer,string_buffer);
#else
    sprintf(fixed_buffer,"\tSTV %s, %s, %s\t\t#%s",decode_line[1],decode_line[2],decode_line[3],string_buffer);
#endif
  }
  else if(strcmp(decode_line[0],"LDV")==0 || strcmp(decode_line[0],"LD")==0){
    if(decode_line[1][1] == '0')//Avoid to load the data to zero register.
      return;
#ifndef ADD_ORIGINAL_INSTRUCTION_BEHIND_FIXED_INSTRUCTION
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
  sprintf(fixed_buffer,"");
  string_buffer[0]=' ';

  if( size == 4 ){ 
    sprintf(fixed_buffer,"%s\tMVR R%d.1110, R%s.yzww, <4>",fixed_buffer, 3, decode_line[2]);
    sprintf(fixed_buffer,"%s\t#%s",fixed_buffer,string_buffer);
    //sprintf(fixed_buffer,"%s\tNOP 9\n",fixed_buffer);
    if(type=='v')  //only for VS.
      NOP(fixed_buffer);
    sprintf(fixed_buffer,"%s\tMVR R%d.0001, R%s.xyzx, <4>\n",fixed_buffer, 3, decode_line[3]);
    if(type=='v')
      NOP(fixed_buffer);
    sprintf(fixed_buffer,"%s\tMVR R%s.1111, R%d.xyzw, <4>",fixed_buffer, decode_line[1], 3);
    sprintf(fixed_buffer,"%s\t#end of %s",fixed_buffer,string_buffer);
  }
  else if( size == 8 ){
    sprintf(fixed_buffer,"%s\tMVR R%d.1100, R%s.zwzw, <4>",fixed_buffer, 3, decode_line[2]);
    sprintf(fixed_buffer,"%s\t#%s",fixed_buffer,string_buffer);
    if(type=='v')
      NOP(fixed_buffer);
    sprintf(fixed_buffer,"%s\tMVR R%d.0011, R%s.xyxy, <4>\n",fixed_buffer, 3, decode_line[3]);
    if(type=='v')
      NOP(fixed_buffer);
    sprintf(fixed_buffer,"%s\tMVR R%s.1111, R%d.xyzw, <4>",fixed_buffer, decode_line[1], 3);
    sprintf(fixed_buffer,"%s\t#end of %s",fixed_buffer,string_buffer);
  }
  else if( size == 12 ){
    sprintf(fixed_buffer,"%s\tMVR R%d.1000, R%s.wyzw, <4>",fixed_buffer, 3, decode_line[2]);
    sprintf(fixed_buffer,"%s\t#%s",fixed_buffer,string_buffer);
    if(type=='v')
      NOP(fixed_buffer);
    sprintf(fixed_buffer,"%s\tMVR R%d.0111, R%s.xxyz, <4>\n",fixed_buffer, 3, decode_line[3]);
    if(type=='v')
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
  sprintf(fixed_buffer,"");
  string_buffer[0]=' ';
  sprintf(fixed_buffer,"%s\tMVR R%d.1010, R%s.xxyy, <2>",fixed_buffer, 3, decode_line[2]);
  sprintf(fixed_buffer,"%s\t#%s",fixed_buffer,string_buffer);
  if(type=='v')  //only for VS.
    NOP(fixed_buffer);
  sprintf(fixed_buffer,"%s\tMVR R%d.0101, R%s.xxyy, <2>\n",fixed_buffer, 3, decode_line[3]);
  if(type=='v')  //only for VS.
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
  FILE *fw1 = fopen("constant_variable_v","w");
  FILE *fw2 = fopen("constant_variable_f","w");
  
  type = argv[1][0];
  //printf("type:%c\n",type);
  if( fp == NULL)
    perror("file read doesn't exist\n");

  if( fw == NULL && fw1 == NULL && fw2 == NULL )
    perror("File written doesn't exist\n");

  char string_buffer[300];

  int unlock = 1;
  int StartandEnd =0; //0=> start,1=>end
  int reset_the_stack_register=0;
  char gpu_buffer[200];
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
	//Remove the junk code for main function.
	if( !strncmp(fnname,"main:",5) ){
	  if(StartandEnd){//end of main function
	    if(!strncmp(decode_line[1],"OpenGLES.END",12)){
	      fputs(string_buffer,fw);
	      while(fgets( string_buffer , 300 , fp ) ){
		if(string_buffer[0]!='.'){
#ifdef SET_NOP_NINE
		  if(type == 'v'){
		    if(string_buffer[1]=='R'&&string_buffer[2]=='E'&&string_buffer[3]=='T'){
		      fgets( string_buffer , 300 , fp );
		      if(!strncmp(string_buffer,"\tNOP",4))//To avoid that next instrunction is NOP. It will count the instruction again. 
			fgets( string_buffer , 300 , fp );
		      strcpy(fnname,"");//initailize the fnname to avoid to come it again. The program might not change the value of decode_line[1].
		      break;
		    }
		  }
		  else{
		    if(string_buffer[1]=='b'&&string_buffer[2]=='l'&&string_buffer[3]=='r'){
		      fgets( string_buffer , 300 , fp );
		      strcpy(fnname,"");//initailize the fnname to avoid to come it again. The program might not change the value of decode_line[1].
		      break;
		    }
		  }
#else
		  if(string_buffer[1]=='b'&&string_buffer[2]=='l'&&string_buffer[3]=='r'){
		    fgets( string_buffer , 300 , fp );
		    strcpy(fnname,"");//initailize the fnname to avoid to come it again. The program might not change the value of decode_line[1].
		    break;
		  }
#endif
		}
		else //if label is below opengl.end function.
		  fputs(string_buffer,fw);
	      }
	    } 
	  }
	  else{//beginning of main function
	    int error_flag=1;
	    fputs(string_buffer,fw);
	    while(fgets( string_buffer , 300 , fp ) ){
	      if(string_buffer[0]!='\t') //only print the string which starts without '\t'. e.g: .Ltmp2
		fputs(string_buffer,fw);
	      else if(string_buffer[1]=='m'&&string_buffer[2]=='t'){
		fputs("\tBL OpenGLES.Start\t\t# size:4\n",fw); //This function should be the first one. In order to avoid some instruction using R2 uninitialized.
		while(fgets( string_buffer , 300 , fp ) ){
		  if(!strncmp(string_buffer,"\tBL OpenGLES.Start",18)){
		    error_flag =0;
		    break;
		  }
		  else{
		    fputs(string_buffer,fw);
		  }
		}
		if(error_flag){
		  perror("This message come out!!! If there are user defined function in this shader code , it might be wrong!!!!!!!!!!!!!!\n");
		  exit(1);
		}
		fgets( string_buffer , 300 , fp ) ; //Add this line to save the next line.
		StartandEnd =1;
		break;
	      }
	    }
	  }
	}
	else {
	  //Delete this junk code for user defined function.	  
	  if( !strncmp(decode_line[0],"mflr",4)) 
	    continue;
	  else if( !strncmp(decode_line[0],"mtspr",5) ){
	    if(reset_the_stack_register==0){
	      fprintf(fw,"\tADDI R2, R2, 256\t\t#Add this instruction from backend_fixer.\n",string_buffer);
	      reset_the_stack_register=2;
	    }
	    continue;
	  }
	  else if( !strncmp(decode_line[0],"mfspr",5) )
	    continue;
	  else if( !strncmp(decode_line[0],"mtlr",4) )
	    continue;
	  else if( !strncmp(decode_line[0],"ST",2) && decode_line[0][2]!='V'){ //handle stwu instructions.
	    //printf("here i stand???? %s\n",decode_line[0]);
	    string_buffer[0]=' ';
	    int remove_bslash=strlen(string_buffer)-1;
	    string_buffer[remove_bslash]=' ';
	    strcat(string_buffer,"set_mark_4");
	    fprintf(fw,"\tSTV r2, 256(1), <1>\t\t#%s\n", string_buffer); //replace ST with STV.
	    fgets( string_buffer , 300 , fp);
	    decode(string_buffer);
	    string_buffer[0]=' ';
	    remove_bslash=strlen(string_buffer)-1;
	    string_buffer[remove_bslash]=' ';
	    fprintf(fw,"\tADDI R2, R2, 256\t\t#%s\n",string_buffer);
	    reset_the_stack_register=1;	    
	    continue;
	  }
	  else if( !strncmp(decode_line[0],"ADDI",4) ){
	    if(!strcmp(decode_line[1],decode_line[2])){
	      //fprintf(fw,"\tADDI R2, R2, %s\t\t#%s",decode_line[3],string_buffer);
	      fprintf(fw,"\tADDI R2, R2, -256\t\t#%s",string_buffer);
	    }
	    else{
	      fputs(string_buffer,fw); //To avoid to delete some addi instructions. e.g. ADDI r4, r1, 32.
	    }
	    continue;
	  }
	  else if( !strncmp(decode_line[0],"blr",3) ){
	    if(reset_the_stack_register==2){
	      fprintf(fw,"\tADDI R2, R2, -256\t\t#Add this instruction from backend_fixer\n",string_buffer);
	    }
	    fputs("\tBL OpenGLES.BackToMainFunction\n",fw);
	    continue;
	  }
	  else if( !strncmp(decode_line[0],"gpu_bnuf",8) ){
	    //fprintf(fw,"\tgpu_blef %s, %s, %s, %s\t\t#Modify this instruction from backend_fixer.Original instruction is gpu_bnuf\n",decode_line[1],decode_line[2],decode_line[3],decode_line[4]);
	    fprintf(fw,"\tgpu_%s %s, %s, %s, %s\n",gpu_buffer,decode_line[1],decode_line[2],decode_line[3],decode_line[4]);
	    continue;
	  }
	  else if( !strncmp(decode_line[0],"gpu_",4) ){//Record the behavior of branch. e.g. gpu_beq,gpu_bne
	    strcpy(gpu_buffer,decode_line[0]+4);
	  }
	}
	
	//*********************************without dealing user-defined functions.
		
	if(!strncmp(decode_line[0],".LCPI",5)){
	  if(type=='v')
	    fputs(string_buffer,fw1);
	  else
	    fputs(string_buffer,fw2);
	  while(fgets( string_buffer , 300 , fp ) ){
	    if(type=='v')
	      fputs(string_buffer,fw1);
	    else
	      fputs(string_buffer,fw2);
	    if(!strncmp(string_buffer,"\t.type",6))
	      break;
	  }
	  continue; //Avoid to save the  ".type XXXXX" again.
	}
	
	if( !strncmp(decode_line[0],"gpu_bnuf",8) ){//Dealing this string in main function.
	  //fprintf(fw,"\tgpu_blef %s, %s, %s, %s\t\t#Modify this instruction from backend_fixer.Original instruction is gpu_bnuf\n",decode_line[1],decode_line[2],decode_line[3],decode_line[4]);
	  fprintf(fw,"\tgpu_%s %s, %s, %s, %s\n",gpu_buffer,decode_line[1],decode_line[2],decode_line[3],decode_line[4]);
	  continue;
	}
	if( !strncmp(decode_line[0],"gpu_",4) ){//Record the behavior of branch. e.g. gpu_beq,gpu_bne	
	  //printf("hi see it\n");
	  strcpy(gpu_buffer,decode_line[0]+4);
	}


	fputs(string_buffer,fw);
      }//end of else

    }//end of if(unlock)
  
  }

  fclose(fp);
  fclose(fw);
  fclose(fw1);
  fclose(fw2);
  

  FILE *new_fw = fopen(output_file,"r");
  FILE *new_fw1;
  FILE *new_fw2 = fopen("test123","w+");
  if(type=='v')
    new_fw1 = fopen("constant_variable_v","r");
  else
    new_fw1 = fopen("constant_variable_f","r");

  if(new_fw1 == NULL && !new_fw2 )
    printf("File does not exist\n");
  
  fgets(string_buffer,300,new_fw);
  fputs(string_buffer,new_fw2);
  fgets(string_buffer,300,new_fw);
  fputs(string_buffer,new_fw2);
  while(fgets(string_buffer,300,new_fw1)){
    fputs(string_buffer,new_fw2);
  }
  while(fgets(string_buffer,300,new_fw)){
    fputs(string_buffer,new_fw2);
  }
  fclose(new_fw);
  fclose(new_fw1);
  fclose(new_fw2);

  sprintf(string_buffer,"cp test123 %s",output_file);
  system(string_buffer);
  //system("cp new_fragment.s 1234");


  
}
/********************************note**************************************
string_buffer is the original instruction from vertex.s
***************************************************************************/
/*****

remove the nop instruction in FS assembly code.














*****/
