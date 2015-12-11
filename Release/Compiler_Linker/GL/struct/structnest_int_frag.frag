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
struct nestb
{
	int b;
};

struct nesta
{
	int a;
	nestb nest_b;
};

struct nest
{
	nesta nest_a;
};

void main (void)
{
	nest s = nest(nesta(1, nestb(12) )  );
	gl_FragColor = vec4( vec3(  (s.nest_a.a + s.nest_a.nest_b.b) / 13  ), 1.0);
}
