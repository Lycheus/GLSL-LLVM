uniform vec4 Color_RGB;
uniform sampler2D tex0;

varying vec2 texcoord0;

void main()
{

//	vec4 Color1 = vec4(0.0,0.0,0.0,0.0);
//	gl_FragColor = vec4(1.0,0.0,0.0,0.0);
	
	vec4 Color_Texture = texture2D(tex0,texcoord0);
	gl_FragColor = 0.8*Color_Texture + 0.2*Color_RGB;
}
