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
//test for else elif mismatch
#define test(x,y) (x+y)

void  main(void){
 int sum =0;
 #define x 8
 #endif
 #if (x==8)
   #undef x
 #endif
 
 #if 1
   #undef x
 #endif

 #if 1 
   #define t4 4
 #endif

 sum=test(3,6)+t4;
 #if 1
  #if 1
   #if 1
    #if 1
     #if 0
       #undef test
     #else
       #if 1
         #undef test
       #endif
       #if 0 
        #undef test
       #else
          #if 0
             #undef test
          #else
             #if 1 
                #undef test
             #else
               #undef test
             #else
               #jdhgj
             #endif
          #endif
       #endif
     #endif
    #endif
   #endif
  #endif
 #endif 
            
}
