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
struct s{
    float f;
    vec3  v;
} s1 ;
void main()
{
    vec4 v = vec4(float(vec2(1,2)), 5,6,7);  // 1, 5, 6, 7
    vec4 v1 = vec4(3, vec2(ivec2(1,2)), 4);  // 3, 1, 2, 4
    vec4 v2 = vec4(8, 9, vec4(ivec4(1,2,3,4))); // 8,9, 1,2 
    vec2 v3 = vec2(v2);  // 8,9
    vec4 v4 = vec4(v3, v2.z, v2.w);  // 8,9,1,2

    const vec4 v5 = vec4(2.0, s(2.0, vec3(3,4,5)).v); // 2,3,4,5
    gl_FragColor = v5 + v + v1 + v4 ;  // 14, 18, 13, 18
}
