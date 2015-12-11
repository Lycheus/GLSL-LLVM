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

attribute vec4 gtf_Color;
attribute vec4 gtf_Vertex;
uniform mat4 gtf_ModelViewProjectionMatrix;
//
// vec3array_vert.vert: Simple vertex shader using vec3 to get colors.
//
//

varying vec4 color;
uniform vec3 lightPosition[2];

void main(void)
{
    vec3 v[2];
  
    v[1] = vec3(gtf_Color.r, gtf_Color.g, gtf_Color.b);

    v[0] = lightPosition[1];
	
    color =  vec4(v[1] + v[1], 0.0)/2.0;
    gl_Position     = gtf_ModelViewProjectionMatrix * gtf_Vertex;
}
