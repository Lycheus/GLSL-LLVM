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

attribute vec4 gtf_Color;
attribute vec4 gtf_Vertex;
uniform mat4 gtf_ModelViewProjectionMatrix;
varying vec4 color;

void main (void)
{
	const float min_c = 0.25;
	const float max_c = 0.75;
	vec3 c = gtf_Color.rgb;
	if(c[0] > max_c) c[0] = max_c;
	if(c[0] < min_c) c[0] = min_c;
	if(c[1] > max_c) c[1] = max_c;
	if(c[1] < min_c) c[1] = min_c;
	if(c[2] > max_c) c[2] = max_c;
	if(c[2] < min_c) c[2] = min_c;

	color = vec4(c, 1.0);
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}
