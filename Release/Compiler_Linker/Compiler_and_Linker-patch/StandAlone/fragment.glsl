//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Simple fragment shaders to test my simple ES triangle app.
//
//=================================================================================================================================
// $Id:  $ 
// 
// Last check-in:  $DateTime:  $ 
// Last edited by: $Author:  $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================
/* Dondum //this */
//precision mediump float;

uniform sampler2D tex0;
varying vec4 texcoord0;
varying vec4 outcolor;

vec4 testTexture()
{
   return texture2D(tex0, texcoord0.xy);
}

vec4 testColorVarying()
{
   return outcolor;
}

void main()
{    
   gl_FragColor = testTexture();
   //gl_FragColor = testColorVarying();
}
