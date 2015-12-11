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
    const vec4 v = vec4(normalize(vec4(1)));    // Builtin functions are constant expressions if all their parameters are constant expressions - code ok
    const vec4 v1 = vec4(clamp(1.0, .20, 3.0)); // Builtin functions are constant expressions if all their parameters are constant expressions - code ok
    float f = 1.0;
    const vec4 v2 = vec4(float(vec4(1,2,3,f))); // f is not constant - code fails and test does not compile (expected)

    gl_FragColor = v + v1 + v2;
}
