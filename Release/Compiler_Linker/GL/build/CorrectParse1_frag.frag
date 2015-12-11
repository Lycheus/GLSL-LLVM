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
uniform vec3 a[8];

uniform bool ub;
varying mat4 vm;

int foo(float);

float bar(int i)
{
    return float(i);
}

void main (void)
{
    const int x = 3;
    mat4 a[4]; 
    vec4 v;

    for (float f = 0.0; f != 3.0; ++f)
    {
    }

    vec3 v3[x + x];

    int vi = foo(2.3);

    vec3 v3_1 = v3[x];

    float f1 = a[x][2].z * float(x);  
    f1 = a[x][2][2] * float(x);
    f1 = v[2] * v[1];

    const int ci = 2;

}

int foo(float f)
{
    return 2;
}
