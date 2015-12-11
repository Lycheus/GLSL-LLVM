#define num 7
#define pi 3.14
attribute vec4 wave_Vertex;
//attribute vec3 wave_Normal;
//attribute vec4 wave_Tex;
uniform float time;
uniform float ami[7];
uniform float wl[7];
uniform float qq[7];
uniform float dirx[7];
uniform float diry[7];
uniform float s;
varying float facing;

uniform vec3 lightPosition; 
varying vec3 lightDir;
varying vec3 ecPosition;
varying float Intensity;

varying vec3 viewVec;
varying vec3 normal;

void main(void){
  vec4 v = vec4(wave_Vertex);

float total=0.0,total1=0.0,total2=0.0, norm1=0.0,norm2=0.0,norm3=0.0 ,binorm1=0.0,binorm2=0.0,binorm3=0.0, tg1=0.0,tg2=0.0,tg3=0.0;
  for(int i=0;i<7;i++){
    total=total+(qq[i]*ami[i]*dirx[i]*cos((2.0*3.14/wl[i])*(dirx[i]*v.x+diry[i]*v.y)+s*(2.0*3.14*time/wl[i])));  //sqrt(2.0*3.14*9.8/wl[i])
    total1=total1+(qq[i]*ami[i]*diry[i]*cos((2.0*3.14/wl[i])*(dirx[i]*v.x+diry[i]*v.y)+s*(2.0*3.14*time/wl[i])));
    total2=total2+(ami[i]*sin((2.0*3.14/wl[i])*(dirx[i]*v.x+diry[i]*v.y)+s*(2.0*3.14*time/wl[i])));

    norm1= norm1 + (dirx[i]*ami[i]*(2.0*3.14/wl[i])*cos(2.0*3.14/wl[i]*(dirx[i]*v.x+diry[i]*v.y)+s*(2.0*3.14*time/wl[i])));
    norm2= norm2 + (dirx[i]*ami[i]*(2.0*3.14/wl[i])*cos(2.0*3.14/wl[i]*(diry[i]*v.x+diry[i]*v.y)+s*(2.0*3.14*time/wl[i])));
    norm3= norm3 + (qq[i]*ami[i]*(2.0*3.14/wl[i])*sin(2.0*3.14/wl[i]*(dirx[i]*v.x+diry[i]*v.y)+s*(2.0*3.14*time/wl[i])));
    
    binorm1=binorm1 + (qq[i]*dirx[i]*dirx[i]*ami[i]*(2.0*3.14/wl[i])*sin(2.0*3.14/wl[i]*(dirx[i]*v.x+diry[i]*v.y)+s*(2.0*3.14*time/wl[i])));
    binorm2=binorm2 + (qq[i]*dirx[i]*diry[i]*ami[i]*(2.0*3.14/wl[i])*sin(2.0*3.14/wl[i]*(dirx[i]*v.x+diry[i]*v.y)+s*(2.0*3.14*time/wl[i])));
    binorm3=binorm3 + (dirx[i]*ami[i]*(2.0*3.14/wl[i])*cos(2.0*3.14/wl[i]*(dirx[i]*v.x+diry[i]*v.y)+s*(2.0*3.14*time/wl[i])));

    tg1=tg1 + (qq[i]*dirx[i]*diry[i]*ami[i]*(2.0*3.14/wl[i])*sin(2.0*3.14/wl[i]*(dirx[i]*v.x+diry[i]*v.y)+s*(2.0*3.14*time/wl[i])));
    tg2=tg2 + (qq[i]*diry[i]*diry[i]*ami[i]*(2.0*3.14/wl[i])*sin(2.0*3.14/wl[i]*(dirx[i]*v.x+diry[i]*v.y)+s*(2.0*3.14*time/wl[i])));
    tg3=tg3 + (diry[i]*ami[i]*(2.0*3.14/wl[i])*cos(2.0*3.14/wl[i]*(dirx[i]*v.x+diry[i]*v.y)+s*(2.0*3.14*time/wl[i])));
  }
  v.x = v.x+total;
  v.y = v.y+total1;
	v.z = total2;

  ecPosition = vec3(gl_ModelViewMatrix * v);
  viewVec=normalize(-ecPosition);

  vec3 binormal;
  vec3 tang;
  normal= vec3(-norm1, -norm2, 1.0-norm3);
  normal= normalize(gl_NormalMatrix*normal);
  tang= vec3(-tg1, 1.0-tg2, tg3);
  tang= normalize(gl_NormalMatrix*tang);
  binormal= vec3(1.0-binorm1, -binorm2, binorm3);
  binormal= normalize(gl_NormalMatrix*binormal);   //cross(normal,tang);
  
  facing=dot(normal,viewVec)/(length(normal)*length(viewVec));
  
  vec3 lightVec = normalize(lightPosition-ecPosition);
  vec3 reflectVec =reflect(-lightVec,normal);
  float spec = clamp(dot(reflectVec,viewVec),0.0,1.0);
  spec = pow(spec, 8.0);
  Intensity = 0.9* max(dot(lightVec,normal),0.0)+ 0.1*spec;

  gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_ModelViewProjectionMatrix * v;


   vec3 v2;
   v2.x = dot(lightPosition, tang);
   v2.y = dot(lightPosition, binormal);
   v2.z = dot(lightPosition, normal);
   lightDir = normalize(v2);

   v2.x = dot(ecPosition, tang);
   v2.y = dot(ecPosition, binormal);
   v2.z = dot(ecPosition, normal);
   ecPosition =  normalize(v2);

//	gl_Position=wave_Vertex;
} 
