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
// simple macro expansions.
// Tests for Too few macro arguments, too many macro arguments.
// Macros with no arguments.

#define t1 -1
#define t2 2

#define test -258
#define test1 (test*test)
#define test2(x) (x+test1)
#define test3() (test2(8)*(test*test1))
#define test4(x,y) (x+y)

void main(void)
{
 int sum =0;
 sum = test3();
 sum = test3(3);

 sum = test2(9);
 sum = test2(9,8);

 sum = test4;
 sum = test2(8,5,78,9);
 sum = sum + test1;
 sum = 8+58+sum;
 sum = sum +test;
 sum = (t1+t2);
 sum = test4(test3(),test2(test3())); 
 sum = test4(3,8,5);
 sum = test4();
}
