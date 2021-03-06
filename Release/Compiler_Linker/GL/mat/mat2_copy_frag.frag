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

/* This epsilon will work as long as the magnitude of the float is < 128.
 * This can be seen by taking the spec relative mediump precision of 2^-10:
 * 0.125 / 2^-10 = 128
 */
#define ERROR_EPSILON (0.125)

void main (void)
{
	float x;
	// Declare a 2 by 2 matrix with unique elements.
	mat2 a = mat2(1.0, 2.0,   // 1.0 4.0
	              4.0, 8.0);  // 2.0 8.0

	// Copy the matrix to another non-const matrix.
	mat2 b = a;

	// Check each element of the copy.
	bool elms = true;
	if(b[0][0] != 1.0) elms = false;
	if(b[0][1] != 2.0) elms = false;
	if(b[1][0] != 4.0) elms = false;
	if(b[1][1] != 8.0) elms = false;

	// Add up each row of the copy.
	bool rows = true;
	x = b[0][0] + b[1][0];
	if(x < 5.0-ERROR_EPSILON || x > 5.0+ERROR_EPSILON) rows = false;
	x = b[0][1] + b[1][1];
	if(x < 10.0-ERROR_EPSILON || x > 10.0+ERROR_EPSILON) rows = false;

	// Add up each column of the copy.
	bool cols = true;
	x = b[0][0] + b[0][1];
	if(x < 3.0-ERROR_EPSILON || x > 3.0+ERROR_EPSILON) cols = false;
	x = b[1][0] + b[1][1];
	if(x < 12.0-ERROR_EPSILON || x > 12.0+ERROR_EPSILON) cols = false;

	// Check if all of the operations were successful.
	float gray = elms && rows && cols ? 1.0 : 0.0;

	// Assign the fragment color.
	gl_FragColor = vec4(gray, gray, gray, 1.0);
}

