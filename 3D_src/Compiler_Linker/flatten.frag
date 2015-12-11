varying float facing;
varying vec3 lightDir;
varying vec3 ecPosition;

uniform vec3 surfaceColor;
uniform float BumpDensity;
uniform float BumpSize;
uniform float SpecularFactor;

uniform sampler2D waterTexture;
varying float Intensity;

varying vec3 viewVec;
varying vec3 normal;
void main(void)
{
  vec4 watercolor= vec4(0.16, 0.59, 0.79, 1.0);
  vec4 waterhighcolor= vec4(0.0, 0.22, 0.47, 1.0);
  vec4 vColor;

float eDotN = dot(viewVec, normal);
vec3 vEyeReflected = 2.0* eDotN * normal - viewVec;
vec4 reflection = texture2D(waterTexture, vec2(vEyeReflected));
float Fresnel = (1.0 - eDotN)*reflection.g ;
vec4 diffuse = watercolor * 0.5 * eDotN;
vec4 color = clamp(diffuse + Fresnel*(reflection * 1.2 * eDotN-diffuse) + reflection * 0.3, 0.0 , 1.0 );
vec4 specular = waterhighcolor * pow(reflection.g,8.0) * eDotN ;
vColor = saturate(color + specular);
  gl_FragColor = vec4(vColor);
//	gl_FragColor = vec4(1.0,0.0,0.0,0.0);
}