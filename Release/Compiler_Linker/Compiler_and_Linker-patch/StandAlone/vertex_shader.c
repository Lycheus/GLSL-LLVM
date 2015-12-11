#include "context.h"

vec4 position;
vec4 color;
vec4 texcoord;
mat4 mvp;
vec4 texcoord0;
vec4 outcolor;


void vertex_shader_a(){
Mul_mat4_single(gl_Position,mvp,position);
memcpy(&outcolor,&color,sizeof(outcolor));
memcpy(&texcoord0,&texcoord,sizeof(texcoord0));
}
