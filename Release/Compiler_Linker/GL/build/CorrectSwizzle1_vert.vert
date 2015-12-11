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

attribute vec4 gtf_Normal;
uniform mat4 gtf_NormalMatrix;
void main(void)
{
   vec4 v = vec4(1,2,3,4);
   vec3 v3 = vec3(5,6,7);
   vec4 v4  = vec4(normalize(v3.yzy).xyz.zyx, 1.0);
   gl_Position = v4 + vec4(normalize(gtf_NormalMatrix * gtf_Normal).xyz.zyx, v4.y);
}
