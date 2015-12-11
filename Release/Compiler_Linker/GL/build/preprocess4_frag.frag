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
// #error and #pragma directives -- test cases.
// tests for errors in #pragma directive.

#pragma optimize(on)
#pragma debug(off)

int foo(int);

void main(void)
{
 int sum =0;
 #error ;
 #error 78
 #error c
 #error "message to the user "
 #error message to the user
 #error 
 #error
 #define t1 1
 sum = t1*t1;
 foo(sum);

}

#pragma optimize(off)
#pragma bind(on)
#pragma pack(off)

int foo(int test)
{
 int binding=0;
 binding = test;
 return binding;
}

#line 4
#pragma
#line 5 6
#pragma optmimize on
#pragma debug off
#pragma debug(off
#line 9
#prgma bind(off)
#pragma bind
#pragma (on)
#pragma on (on) 
#pragma optmize(on

 
