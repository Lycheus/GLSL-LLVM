attribute vec4 rm_Vertex;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewMatrix1;


void main()
{	
	//gl_Position =  rm_Vertex * ModelViewMatrix1 * ModelViewMatrix * ProjectionMatrix;
	gl_Position =  ProjectionMatrix * ModelViewMatrix * ModelViewMatrix1 * rm_Vertex;
}
