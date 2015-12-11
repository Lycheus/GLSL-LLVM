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
	vec3 x = (color.rgb + 0.01) / 1.01;
	vec3 y = vec3(0.0);
	vec3 z;   // x-1 / x+1
	int n = 50;

	// ln(x) = 2[x-1 + 1 (x-1)^3 + 1 (x-1)^5 + ...] for x > 0
	//          [x+1   3 (x+1)     5 (x+1)        ]
	// Note: z will always be negative between 0.01 and 1.0 and
	// so will y since it is raised to an odd power, and the shader spec
	// does not support pow(-x, y) where y is not a compile time constant
	z = abs((x - 1.0) / (x + 1.0));
	vec3 p = z;
	for(int i = 1; i <= 101; i += 2)
	{
		y += p / float(i);
		p *= z * z;
	}
	y *= -2.0;

	gl_FragColor = vec4(y / -4.61, 1.0);
}
