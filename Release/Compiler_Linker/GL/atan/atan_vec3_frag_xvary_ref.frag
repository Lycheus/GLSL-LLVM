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
	vec3 c = 4.0 * 2.0 * (color.rgb - 0.5);
	vec3 atan_c = vec3(0.0);
	vec3 scale = vec3(1.0);
	vec3 sign = vec3(1.0);
	vec4 result = vec4(0.0, 0.0, 0.0, 1.0);

		
	if(c[0] < 0.0)
	{
		sign[0] = -1.0;
		c[0] *= -1.0;
	}

	if(c[0] <= 1.0)
	{
		// Taylors series expansion for atan
		atan_c[0] += scale[0] * pow(c[0], float(1)) / float(1);
		scale[0] *= -1.0;
		atan_c[0] += scale[0] * pow(c[0], float(3)) / float(3);
		scale[0] *= -1.0;
		atan_c[0] += scale[0] * pow(c[0], float(5)) / float(5);
		scale[0] *= -1.0;
		atan_c[0] += scale[0] * pow(c[0], float(7)) / float(7);
		scale[0] *= -1.0;
		atan_c[0] += scale[0] * pow(c[0], float(9)) / float(9);
		scale[0] *= -1.0;
		atan_c[0] += scale[0] * pow(c[0], float(11)) / float(11);
		scale[0] *= -1.0;

		result[0] = sign[0] * atan_c[0] / M_PI + 0.5;
	}
	else
	{
		c[0] = 1.0 / c[0];

		// Taylors series expansion for atan
		atan_c[0] += scale[0] * pow(c[0], float(1)) / float(1);
		scale[0] *= -1.0;
		atan_c[0] += scale[0] * pow(c[0], float(3)) / float(3);
		scale[0] *= -1.0;
		atan_c[0] += scale[0] * pow(c[0], float(5)) / float(5);
		scale[0] *= -1.0;
		atan_c[0] += scale[0] * pow(c[0], float(7)) / float(7);
		scale[0] *= -1.0;
		atan_c[0] += scale[0] * pow(c[0], float(9)) / float(9);
		scale[0] *= -1.0;
		atan_c[0] += scale[0] * pow(c[0], float(11)) / float(11);
		scale[0] *= -1.0;

		result[0] = sign[0] * (M_PI / 2.0 - atan_c[0]) / M_PI + 0.5;
	}
		
	if(c[1] < 0.0)
	{
		sign[1] = -1.0;
		c[1] *= -1.0;
	}

	if(c[1] <= 1.0)
	{
		// Taylors series expansion for atan
		atan_c[1] += scale[1] * pow(c[1], float(1)) / float(1);
		scale[1] *= -1.0;
		atan_c[1] += scale[1] * pow(c[1], float(3)) / float(3);
		scale[1] *= -1.0;
		atan_c[1] += scale[1] * pow(c[1], float(5)) / float(5);
		scale[1] *= -1.0;
		atan_c[1] += scale[1] * pow(c[1], float(7)) / float(7);
		scale[1] *= -1.0;
		atan_c[1] += scale[1] * pow(c[1], float(9)) / float(9);
		scale[1] *= -1.0;
		atan_c[1] += scale[1] * pow(c[1], float(11)) / float(11);
		scale[1] *= -1.0;

		result[1] = sign[1] * atan_c[1] / M_PI + 0.5;
	}
	else
	{
		c[1] = 1.0 / c[1];

		// Taylors series expansion for atan
		atan_c[1] += scale[1] * pow(c[1], float(1)) / float(1);
		scale[1] *= -1.0;
		atan_c[1] += scale[1] * pow(c[1], float(3)) / float(3);
		scale[1] *= -1.0;
		atan_c[1] += scale[1] * pow(c[1], float(5)) / float(5);
		scale[1] *= -1.0;
		atan_c[1] += scale[1] * pow(c[1], float(7)) / float(7);
		scale[1] *= -1.0;
		atan_c[1] += scale[1] * pow(c[1], float(9)) / float(9);
		scale[1] *= -1.0;
		atan_c[1] += scale[1] * pow(c[1], float(11)) / float(11);
		scale[1] *= -1.0;

		result[1] = sign[1] * (M_PI / 2.0 - atan_c[1]) / M_PI + 0.5;
	}
	
	
	if(c[2] < 0.0)
	{
		sign[2] = -1.0;
		c[2] *= -1.0;
	}

	if(c[2] <= 1.0)
	{
		// Taylors series expansion for atan
		atan_c[2] += scale[2] * pow(c[2], float(1)) / float(1);
		scale[2] *= -1.0;
		atan_c[2] += scale[2] * pow(c[2], float(3)) / float(3);
		scale[2] *= -1.0;
		atan_c[2] += scale[2] * pow(c[2], float(5)) / float(5);
		scale[2] *= -1.0;
		atan_c[2] += scale[2] * pow(c[2], float(7)) / float(7);
		scale[2] *= -1.0;
		atan_c[2] += scale[2] * pow(c[2], float(9)) / float(9);
		scale[2] *= -1.0;
		atan_c[2] += scale[2] * pow(c[2], float(11)) / float(11);
		scale[2] *= -1.0;

		result[2] = sign[2] * atan_c[2] / M_PI + 0.5;
	}
	else
	{
		c[2] = 1.0 / c[2];

		// Taylors series expansion for atan
		atan_c[2] += scale[2] * pow(c[2], float(1)) / float(1);
		scale[2] *= -1.0;
		atan_c[2] += scale[2] * pow(c[2], float(3)) / float(3);
		scale[2] *= -1.0;
		atan_c[2] += scale[2] * pow(c[2], float(5)) / float(5);
		scale[2] *= -1.0;
		atan_c[2] += scale[2] * pow(c[2], float(7)) / float(7);
		scale[2] *= -1.0;
		atan_c[2] += scale[2] * pow(c[2], float(9)) / float(9);
		scale[2] *= -1.0;
		atan_c[2] += scale[2] * pow(c[2], float(11)) / float(11);
		scale[2] *= -1.0;

		result[2] = sign[2] * (M_PI / 2.0 - atan_c[2]) / M_PI + 0.5;
	}

	gl_FragColor = result;
}
