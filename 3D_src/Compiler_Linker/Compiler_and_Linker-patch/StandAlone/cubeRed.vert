attribute vec4 rm_Vertex;

uniform mat4 ModelViewMatrix1;


void main()
{		
	gl_Position =  gl_ProjectionMatrix * gl_ModelViewMatrix * ModelViewMatrix1 * rm_Vertex;
}
