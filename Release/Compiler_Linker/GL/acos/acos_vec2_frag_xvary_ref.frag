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
 *   GL/acos/acos_vec2_frag_xvary_ref.frag
 *   GL/asin/asin_vec2_frag_xvary_ref.frag
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
	vec2 c = 2.0 * (color.rg - 0.5);
	
	vec2 arrVal = (c + vec2(1.0, 1.0)) * 8.0;
	int arr0x = int(floor(arrVal.x));
	int arr0y = int(floor(arrVal.y));
	vec2 weight = arrVal - floor(arrVal);
	vec2 asin_c = vec2(0.0);

	if (arr0x == 0)
		asin_c.x = lerp(asinValues[0], asinValues[1], weight.x);
	else if (arr0x == 1)
		asin_c.x = lerp(asinValues[1], asinValues[2], weight.x);
	else if (arr0x == 2)
		asin_c.x = lerp(asinValues[2], asinValues[3], weight.x);
	else if (arr0x == 3)
		asin_c.x = lerp(asinValues[3], asinValues[4], weight.x);
	else if (arr0x == 4)
		asin_c.x = lerp(asinValues[4], asinValues[5], weight.x);
	else if (arr0x == 5)
		asin_c.x = lerp(asinValues[5], asinValues[6], weight.x);
	else if (arr0x == 6)
		asin_c.x = lerp(asinValues[6], asinValues[7], weight.x);
	else if (arr0x == 7)
		asin_c.x = lerp(asinValues[7], asinValues[8], weight.x);
	else if (arr0x == 8)
		asin_c.x = lerp(asinValues[8], asinValues[9], weight.x);
	else if (arr0x == 9)
		asin_c.x = lerp(asinValues[9], asinValues[10], weight.x);
	else if (arr0x == 10)
		asin_c.x = lerp(asinValues[10], asinValues[11], weight.x);
	else if (arr0x == 11)
		asin_c.x = lerp(asinValues[11], asinValues[12], weight.x);
	else if (arr0x == 12)
		asin_c.x = lerp(asinValues[12], asinValues[13], weight.x);
	else if (arr0x == 13)
		asin_c.x = lerp(asinValues[13], asinValues[14], weight.x);
	else if (arr0x == 14)
		asin_c.x = lerp(asinValues[14], asinValues[15], weight.x);
	else if (arr0x == 15)
		asin_c.x = lerp(asinValues[15], asinValues[16], weight.x);
        else if (arr0x == 16)
                asin_c.x = asinValues[16];
		
	if (arr0y == 0)
		asin_c.y = lerp(asinValues[0], asinValues[1], weight.y);
	else if (arr0y == 1)
		asin_c.y = lerp(asinValues[1], asinValues[2], weight.y);
	else if (arr0y == 2)
		asin_c.y = lerp(asinValues[2], asinValues[3], weight.y);
	else if (arr0y == 3)
		asin_c.y = lerp(asinValues[3], asinValues[4], weight.y);
	else if (arr0y == 4)
		asin_c.y = lerp(asinValues[4], asinValues[5], weight.y);
	else if (arr0y == 5)
		asin_c.y = lerp(asinValues[5], asinValues[6], weight.y);
	else if (arr0y == 6)
		asin_c.y = lerp(asinValues[6], asinValues[7], weight.y);
	else if (arr0y == 7)
		asin_c.y = lerp(asinValues[7], asinValues[8], weight.y);
	else if (arr0y == 8)
		asin_c.y = lerp(asinValues[8], asinValues[9], weight.y);
	else if (arr0y == 9)
		asin_c.y = lerp(asinValues[9], asinValues[10], weight.y);
	else if (arr0y == 10)
		asin_c.y = lerp(asinValues[10], asinValues[11], weight.y);
	else if (arr0y == 11)
		asin_c.y = lerp(asinValues[11], asinValues[12], weight.y);
	else if (arr0y == 12)
		asin_c.y = lerp(asinValues[12], asinValues[13], weight.y);
	else if (arr0y == 13)
		asin_c.y = lerp(asinValues[13], asinValues[14], weight.y);
	else if (arr0y == 14)
		asin_c.y = lerp(asinValues[14], asinValues[15], weight.y);
	else if (arr0y == 15)
		asin_c.y = lerp(asinValues[15], asinValues[16], weight.y);
        else if (arr0y == 16)
                asin_c.y = asinValues[16];

	// acos(x) = PI/2 - asin(x)
	gl_FragColor = vec4(0.5 - asin_c / M_PI, 0.0, 1.0);
}
