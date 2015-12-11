#ifndef LINKER_H
#define LINKER_H

//#include "sct_table.h"
#ifdef COMPILER_STANDALONE
#include "APIContext.h"
#else
#include "../context.h"
#endif
#define LoadAttributeFromSCT
#define max_number_of_varying_variables 15
#define max_number_of_uniform_variables 256
//#define SET_UNIFORM_MATRIX_UNIT
//#define For_Attila
#ifdef For_Attila
#else
//#define SET_NOP_NINE
#endif


int linker(int file_number, char *filename[], struct varying_form *input_varying, char type, char *output_filename,struct programobject *input_object,int fd);
void putoutmemory(char type,int fd);
void putoutthefile(char *S);
void print_variable();

#endif
