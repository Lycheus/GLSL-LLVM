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
varying vec4 color;

void main (void)
{
	int setval = 30;
	
	if(--setval!=29)
		if( (setval+=11) == 40)
			if(setval/4 == 11)
				setval = 11;
			else if(setval/4 == 10)
				if(setval-3 == 37)
					setval=12;
				else setval = 9;
			else setval = 10;
		else setval = 30;
	else if(setval == 29)
		if((setval+=19) != 48)
			setval = 13;
		else if((setval+=19) == 29)
			setval = 28;
		else setval = 53;
	else setval = 32;
	float gray;
	if( setval == 53 )
	gray=1.0;
	else gray=0.0;
	gl_FragColor = vec4(gray, gray, gray, 1.0);
}
