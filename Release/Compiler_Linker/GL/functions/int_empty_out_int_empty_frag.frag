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

// Function declaration.
int function(out int par);

void main (void)
{
	int par = 1;
	int ret = 0;

	float gray = 0.0;

	ret = function(par);

	// The parameter should be changed by the function and the function should return 1.
	if((par == 0) && (ret == 1))
	{
		gray = 1.0;
	}

	gl_FragColor = vec4(gray, gray, gray, 1.0);
}

// Function definition.
int function(out int par)
{
	// Test parameter qualifier (default is "in").
	par = 0;

	return 1;
}
