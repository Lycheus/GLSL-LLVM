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

vec3 floor_ref(vec3 x)
{
	if(x[0] >= 0.0)
		x[0] = float(int(x[0]));
	else
		x[0] = float(int(x[0]) - 1);
	if(x[1] >= 0.0)
		x[1] = float(int(x[1]));
	else
		x[1] = float(int(x[1]) - 1);
	if(x[2] >= 0.0)
		x[2] = float(int(x[2]));
	else
		x[2] = float(int(x[2]) - 1);
	return x;
}

void main (void)
{
	vec3 c = 10.0 * 2.0 * (color.rgb - 0.5);
	gl_FragColor = vec4((floor_ref(c) + 10.0) / 20.0, 1.0);
}
