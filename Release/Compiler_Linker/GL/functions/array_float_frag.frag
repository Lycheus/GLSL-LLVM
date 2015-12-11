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
varying vec4 color;



void qualifiers(in float a[4], out float b[4], inout float c[4], const in float d[4], float e[4])
{
	b[0] = a[0];
	c[0] += d[0];
	a[0] += 1.0;
	e[0] += 1.0;
	
	b[1] = a[1];
	c[1] += d[1];
	a[1] += 1.0;
	e[1] += 1.0;
	
	b[2] = a[2];
	c[2] += d[2];
	a[2] += 1.0;
	e[2] += 1.0;
	
	b[3] = a[3];
	c[3] += d[3];
	a[3] += 1.0;
	e[3] += 1.0;
}



void main (void)
{
	float a[4];
	float b[4];
	float c[4];
	float d[4];
	float e[4];
	float q = 0.0;
	float q2 = 0.0;

	a[0] = 1.0;
	b[0] = 2.0;
	c[0] = 3.0;
	d[0] = 4.0;
	e[0] = 1.0;
	
	a[1] = 1.0;
	b[1] = 2.0;
	c[1] = 3.0;
	d[1] = 4.0;
	e[1] = 1.0;
	
	a[2] = 1.0;
	b[2] = 2.0;
	c[2] = 3.0;
	d[2] = 4.0;
	e[2] = 1.0;
	
	a[3] = 1.0;
	b[3] = 2.0;
	c[3] = 3.0;
	d[3] = 4.0;
	e[3] = 1.0;

	qualifiers(a, b, c, d, e);

	// randomly test a value
	if(a[0] == 1.0) q += 1.0;
	if(b[1] == 1.0) q += 2.0;
	if(c[2] == 7.0) q += 4.0;
	if(d[3] == 4.0) q2 += 1.0;
	if(e[0] == 1.0) q2 += 2.0;

	gl_FragColor = vec4(vec2(q / 7.0, q2 / 3.0), 1.0, 1.0);
}
