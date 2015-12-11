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

const int array_size = 2;

void main (void)
{
	const mat4 a = mat4( 1.0,  2.0,  3.0,  4.0,
		             5.0,  6.0,  7.0,  8.0,
			     9.0, 10.0, 11.0, 12.0,
			    13.0, 14.0, 15.0, 16.0);
	const mat4 b = mat4(17.0, 18.0, 19.0, 20.0,
		            21.0, 22.0, 23.0, 24.0,
			    25.0, 26.0, 27.0, 28.0,
			    29.0, 30.0, 31.0, 32.0);
	mat4 array[array_size];
	float gray;

	array[0] = a;
	array[1] = b;

	if((array[0] == a) && (array[1] == b))
		gray = 1.0;
	else
		gray = 0.0;

	gl_FragColor = vec4(gray, gray, gray, 1.0);
}

