#ifndef __GLCOMPILER_H__
#define __GLCOMPILER_H__

#ifdef COMPILER_STANDALONE
#include "APIContext.h"
#else
#include "../context.h"
#endif

#define UNDEFINED -1

//#define INFO_FOR_API

struct addr_name
{
  int address;
  char type;
  char name[32];
  struct addr_name *next;
} *AddrName = NULL;

extern "C" void glCompileShader(GLuint shader);
extern "C" void glLinkProgram(GLuint program);

#endif
