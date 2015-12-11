attribute vec4 position;
attribute vec4 color;
attribute vec4 texcoord;

uniform mat4 mvp;
varying vec4 texcoord0;
varying vec4 outcolor;

void main()
{
	gl_Position = mvp*position;
	outcolor = color;
	texcoord0 = texcoord;
}

