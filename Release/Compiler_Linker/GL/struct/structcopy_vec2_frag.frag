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
	vec2 a;
	vec2 b;
};


void main (void)
{
	sabcd s1 = sabcd(vec2(12.0, 29.0), vec2(13.0, 26.0) );
	sabcd s2 = sabcd(vec2(0.0, 0.0), vec2(0.0, 0.0) );
	s2 = s1;
	gl_FragColor =  vec4( vec3(  (s2.a[0] + s2.a[1] + s2.b[0] + s2.b[1]) / 80.0  ), 1.0);
}
