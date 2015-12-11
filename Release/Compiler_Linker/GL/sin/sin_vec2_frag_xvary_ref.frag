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

float lerp(float a, float b, float s)
{
	return a + (b - a) * s;
}

void main (void)
{
	float sinValues[17];
	sinValues[0] = 0.0;
	sinValues[1] = 0.382683;
	sinValues[2] = 0.707107;
	sinValues[3] = 0.92388;
	sinValues[4] = 1.0;
	sinValues[5] = 0.92388;
	sinValues[6] = 0.707107;
	sinValues[7] = 0.382683;
	sinValues[8] = 0.0;
	sinValues[9] = -0.382683;
	sinValues[10] = -0.707107;
	sinValues[11] = -0.92388;
	sinValues[12] = -1.0;
	sinValues[13] = -0.923879;
	sinValues[14] = -0.707107;
	sinValues[15] = -0.382683;
	sinValues[16] = 0.0;

	const float M_PI = 3.14159265358979323846;
	vec2 c = 2.0 * M_PI * color.rg;
	
	vec2 arrVal = c * 2.546478971;
	int arr0x = int(floor(arrVal.x));
	int arr0y = int(floor(arrVal.y));
	vec2 weight = arrVal - floor(arrVal);
	vec2 sin_c = vec2(0.0, 0.0);

	if (arr0x == 0)
		sin_c.x = lerp(sinValues[0], sinValues[1], weight.x);
	else if (arr0x == 1)
		sin_c.x = lerp(sinValues[1], sinValues[2], weight.x);
	else if (arr0x == 2)
		sin_c.x = lerp(sinValues[2], sinValues[3], weight.x);
	else if (arr0x == 3)
		sin_c.x = lerp(sinValues[3], sinValues[4], weight.x);
	else if (arr0x == 4)
		sin_c.x = lerp(sinValues[4], sinValues[5], weight.x);
	else if (arr0x == 5)
		sin_c.x = lerp(sinValues[5], sinValues[6], weight.x);
	else if (arr0x == 6)
		sin_c.x = lerp(sinValues[6], sinValues[7], weight.x);
	else if (arr0x == 7)
		sin_c.x = lerp(sinValues[7], sinValues[8], weight.x);
	else if (arr0x == 8)
		sin_c.x = lerp(sinValues[8], sinValues[9], weight.x);
	else if (arr0x == 9)
		sin_c.x = lerp(sinValues[9], sinValues[10], weight.x);
	else if (arr0x == 10)
		sin_c.x = lerp(sinValues[10], sinValues[11], weight.x);
	else if (arr0x == 11)
		sin_c.x = lerp(sinValues[11], sinValues[12], weight.x);
	else if (arr0x == 12)
		sin_c.x = lerp(sinValues[12], sinValues[13], weight.x);
	else if (arr0x == 13)
		sin_c.x = lerp(sinValues[13], sinValues[14], weight.x);
	else if (arr0x == 14)
		sin_c.x = lerp(sinValues[14], sinValues[15], weight.x);
	else if (arr0x == 15)
		sin_c.x = lerp(sinValues[15], sinValues[16], weight.x);
        else if (arr0x == 16)
                sin_c.x = sinValues[16];

	if (arr0y == 0)
		sin_c.y = lerp(sinValues[0], sinValues[1], weight.y);
	else if (arr0y == 1)
		sin_c.y = lerp(sinValues[1], sinValues[2], weight.y);
	else if (arr0y == 2)
		sin_c.y = lerp(sinValues[2], sinValues[3], weight.y);
	else if (arr0y == 3)
		sin_c.y = lerp(sinValues[3], sinValues[4], weight.y);
	else if (arr0y == 4)
		sin_c.y = lerp(sinValues[4], sinValues[5], weight.y);
	else if (arr0y == 5)
		sin_c.y = lerp(sinValues[5], sinValues[6], weight.y);
	else if (arr0y == 6)
		sin_c.y = lerp(sinValues[6], sinValues[7], weight.y);
	else if (arr0y == 7)
		sin_c.y = lerp(sinValues[7], sinValues[8], weight.y);
	else if (arr0y == 8)
		sin_c.y = lerp(sinValues[8], sinValues[9], weight.y);
	else if (arr0y == 9)
		sin_c.y = lerp(sinValues[9], sinValues[10], weight.y);
	else if (arr0y == 10)
		sin_c.y = lerp(sinValues[10], sinValues[11], weight.y);
	else if (arr0y == 11)
		sin_c.y = lerp(sinValues[11], sinValues[12], weight.y);
	else if (arr0y == 12)
		sin_c.y = lerp(sinValues[12], sinValues[13], weight.y);
	else if (arr0y == 13)
		sin_c.y = lerp(sinValues[13], sinValues[14], weight.y);
	else if (arr0y == 14)
		sin_c.y = lerp(sinValues[14], sinValues[15], weight.y);
	else if (arr0y == 15)
		sin_c.y = lerp(sinValues[15], sinValues[16], weight.y);
        else if (arr0y == 16)
                sin_c.y = sinValues[16];
//sin_c.x = 0.0;
	gl_FragColor = vec4(0.5 * sin_c + 0.5, 0.0, 1.0);
	//gl_FragColor = vec4(0.0,0.5 * sin_c.y + 0.5, 0.0, 1.0);
}
