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
// #line directive-- test cases.
// chks for Invalid directives, all possible #line errors
// Also checks the correct verions of #line dorective.

#define t1 1
#define t2 2

#
#
#
#
#line 8
#line "" 
#line 3 3

#linekfj
#line c c 
#line t1 t2
#line 77 89
#line 65.4 
#line message to the user
#line
#line345

void main(void)
{
 int sum =1;
 sum = __LINE__;
 sum = __FILE__;
 #line 4 5
 sum = __LINE__;
 sum = __FILE__;
 #line 9
 sum = __LINE__ + __FILE__ ;
 sum = __FILE__;
 #
 #
 sum = __VERSION__;
 sum = sum + __LINE__ ;
 #line 4 5
 #line 5 8
 sum = __LINE__;
 sum = __FILE__;
 sum = __VERSION__;

}

 

