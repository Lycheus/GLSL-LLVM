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



struct sabcd
{
	float a;
	float b;
	float c;
	float d;
};



sabcd qualifiers(in sabcd a, out sabcd b, inout sabcd c, const in sabcd d,
sabcd e)
{
        sabcd one = sabcd(1.0, 1.0, 1.0, 1.0);
        
        b = a;

        c.a += d.a;
        c.b += d.b;
        c.c += d.c;
        c.d += d.d;

        a.a += one.a;
        a.b += one.b;
        a.c += one.c;
        a.d += one.d;

        return e;
}

void main (void)
{
	sabcd a = sabcd(1.0, 1.0, 1.0, 1.0);
	sabcd b = sabcd(2.0, 2.0, 2.0, 2.0);
	sabcd c = sabcd(3.0, 3.0, 3.0, 3.0);
	sabcd d = sabcd(4.0, 4.0, 4.0, 4.0);
	sabcd e = sabcd(1.0, 1.0, 1.0, 1.0);
	sabcd f = sabcd(0.0, 0.0, 0.0, 0.0);
	sabcd one = sabcd(1.0, 1.0, 1.0, 1.0);
	sabcd four = sabcd(4.0, 4.0, 4.0, 4.0);
	sabcd seven = sabcd(7.0, 7.0, 7.0, 7.0);
	float q = 0.0;
	float q2 = 0.0;

	f = qualifiers(a, b, c, d, e);

	if(a == one) q += 1.0;
	if(b == one) q += 2.0;
	if(c == seven) q += 4.0;
	if(d == four) q2 += 1.0;
	if(e == one) q2 += 2.0;
	if(f == one) q2 += 4.0;

	color = vec4(vec2(q / 7.0, q2 / 7.0), 1.0, 1.0);
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}
