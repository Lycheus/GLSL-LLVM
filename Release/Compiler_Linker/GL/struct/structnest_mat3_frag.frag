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

#ifdef GL_ES
precision mediump float;
#endif
struct nestb
{
	mat3 b;
};

struct nesta
{
	mat3 a;
	nestb nest_b;
};

struct nest
{
	nesta nest_a;
};

void main (void)
{
	nest s = nest(nesta( mat3(11, 13, 29, 33, 63, 13, 49, 57, 71), nestb( mat3(12, 19, 79, 81, 35, 51, 73, 66, 23) ) ) );
	float sum1=0.0,sum2=0.0;
	int i,j;

	sum1 = sum1 + s.nest_a.a[0][0];
	sum2 = sum2 + s.nest_a.nest_b.b[0][0];
	
	sum1 = sum1 + s.nest_a.a[0][1];
	sum2 = sum2 + s.nest_a.nest_b.b[0][1];
	
	sum1 = sum1 + s.nest_a.a[0][2];
	sum2 = sum2 + s.nest_a.nest_b.b[0][2];
	
	sum1 = sum1 + s.nest_a.a[1][0];
	sum2 = sum2 + s.nest_a.nest_b.b[1][0];
	
	sum1 = sum1 + s.nest_a.a[1][1];
	sum2 = sum2 + s.nest_a.nest_b.b[1][1];
	
	sum1 = sum1 + s.nest_a.a[1][2];
	sum2 = sum2 + s.nest_a.nest_b.b[1][2];
	
	sum1 = sum1 + s.nest_a.a[2][0];
	sum2 = sum2 + s.nest_a.nest_b.b[2][0];
	
	sum1 = sum1 + s.nest_a.a[2][1];
	sum2 = sum2 + s.nest_a.nest_b.b[2][1];
	
	sum1 = sum1 + s.nest_a.a[2][2];
	sum2 = sum2 + s.nest_a.nest_b.b[2][2];

	gl_FragColor = vec4( vec3( ( sum1 + sum2 )/ 778.0 ), 1.0);
}
