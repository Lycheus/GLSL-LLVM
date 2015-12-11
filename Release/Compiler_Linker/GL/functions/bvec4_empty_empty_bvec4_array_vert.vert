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
bvec4 function(bvec4 par[3]);
bool is_all(const in bvec4 par, const in bool value);
bool is_all(const in bvec4 array[3], const in bvec4 value);
void set_all(out bvec4 array[3], const in bvec4 value);

void main (void)
{
	bvec4 par[3];
	bvec4 ret = bvec4(false, false, false, false);

	float gray = 0.0;

	// Initialize the entire array to true.
	set_all(par, bvec4(true, true, true, true));

	ret = function(par);

	// The parameter should remain unchanged by the function and the function should return true.
	if(is_all(par, bvec4(true, true, true, true)) && is_all(ret, true))
	{
		gray = 1.0;
	}

	color = vec4(gray, gray, gray, 1.0);
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}

// Function definitions.
bvec4 function(bvec4 par[3])
{
	// Return the value of the array.
	if(is_all(par, bvec4(true, true, true, true)))
	{
		// Test parameter qualifier (default is "in").
		set_all(par, bvec4(false, false, false, false));

		return bvec4(true, true, true, true);
	}
	else
		return bvec4(false, false, false, false);
}

bool is_all(const in bvec4 par, const in bool value)
{
	bool ret = true;

	if(par[0] != value)
		ret = false;
	if(par[1] != value)
		ret = false;
	if(par[2] != value)
		ret = false;
	if(par[3] != value)
		ret = false;

	return ret;
}

bool is_all(const in bvec4 array[3], const in bvec4 value)
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

void set_all(out bvec4 array[3], const in bvec4 value)
{
	array[0] = value;
	array[1] = value;
	array[2] = value;
}
