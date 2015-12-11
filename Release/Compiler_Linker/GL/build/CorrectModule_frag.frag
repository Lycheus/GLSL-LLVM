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
vec4 test_function4(float);
vec4 test_function1(float);
vec4 test_function2(float);
vec4 test_function3(float);
float f = 2.6;


vec4 test_function1(float ff)
{
    vec4 func_vec4 = vec4(ff+f);
    return func_vec4;
}

float f1 = 1.5;

vec4 test_function4(float ff)
{
    vec4 func_vec4 = vec4(f1);
    return func_vec4;
}

float f2 = 3.5;

void main()
{
    vec4 v1 = test_function4(f2);
    vec4 v2 = test_function1(f2);
    vec4 v3 = test_function2(f2);
    vec4 v4 = test_function3(f2);
    
    if (f1 > f2) {
        gl_FragColor = v1 + v2 + v3 + v4;
    } else
        gl_FragColor = v1 + v2 + v3 + v4;
}

float f4 = 5.5;
vec4 test_function3(float ff)
{
    if (ff > f4) 
	return vec4(ff);
    else
        return vec4(f4);
}

float f3 = 4.5;
vec4 test_function2(float ff)
{
    vec4 func_vec4 = vec4(ff+f3);
    return func_vec4;
}

float f5 = 6.5;
