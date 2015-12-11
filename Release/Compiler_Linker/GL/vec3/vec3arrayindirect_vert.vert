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

attribute vec4 gtf_Vertex;
uniform mat4 gtf_ModelViewProjectionMatrix;
//
// vec3arrayindirect_vert.vert: Vertex shader solid color 
// The vec3 values are determined at runtime.
//
// Copyright (c) 2004: 3Dlabs, Inc.
//

uniform vec3 lightPosition[2];
varying vec4  color;

void main(void)
{
     color = vec4(0.0);

     for (int i = 0; i < 2; i++)
     {
          color += vec4(lightPosition[i], 0.0);
     }

     color /= 2.0;
    
     gl_Position     = gtf_ModelViewProjectionMatrix * gtf_Vertex;
   
  
}
