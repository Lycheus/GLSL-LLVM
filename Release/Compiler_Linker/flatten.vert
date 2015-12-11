#define num 7
#define pi 3.14
attribute vec4 wave_Vertex;
attribute vec3 wave_Normal;
attribute vec4 wave_Tex;
uniform float time;
uniform float ami[num];
uniform float wl[num];
uniform float qq[num];
uniform float dirx[num];
uniform float diry[num];
uniform float s;

uniform float shaderr;

uniform vec3 lightPosition; 
uniform mat3 NewModelViewMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;

varying vec3 viewVec;
varying vec3 normal;
varying vec4 texcoord0;

void main(void){
	if(shaderr == 0.0)
	{
		float total=0.0,total1=0.0,total2=0.0, norm1=0.0,norm2=0.0,norm3=0.0 ,binorm1=0.0,binorm2=0.0,binorm3=0.0, tg1=0.0,tg2=0.0,tg3=0.0;

		vec4 v = vec4(wave_Vertex);
		for(int i=0;i<num;i++){
			//total=total+(qq[i]*ami[i]*dirx[i]*cos((2.0*pi/wl[i])*(dirx[i]*v.x+diry[i]*v.y)+s*(2.0*pi*time/wl[i])));  //sqrt(2.0*pi*9.8/wl[i])
			//total1=total1+(qq[i]*ami[i]*diry[i]*cos((2.0*pi/wl[i])*(dirx[i]*v.x+diry[i]*v.y)+s*(2.0*pi*time/wl[i])));
			total2=total2+(ami[i]*sin((2.0*pi/wl[i])*(dirx[i]*v.x+diry[i]*v.y)+s*(2.0*pi*time/wl[i])));

			norm1= norm1 + (dirx[i]*ami[i]*(2.0*pi/wl[i])*cos(2.0*pi/wl[i]*(dirx[i]*(v.x+25.0)+diry[i]*(v.y+25.0))+s*(2.0*pi*time/wl[i])));
			norm2= norm2 + (dirx[i]*ami[i]*(2.0*pi/wl[i])*cos(2.0*pi/wl[i]*(diry[i]*(v.x+25.0)+diry[i]*(v.y+25.0))+s*(2.0*pi*time/wl[i])));
			norm3= norm3 + (qq[i]*ami[i]*(2.0*pi/wl[i])*sin(2.0*pi/wl[i]*(dirx[i]*(v.x+25.0)+diry[i]*(v.y+25.0))+s*(2.0*pi*time/wl[i])));
		}
		//v.x = v.x+total;
		//v.y = v.y+total1;
		v.z = total2/10.0;

		//vec3 ecPosition = vec3(ModelViewMatrix * v);
		vec3 ecPosition = vec3(v);
		ecPosition = NewModelViewMatrix * ecPosition;		 
		viewVec=normalize(-ecPosition);

		vec3 binormal;
		vec3 tang;
		normal= vec3(-norm1, -norm2, 1.0-norm3);
		normal= normalize(NormalMatrix*normal);
			
		gl_Position =  ProjectionMatrix * ModelViewMatrix * v;
	}
	else if(shaderr==1.0)
	{
		texcoord0 = wave_Tex;
		gl_Position =  ProjectionMatrix * ModelViewMatrix * wave_Vertex;
	}
	else if(shaderr==2.0)
	{
		texcoord0 = wave_Tex;
		gl_Position =  ProjectionMatrix * ModelViewMatrix * wave_Vertex;
	}
	else if(shaderr==3.0)
	{
		texcoord0 = wave_Tex;
		gl_Position =  ProjectionMatrix * ModelViewMatrix * wave_Vertex;
	}
	else if(shaderr==4.0)
	{
		texcoord0 = wave_Tex;
		gl_Position =  ProjectionMatrix * ModelViewMatrix * wave_Vertex;
	}
	else if(shaderr==5.0)
	{
		texcoord0 = wave_Tex;
		gl_Position =  ProjectionMatrix * ModelViewMatrix * wave_Vertex;
	}
} 
