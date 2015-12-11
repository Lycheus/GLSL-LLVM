#include "context.h"

vec4 myColor;

void fragment_shader_a()
{
	memcpy(&gl_FragColor, &myColor, sizeof(myColor));
}