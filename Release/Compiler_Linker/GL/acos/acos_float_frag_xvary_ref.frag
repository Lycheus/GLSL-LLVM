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

/* The following files are direct copies of each other:
 *
 *   GL/acos/acos_float_frag_xvary_ref.frag
 *   GL/asin/asin_float_frag_xvary_ref.frag
 *
 * Care should be taken to apply any changes to both.  Only the last
 * line where gl_FragColor is assigned should be different.
 */

#ifdef GL_ES
precision mediump float;
#endif
varying vec4 color;

float lerp(float a, float b, float s)
{
	return a + (b - a) * s;
}

void main (void)
{
	float asinValues[17];
	asinValues[0] = -1.5708;
	asinValues[1] = -1.06544;
	asinValues[2] = -0.848062;
	asinValues[3] = -0.675132;
	asinValues[4] = -0.523599;
	asinValues[5] = -0.384397;
	asinValues[6] = -0.25268;
	asinValues[7] = -0.125328;
	asinValues[8] = 0.0;
	asinValues[9] = 0.125328;
	asinValues[10] = 0.25268;
	asinValues[11] = 0.384397;
	asinValues[12] = 0.523599;
	asinValues[13] = 0.675132;
	asinValues[14] = 0.848062;
	asinValues[15] = 1.06544;
	asinValues[16] = 1.5708;
	
	const float M_PI = 3.14159265358979323846;
	float c = 2.0 * (color.r - 0.5);
	
	float arrVal = (c + 1.0) * 8.0;
	int arr0 = int(floor(arrVal));
	float weight = arrVal - floor(arrVal);
	float asin_c = 0.0;
	
	if (arr0 == 0)
		asin_c = lerp(asinValues[0], asinValues[1], weight);
	else if (arr0 == 1)
		asin_c = lerp(asinValues[1], asinValues[2], weight);
	else if (arr0 == 2)
		asin_c = lerp(asinValues[2], asinValues[3], weight);
	else if (arr0 == 3)
		asin_c = lerp(asinValues[3], asinValues[4], weight);
	else if (arr0 == 4)
		asin_c = lerp(asinValues[4], asinValues[5], weight);
	else if (arr0 == 5)
		asin_c = lerp(asinValues[5], asinValues[6], weight);
	else if (arr0 == 6)
		asin_c = lerp(asinValues[6], asinValues[7], weight);
	else if (arr0 == 7)
		asin_c = lerp(asinValues[7], asinValues[8], weight);
	else if (arr0 == 8)
		asin_c = lerp(asinValues[8], asinValues[9], weight);
	else if (arr0 == 9)
		asin_c = lerp(asinValues[9], asinValues[10], weight);
	else if (arr0 == 10)
		asin_c = lerp(asinValues[10], asinValues[11], weight);
	else if (arr0 == 11)
		asin_c = lerp(asinValues[11], asinValues[12], weight);
	else if (arr0 == 12)
		asin_c = lerp(asinValues[12], asinValues[13], weight);
	else if (arr0 == 13)
		asin_c = lerp(asinValues[13], asinValues[14], weight);
	else if (arr0 == 14)
		asin_c = lerp(asinValues[14], asinValues[15], weight);
	else if (arr0 == 15)
		asin_c = lerp(asinValues[15], asinValues[16], weight);
	else if (arr0 == 16)
		asin_c = asinValues[16];

	// acos(x) = PI/2 - asin(x)
	gl_FragColor = vec4(0.5 - asin_c / M_PI, 0.0, 0.0, 1.0);
}
