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

// Function declarations.
int function(in int par[3]);
bool is_all(const in int array[3], const in int value);
void set_all(out int array[3], const in int value);

void main (void)
{
	int par[3];
	int ret = 0;

	float gray = 0.0;

	// Initialize the entire array to 1.
	set_all(par, 1);

	ret = function(par);

	// The parameter should remain unchanged by the function and the function should return 1.
	if(is_all(par, 1) && (ret == 1))
	{
		gray = 1.0;
	}

	gl_FragColor = vec4(gray, gray, gray, 1.0);
}

// Function definition.
int function(in int par[3])
{
	// Return the value of the array.
	if(is_all(par, 1))
	{
		// Test parameter qualifier (default is "in").
		set_all(par, 0);

		return 1;
	}
	else
		return 0;
}

bool is_all(const in int array[3], const in int value)
{
	bool ret = true;

	if(array[0] != value)
		ret = false;
	if(array[1] != value)
		ret = false;
	if(array[2] != value)
		ret = false;

	return ret;
}

void set_all(out int array[3], const in int value)
{
	array[0] = value;
	array[1] = value;
	array[2] = value;
}
