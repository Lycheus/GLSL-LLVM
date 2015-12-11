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
// testing for char constants in #if and #elif
// Also checking whether reserved words can be redefined.

#define t1 c
#define t2 d
#define asm a

 #if(t1==c)
  #define t3 3
 #elif(t1==d)
  #define t4 4
 #elif(t2==c)
  #define t5 5
 #endif
 
 #ifndef t1
   #define t7 7
 #elif (t2==d)
  #define t6 6
 #endif

 #if (t2=='d')
  #define half 5
 #else
  #define half 8
 #endif

 #ifdef t22
  #define x 5
 #endif

 void main(void)
  {
   int sum =0,a=9;
   
   sum = half + sum;
   sum = asm + a;
   
  }
 
