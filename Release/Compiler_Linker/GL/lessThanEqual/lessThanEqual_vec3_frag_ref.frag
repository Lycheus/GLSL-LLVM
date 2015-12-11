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

bvec3 lte(in vec3 a, in vec3 b)
{
	bvec3 result;
	if(a[0] <= b[0]) result[0] = true;
	else result[0] = false;
	if(a[1] <= b[1]) result[1] = true;
	else result[1] = false;
	if(a[2] <= b[2]) result[2] = true;
	else result[2] = false;
	return result;
}

void main (void)
{
	vec3 c = floor(10.0 * color.rgb - 4.5);   // round to the nearest integer
	vec3 result = vec3(lte(c, vec3(0.0))); 
	gl_FragColor = vec4(result, 1.0);
}
