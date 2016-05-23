#version 330

layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = gl_FragCoord;
}
