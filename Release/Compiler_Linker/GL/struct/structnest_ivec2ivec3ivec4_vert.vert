/*
** Copyright (c) 2008 The Khronos Group Inc.
** 
** All Rights Reserved.  This code is protected by copyright laws and 
** contains material proprietary to the Khronos Group, Inc.  This is 
** UNPUBLISHED PROPRIETARY SOURCE CODE that may not be disclosed in whole 
** or in part to third parties, and may not be reproduced, republished, 
** distributed, transmitted, displayed, broadcast or otherwise exploited 
** in any manner without the express prior written permission of Khronos 
** Group.
** 
** The receipt or possession of this code does not convey any rights to 
** reproduce, disclose, or distribute its contents, or to manufacture, 
** use, or sell anything that it may describe, in whole or in part other 
** than under the terms of the Khronos Adopters Agreement or Khronos 
** Conformance Test Source License Agreement as executed between Khronos 
** and the recipient.
*/

attribute vec4 gtf_Vertex;
uniform mat4 gtf_ModelViewProjectionMatrix;
varying vec4 color;

struct nestb
{
	ivec2 a2;
	ivec3 b2;
	ivec4 c2;
};

struct nesta
{
	ivec2 a1;
	ivec3 b1;
	ivec4 c1;
	nestb nest_b;
};

struct nest
{
	nesta nest_a;
};

void main (void)
{

	nest s = nest( nesta( ivec2(12, 13), ivec3(14, 0, 139), ivec4(25, 17, 145, 163 ), 
                       nestb( ivec2(28, 0), ivec3(0, 0, 1), ivec4(0, 17, 148, 0 ) 
                            ) 
                            ) 
                      );
	
	color = vec4( vec3(  float(s.nest_a.a1[0] + s.nest_a.a1[1] + s.nest_a.b1[0] + s.nest_a.b1[1] + s.nest_a.b1[2] + s.nest_a.c1[0] + s.nest_a.c1[1] + s.nest_a.c1[2] + s.nest_a.c1[3] + s.nest_a.nest_b.a2[0] + s.nest_a.nest_b.a2[1] + s.nest_a.nest_b.b2[0] + s.nest_a.nest_b.b2[1] + s.nest_a.nest_b.b2[2] + s.nest_a.nest_b.c2[0] + s.nest_a.nest_b.c2[1] + s.nest_a.nest_b.c2[2] + s.nest_a.nest_b.c2[3]) / 722.0   ), 1.0);
	
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}