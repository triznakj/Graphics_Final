#version 330

smooth in vec4 smoothColor;

layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = smoothColor;
}
