#ifndef SCT_TABLE_H
#define SCT_TABLE_H

typedef unsigned char GLboolean;
typedef int GLint;
typedef void GLvoid;
typedef int GLfixed;
typedef GLfixed vec4[4];

typedef struct uniformobject{
  int programnumber;
  char *name;
  GLint location;
  GLint sizei;
  const GLvoid **data_ptr;
  GLint doublename;
} UniformObject;
/*
struct uniformobject{
  unsigned int count;
  int programnumber[256];

  char *name[256];

  GLint sizei[256];
  GLint location[256];

  const GLvoid **data_ptr[256];
  GLint doublename[256];

};
*/

typedef struct shaderobject{
  char *type;
  int count;
  int count_file;
  const char *pointer;
  unsigned int shader_length;
  struct shaderobjects *next;
} ShaderObjectList;

typedef struct programobject{
  unsigned int programobject_count;

  ShaderObjectList *shaders[2];

  UniformObject uniform[256];

  unsigned int vertex_uniform_number;
  unsigned int fragment_uniform_number;

  vec4 attribute_data[8];
  char *attribute_name[8];
  GLboolean attribute_flag[8];
  const GLvoid *attribute_data_ptr[8];
} ProgramObject;
#endif
