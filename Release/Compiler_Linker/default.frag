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
	float c = 2.0 * M_PI * color.r;

	float arrVal = c * 2.546478971;
	int arr0 = int(floor(arrVal));
	float weight = arrVal - floor(arrVal);
	float sin_c = 0.0;
	
	if (arr0 == 0)
		sin_c = lerp(sinValues[0], sinValues[1], weight);
	else if (arr0 == 1)
		sin_c = lerp(sinValues[1], sinValues[2], weight);
	else if (arr0 == 2)
		sin_c = lerp(sinValues[2], sinValues[3], weight);
	else if (arr0 == 3)
		sin_c = lerp(sinValues[3], sinValues[4], weight);
	else if (arr0 == 4)
		sin_c = lerp(sinValues[4], sinValues[5], weight);
	else if (arr0 == 5)
		sin_c = lerp(sinValues[5], sinValues[6], weight);
	else if (arr0 == 6)
		sin_c = lerp(sinValues[6], sinValues[7], weight);
	else if (arr0 == 7)
		sin_c = lerp(sinValues[7], sinValues[8], weight);
	else if (arr0 == 8)
		sin_c = lerp(sinValues[8], sinValues[9], weight);
	else if (arr0 == 9)
		sin_c = lerp(sinValues[9], sinValues[10], weight);
	else if (arr0 == 10)
		sin_c = lerp(sinValues[10], sinValues[11], weight);
	else if (arr0 == 11)
		sin_c = lerp(sinValues[11], sinValues[12], weight);
	else if (arr0 == 12)
		sin_c = lerp(sinValues[12], sinValues[13], weight);
	else if (arr0 == 13)
		sin_c = lerp(sinValues[13], sinValues[14], weight);
	else if (arr0 == 14)
		sin_c = lerp(sinValues[14], sinValues[15], weight);
	else if (arr0 == 15)
		sin_c = lerp(sinValues[15], sinValues[16], weight);
        else if (arr0 == 16)
                sin_c = sinValues[16];

	gl_FragColor = vec4(0.5 * sin_c + 0.5, 0.0, 0.0, 1.0);
}
