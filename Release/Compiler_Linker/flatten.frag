uniform sampler2D waterTexture;

varying vec3 viewVec;
varying vec3 normal;
varying vec2 texcoord0;
uniform float shaderr;

void main(void)
{

	if(shaderr==0.0)
	{
		vec4 watercolor= vec4(0.39, 0.71, 0.95, 1.0);
		vec4 waterhighcolor= vec4(0.5, 0.9, 0.95, 1.0);
		vec4 vColor;

		float eDotN = dot(viewVec, normal);
		vec3 vEyeReflected = 2.0* eDotN * normal - viewVec;
		vec4 reflection = texture2D(waterTexture, vec2(vEyeReflected));
		float Fresnel = (1.5 - eDotN)*reflection.g ;
		
		vec4 diffuse = watercolor * 0.5 * eDotN;
		
		vec4 color = clamp(diffuse + Fresnel*(reflection * 1.2 * eDotN-diffuse) + reflection * 0.3, 0.0 , 1.0 );
		
		vec4 specular = waterhighcolor * pow(reflection.g,8.0) * eDotN ;
		
		//vColor = saturate(color + specular);
		vColor = clamp(color + specular, 0.0, 1.0);
		
		gl_FragColor = vec4(0.9*vColor.x+0.1, 0.65*vColor.y+0.35, 0.62*vColor.z+0.38, vColor.w);
		//	gl_FragColor = vec4(1.0,0.0,0.0,0.0);
	}
	else if(shaderr==1.0)
	{
		gl_FragColor = texture2D(waterTexture,texcoord0);
	}
	else if(shaderr==2.0)
	{
		gl_FragColor = texture2D(waterTexture,texcoord0);
		gl_FragColor = gl_FragColor * 0.3 + 0.07;
	 }
	else if(shaderr==3.0)
	{
		gl_FragColor = vec4(1.0, 0.97, 0.75, 1.0);
	}
	else if(shaderr==4.0)
	{
		gl_FragColor = texture2D(waterTexture,texcoord0);
		gl_FragColor.x = gl_FragColor.x * 0.1 + 0.9;
		gl_FragColor.y = gl_FragColor.y * 0.1 + 0.9;
		gl_FragColor.z = gl_FragColor.z * 0.1 + 0.9;
		gl_FragColor.w = gl_FragColor.w * 0.1 + 0.45;
	}
	else if(shaderr==5.0)
	{
		gl_FragColor = texture2D(waterTexture,texcoord0);
		//gl_FragColor.x = gl_FragColor.x * 0.8 + 0.1;
		//gl_FragColor.y = gl_FragColor.y * 0.8 + 0.1;
		//gl_FragColor.z = gl_FragColor.z * 0.8 + 0.2;
		//gl_FragColor.w = gl_FragColor.w * 0.8 + 0.1;
	}
}