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

struct sabcd
{
	ivec2 a;
	ivec3 b;
	ivec4 c;
};

void main (void)
{
	sabcd s = sabcd( ivec2(12, 13), ivec3(14, 16, 19), ivec4(33, 42, 29, 122 ) );
		
	sabcd s2 = sabcd( ivec2(0, 0), ivec3(0, 0, 0), ivec4(0, 0, 0, 0 ) );
	s2 = s;		
	
	gl_FragColor = vec4( vec3(  float(s2.a[0] + s2.a[1] + s2.b[0] + s2.b[1] + s2.b[2] + s2.c[0] + s2.c[1] + s2.c[2] + s2.c[3]) / 300.0  ), 1.0);
}
