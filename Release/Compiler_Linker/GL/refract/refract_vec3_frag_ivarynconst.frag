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
	const float M_PI = 3.14159265358979323846;

	// Both are unit vectors
	vec3 v1;
	vec3 v2 = normalize(vec3(1.0, 1.0, 1.0));


	float theta = color.g * 2.0 * M_PI;
	float phi = color.b * 2.0 * M_PI;
	v1.x = cos(theta) * sin(phi);
	v1.y = sin(theta) * sin(phi);
	v1.z = cos(phi);

	gl_FragColor = vec4((refract(v1, v2, 0.5) + 1.0) / 2.0, 1.0);
}
