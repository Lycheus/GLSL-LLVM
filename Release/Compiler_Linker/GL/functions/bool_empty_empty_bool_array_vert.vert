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

// Function declarations.
bool function(bool par[3]);
bool is_all(const in bool array[3], const in bool value);
void set_all(out bool array[3], const in bool value);

void main (void)
{
	bool par[3];
	bool ret = false;

	float gray = 0.0;

	// Initialize the entire array to true.
	set_all(par, true);

	ret = function(par);

	// The parameter should remain unchanged by the function and the function should return true.
	if(is_all(par, true) && ret)
	{
		gray = 1.0;
	}

	color = vec4(gray, gray, gray, 1.0);
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}

// Function definitions.
bool function(bool par[3])
{
	// Return the value of the array.
	if(is_all(par, true))
	{
		// Test parameter qualifier (default is "in").
		set_all(par, false);

		return true;
	}
	else
		return false;
}

bool is_all(const in bool array[3], const in bool value)
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

void set_all(out bool array[3], const in bool value)
{
	array[0] = value;
	array[1] = value;
	array[2] = value;
}
