#version 330

uniform mat4 Lp;  //light projection matrix
uniform mat4 mT;
uniform mat4 mR;
uniform mat4 M;  //modelview matrix: M = C * mR * mT
uniform mat4 Lv; //light view matrix
uniform mat4 Lr;  //light rotation matrix

uniform vec4 lightPos;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 colorIn;

vec4 positionInLightView(vec3 pos)
{
	//TODO: move vertex, put in light view

	return vec4(pos, 1);
}

void main()
{
	gl_Position = positionInLightView(pos);
}
