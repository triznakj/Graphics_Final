#version 330

uniform float time;
uniform mat4 T;

in vec2 pos;
in vec4 color;

out vec4 smoothColor;

void main()
{
	//TODO accept a mat4 for transformations
	//use the matrix to adjust gl_Position
	gl_Position = vec4(pos, 0, 1);
	gl_Position = T * gl_Position;
	smoothColor = color;
}