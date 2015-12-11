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
// operator precedence and some macro expansions.

#define test (1+2)
#define test1 (test*4)
#define test2 (test1/test)
//#define test3 (-1+2*3/4%test)
#define test3 (-1+2*3/4)
//#define test4 (test & test1 |test2)
#define test4 (test)
#define test5 (!8+~4+4-6)
#define test6 (test1>>1)
#define test7 (test1<<1)
#define test8 (test2^6)
#define test9 (test4 || test5 && test1)
#define test10 (0)

void main(void)
{
 int sum =0;
 sum = test4;
 sum = test3*test2+test1-test;
// sum = test3/test6 + test4*test7 - test7 % test9;
// sum = test3/test6 + test4*test7 - test7;
 sum = test10*test5;
}

