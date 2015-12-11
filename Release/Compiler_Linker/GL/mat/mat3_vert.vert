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

#define ERROR_EPSILON 0.1

void main (void)
{
	float x;
	// Declare a 3 by 3 matrix with unique elements.
	mat3 a = mat3( 1.0,   2.0,   4.0,   // 1.0  8.0  64.0
	               8.0,  16.0,  32.0,   // 2.0 16.0 128.0
	              64.0, 128.0, 256.0);  // 4.0 32.0 256.0

	// Check each element.
	bool elms = true;
	if(a[0][0] !=   1.0) elms = false;
	if(a[0][1] !=   2.0) elms = false;
	if(a[0][2] !=   4.0) elms = false;
	if(a[1][0] !=   8.0) elms = false;
	if(a[1][1] !=  16.0) elms = false;
	if(a[1][2] !=  32.0) elms = false;
	if(a[2][0] !=  64.0) elms = false;
	if(a[2][1] != 128.0) elms = false;
	if(a[2][2] != 256.0) elms = false;

	// Add up each row.
	bool rows = true;
	x = a[0][0] + a[1][0] + a[2][0];
	if( x < 73.0-ERROR_EPSILON || x > 73.0+ERROR_EPSILON ) rows = false;
	x = a[0][1] + a[1][1] + a[2][1]; 
	if(x < 146.0-ERROR_EPSILON || x > 146.0+ERROR_EPSILON ) rows = false;
	x = a[0][2] + a[1][2] + a[2][2];
	if(x < 292.0-ERROR_EPSILON || x > 292.0+ERROR_EPSILON ) rows = false;

	// Add up each column.
	bool cols = true;
	x = a[0][0] + a[0][1] + a[0][2];
	if( x < 7.0-ERROR_EPSILON || x > 7.0+ERROR_EPSILON ) cols = false;
	x = a[1][0] + a[1][1] + a[1][2];
	if(x < 56.0-ERROR_EPSILON || x > 56.0+ERROR_EPSILON) cols = false;
	x = a[2][0] + a[2][1] + a[2][2];
	if(x < 448.0-ERROR_EPSILON || x > 448.0+ERROR_EPSILON) cols = false;

	// Check if all of the operations were successful.
	float gray = elms && rows && cols ? 1.0 : 0.0;

	// Assign the varying variable color.
	color = vec4(gray, gray, gray, 1.0);

	// Transform the vertex position.
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}

