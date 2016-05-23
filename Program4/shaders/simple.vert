#version 330

uniform mat4 m;

in vec3 pos;
in vec3 colorIn;

smooth out vec4 smoothColor;

void main()
{	
	vec4 p = vec4(pos, 1);
	gl_Position = m*p;
	smoothColor = vec4(colorIn, 1);
}
