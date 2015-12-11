#ifndef LINK_VAR_H
#define LINK_VAR_H
#include "linker.h"
struct attribute_form
{
  int index[8];
  char att_name[8][128];
};

struct varying_form
{
  int index[max_number_of_varying_variables];
  char vary_name[max_number_of_varying_variables][128];
};

struct uniform_form{
  int index[max_number_of_uniform_variables];
  char uniform_name[max_number_of_uniform_variables][128];
  char IsSampler2D[max_number_of_uniform_variables]; //Kaymon add it.
  char IsSamplerCube[max_number_of_uniform_variables]; //Kaymon add it.
  char IsNormal[max_number_of_uniform_variables]; //Kaymon add it.
  int size[max_number_of_uniform_variables];
  int store_regi[max_number_of_uniform_variables];
};

struct variable_form
{
  char name[36];
  float value;
  float value1;
  float value2;
  float value3;
  int address;
  int vector_size;
  struct variable_form *next;
};

#endif
