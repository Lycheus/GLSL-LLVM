varying vec2 TexCoord;
uniform sampler2D Tex;
varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector;

uniform vec4 specular_light, specular_material;
uniform float specularExp;

uniform int shaderr;

void main()
{
	if(shaderr==1)
	{
		gl_FragColor = texture2D(Tex,TexCoord);
	}
	else
	{
		vec3 temp_normal;
		temp_normal=normal+1;

		temp_normal=normalize(temp_normal);
		
		gl_FragColor[0]=temp_normal[0];
		gl_FragColor[1]=temp_normal[1];
		gl_FragColor[2]=temp_normal[2];
		gl_FragColor[3]=1;
	}

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

/*		color_light +=  gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV,gl_FrontMaterial.shininess);
		color_light += diffuse * NdotL;*/
	}
	color_light=color_light/3;
	gl_FragColor=(gl_FragColor+color_light)/2;
}
