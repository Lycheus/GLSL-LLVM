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
#define t1 2.3333333333333333
#define t2 (0.978293600-1.0)
#define t3 .9090909090
#define t4 26578235.000000083487
#define t5 78e-03
#define t6 78.100005E+05
#define t7 6278.78e-5

void main(void){
    float tes=2e-3;
    float test=3.2e-5;
    float test1=0.99995500;
    float test2=6789.983;

    test = t1+t2;
    test = t3-t4;
    tes  = t5 * t6;
    test2 = t7;
    
    gl_FragColor = vec4(test, tes, test1, test2);
}
