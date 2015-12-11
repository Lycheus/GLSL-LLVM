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
	float c = 4.0 * 2.0 * (gtf_Color.r - 0.5);
	float atan_c = 0.0;
	float scale = 1.0;
	float sign = 1.0;
	vec4 result;

	if(c < 0.0)
	{
		sign = -1.0;
		c *= -1.0;
	}

	if(c <= 1.0)
	{
		// Taylors series expansion for atan
		for(int i = 1; i < 12; i += 2)
		{
			atan_c += scale * pow(c, float(i)) / float(i);
			scale *= -1.0;
		}

		result = vec4(sign * atan_c / M_PI + 0.5, 0.0, 0.0, 1.0);
	}
	else
	{
		c = 1.0 / c;

		// Taylors series expansion for atan
		for(int i = 1; i < 12; i += 2)
		{
			atan_c += scale * pow(c, float(i)) / float(i);
			scale *= -1.0;
		}

		result = vec4(sign * (M_PI / 2.0 - atan_c) / M_PI + 0.5, 0.0, 0.0, 1.0);
	}

	color = result;
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}
