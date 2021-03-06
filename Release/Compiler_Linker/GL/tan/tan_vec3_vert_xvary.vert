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
	vec3 c = 0.5 * M_PI * 2.0 * (gtf_Color.rgb - 0.5);
	vec3 o;
	if(abs(c.r) < 0.5)   // -45..45
		o.r = 0.5 * tan(c.r) + 0.5;
	else   // 45..90, -45..-90
		o.r = 0.5 / tan(c.r) + 0.5;

	if(abs(c.g) < 0.5)   // -45..45
		o.g = 0.5 * tan(c.g) + 0.5;
	else   // 45..90, -45..-90
		o.g = 0.5 / tan(c.g) + 0.5;

	if(abs(c.b) < 0.5)   // -45..45
		o.b = 0.5 * tan(c.b) + 0.5;
	else   // 45..90, -45..-90
		o.b = 0.5 / tan(c.b) + 0.5;

	color = vec4(o, 1.0);
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}
