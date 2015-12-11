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

attribute vec4 gtf_Vertex;
uniform mat4 gtf_ModelViewProjectionMatrix;
varying vec4 color;

struct sabcd
{ 
 mat4 a;
};

void main (void)
{
	sabcd s = sabcd(mat4(12.0, 29.0, 13.0, 26.0, 
			     71.0, 63.0, 90.0, 118.0, 
			     128.0, 44.0, 57.0, 143.0, 
			     151.0, 14.0, 15.0, 21.0 ) );
	float sum=0.0;
	
	sum = sum + s.a[0][0];
	sum = sum + s.a[0][1];
	sum = sum + s.a[0][2];
	sum = sum + s.a[0][3];
	
	sum = sum + s.a[1][0];
	sum = sum + s.a[1][1];
	sum = sum + s.a[1][2];
	sum = sum + s.a[1][3];
	
	sum = sum + s.a[2][0];
	sum = sum + s.a[2][1];
	sum = sum + s.a[2][2];
	sum = sum + s.a[2][3];
	
	sum = sum + s.a[3][0];
	sum = sum + s.a[3][1];
	sum = sum + s.a[3][2];
	sum = sum + s.a[3][3];
		 
	color = vec4( vec3(  sum / 995.0  ), 1.0);
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}
