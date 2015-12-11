varying vec4 Color_Light;
uniform sampler2D Tex;
varying vec2 TexCoord;
varying vec3 normal;

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

	gl_FragColor=(gl_FragColor+Color_Light)/2;
}
