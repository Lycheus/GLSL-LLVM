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

struct sabcd
{ 
 mat3 a;
};

void main (void)
{
	sabcd s = sabcd(mat3(12.0, 29.0, 13.0, 26.0, 71.0, 63.0, 90.0, 118.0, 128.0) );
	sabcd s2 = sabcd(mat3(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0) );
	s2 = s;
	float sum=0.0;
	int i,j;
	
	sum = sum + s2.a[0][0];
	sum = sum + s2.a[0][1];
	sum = sum + s2.a[0][2];
	sum = sum + s2.a[1][0];
	sum = sum + s2.a[1][1];
	sum = sum + s2.a[1][2];
	sum = sum + s2.a[2][0];
	sum = sum + s2.a[2][1];
	sum = sum + s2.a[2][2];
	
	gl_FragColor =  vec4( vec3(  sum / 550.0  ), 1.0);
}
