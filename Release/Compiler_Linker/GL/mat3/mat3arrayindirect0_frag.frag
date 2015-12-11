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
// mat3arrayindirect0_frag.frag: Fragment shader solid color 
// The vec3 values are determined at runtime.
//
// Copyright (c) 2004: 3Dlabs, Inc.
//

uniform mat3 testmat3[2];
varying vec4  color;

void main(void)
{
     vec3 result = vec3(0.0, 0.0, 0.0);
  
  /*
	// No indirect indexing in fragment shaders
     for(int j = 0; j < 3; j++)
     {
	result += testmat3[0][j] + testmat3[1][j];
     }
	*/
	result += testmat3[0][0] + testmat3[1][0];
	result += testmat3[0][1] + testmat3[1][1];
	result += testmat3[0][2] + testmat3[1][2];
     gl_FragColor = vec4(result/2.0, 0.5);
}
