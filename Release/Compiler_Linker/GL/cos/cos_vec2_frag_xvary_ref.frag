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
	vec2 c = 2.0 * M_PI * ( fract(abs(color.rg)) - 0.5 );
	float sign =  1.0;
	vec2 cos_c = vec2(-1.0, -1.0);
	float fact_even = 1.0;
	float fact_odd  = 1.0;
	vec2 sum;
	vec2 exp;
	
	// At this point c is in the range [-PI, PI)
	
	// Taylor-Maclaurin series expansion for cosine
	//	
	// Apply the property that pow(a, b + c) = pow(a, b) * pow(a, c)
	// and the property that 1.0/(a*b) = 1.0/a * 1.0/b
	// to make sure no register ever overflows the range (-16384, +16384)
	// mandated for mediump variables.
	
	for(int i = 2; i <= 10; i += 2)
	{
		// fact_even will hold at most the value 3840.
		fact_even *= float(i);
		
		// fact_odd will always be smaller than fact_even
		fact_odd  *= float(i-1);
		
		// exp is at most (5,5)
		exp = vec2(float(i/2), float(i/2));
		
		// pow(c, exp) takes at most the value pow(PI, 5), which is approx. 306
		// abs(sum) is at most PI/2.0
		sum = sign * pow(abs(c), exp)/fact_even;
		
		// abs(sum/fact_odd) is at most PI/2.0
		// cos_c is always bound in the range [-1.0, 1.0)
		cos_c += pow(abs(c), exp)*(sum/fact_odd);
				
		sign = -sign;
	}
	
	gl_FragColor = vec4(0.5 * cos_c + 0.5, 0.0, 1.0);
}
