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

const float FloatConst1 = 3.0 * 8.0, floatConst2 = 4.0;
const bool BoolConst1 = true && true || false;
const bool BoolConst2 = false || !false && false;

void main (void)
{
    float float1 = 4.0, float2 = floatConst2;
    int int_1 = int(FloatConst1);
    vec4 vec4_1;
    vec3 vec3_1 = vec3(1, 1, 1);
	vec3 vec3_2 = vec3(0, 0, 0);
//  unsigned int unsigned_int_1;
    bool bool4, bool5;

    bool4 = bool5;
    //float1 = bool5;
    //bool5 = float1;

    bool4 = 4 > 5;
    bool4 = !(3.2 != 0.0);
    bool4 = bool(float1);
    bool4 = bool(int_1);
    float1 = float(bool4);
    float1 = float(int_1);
    int_1 = int(float1);
    int_1 = int(bool4); 

    {
        int a, b, c;
        
        a = b;
        b = c;
        {
            int b, c, d;

            b = c;
            c = d;
            {
                int a, d, e;
                
                a = d;
                d = e;
            }
            {
                int a, b, c;
                a = b;
                b = c;
            }
        }
        a = b;
        b = c;
    }

    {
        float f1, f2;
        vec3 v31, v32;

        max(f1, f2);
        max(v31, v32);

        vec4 v4 = vec4(3.0);
        vec3 v3 = -vec3(2.0, 1.0, 3.0);
        mat2 m2 = mat2(3.0, 4.0, 6.0, 3.0);
        //mat4 m4 = mat4(1.0, m2, v3, v4, m2);
    }

    if (BoolConst1)
        ++vec3_1;
    else
        --vec3_1;

    if (BoolConst2)
        ++vec3_1;
    else
        --vec3_1;

    if (BoolConst1 || BoolConst2)
        ++vec3_1;
    else
        --vec3_1;

    if (BoolConst2 && BoolConst1)
        ++vec3_1;
    else
        --vec3_1;

    if (FloatConst1 != 0.0)
        --int_1;
    else
        ++int_1;

    if (0 != 0)
        ++int_1;
    else
        --int_1;

    bool4 = BoolConst1 && ! (int_1 != 0) && ! BoolConst1  && ! (FloatConst1 != 0.0) && (FloatConst1 != 0.0) && (float1 != 0.0);
    
    float1 = 5 != 0 ? float1 : float(int_1);
    float1 = BoolConst1 ? float1 : float(int_1);

    if (float1 != float1)
        ++int_1;
    else
        --int_1;

    float1 = float1 != float1 ? float1 : float(int_1);

    --int_1;
    ++float1;
    (vec4_1.x)--;
    vec3_1++;

    if (vec3_1.x > vec3_2.x)
        float1 = 4.0 + 6.0;

    if (bool4 ^^ bool5)
        float1 *= 2.4;

    if (false ^^ false)
        float1 *= 2.5;

    if (true ^^ false)
        float1 *= 2.6;

    {
        int i;
    }

    if (bool4) {
        int i;
    } else {
        int i;
        i = 5;
    }

    mat4 m1;
    m1[2][1] = 4.0;

    gl_Position = vec4(float1+float1, float1, float1, float(int_1));
}
