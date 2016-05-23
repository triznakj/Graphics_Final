#version 330

//TODO upload a uniform sampler2D texture sampler id
uniform sampler2D texSampler;
in vec2 texMapping;

out vec4 fragColor;

void main()
{
	fragColor = vec4(0.1, 0.6, 0.8, 1.0);
	//fragColor = vec4(texMapping, 1, 0); //this might be helpful for testing
	fragColor = texture(texSampler, texMapping);
	//TODO use the texture() function with uploaded sampler id to get a texture color
}
