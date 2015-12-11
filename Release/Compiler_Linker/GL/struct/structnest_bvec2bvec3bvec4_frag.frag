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

struct nestb
{
	bvec2 a2;
	bvec3 b2;
	bvec4 c2;
};

struct nesta
{
	bvec2 a1;
	bvec3 b1;
	bvec4 c1;
	nestb nest_b;
};

struct nest
{
	nesta nest_a;
};

void main (void)
{
	nest s = nest( nesta( bvec2(12, 13), bvec3(14.0, 0.0, 139.0), bvec4(25.5, 17.0, 145, 163 ), 
                       nestb( bvec2(28, 0), bvec3(0.0, 0.0, 1.0), bvec4(0.0, 17.0, 145, 0 ) 
                            ) 
                            ) 
                      );
	float gray = 0.0;
	if( ( s.nest_a.a1[0] ) && ( s.nest_a.a1[1] ) &&
            ( s.nest_a.b1[0] ) && (! (s.nest_a.b1[1]) ) && ( s.nest_a.b1[2] ) && 
            ( s.nest_a.c1[0] ) && ( s.nest_a.c1[1] ) && ( s.nest_a.c1[2] ) && ( s.nest_a.c1[3] ) && 
            ( s.nest_a.nest_b.a2[0] ) && ( !( s.nest_a.nest_b.a2[1] ) ) && 
            (! ( s.nest_a.nest_b.b2[0] ) ) && (! ( s.nest_a.nest_b.b2[1] ) ) && (s.nest_a.nest_b.b2[2]) && 
            (! ( s.nest_a.nest_b.c2[0] ) ) && (s.nest_a.nest_b.c2[1]) && (s.nest_a.nest_b.c2[2]) && (! ( s.nest_a.nest_b.c2[3] ) ) 
          )
	  gray=1.0;
	else 
          gray =0.0;

	gl_FragColor = vec4(gray, gray, gray, 1.0);
}