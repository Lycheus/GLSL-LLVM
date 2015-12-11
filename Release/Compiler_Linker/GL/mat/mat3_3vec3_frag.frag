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
	mat3 m = mat3(color.rgb, color.rgb, color.rgb);
	vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 result = vec4(1.0, 1.0, 1.0, 1.0);

	if(m[0][0] != color.r) result = black;
	if(m[0][1] != color.g) result = black;
	if(m[0][2] != color.b) result = black;
	if(m[1][0] != color.r) result = black;
	if(m[1][1] != color.g) result = black;
	if(m[1][2] != color.b) result = black;
	if(m[2][0] != color.r) result = black;
	if(m[2][1] != color.g) result = black;
	if(m[2][2] != color.b) result = black;

	gl_FragColor = result;
}
