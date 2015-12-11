#include <stdio.h>
#include <string.h>
#include "inlineFunction.h"
//#include "sct_table.h"
#include "link_var.h"
#include "linker.h"
#include "Sub3y1_LLVM.h"
#include "GLCompiler.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int fragment_count=0;
int vertex_count=0;

#ifdef COMPILER_STANDALONE
struct context mycontext;
#else
extern struct context mycontext;
#endif
struct context *myctx=&mycontext;
#define test_attribute
#define test_viewport
//int Compile_OpenGL(FILE* fp,char *filename);

//void load_IR(FILE *IR_file,FILE *out,int vertexOrFrag,struct programobject *input_object,struct varying_form *varying_table);

void mygllinker(struct programobject *input_object,int fd);

void init_varying(struct varying_form *varying_table)
{
  int i;
  for(i=0;i<max_number_of_varying_variables;i++)
    {
      varying_table->index[i]=UNDEFINED;
      strcpy(varying_table->vary_name[i],"");
    }
}

extern "C" void glCompileShader(GLuint shader)
{
#ifndef API_DIR
  perror("API_DIR macro needs to be set.");
#endif
#ifndef COMPILER_DIR
  perror("COMPILER_DIR macro needs to be set.");
#endif
  /*
    Compile the OpenGL shading language
   */
  myglCompileShader(&myctx->shader[shader]);
}

extern "C" void glLinkProgram(GLuint program)
{
  int fd;
#ifdef COMPILER_STANDALONE
  fd = 1; //Example: int fd=open("/dev/himax_3d",O_RDWR| O_CREAT, S_IRUSR | S_IWUSR);
#else
  fd = myctx->fd;
#endif
  if( myctx->shader[program].CompileStatus != false )
    mygllinker(&myctx->program[program],fd);
}

void mygllinker(struct programobject *input_object,int fd)
{
  FILE *input,*output;
  struct varying_form varying_table;
  char filename[32];
  char **filename_list;
  int file_count;
  init_varying(&varying_table);

  //vertex shader
  //input=fopen("vertex.s","r");
  //output=fopen("final_bin_vertex","w");
  /*
    Use the link list to find out the number of the file.
   */
  //We should add some code.

  file_count=1;
  filename_list=(char**)malloc(file_count * sizeof(char*));
  //If you want to test the SCT, you can modify this filename
  //Default filename: vertex.s
  strcpy(filename,"vertex.s");
  filename_list[0]=filename;
  printf("vertex link\n");
  linker(1,filename_list,&varying_table,'v',(char *)"final_bin_vertex",input_object,fd);


  //fragment shader
  //input=fopen("fragment.s","r");
  //output=fopen("final_bin_fragment","w");
  /*
    Use the link list to find out the number of the file.
   */
  //We should add some code.
  
  file_count=1;
  //filename_list=(char**)malloc(file_count * sizeof(char*));


  //If you want to test the SCT, you can modify this filename
  //Default filename: fragment.s
  strcpy(filename,"fragment.s");
  filename_list[0]=filename;
  printf("fragment link\n");
  linker(1,filename_list,&varying_table,'f',(char *)"final_bin_fragment",input_object,fd);
  
#ifdef INFO_FOR_API

  for(int i=0;i<256;i++)
    {
      if((i>=input_object->vertex_uniform_number && i<128)||i>=(input_object->fragment_uniform_number+128))
	{
	  if(i%5!=4)
	    printf("%d: %s\t",i,"(null)");
	  else
	    printf("%d: %s\n",i,"(null)");
	  continue;
	}
      if(i%5!=4)
  	printf("%d: %s\t",i,input_object->uniform[i].name);
      else
  	printf("%d: %s\n",i,input_object->uniform[i].name);
    }
  
  printf("\nvertex shader length: %d\n",input_object->shaders[0]->shader_length);
  printf("fragment shader length: %d\n",input_object->shaders[1]->shader_length);
  printf("vertex uniform number: %d\n",input_object->vertex_uniform_number);
  printf("fragment uniform number: %d\n",input_object->fragment_uniform_number);

#endif
  
  //load_IR(input,output,1,input_object,varying_table);  
  
}


#define streq(A,B) (strcmp(A,B)==0)
//This is an example. When integrating, this function can be marked.
#ifdef COMPILER_STANDALONE

char *file2string(const char *path)
{
  FILE *fd;
  long len,r;
  char *str;
  
  if (!(fd = fopen(path, "r")))
    {
      fprintf(stderr, "Can't open file '%s' for reading\n", path);
      return NULL;
    }
  
  fseek(fd, 0, SEEK_END);
  len = ftell(fd);
  
  printf("File '%s' is %ld long\n", path, len);
  
  fseek(fd, 0, SEEK_SET);
  
  if (!(str = (char*)malloc(len * sizeof(char)+1)))
    {
      fprintf(stderr, "Can't malloc space for '%s'\n", path);
      return NULL;
    }
  
  r = fread(str, sizeof(char), len, fd);
  
  str[r] = '\0'; /* Shader sources have to term with null */
  
  fclose(fd);
  
  return str;
}

int main(int argc, char **argv){

  //int fd=open("/dev/himax_3d",O_RDWR| O_CREAT, S_IRUSR | S_IWUSR);

  //char type_v='v',type_f='f';
  //struct shaderobject shader_vert;
  //struct shaderobject shader_frag;
  //struct programobject object;

  /*
  system("rm -f runafragment");
  if ((argc==2)&&(streq(argv[2],"-f")||streq(argv[2],"-frag")||streq(argv[2],"-fragment")))
  { FILE *F=fopen("runafragment","w");
    fprintf(F,"fragment shader compilation requested");
    fclose(F);
  }
  */
  //USER DEFINE
  char *vsSource = file2string("red_vertex");
  char *fsSource = file2string("red_fragment");

  //shader_vert.pointer=fopen("red_vertex","r");
  //shader_frag.pointer=fopen("red_fragment","r");

#ifdef RED_CUBE
  //vsSource = file2string("red_vertex");
  //fsSource = file2string("red_fragment");
#ifdef COMPILER_STANDALONE   //For compiler_standalone
  myctx->program[0].attribute_name[0] = "rm_Vertex";
  myctx->program[0].attribute_name[1] = "rm_Normal";
  myctx->program[0].attribute_name[2] = "texcoord";
#endif
#endif
#ifdef TEXTURE_CUBE
  free(vsSource);
  free(fsSource);
  vsSource = file2string("texture_vertex");
  fsSource = file2string("texture_fragment");
#ifdef COMPILER_STANDALONE   //For compiler_standalone
  myctx->program[0].attribute_name[0] = "rm_Vertex";
  myctx->program[0].attribute_name[1] = "rm_Normal";
  myctx->program[0].attribute_name[2] = "texcoord";
#endif
#endif
#ifdef LIGHTING_CUBE
  free(vsSource);
  free(fsSource);
  vsSource = file2string("lighting_vertex");
  fsSource = file2string("lighting_fragment");
#ifdef COMPILER_STANDALONE   //For compiler_standalone
  myctx->program[0].attribute_name[0] = "rm_Vertex";
  myctx->program[0].attribute_name[1] = "rm_Normal";
  myctx->program[0].attribute_name[2] = "texcoord";
#endif
#endif
#ifdef LIGHTING_CUBE2
  free(vsSource);
  free(fsSource);
  vsSource = file2string("lighting_vertex2");
  fsSource = file2string("lighting_fragment");
#ifdef COMPILER_STANDALONE   //For compiler_standalone
  myctx->program[0].attribute_name[0] = "rm_Vertex";
  myctx->program[0].attribute_name[1] = "rm_Normal";
  myctx->program[0].attribute_name[2] = "texcoord";
#endif
#endif
#ifdef NEW_LIGHTING_CUBE
  free(vsSource);
  free(fsSource);
  vsSource = file2string("new_lighting_vertex");
  fsSource = file2string("lighting_fragment");
#ifdef COMPILER_STANDALONE   //For compiler_standalone
  myctx->program[0].attribute_name[0] = "rm_Vertex";
  myctx->program[0].attribute_name[1] = "rm_Normal";
  myctx->program[0].attribute_name[2] = "texcoord";
#endif
#endif
#ifdef MORPHING_BALL_IN_VS
  free(vsSource);
  free(fsSource);
  vsSource = file2string("morphing_ball_VS_vertex");
  fsSource = file2string("morphing_ball_VS_fragment");
#ifdef COMPILER_STANDALONE   //For compiler_standalone
  myctx->program[0].attribute_name[0] = "rm_Vertex";
  myctx->program[0].attribute_name[1] = "rm_Normal";
  myctx->program[0].attribute_name[2] = "texcoord";
#endif
#endif
#ifdef OCEAN_SIMULATION
  free(vsSource);
  free(fsSource);
  vsSource = file2string("flatten.vert");
  fsSource = file2string("flatten.frag");
#ifdef COMPILER_STANDALONE   //For compiler_standalone
  myctx->program[0].attribute_name[0] = "wave_Vertex";
  myctx->program[0].attribute_name[1] = "wave_Normal";
  myctx->program[0].attribute_name[2] = "wave_Tex";
#endif
#endif
#ifdef DEFAULT_TESTBENCH
  free(vsSource);
  free(fsSource);
  vsSource = file2string("default.vert");
  fsSource = file2string("default.frag");
  //myctx->program[0].attribute_name[0] = "gtf_Color"; //For conformance benchmark
  //myctx->program[0].attribute_name[1] = "gtf_Vertex";//For conformance benchmark
#endif

#ifdef COMPILER_STANDALONE   //For compiler_standalone
  //myctx->program[0].width = 256;
  //myctx->program[0].height = 256;
  myctx->program[0].width = 320;
  myctx->program[0].height = 240;
#ifdef OCEAN_SIMULATION
  myctx->program[0].width = 320;//for ocean
  myctx->program[0].height = 320;//for ocean
#endif
#ifdef DEFAULT_TESTBENCH
  myctx->program[0].width = 256;//for ocean
  myctx->program[0].height = 256;//for ocean
#endif
  myctx->program[0].x = 0;
  myctx->program[0].y = 0;
#endif

  myctx->shader[0].type=(char*)"vertex_shader";
  myctx->shader[1].type=(char*)"fragment_shader";

  myctx->shader[0].pointer=vsSource;
  printf("Vertex compile\n");
  glCompileShader((GLuint)0);

  myctx->shader[1].pointer=fsSource;
  printf("Fragment compile\n");
  glCompileShader((GLuint)1);

  myctx->program[0].shaders[0]=&myctx->shader[0];
  myctx->program[0].shaders[1]=&myctx->shader[1];

  //GL_linker(&object,fd);
  glLinkProgram((GLuint)0);
  //system("rm -f runafragment");
}
#endif
