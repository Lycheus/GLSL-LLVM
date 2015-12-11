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

vec3 bar(vec3, vec3);

uniform vec2 v;

bool foo(out vec3);

void main()
{
    bool b1, b2, b3, b4, b5, b6;
    
    b1 = any(lessThan(v, v));

    b2 = all(lessThanEqual(v, v));
        
    b3 = any(not(greaterThan(v, v)));
        
    b4 = any(greaterThanEqual(v, v));
        
    b5 = any(notEqual(v, v));
        
    b6 = any(equal(v, v));
 
    vec2 u;   
    if (b1 && b2 && b3 && b4 && b5 && b6)
        u = v;
    
    gl_Position = vec4(u, u);
}
