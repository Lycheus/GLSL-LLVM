#include "context.h"
#include "fx_Math.h"
#include <stdio.h>

#define DUMP_VERTEX_SHADER 1

#define num 7
#define pi 3.14
vec4 rm_Vertex;
//attribute vec3 wave_Normal;
//attribute vec4 wave_Tex;
float times;
float ami[num];
float wl[num];
float qq[num];
float dirx[num];
float diry[num];
float s;

float shaderr;

vec3 lightPosition; 

extern vec3 viewVec;
extern vec3 normal;
extern vec4 texcoord0;

mat4 ModelViewMatrix2;
mat4 ProjectionMatrix;
vec4 rm_Vertex;
vec4 rm_Normal;
vec4 texcoord;
mat4 matViewInverseTranspose;
//*****************useless*********************
mat4 view_matrix;
mat4 mvp;
vec3 vPositionES;
vec3 vNormalES;
//****************v*****************************
void vertex_shader_a()
{

#if DUMP_VERTEX_SHADER
	FILE *fp;
	if((fp = fopen("dump/dump_vertex_shader.txt", "a+"))==NULL){
		printf("open vertex_pre_RM file error!!\n");
		return ;
	}
	
	fprintf(fp,"---------------------------------------------------\n");
	fprintf(fp,"times:%f\n\n",times);
	fprintf(fp,"ami:%f %f %f %f %f %f %f\n\n",ami[0],ami[1],ami[2],ami[3],ami[4],ami[5],ami[6]);
	fprintf(fp,"wl:%f %f %f %f %f %f %f\n\n",wl[0],wl[1],wl[2],wl[3],wl[4],wl[5],wl[6]);
	fprintf(fp,"qq:%f %f %f %f %f %f %f\n\n",qq[0],qq[1],qq[2],qq[3],qq[4],qq[5],qq[6]);
	fprintf(fp,"dirx:%f %f %f %f %f %f %f\n\n",dirx[0],dirx[1],dirx[2],dirx[3],dirx[4],dirx[5],dirx[6]);
	fprintf(fp,"diry:%f %f %f %f %f %f %f\n\n",diry[0],diry[1],diry[2],diry[3],diry[4],diry[5],diry[6]);
	fprintf(fp,"s:%f\n\n",s);
	fprintf(fp,"shaderr:%f\n\n",shaderr);
	fprintf(fp,"lightPosition:%d %d %d\n\n",lightPosition[0],lightPosition[1],lightPosition[2]);
	
	fclose(fp);
#endif	


	//****************************float**********************************************
	vec4 temp1;

	if(shaderr==0)
	{
		vec4 v;
		float v1[4];
		int i;	
		float total=0.0,total1=0.0,total2=0.0, norm1=0.0,norm2=0.0,norm3=0.0 ,binorm1=0.0,binorm2=0.0,binorm3=0.0, tg1=0.0,tg2=0.0,tg3=0.0;
		vec3 temp;
		
		struct context *ctx = &mycontext;
		for(i=0;i<4;i++)
			v1[i]=fxtof(rm_Vertex[i]);
		
		for(i=0;i<7;i++){
			total=total+(qq[i]*ami[i]*dirx[i]*cos((2.0*pi/wl[i])*(dirx[i]*v1[0]+diry[i]*v1[1])+s*(2.0*pi*times/wl[i])));
			total1=total1+(qq[i]*ami[i]*diry[i]*cos((2.0*pi/wl[i])*(dirx[i]*v1[0]+diry[i]*v1[1])+s*(2.0*pi*times/wl[i])));
			total2=total2+(ami[i]*sin((2.0*pi/wl[i])*(dirx[i]*v1[0]+diry[i]*v1[1])+s*(2.0*pi*times/wl[i])));
	
			norm1= norm1 + (dirx[i]*ami[i]*(2.0*pi/wl[i])*cos(2.0*pi/wl[i]*(dirx[i]*(v1[0]+25)+diry[i]*(v1[1]+25))+s*(2.0*pi*times/wl[i])));
			norm2= norm2 + (dirx[i]*ami[i]*(2.0*pi/wl[i])*cos(2.0*pi/wl[i]*(diry[i]*(v1[0]+25)+diry[i]*(v1[1]+25))+s*(2.0*pi*times/wl[i])));
			norm3= norm3 + (qq[i]*ami[i]*(2.0*pi/wl[i])*sin(2.0*pi/wl[i]*(dirx[i]*(v1[0]+25)+diry[i]*(v1[1])+25)+s*(2.0*pi*times/wl[i])));
	
		}
	
		//v1[0] = v1[0]+total/10;
		//v1[1] = v1[1]+total1/10;
		v1[2] = total2/10;
	
		for(i=0;i<4;i++)
			v[i]=ftofx(v1[i]);

		vec3 ecPosition;
		mat3 MVM;
		for(i=0;i<3;i++)
		{
			MVM[i][0]=ModelViewMatrix2[i][0];
			MVM[i][1]=ModelViewMatrix2[i][1];
			MVM[i][2]=ModelViewMatrix2[i][2];
		}
	
	
	
		Mul_mat3_single(ecPosition,MVM,v);//fixed
	
	//	fprintf(fp,"ecPosition %f %f %f\n",fxtof(ecPosition[0]),fxtof(ecPosition[1]),fxtof(ecPosition[2]));
		for(i=0;i<3;i++)
			viewVec[i]=-ecPosition[i];
	//	fprintf(fp,"viewVec %f %f %f\n",fxtof(viewVec[0]),fxtof(viewVec[1]),fxtof(viewVec[2]));
		normalizef(viewVec,viewVec,3);
	//	fprintf(fp,"viewVec %f %f %f\n",fxtof(viewVec[0]),fxtof(viewVec[1]),fxtof(viewVec[2]));
	
	
		//normal= vec3(-norm1, -norm2, 1.0-norm3);
		//normal= normalize(gl_NormalMatrix*normal);
	
		normal[0]=ftofx(-norm1);
		normal[1]=ftofx(-norm2);
		normal[2]=ftofx(1.0-norm3);
	
		mat3 MVMI;
		for(i=0;i<3;i++)
		{
			MVMI[i][0]=matViewInverseTranspose[i][0];
			MVMI[i][1]=matViewInverseTranspose[i][1];
			MVMI[i][2]=matViewInverseTranspose[i][2];
		}
		vec3 tempnormal;
		Mul_mat3_single(tempnormal,MVMI,normal);

		for(i=0;i<3;i++)
			normal[i]=tempnormal[i];
		normalizef(normal,normal,3);

		Mul_mat4_single(temp1,ModelViewMatrix2,v);
		Mul_mat4_single(gl_Position,ProjectionMatrix,temp1);
	}
	else if(shaderr==1)
	{
		memcpy(&texcoord0, &texcoord, sizeof(texcoord0));
		Mul_mat4_single(temp1,ModelViewMatrix2,rm_Vertex);
		Mul_mat4_single(gl_Position,ProjectionMatrix,temp1);
		
	}
	else if(shaderr==2)
	{
		memcpy(&texcoord0, &texcoord, sizeof(texcoord0));
		Mul_mat4_single(temp1,ModelViewMatrix2,rm_Vertex);
		Mul_mat4_single(gl_Position,ProjectionMatrix,temp1);
	}
	else if(shaderr==3)
	{
	//	normalizef(rm_Vertex,rm_Vertex,4);
		memcpy(&texcoord0, &texcoord, sizeof(texcoord0));
		Mul_mat4_single(temp1,ModelViewMatrix2,rm_Vertex);
		Mul_mat4_single(gl_Position,ProjectionMatrix,temp1);
	}
	else if(shaderr==4)
	{
	//	normalizef(rm_Vertex,rm_Vertex,4);
		memcpy(&texcoord0, &texcoord, sizeof(texcoord0));
		Mul_mat4_single(temp1,ModelViewMatrix2,rm_Vertex);
		Mul_mat4_single(gl_Position,ProjectionMatrix,temp1);
	}
	else if(shaderr==5)
	{
		memcpy(&texcoord0, &texcoord, sizeof(texcoord0));
		Mul_mat4_single(temp1,ModelViewMatrix2,rm_Vertex);
		Mul_mat4_single(gl_Position,ProjectionMatrix,temp1);
	}

	
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
//! Inputs: matViewInverseTranspose, view_matrix, mvp, rm_Vertex, rm_Normal
#define DUMP_VERTEX_SHADER 1
//! Outputs: vPosition, vPositionES, vNormalES, gl_Position
//-----------------------------------
mat4 matViewInverseTranspose;
mat4 view_matrix;
mat4 mvp;


float timeflag;
vec4 rm_Vertex;
vec4 rm_Normal;
vec2 texcoord;

float shaderr;

vec3 vPositionES;
vec3 vNormalES;


mat4 ModelViewMatrix1;
mat4 ModelViewMatrix2;
mat4 ProjectionMatrix;


vec4 ambient_light, ambient_material;
vec4 diffuse_light, diffuse_material;
vec4 specular_light, specular_material;
vec3 eye_Pos;
vec4 light_Pos;
GLfixed specularExp;

extern vec4 texcoord0;
extern vec4 color_light;

void vertex_shader_a()
{
	int i;
	
	vec4 temp1,temp2;
	vec4 diffuse,ambient,lightDir,halfVector;
	vec3 n,halfV,normal;
	float NdotL,NdotHV;

	Mul_mat4_single(temp1,ModelViewMatrix1,rm_Vertex);
	Mul_mat4_single(temp2,ModelViewMatrix2,temp1);
	Mul_mat4_single(gl_Position,ProjectionMatrix,temp2);

	struct context *ctx = &mycontext;

//	Mul_mat4_single(gl_Position, mvp, rm_Vertex);
	
//	Mul_mat4_single(a, ModelViewMatrix, rm_Vertex);

//	memcpy(&vPositionES, &a,sizeof(vPositionES));

	memcpy(&texcoord0, &texcoord, sizeof(texcoord0));
	
//	TransformNormalIntoEyeCoords(vNormalES,ctx->ModelviewMatrixStack.Top->inv,rm_Normal);
	Mul_mat4_single(temp1,ModelViewMatrix1,rm_Normal);
	TransformNormalIntoEyeCoords(normal,ctx->ModelviewMatrixStack.Top->inv,temp1);
//	TransformNormalIntoEyeCoords(normal,ctx->ModelviewMatrixStack.Top->inv,a);
	
	

	for(i=0;i<3;i++)
	{
		
		lightDir[i]=light_Pos[i];
		diffuse[i]=Mulfx(diffuse_material[i], diffuse_light[i]);
		ambient[i]=Mulfx(ambient_material[i], ambient_light[i]);
		halfVector[i]=light_Pos[i]+eye_Pos[i];
	}
	normalizef(halfVector,halfVector,3);

///////////////////////////////////////////////////////////////////////////////////////////
	
	

	for(i=0;i<4;i++)
		color_light[i]=ambient[i];

	normalizef(n,normal,4);

	NdotL = dot(n,lightDir,3);
	if(NdotL<0)
		NdotL=0;

	if(NdotL>0)
	{
		normalizef(halfV,halfVector,3);
		NdotHV = dot(n,halfV);
		if(NdotHV<0)
			NdotHV=0;
		for(i=0;i<4;i++)
			color_light[i]+=Mulfx(specular_material[i] , Mulfx(specular_light[i] , ftofx(pow(fxtof(NdotHV), fxtof(specularExp)))));
			color_light[i]+=Mulfx(diffuse[i],NdotL);
	}

	for(i=0;i<4;i++)
		color_light[i]=color_light[i]/3;

#if DUMP_VERTEX_SHADER
	FILE *fp;
	if((fp = fopen("dump_vertex_shader.txt", "a+"))==NULL){
		printf("open vertex_pre_RM file error!!\n");
		return ;
	}

	fprintf(fp,"---------------------------------------------------\n");
	fprintf(fp,"normal:%d %d %d %d\n\n",normal[0],normal[1],normal[2],normal[3]);
	fprintf(fp,"lightDir:%d %d %d %d\n\n",lightDir[0],lightDir[1],lightDir[2],lightDir[3]);
	fprintf(fp,"diffuse:%d %d %d %d\n\n",diffuse[0],diffuse[1],diffuse[2],diffuse[3]);
	fprintf(fp,"ambient:%d %d %d %d\n\n",ambient[0],ambient[1],ambient[2],ambient[3]);
	fprintf(fp,"halfVector:%d %d %d %d\n\n",halfVector[0],halfVector[1],halfVector[2],halfVector[3]);
	fprintf(fp,"color_light:%d %d %d %d\n\n",color_light[0],color_light[1],color_light[2],color_light[3]);

	
	fclose(fp);
#endif

	
}
*/
