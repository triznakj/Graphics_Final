#version 330

uniform mat4 P;
uniform mat4 C;

in vec2 pos;
in vec2 texCoord;

out vec2 texMapping;

void main()
{
	vec4 p = vec4(pos, 0, 1);

	texMapping = texCoord;
	gl_Position = P*C*p;
}
