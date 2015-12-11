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

attribute vec4 gtf_Vertex;
uniform mat4 gtf_ModelViewProjectionMatrix;
varying vec4 color;

#define ERROR_EPSILON 0.1

void main (void)
{
	struct second_nest
	{
	   float sc_nt;
	   mat2 sc_mt2;
	   vec4 sc_vc4;
	};

	struct nest
	{
	   ivec3 nt_ivc3;
	   bvec4 nt_bvc4;
	   second_nest nt_scne;
	};

	struct test_t
	{
	   float t_fl; 
	   vec2 t_vc2;
	   vec3 t_vc3;
	   mat4 t_mt4;
           nest t_nested;
	   float t_2fl;
	};
	
	vec4 mt1 = vec4(31.0, 32.0, 33.0, 34.0);
	vec4 mt2 = vec4(35.0, 36.0, 37.0, 38.0);
	vec4 mt3 = vec4(39.0, 40.0, 41.0, 42.0);
	vec4 mt4 = vec4(43.0, 44.0, 45.0, 46.0);
	int i=0;
	float sum1=0.0, sum2=0.0, sum3=0.0, sum4=0.0;

	test_t a = test_t(23.0, vec2(12.0, 13.0), 
		          
	                  vec3(163.0, 173.0, 183.0), 

		          mat4(mt1, mt2, mt3, mt4),

		                nest( ivec3(73, 74, 75), 

			        bvec4(12, 0, 17.0, 193.0 ), 

			            second_nest(144.0,  mat2(22.0, 23.0, 24.0, 25.0),  vec4(57.0, 58.0, 59.0, 60.0 ) 
				    ) 
				), 

			  203.0  
		    );

	sum1 =  a.t_mt4[0][0] + a.t_mt4[0][1] + a.t_mt4[0][2] + a.t_mt4[0][3];
	sum2 =  a.t_mt4[1][0] + a.t_mt4[1][1] + a.t_mt4[1][2] + a.t_mt4[1][3];
	sum3 =  a.t_mt4[2][0] + a.t_mt4[2][1] + a.t_mt4[2][2] + a.t_mt4[2][3];
	sum4 =  a.t_mt4[3][0] + a.t_mt4[3][1] + a.t_mt4[3][2] + a.t_mt4[3][3];
	
	float gray;
	if( ( a.t_fl == 23.0 ) && 

	    (a.t_vc2[0] == 12.0) && (a.t_vc2[1] == 13.0) && 
	    
	    (a.t_vc3[0] == 163.0)  && (a.t_vc3[1] == 173.0)  && (a.t_vc3[2] == 183.0) &&  
            
            (sum1 > 130.0-ERROR_EPSILON && sum1 < 130.0+ERROR_EPSILON )  && (sum2 > 146.0-ERROR_EPSILON && sum2 < 146.0+ERROR_EPSILON  )  && (sum3 >162.0-ERROR_EPSILON && sum3 < 162.0+ERROR_EPSILON ) && (sum4 > 178.0-ERROR_EPSILON && sum4 < 178.0+ERROR_EPSILON ) && 
                (a.t_nested.nt_ivc3[0] == 73 ) && (a.t_nested.nt_ivc3[1] == 74 ) && (a.t_nested.nt_ivc3[2] == 75 ) &&
                
	        (a.t_nested.nt_bvc4[0] == true) && (a.t_nested.nt_bvc4[1] == false) && 
 
                (a.t_nested.nt_bvc4[2] == true ) && (a.t_nested.nt_bvc4[0] == true) &&
                     
                     (a.t_nested.nt_scne.sc_nt == 144.0) && 
                     
                     (a.t_nested.nt_scne.sc_mt2[0][0] == 22.0 ) && (a.t_nested.nt_scne.sc_mt2[0][1] == 23.0 ) && 

                     (a.t_nested.nt_scne.sc_mt2[1][0] == 24.0 ) && (a.t_nested.nt_scne.sc_mt2[1][1] == 25.0 ) &&
	              
                     (a.t_nested.nt_scne.sc_vc4[0] == 57.0 ) && (a.t_nested.nt_scne.sc_vc4[1] == 58.0 ) && 

                     (a.t_nested.nt_scne.sc_vc4[2] == 59.0 ) && (a.t_nested.nt_scne.sc_vc4[3] == 60.0) && 
           
            (a.t_2fl == 203.0) 
          )
	gray=1.0;
	else gray=0.0;

	color = vec4(gray, gray, gray, 1.0);
	gl_Position = gtf_ModelViewProjectionMatrix * gtf_Vertex;
       
}
