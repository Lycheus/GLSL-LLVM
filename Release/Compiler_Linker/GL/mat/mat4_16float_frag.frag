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

/* This epsilon will work as long as the magnitude of the float is < 128.
 * This can be seen by taking the spec relative mediump precision of 2^-10:
 * 0.125 / 2^-10 = 128
 */
#define ERROR_EPSILON (0.125)

void main (void)
{
	mat4 a = mat4( 1.0, 2.0, 3.0, 4.0, 
				   5.0, 6.0, 7.0, 8.0, 
                   9.0, 10.0, 11.0, 12.0, 
                  13.0, 14.0, 15.0, 16.0);
	float gray,sum1=0.0,sum2=0.0,sum3=0.0,sum4=0.0;
	int i;


	sum1 += a[0][0];
	sum2 += a[1][0];
	sum3 += a[2][0];
	sum4 += a[3][0];
	
	sum1 += a[0][1];
	sum2 += a[1][1];
	sum3 += a[2][1];
	sum4 += a[3][1];
	
	sum1 += a[0][2];
	sum2 += a[1][2];
	sum3 += a[2][2];
	sum4 += a[3][2];
	
	sum1 += a[0][3];
	sum2 += a[1][3];
	sum3 += a[2][3];
	sum4 += a[3][3];

	if( ( sum1 > 10.0-ERROR_EPSILON && sum1 < 10.0+ERROR_EPSILON  ) && 
		( sum2 > 26.0-ERROR_EPSILON && sum2 < 26.0+ERROR_EPSILON) && 
		( sum3 > 42.0-ERROR_EPSILON && sum3 < 42.0+ERROR_EPSILON) && 
		( sum4 > 58.0-ERROR_EPSILON && sum4 < 58.0+ERROR_EPSILON) )
	gray=1.0;
	else gray=0.0;
	gl_FragColor = vec4(gray, gray, gray, 1.0);
}
