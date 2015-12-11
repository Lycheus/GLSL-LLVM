#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include "sct_table.h"
#ifdef COMPILER_STANDALONE
#include "APIContext.h"
#else
#include "../context.h"
#endif
#include "linker.h"

//#define USE_FILE 1
#define INTEGRATE 1
#define ATTILA

//#define perr(X) {printf("%s error!!\n",X);input_shader->CompileStatus=GL_FALSE;return;}
#define perr(X) {printf("%s error!!\n",X);input_shader->CompileStatus=GL_FALSE;exit(1);}
/*
void perr(char const* msg ){
  FILE *p = fopen("conformance_error","a");
  //printf("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDddd\n");
  fprintf(p,"%s is failed\n",msg);
  //exit(1);
}
*/
static char temp_string[200];


void myglCompileShader(struct shaderobject *input_shader) // Compile the OpenGL shading language
{

#if USE_FILE
  char temp_shader_code[120]; //assume that the file format is 120 per row.
  FILE *writeToShader;
  //FILE *writeToShader1;
  
  //ShaderObject X = findShader(ShaderObj);
  //sprintf (Prep,"cp StandAlone/shader_source_%d minimal.", input_shader->count_file); 
  if (input_shader->type[0]=='v'){
    bzero(temp_shader_code,sizeof(temp_shader_code));

    sprintf(temp_string,"%s/StandAlone/minimal.vert",COMPILER_DIR);
    writeToShader = fopen(temp_string,"w");

    //writeToShader = fopen("Compiler_and_Linker-patch/StandAlone/minimal.vert","w");
    //writeToShader1 = fopen("StandAlone/minimal.vert","w");
    while(fgets(temp_shader_code,sizeof(temp_shader_code),(FILE *)input_shader->pointer)!=NULL){
      fputs(temp_shader_code,writeToShader);
      //fputs(temp_shader_code,writeToShader1);
      //fwrite(temp_shader_code,1,sizeof(temp_shader_code),writeToShader);
      bzero(temp_shader_code,sizeof(temp_shader_code));
    }
  }else{
    sprintf(temp_string,"%s/StandAlone/minimal.frag",COMPILER_DIR);
    writeToShader = fopen(temp_string,"w");
    //writeToShader = fopen("Compiler_and_Linker-patch/StandAlone/minimal.frag","w");
    bzero(temp_shader_code,sizeof(temp_shader_code));
    while(fgets(temp_shader_code,sizeof(temp_shader_code),(FILE *)input_shader->pointer)!=NULL){
      fputs(temp_shader_code,writeToShader);
      //fwrite(temp_shader_code,1,sizeof(temp_shader_code),writeToShader);
      bzero(temp_shader_code,sizeof(temp_shader_code));
    }
  }

  fclose(writeToShader);
#else
  FILE *p_write;
  if (input_shader->type[0]=='v'){
    sprintf(temp_string,"%s/StandAlone/minimal.vert",COMPILER_DIR);
    p_write=fopen(temp_string,"w");
    //p_write=fopen("Compiler_and_Linker-patch/StandAlone/minimal.vert","w");
    fwrite((char *)input_shader->pointer,strlen(input_shader->pointer),1,p_write);
  }else{
    sprintf(temp_string,"%s/StandAlone/minimal.frag",COMPILER_DIR);
    p_write=fopen(temp_string,"w");
    //p_write=fopen("Compiler_and_Linker-patch/StandAlone/minimal.frag","w");
    fwrite((char *)input_shader->pointer,strlen(input_shader->pointer),1,p_write);
  }
  fclose(p_write);
    
#endif

  //Use File string.
  char* filename = (char*)malloc(32);
  char s3y1_preprocessor[569]; // 569 = 12 + 255 + 47 + 255
  char execute_s3y1[326];// = (char*)malloc(292); // 326 = 20 + 255 + 51
  //End of file string.
  int system_return_value=0;

  if(input_shader->type[0]=='v')      // [vert]
    {
      //system("./glCompileVertexShader"); //Reorder the order of tools.
      //sprintf(temp_string,"%s/StandAlone/fixer %s/StandAlone/pre_minimal.vert",COMPILER_DIR,COMPILER_DIR);
      //if(system("./Compiler_and_Linker-patch/StandAlone/fixer ./Compiler_and_Linker-patch/StandAlone/minimal.vert ")<0)//Kaymon
      //if(system(temp_string)!=0)//Kaymon
	//perr("fixer");
      bzero(&execute_s3y1,sizeof(execute_s3y1));
      strcpy(filename, "minimal.vert");
      sprintf(temp_string,"/opt/llvm-obj/Release+Debug/bin/clang -E -x c -D'GL_ES' %s/StandAlone/",COMPILER_DIR);
      strcpy(s3y1_preprocessor, temp_string); // 51
      //strcpy(s3y1_preprocessor, "/opt/llvm-obj/Release/bin/clang -E -x c -D'GL_ES' ./Compiler_and_Linker-patch/StandAlone/"); // 51
      strcat(s3y1_preprocessor, filename); // 255
      sprintf(temp_string," -o %s/StandAlone/pre_",COMPILER_DIR);
      strcat(s3y1_preprocessor, temp_string); // 47
      //strcat(s3y1_preprocessor, " -o ./Compiler_and_Linker-patch/StandAlone/pre_"); // 47
      strcat(s3y1_preprocessor, filename); // 25
      //sprintf(s3y1_preprocessor,"cp Compiler_and_Linker-patch/StandAlone/%s Compiler_and_Linker-patch/StandAlone/pre_%s",filename,filename);
      printf("%s\n",s3y1_preprocessor);
      if(system(s3y1_preprocessor)!=0) // Preprecessor: c preprocessor on GLSL
	perr("vertex: cp pre_vertex");

      sprintf(temp_string,"%s/StandAlone/fixer %s/StandAlone/pre_minimal.vert",COMPILER_DIR,COMPILER_DIR);
      //if(system("./Compiler_and_Linker-patch/StandAlone/fixer ./Compiler_and_Linker-patch/StandAlone/minimal.vert ")<0)//Kaymon
      printf("%s\n",temp_string);
      if(system(temp_string)!=0)//Kaymon
	perr("fixer");
      bzero(&execute_s3y1,sizeof(execute_s3y1));

      sprintf(temp_string,"%s/BuildLinux.sh %s pre_",API_DIR,API_DIR);
      strcpy(execute_s3y1, temp_string); // 20   
      //strcpy(execute_s3y1, "./BuildLinux.sh pre_"); // 20   
      strcat(execute_s3y1, filename); // 255
      strcat(execute_s3y1, " > vertex.as"); // 12 , vertex after subgroup 3 year 1
      printf("%s\n",execute_s3y1);
      if(system(execute_s3y1)!=0) // Frontend: execute Subgroup 3 year 1
	perr("vertex: BuildLinux.sh"); //Always return the same returning value.
      //system("./Compiler_and_Linker-patch/format vertex.as > vertex.ll"); // first time edit vertex file and fragment file
      //printf("%s\n","./Compiler_and_Linker-patch/format vertex.as > pre_vertex.ll");
      printf("%s/format vertex.as > pre_vertex.ll\n",COMPILER_DIR);
      sprintf(temp_string,"%s/format vertex.as > pre_vertex.ll",COMPILER_DIR);
      //if(system("./Compiler_and_Linker-patch/format vertex.as > pre_vertex.ll")<0) // first time edit vertex file and fragment file
      if(system(temp_string)!=0) // first time edit vertex file and fragment file
	perr("vertex: format");
      //printf("%s\n","./Compiler_and_Linker-patch/add_metadata pre_vertex.ll vertex.ll");
      printf("%s/add_metadata pre_vertex.ll vertex.ll\n",COMPILER_DIR);
      sprintf(temp_string,"%s/add_metadata pre_vertex.ll vertex.ll",COMPILER_DIR);
      //if(system("./Compiler_and_Linker-patch/add_metadata pre_vertex.ll vertex.ll")<0)
      if(system(temp_string)!=0)
	perr("vertex: add_metadata");
      printf("%s\n","/opt/llvm-obj/Release+Debug/bin/llvm-as vertex.ll -f -o vertex.bc");
      if(system("/opt/llvm-obj/Release+Debug/bin/llvm-as vertex.ll -f -o vertex.bc") != 0) // Backend: llvm-as
	  perr("vertex: llvm-as");
      //system("./Compiler_and_Linker-patch/llvm-obj/Release/bin/llvm-as lighting_vertex.ll -f -o vertex.bc"); // Backend: llvmas
      //system("./Compiler_and_Linker-patch/llvm-obj/Release/bin/llvm-as morphing_ball_VS_vertex.ll -f -o vertex.bc"); // Backend: llvmas
#ifdef SET_NOP_NINE
      printf("%s\n","/opt/llvm-obj/Release+Debug/bin/llc vertex.bc -march=newppc32 -mattr=altivec --use-gpu-branches --use-gpu-stack-address --use-new-matrix-address -o pre_vertex.s"); // llc
      if(system("/opt/llvm-obj/Release+Debug/bin/llc vertex.bc -march=newppc32 -mattr=altivec --use-gpu-branches --use-gpu-stack-address --use-new-matrix-address -o pre_vertex.s")!=0) // llc
	perr("vertex: llc");

      sprintf(temp_string,"%s/peep < pre_vertex.s > pre_vertex2.s",COMPILER_DIR);
      //if(system("./Compiler_and_Linker-patch/peep < pre_vertex.s > pre_vertex2.s")<0)
      if(system(temp_string)!=0)
	;//perr("vertex: peep"); //Always return the same returning value.

      sprintf(temp_string,"%s/FixTheNOPs pre_vertex2.s vert",COMPILER_DIR);
      //if(system("./Compiler_and_Linker-patch/FixTheNOPs pre_vertex2.s vert")<0)
      if(system(temp_string)!=0)
	perr("vertex: FixTheNOPs");

      if(system("mv newpre_vertex2.s vertex.s")!=0)
	perr("vertex: mv");


#else
      printf("%s\n","/opt/llvm-obj/Release+Debug/bin/llc vertex.bc -march=newppc32 -mattr=altivec --use-gpu-branches --use-gpu-stack-address --use-new-matrix-address -o pre_vertex.s"); // llc
      //printf("/opt/llvm-obj/Release/bin/llc vertex.bc -march=newppc32 -mattr=altivec --use-gpu-branches -o vertex.s"); // llc
      //sheng-chang add if expression
      if((system("/opt/llvm-obj/Release+Debug/bin/llc vertex.bc -march=newppc32 -mattr=altivec --use-gpu-branches --use-gpu-stack-address  --use-new-matrix-address -o vertex.s"))!=0) // llc
      	perr("vertex: llc");
	//system("./Compiler_and_Linker-patch/llvm-obj/Release/bin/llc vertex.bc -march=newppc32 -mattr=altivec -o vertex.s"); // llc
#endif
      //End of glCompileVertexShader.
      //sprintf (Prep,"vertex.s StandAlone/shader_assembly_%d",X.count_file);
#ifdef ATTILA
      sprintf(temp_string,"cp vertex.s orig_vertex.s");
      system(temp_string);
      sprintf(temp_string,"%s/backend_modifier vertex.s",API_DIR);
      //system("./backend_modifier");
      system(temp_string);
      system("mv new_vertex.s vertex.s");
#endif
      //system("cp Compiler_and_Linker-patch/vertex.s Compiler_and_Linker-patch/StandAlone/shader_assembly_vertex_1");
      sprintf(temp_string,"cp vertex.s %s/StandAlone/shader_assembly_vertex_1",COMPILER_DIR);
      //if(system("cp vertex.s Compiler_and_Linker-patch/StandAlone/shader_assembly_vertex_1")<0)
      if(system(temp_string)!=0)
	perr("vertex: cp vertex.s");
    }
  else if(input_shader->type[0]=='f')      // [frag]
    {
      //system("./glCompileFragmentShader");
      printf("%s/StandAlone/fixer %s/StandAlone/minimal.frag\n",COMPILER_DIR,COMPILER_DIR);
      sprintf(temp_string,"%s/StandAlone/fixer %s/StandAlone/minimal.frag",COMPILER_DIR,COMPILER_DIR);
      //if(system("./Compiler_and_Linker-patch/StandAlone/fixer ./Compiler_and_Linker-patch/StandAlone/minimal.frag ")<0)//Kaymon
      if(system(temp_string)!=0)
	perr("fixer");
      strcpy(filename, "minimal.frag");
      sprintf(temp_string,"/opt/llvm-obj/Release+Debug/bin/clang -E -x c -D'GL_ES' %s/StandAlone/",COMPILER_DIR);
      //strcpy(s3y1_preprocessor, "/opt/llvm-obj/Release/bin/clang -E -x c -D'GL_ES' ./Compiler_and_Linker-patch/StandAlone/"); // 51
      strcpy(s3y1_preprocessor, temp_string); // 51
      strcat(s3y1_preprocessor, filename); // 255
      sprintf(temp_string," -o %s/StandAlone/pre_",COMPILER_DIR);
      //strcat(s3y1_preprocessor, " -o ./Compiler_and_Linker-patch/StandAlone/pre_"); // 47
      strcat(s3y1_preprocessor, temp_string); // 47
      strcat(s3y1_preprocessor, filename); // 255
      //sprintf(s3y1_preprocessor,"cp Compiler_and_Linker-patch/StandAlone/%s Compiler_and_Linker-patch/StandAlone/pre_%s",filename,filename);
      //printf("%s\n",s3y1_preprocessor);
      if(system(s3y1_preprocessor)!=0) // Preprecessor: c preprocessor on GLSL
	perr("fragment: cp pre_fragment");
      sprintf(temp_string,"%s/BuildLinux.sh %s pre_",API_DIR,API_DIR);
      //strcpy(execute_s3y1, "./BuildLinux.sh pre_"); // 20
      strcpy(execute_s3y1, temp_string); // 20
      strcat(execute_s3y1, filename); // 255
      strcat(execute_s3y1, " > fragment.as"); // 12 , fragment after subgroup 3 year 1
      //printf("%s\n",execute_s3y1);
      if(system(execute_s3y1)!=0) // Frontend: execute Subgroup 3 year 1
	;//perr("fragment: BuildLinux.sh"); //Always return the same returning value.
      //system("./Compiler_and_Linker-patch/format fragment.as > fragment.ll"); // first time edit fragment file and fragment file
      //printf("%s\n","./Compiler_and_Linker-patch/format fragment.as > pre_fragment.ll"); 
      sprintf(temp_string,"%s/format fragment.as > pre_fragment.ll",COMPILER_DIR);
      //if(system("./Compiler_and_Linker-patch/format fragment.as > pre_fragment.ll")<0) // first time edit fragment file and fragment file
      if(system(temp_string)!=0)
	perr("fragment: format");
      //printf("%s\n","./Compiler_and_Linker-patch/add_metadata pre_fragment.ll fragment.ll");
      sprintf(temp_string,"%s/add_metadata pre_fragment.ll fragment.ll",COMPILER_DIR);
      //if(system("./Compiler_and_Linker-patch/add_metadata pre_fragment.ll fragment.ll")<0)
      if(system(temp_string)!=0)
	perr("fragment: add_metadata");
      //printf("%s\n","/opt/llvm-obj/Release/bin/llvm-as fragment.ll -f -o fragment.bc");
      if(system("/opt/llvm-obj/Release+Debug/bin/llvm-as fragment.ll -f -o fragment.bc")!=0) // Backend: llvmas
	perr("fragment: llvm-as");
      //system("./Compiler_and_Linker-patch/llvm-obj/Release/bin/llvm-as morphing_ball_VS_fragment.ll -f -o fragment.bc"); // Backend: llvmas
#ifdef SET_NOP_NINE

      if(system("/opt/llvm-obj/Release+Debug/bin/llc fragment.bc -march=newppc32 -mattr=altivec --use-gpu-branches --use-gpu-stack-address --use-new-matrix-address -o pre_fragment.s")!=0) // llc
	perr("fragment: llc");

      //if(system("./peep < pre_fragment.s > pre_fragment2.s")<0)
      //perr("fragment: peep");

      sprintf(temp_string,"%s/FixTheNOPs pre_fragment.s frag",COMPILER_DIR);
      //if(system("./Compiler_and_Linker-patch/FixTheNOPs pre_fragment.s frag")<0)
      if(system(temp_string)!=0)
	perr("fragment: FixTheNOPs");

      if(system("mv newpre_fragment.s fragment.s")!=0)
	perr("vertex: mv");

#else
      //printf("%s\n","/opt/llvm-obj/Release/bin/llc fragment.bc -march=newppc32 -mattr=altivec --use-gpu-branches -o fragment.s");
      //sheng-chang add if expression
      if((system("/opt/llvm-obj/Release+Debug/bin/llc fragment.bc -march=newppc32 -mattr=altivec --use-gpu-branches --use-gpu-stack-address --use-new-matrix-address -o fragment.s"))!=0) // llc
	perr("fragment: llc");
      //system("./Compiler_and_Linker-patch/llvm-obj/Release/bin/llc fragment.bc -march=newppc32 -mattr=altivec -o fragment.s"); // llc
#endif
      //End of glCompileFragmentShader.

#ifdef ATTILA
      sprintf(temp_string,"cp fragment.s orig_fragment.s");
      system(temp_string);
      sprintf(temp_string,"cat orig_fragment.s| sed \"\/NOP\/d\" > fragment.s");//remove nop 9.
      system(temp_string);
      sprintf(temp_string,"%s/backend_modifier fragment.s",API_DIR);
      //system("./backend_modifier");
      system(temp_string);
      system("mv new_fragment.s fragment.s");
#endif
      //system("cp Compiler_and_Linker-patch/fragment.s Compiler_and_Linker-patch/StandAlone/shader_assembly_fragment_1");
      sprintf(temp_string,"cp fragment.s %s/StandAlone/shader_assembly_fragment_1",COMPILER_DIR);
      //if(system("cp fragment.s Compiler_and_Linker-patch/StandAlone/shader_assembly_fragment_1")<0)
      if(system(temp_string)!=0)
	perr("fragment: cp fragment.s");
    }
  else{
    printf("No vertex/fragment Shaders\n");
    input_shader->CompileStatus = GL_FALSE;
    exit(1);
  }

  input_shader->CompileStatus = GL_TRUE;

}

#ifndef INTEGRATE
int main(void){
  struct shaderobject shader;
  p_shader=&shader;
  char shader_type[32];
#if USE_FILE
  FILE *pfile=fopen("shader_test_vertex","r");
  strcpy(shader_type,"vertex");
  shader.pointer=pfile;
  fclose(pfile);
#else
  char *filename
  strcpy(filename,"minimal.vert");
  shader.pointer=(FILE *)filename;
#endif
  shader.type=shader_type;
  myglCompileShader(p_shader);
  return 0;
}
#endif
