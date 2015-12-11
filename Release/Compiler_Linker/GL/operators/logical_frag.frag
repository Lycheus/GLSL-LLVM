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

void main (void)
{
bool result = true;
	bool a = true;
	bool b = true;
	
	if( (a&&b) )
		result = result && true;
	else 
		result = result && false;
	
	if( (a||b) )
		result = result && true;
	else 
		result = result && false;

	if( !(a^^b) )
		result = result && true;
	else
		result = result && false;

	a = true;
	b = false;
	
	if( !(a&&b) )
		result = result && true;
	else 
		result = result && false;
	
	if( (a||b) )
		result = result && true;
	else 
		result = result && false;

	if( (a^^b) )
		result = result && true;
	else
		result = result && false;

	a = false;
	b = true;

	if( !(a&&b) )
		result = result && true;
	else 
		result = result && false;
	
	if( (a||b) )
		result = result && true;
	else 
		result = result && false;

	if( (a^^b) )
		result = result && true;
	else
		result = result && false;

	a = false;
	b = false;

	if( !(a&&b) )
		result = result && true;
	else 
		result = result && false;
	
	if( !(a||b) )
		result = result && true;
	else 
		result = result && false;

	if( !(a^^b) )
		result = result && true;
	else
		result = result && false;

	float gray;
	if( result )
	gray=1.0;
	else gray=0.0;
	gl_FragColor = vec4(gray, gray, gray, 1.0);
}