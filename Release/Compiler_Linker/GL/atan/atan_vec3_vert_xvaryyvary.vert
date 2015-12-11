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
attribute vec4 gtf_Color;
uniform mat4 gtf_ModelViewProjectionMatrix;
varying vec4 color;

void main (void)
{
	const float M_PI = 3.14159265358979323846;
	vec3 x = 2.0 * (gtf_Color.ggg - 0.5);
	vec3 y = 2.0 * (gtf_Color.bbb - 0.5);
	const float epsilon = 1.0e-4;
	color = vec4(0.0, 0.0, 0.0, 1.0);
	
	// Avoid evaluating atan(0, x) for x < epsilon because it's implementation-dependent
	if(x[0] > epsilon || abs(y[0]) > epsilon)
	{
		color[0] = atan(y[0], x[0]) / (2.0 * M_PI) + 0.5;
	}

	if(x[1] > epsilon || abs(y[1]) > epsilon)
	{
		color[1] = atan(y[1], x[1]) / (2.0 * M_PI) + 0.5;
	}

	if(x[2] > epsilon || abs(y[2]) > epsilon)
	{
		color[2] = atan(y[2], x[2]) / (2.0 * M_PI) + 0.5;
	}
	
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}