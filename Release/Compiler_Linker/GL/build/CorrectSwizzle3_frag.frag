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
    vec4 v = vec4(5,6,7,8);
    // value changes for lhs
    // 8765, 6758, 857, 75 i.e. replace v.zx
    // value changes for rhs
    // 8765, 6758, 86 i.e replace with v.wy
    // replace v.z with v.w
    // replace v.x with v.y
    // add 1.000000 to v.w and v.y
    v.wzyx.zywx.wzy.zy = (v.wzyx.zywx.wx)++;
    gl_FragColor = vec4(v);  // 6,7,8,9
}
