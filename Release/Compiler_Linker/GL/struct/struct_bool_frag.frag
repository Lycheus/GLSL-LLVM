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
struct sabcd
{
	bool a;
	bool b;
	bool c;
	bool d;
};



void main (void)
{
	sabcd s = sabcd(bool(12), bool(0), bool(25.5), bool(0.0));
	float gray = 0.0;
	if( (s.a==true) && (s.b==false) && (s.c == true) && (s.d==false))
	  gray=1.0;
	else 
          gray =0.0;
	
	gl_FragColor = vec4(gray, gray, gray, 1.0);
}
