
attribute vec4 rm_Vertex;
attribute vec3 rm_Normal;
attribute vec2 cube_texs;

varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector;
varying vec2 TexCoord;
uniform int shaderr;
uniform float timeflag;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

uniform vec4 ambient_light, ambient_material;
uniform vec4 diffuse_light, diffuse_material;
uniform vec3 eye_Pos;
uniform vec3 light_Pos;

void main()
{
	if(shaderr==1)
	{
		TexCoord = cube_texs;
	}
	else
	{		
		rm_Vertex=normalize(rm_Vertex);
		rm_Vertex.w=1;
		rm_Vertex.y=rm_Vertex.y-(timeflag);

		if (rm_Vertex.y<-2.52587) {

			rm_Vertex.y=-2.52587;

			vec3 temp;

			temp[0]=rm_Normal.x;
			temp[1]=rm_Normal.y;
			temp[2]=1;

			temp=normalize(temp);

			rm_Vertex.x=rm_Vertex.x+temp[0]*0.3;
			rm_Vertex.z=rm_Vertex.z+temp[1]*0.3;	
		}	
	}
	
	normal = normalize(NormalMatrix * rm_Normal);
	lightDir = normalize(light_Pos);
	halfVector = normalize(light_Pos+eye_Pos);
	diffuse = diffuse_light * diffuse_material;
	ambient = ambient_light * ambient_material;
	
	gl_Position =  ProjectionMatrix * ModelViewMatrix * rm_Vertex;


//	lightDir = normalize(gl_LightSource[0].position);
//	halfVector = normalize(gl_LightSource[0].halfVector.xyz);
//	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
//	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;

}
