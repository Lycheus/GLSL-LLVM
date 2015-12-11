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
	float a;
	float b;
	float c;
	float d;
};



void main (void)
{
	sabcd s = sabcd(1.0, 2.0, 4.0, 8.0);
	sabcd s2 = sabcd(0.0, 0.0, 0.0, 0.0);
	s2 = s;
	gl_FragColor = vec4((s.a + s.b + s.c + s.d) / 15.0, (s2.a + s2.b + s2.c + s2.d) / 15.0, 1.0, 1.0);
}
