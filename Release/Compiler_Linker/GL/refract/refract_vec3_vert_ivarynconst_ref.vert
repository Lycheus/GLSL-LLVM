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

	// Both are unit vectors
	vec3 v1;
	vec3 v2 = normalize(vec3(1.0, 1.0, 1.0));

	float theta = gtf_Color.g * 2.0 * M_PI;
	float phi = gtf_Color.b * 2.0 * M_PI;
	v1.x = cos(theta) * sin(phi);
	v1.y = sin(theta) * sin(phi);
	v1.z = cos(phi);

	vec3 result;
	float eta = 0.5;
	float k = 1.0 - eta * eta * (1.0 - dot(v1, v2) * dot(v1, v2));
	if(k < 0.0)
		result = vec3(0.0);
	else
		result = eta * v1 - (eta * dot(v1, v2) + sqrt(k)) * v2;

	color = vec4((result + 1.0) / 2.0, 1.0);
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}
