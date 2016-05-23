#version 330

uniform mat4 P;  //projection matrix
uniform mat4 C;  //camera matrix
uniform mat4 Lr;  //light rotation matrix

uniform vec4 lightPos;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 colorIn;

smooth out vec4 smoothColor;

void main()
{    
	//hacky easy way to draw the light position
	gl_Position = P*C*Lr*lightPos;
	smoothColor = vec4(1,1,1,1);
}
