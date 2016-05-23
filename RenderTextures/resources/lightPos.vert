#version 330

uniform mat4 P;  //projection matrix
uniform mat4 C;  //camera matrix
uniform mat4 L;  //light rotation matrix

uniform vec4 lightPos;

in vec3 pos;
in vec3 colorIn;

smooth out vec4 smoothColor;

void main()
{    
	//hacky easy way to draw the light position
	gl_Position = P*C*L*lightPos;
	smoothColor = vec4(1,1,1,1);
}
