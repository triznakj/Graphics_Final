#version 330

uniform mat4 P;
uniform mat4 C;
uniform mat4 mT;
uniform mat4 mR;
uniform mat4 M;
uniform mat4 N;
uniform mat4 L;
uniform vec4 lightPos;
uniform vec4 camPos;
uniform int shadingMode;

in vec3 pos;
in vec3 colorIn;

smooth out vec4 smoothColor;

vec4 justColor()
{
    return vec4(colorIn, 1);
}

vec4 gouraud()
{
    return abs(vec4(1) - vec4(colorIn, 1));
}

vec4 phong()
{
    return vec4(1, 1, 1, 1);
}

void main()
{
    //TODO add gouraud and phong shading support
    
	vec4 pos = vec4(pos, 1);
	gl_Position = P*M*pos;
    
    if(shadingMode == 0)
        smoothColor = justColor();
    else if (shadingMode == 1)
        smoothColor = gouraud();
    else
        smoothColor = phong();
}
