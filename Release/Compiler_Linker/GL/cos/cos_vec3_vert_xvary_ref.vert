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

attribute vec4 gtf_Color;
attribute vec4 gtf_Vertex;
uniform mat4 gtf_ModelViewProjectionMatrix;
varying vec4 color;

void main (void)
{
	const float M_PI = 3.14159265358979323846;
	vec3 c = 2.0 * M_PI * gtf_Color.rgb;
	float sign = -1.0;
	vec3 cos_c = vec3(1.0,1.0,1.0);
	float fact = 1.0;

	// Taylor-Maclaurin series expansion for cosine
	for(int i = 2; i <= 20; i += 2)
	{
		fact *= float(i)*float(i-1);
		cos_c += sign*pow(c, vec3(float(i),float(i),float(i)))/fact;
		sign = -sign;
	}

	color = vec4(0.5 * cos_c + 0.5, 1.0);

	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}
