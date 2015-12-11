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
struct s {
    float f;
    vec3 v;
};

void main()
{
    const vec4 v = (vec4(1,2,3,4), vec4(5,6,7,8));  // 5,6,7,8
    const s s1 = (s(9.0, vec3(10,11,12)), s(13.0, vec3(14,15,16))); // 13,14,15,16
    gl_FragColor = v + vec4(s1.f, s1.v);  // 18, 20, 22, 24
}
