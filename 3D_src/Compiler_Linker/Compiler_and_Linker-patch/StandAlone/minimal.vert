attribute vec4 rm_Vertex;
attribute vec3 rm_Normal;
attribute vec2 cube_texs;

varying vec4 Color_Light;
varying vec2 TexCoord;
varying vec3 normal;

uniform int shaderr;
uniform float timeflag;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

uniform vec4 ambient_light, ambient_material;
uniform vec4 diffuse_light, diffuse_material;
uniform vec3 eye_Pos;
uniform vec3 light_Pos;
uniform vec4 specular_light, specular_material;
uniform float specularExp;

void main()
{
	vec4 temp_vertex=rm_Vertex;
	if(shaderr==1)
	{
		TexCoord = cube_texs;
	}
	else
	{		
		temp_vertex=normalize(temp_vertex);
		temp_vertex.w=1;
		temp_vertex.y=temp_vertex.y-(timeflag);

		if (temp_vertex.y<-2.52587) {

			temp_vertex.y=-2.52587;

			vec3 temp;

			temp[0]=rm_Normal.x;
			temp[1]=rm_Normal.y;
			temp[2]=1;

			temp=normalize(temp);

			temp_vertex.x=temp_vertex.x+temp[0]*0.3;
			temp_vertex.z=temp_vertex.z+temp[1]*0.3;	
		}	
	}
	
	vec4 ambient,diffuse;
	vec3 lightDir,halfVector;
	
	normal = normalize(NormalMatrix * rm_Normal);
	lightDir = normalize(light_Pos);
	halfVector = normalize(light_Pos+eye_Pos);
	diffuse = diffuse_light * diffuse_material;
	ambient = ambient_light * ambient_material;

	vec3 n,halfV,viewV,ldir;
	float NdotL,NdotHV;
	vec4 color_light = ambient;

	n = normalize(normal);

	NdotL = max(dot(n,lightDir),0.0);

	if (NdotL > 0.0) {
		halfV = normalize(halfVector);
		NdotHV = max(dot(n,halfV),0.0);

		color_light += specular_light * specular_material * pow(NdotHV,specularExp);
		color_light += diffuse * NdotL;
	}
	Color_Light=color_light/3;

	gl_Position =  ProjectionMatrix * ModelViewMatrix * temp_vertex;


//	lightDir = normalize(gl_LightSource[0].position);
//	halfVector = normalize(gl_LightSource[0].halfVector.xyz);
//	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
//	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;

}
