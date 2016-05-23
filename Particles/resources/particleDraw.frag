#version 330

in vec4 pos;

layout(location = 0) out vec4 fragColor;

void main()
{	
	fragColor = normalize(pos)*0.5+0.5;
}

