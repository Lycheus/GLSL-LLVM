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

void main (void)
{
	const vec3 edge = vec3(0.5, 0.5, 0.5);
	vec3 c = color.rgb;
	if(c[0] >= edge[0])
	{
		c[0] = 1.0;
	}
	else
	{
		c[0] = 0.0;
	}
	if(c[1] >= edge[1])
	{
		c[1] = 1.0;
	}
	else
	{
		c[1] = 0.0;
	}
	if(c[2] >= edge[2])
	{
		c[2] = 1.0;
	}
	else
	{
		c[2] = 0.0;
	}

	gl_FragColor = vec4(c, 1.0);
}
