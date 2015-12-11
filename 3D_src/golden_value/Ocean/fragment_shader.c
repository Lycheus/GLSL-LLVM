#include "context.h"
#include "fx_Math.h"
#include <stdio.h>
#include <math.h>

vec3 viewVec;
vec3 normal;
sampler2D tex0;
vec4 texcoord0;
extern float shaderr;
int run;

void fragment_shader_a()
{
//	printf("shaderr = %f\n",shaderr);

	if(shaderr==0)
	{
//		printf("shaderr = %f\n",shaderr);
		float watercolor[4];
		float waterhighcolor[4];
		vec4 vColor;
		int i;
		struct context *ctx = &mycontext;
	
		char filename[1024];
		FILE *fp;
	
		watercolor[0]=0.39;
		watercolor[1]=0.71;
		watercolor[2]=0.95;
		watercolor[3]=1.0;
	
		waterhighcolor[0]=0.5;
		waterhighcolor[1]=0.9;
		waterhighcolor[2]=0.95;
		waterhighcolor[3]=1.0;
	
		GLfixed eDotN = dot(viewVec, normal,3); //fixed
	
		vec3 vEyeReflected;
		for(i=0;i<3;i++)
			vEyeReflected[i] = ftofx(2.0 * fxtof(eDotN) * fxtof(normal[i]) - fxtof(viewVec[i]));
	
		vec2 tempv;
		tempv[0]=vEyeReflected[0];
		tempv[1]=vEyeReflected[1];
		vec4 reflection;
		texture2D(reflection,tex0, tempv);
		float Fresnel = (1.5 - fxtof(eDotN))*fxtof(reflection[1]);
	
		float diffuse[4];
		vec4 diffusefixed;
		for(i=0;i<4;i++)
			diffuse[i] = watercolor[i] * 0.5 * fxtof(eDotN);//float
	
		vec4 color;
		for(i=0;i<4;i++)
			color[i] = clamp(ftofx(diffuse[i] + Fresnel*(fxtof(reflection[i]) * 1.2 * fxtof(eDotN) - diffuse[i]) + fxtof(reflection[i]) * 0.3), 0, 65536 );
	
		vec4 specular;
		for(i=0;i<4;i++)
			specular[i] = ftofx(waterhighcolor[i] * pow(fxtof(reflection[i]),8) * fxtof(eDotN));//fixed
	
		for(i=0;i<4;i++)
			vColor[i] = clamp(color[i] + specular[i], 0, 65536);
	
	/*	for(i=0;i<4;i++)
			gl_FragColor[i] =0.9*vColor[i]+65536*0.1;*/
		
		gl_FragColor[0] =0.9*vColor[0]+65536*0.1;
		gl_FragColor[1] =0.65*vColor[1]+65536*0.35;
		gl_FragColor[2] =0.62*vColor[2]+65536*0.38;
		if(gl_FragColor[2]>65536)
			gl_FragColor[2]=65536;
		else if(gl_FragColor[2]<0)
			gl_FragColor[2]=0;
		
//		gl_FragColor[2] =clamp(vColor[2]*0.8 + 65536*0.2 , 0, 65536);
	//*/
	}
	else if(shaderr==1)
	{
		texture2D(gl_FragColor, tex0, texcoord0);
	}
	else if(shaderr==2)
	{
		int i;
		texture2D(gl_FragColor, tex0, texcoord0);
		gl_FragColor[0] =gl_FragColor[0]*0.3+0.1*65536*0.7;
		gl_FragColor[1] =gl_FragColor[1]*0.3+0.1*65536*0.7;
		gl_FragColor[2] =gl_FragColor[2]*0.3+0.1*65536*0.7;
		//gl_FragColor[3] =gl_FragColor[3];
	}
	else if(shaderr==3)
	{
	//	texture2D(gl_FragColor, tex0, texcoord0);
		gl_FragColor[0]=65536*1.0;
		gl_FragColor[1]=65536*1.0*0.97;
		gl_FragColor[2]=65536*0.75;
		gl_FragColor[3]=65536*1.0;
	}
	else if(shaderr==4)
	{
		texture2D(gl_FragColor, tex0, texcoord0);
		gl_FragColor[0]=gl_FragColor[0]*0.1+65536*1.0*0.9;
		gl_FragColor[1]=gl_FragColor[1]*0.1+65536*1.0*0.9;
		gl_FragColor[2]=gl_FragColor[2]*0.1+65536*1.0*0.9;
		gl_FragColor[3]=gl_FragColor[3]*0.1+65536*0.5*0.9;
	}
	else if(shaderr==5)
	{
		texture2D(gl_FragColor, tex0, texcoord0);
		gl_FragColor[0]=gl_FragColor[0]*0.8+65536*0.5*0.2;
		gl_FragColor[1]=gl_FragColor[1]*0.8+65536*0.5*0.2;
		gl_FragColor[2]=gl_FragColor[2]*0.8+65536*1.0*0.2;
		gl_FragColor[3]=gl_FragColor[3]*0.8+65536*0.5*0.2;
	}
}

/*
void main(void)
{
  vec4 watercolor= vec4(0.16, 0.59, 0.79, 1.0);
  vec4 waterhighcolor= vec4(0.0, 0.22, 0.47, 1.0);
  vec4 vColor;

float eDotN = dot(viewVec, normal);
vec3 vEyeReflected = 2.0* eDotN * normal - viewVec;
vec4 reflection = texture2D(waterTexture, vec2(vEyeReflected));
float Fresnel = (1.0 - eDotN)*reflection.g ;
vec4 diffuse = watercolor * 0.5 * eDotN;
vec4 color = clamp(diffuse + Fresnel*(reflection * 1.2 * eDotN-diffuse) + reflection * 0.3, 0.0 , 1.0 );
vec4 specular = waterhighcolor * pow(reflection.g,8.0) * eDotN ;
vColor = saturate(color + specular);
  gl_FragColor = vec4(vColor);
//	gl_FragColor = vec4(1.0,0.0,0.0,0.0);
}
*/
/*
//#include "fx_Math.h"
//#include <stdio.h>
//#include <math.h>

//! Inputs: granite_color, light_color, Ks, Ka, Kd, light_Pos, global_ambient

//! Outputs: gl_FragColor
//-------------------------------------
vec4 granite_color;
vec4 light_color;
vec4 Ks;
vec4 Ka;
vec4 Kd;
//vec4 light_Pos;
vec4 global_ambient;
extern float LdotV;
extern vec3 vPosition;
extern vec3 vPositionES;
extern vec3 vNormalES;
//extern float shaderr;


sampler2D tex0;
vec4 texcoord0;
//-------------------------------

vec4 color_RGB;
vec4 color_light;

void fragment_shader_a()
{
	int i;	

	vec4 color_Texture;
	texture2D(color_Texture, tex0, texcoord0);

	for(i=0; i<3; i++){
		gl_FragColor[i] =  color_light[i]*0.5 + color_Texture[i]*0.3 + color_RGB[i]*0.2 ;
	//	gl_FragColor[i] = color_light[i];
	}

}

*/


