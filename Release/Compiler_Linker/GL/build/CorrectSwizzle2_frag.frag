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
void main()
{
    float f, f1, f2;
    f = f1 = f2;
    f += f1 += f2;
    
    vec4 v, v1, v2;
    v = v1 = v2;
    v += v1 += v2;
    v.wx = v1.zx = v2.yx;
    v.wx += v1.zx += v2.yx;

    mat4  m, m1, m2;
    m = m1 = m2;
    m += m1 += m2;
    m[3].wx = m1[2].zx = m2[1].yx;
    m[3].wx += m1[2].zx += m2[1].yx;

    mat4  am[4], am1[4], am2[4];
    am[3] = am1[2] = am2[1];
    am[3] += am1[2] += am2[1];
    am[3][3].wx = am1[2][2].zx = am2[1][1].yx;
    am[3][3].wx += am1[2][2].zx += am2[1][1].yx;
    am[3][3].wx += am1[2][2].zx += ++(am2[1][1].yx);
    am[3][3].wx += am1[2][2].zx += (am2[1][1].yx)++;

    gl_FragColor = vec4(am[3][3].z, m[3].w, v.w, f);
}
