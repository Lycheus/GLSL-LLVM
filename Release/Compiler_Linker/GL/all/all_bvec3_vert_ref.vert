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
attribute vec4 gtf_Color;
uniform mat4 gtf_ModelViewProjectionMatrix;
varying vec4 color;

bool _all(in bvec3 a)
{
	bool temp = true;
	
	if(!a[0]) temp = false;
	if(!a[1]) temp = false;
	if(!a[2]) temp = false;

	return temp;
}

void main (void)
{
	vec3 c = floor(4.0 * gtf_Color.rgb);   // 3/4 true, 1/4 false
	color = vec4(vec3(_all(bvec3(c))), 1.0);
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}
