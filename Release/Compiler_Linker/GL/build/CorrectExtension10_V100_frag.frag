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

#version 100
#extension extensionfoo : enable  // warning extension not supported 
#extension extensionfoo : disable  // warning extension not supported 
#extension extensionfoo : warn  // warning extension not supported 

#extension all : disable // no error in the program
#extension all : warn // no error in the program

#extension extensionfoo : enable  // warning extension not supported 
#extension extensionfoo : disable  // warning extension not supported 
#extension extensionfoo : warn  // warning extension not supported 
#ifdef GL_ES
precision mediump float;
#endif

void main()
{
}