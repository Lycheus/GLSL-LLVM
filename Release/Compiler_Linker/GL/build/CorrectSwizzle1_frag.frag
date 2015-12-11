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
    vec2 v = vec2(1,5);
    // at the end of next statement, values in 
    // v.x = 12, v.y = 12
    v.xy += v.yx += v.xy; 
    // v1 and v2, both are initialized with 12
    vec2 v1 = v, v2 = v;
    
    v1.xy += v2.yx += ++(v.xy);  // v1 = 37, v2 = 25 each  
    v1.xy += v2.yx += (v.xy)++;  // v1 = 75, v2 = 38 each  
    gl_FragColor = vec4(v1,v2);  // 75, 75, 38, 38
}
