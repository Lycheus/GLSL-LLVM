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
	float x = 2.0 * (color.g - 0.5);
	float y = 2.0 * (color.b - 0.5);
	float atan_c = 0.0;
	float scale = 1.0;
	float sign = 1.0;
	vec4 result = vec4(0.0, 0.0, 0.0, 1.0);
	const float epsilon = 1.0e-4;
	
	// Avoid evaluating atan(0, x) for x < epsilon because it's implementation-dependent
	if(x > epsilon || abs(y) > epsilon)
	{
		if(x < 0.0 ^^ y < 0.0)
		{
			sign = -1.0;
		}

		if(abs(y) <= abs(x))
		{
			float c = abs(y / x);

			// Taylors series expansion for atan
			for(int i = 1; i < 12; i += 2)
			{
				atan_c += scale * pow(c, float(i)) / float(i);
				scale *= -1.0;
			}

			result = vec4(sign * atan_c / (2.0 * M_PI) + 0.5, 0.0, 0.0, 1.0);
		}
		else
		{
			float c = abs(x / y);

			// Taylors series expansion for atan
			for(int i = 1; i < 12; i += 2)
			{
				atan_c += scale * pow(c, float(i)) / float(i);
				scale *= -1.0;
			}

			result = vec4(sign * (M_PI / 2.0 - atan_c) / (2.0 * M_PI) + 0.5, 0.0, 0.0, 1.0);
		}

		if(x < 0.0)
			if(y < 0.0) result.r -= 0.5;
			else if(y > 0.0) result.r += 0.5;
	}
	
	gl_FragColor = result;
}

