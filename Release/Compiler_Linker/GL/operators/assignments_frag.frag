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
	int m = 12;
	int n = 102;
	bool result = true;
	int r = m;	

	if( r==12 ) 
		result = result && true;
	else 
		result = result && false;
	
	r += m;

	if( r == 24 )
		result = result && true;
	else 
		result = result && false;

	r-= m;

	if( r == 12 )
		result = result && true;
	else 
		result = result && false;

	r*= m;

	if ( r == 144 )
		result = result && true;
	else
		result = result && false;

	r/= m;
	
	// Integer divide can be implemented via float reciprocal,
	// so the result need not be exact
	if( r >= 11 && r <= 13 )
		result = result && true;
	else 
		result = result && false;

	float gray;
	if( result )
	gray=1.0;
	else gray=0.0;
	gl_FragColor = vec4(gray, gray, gray, 1.0);
}
