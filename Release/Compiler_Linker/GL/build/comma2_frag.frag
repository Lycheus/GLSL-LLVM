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
    const vec4 v = (vec4(1,2,3,4), vec4(5,6,7,8), 1.2); // right most value of comma operator shoul be a vec4
    const vec4 v1 = (vec3(0.2, 2.0), vec4(1,2,3,4), vec4(5,6,7,8)); 
    const vec4 v2 = (vec4(1,2,3,4), vec2(2.1, 2),  vec4(5,6,7,8));  
    gl_FragColor = v + v1 + v2;
}
