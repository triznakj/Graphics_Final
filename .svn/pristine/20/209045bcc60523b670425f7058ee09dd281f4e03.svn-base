#version 330

uniform mat4 P;
uniform mat4 C;

in vec3 pos;
in vec4 vertColor;

out vec4 smoothColor;

void main()
{
	vec4 p = vec4(pos, 1);

	smoothColor = vertColor;
	gl_Position = P*C*p;
}
