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
// tests for macro redifinition (t2) and the #if and #else nestings.
// takes care of elif also. 

#define t1 (1+2) 
#define t2 2
#define t2 3 

// testing the if depth
#if (t1==3)
  #define t3 3
  #if defined t2
    #define t4 4
      #if defined(t3)
          #define t5 5
             #ifdef t5
               #define t6 6
                 #ifndef t7
                   #define t7 7
                 #else
                   #define t8 8
                 #endif
             #endif
      #else
         #ifndef t8
             #define t8 8
         #elif (t8==8)
            #define t9 9
         #else
            #if defined t7
              #define t9 9
            #endif
         #endif
      #endif
  #else
    #define t10 10
  #endif
#endif


#define t8 8 
#define t9 9 
#define t10 10

void main(void) 
{
 int sum=1 ;
 sum = t1+t2;
 sum = t3+t4;
 sum = t5+t6;
 sum = t7+t8;
 sum = t9+t10;
}    
              
