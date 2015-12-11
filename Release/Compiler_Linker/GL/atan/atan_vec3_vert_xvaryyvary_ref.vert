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
	vec3 c;
	vec3 atan_c = vec3(0.0);
	vec3 scale = vec3(1.0);
	vec3 sign = vec3(1.0);
	vec4 result = vec4(0.0, 0.0, 0.0, 1.0);
	const float epsilon = 1.0e-4;

	// Avoid evaluating atan(0, x) for x < epsilon because it's implementation-dependent
	if(x[0] > epsilon || abs(y[0]) > epsilon)
	{
		if(x[0] < 0.0 ^^ y[0] < 0.0)
		{
			sign[0] = -1.0;
		}

		if(abs(y[0]) <= abs(x[0]))
		{
			c[0] = abs(y[0] / x[0]);
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

			result[0] = sign[0] * atan_c[0] / (2.0 * M_PI) + 0.5;
		}
		else
		{
			c[0] = abs(x[0] / y[0]);

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

			result[0] = sign[0] * (M_PI / 2.0 - atan_c[0]) / (2.0 * M_PI) + 0.5;
		}

		if(x[0] < 0.0)
			if(y[0] < 0.0) result[0] -= 0.5;
			else if(y[0] > 0.0) result[0] += 0.5;
	}
			
	if(x[1] > epsilon || abs(y[1]) > epsilon)
	{
		if(x[1] < 0.0 ^^ y[1] < 0.0)
		{
			sign[1] = -1.0;
		}

		if(abs(y[1]) <= abs(x[1]))
		{
			c[1] = abs(y[1] / x[1]);
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

			result[1] = sign[1] * atan_c[1] / (2.0 * M_PI) + 0.5;
		}
		else
		{
			c[1] = abs(x[1] / y[1]);

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

			result[1] = sign[1] * (M_PI / 2.0 - atan_c[1]) / (2.0 * M_PI) + 0.5;
		}

		if(x[1] < 0.0)
			if(y[1] < 0.0) result[1] -= 0.5;
			else if(y[1] > 0.0) result[1] += 0.5;
	}

	if(x[2] > epsilon || abs(y[2]) > epsilon)
	{
		if(x[2] < 0.0 ^^ y[2] < 0.0)
		{
			sign[2] = -1.0;
		}

		if(abs(y[2]) <= abs(x[2]))
		{
			c[2] = abs(y[2] / x[2]);
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

			result[2] = sign[2] * atan_c[2] / (2.0 * M_PI) + 0.5;
		}
		else
		{
			c[2] = abs(x[2] / y[2]);

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

			result[2] = sign[2] * (M_PI / 2.0 - atan_c[2]) / (2.0 * M_PI) + 0.5;
		}

		if(x[2] < 0.0)
			if(y[2] < 0.0) result[2] -= 0.5;
			else if(y[2] > 0.0) result[2] += 0.5;
	}

	color = result;
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}
