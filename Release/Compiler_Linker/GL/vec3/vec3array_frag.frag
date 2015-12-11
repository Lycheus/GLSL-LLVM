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
//
// vec3array_frag.frag: Simple Fragment shader using vec3 to get colors.
//
//

varying vec4 color;

uniform vec3 lightPosition[2];

void main(void)
{
    vec3 v[2];
  
    v[1] = vec3(color.r, color.g, color.b);
	

    v[0] = lightPosition[1];
	

    gl_FragColor =  vec4(v[1] + v[1], 0.0)/2.0;
}
