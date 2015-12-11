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
	mat3 m1 = mat3(gtf_Color.rgb, gtf_Color.rgb, gtf_Color.rgb);
	mat3 m2 = mat3(1.0, 0.5, 0.5, 0.5, 1.0, 0.5, 0.5, 0.5, 1.0);
	mat3 m3 = mat3(0.0);
	vec3 result = vec3(0.0, 0.0, 0.0);

	m3 = matrixCompMult(m1, m2);

	result[0] += m3[0][0];
	result[0] += m3[0][1];
	result[0] += m3[0][2];
	
	result[1] += m3[1][0];
	result[1] += m3[1][1];
	result[1] += m3[1][2];
	
	result[2] += m3[2][0];
	result[2] += m3[2][1];
	result[2] += m3[2][2];

	color = vec4(result / 2.0, 1.0);
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}
