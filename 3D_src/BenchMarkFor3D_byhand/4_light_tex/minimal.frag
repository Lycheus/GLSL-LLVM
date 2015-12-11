
uniform vec4 Color_RGB;
uniform sampler2D tex0;

varying vec2 texcoord0;

varying vec4 Color_Light;


void main()
{
	vec4 Color_Texture = texture2D(tex0,texcoord0);
//	gl_FragColor = vec4(1.0,0.0,0.0,0.0);
	gl_FragColor = Color_Texture*0.3 + Color_RGB*0.2 + Color_Light*0.5 ;
//	gl_FragColor = (Color_Texture + Color_RGB + Color_Light )/3 ;
//	gl_FragColor = Color_Light;
}
