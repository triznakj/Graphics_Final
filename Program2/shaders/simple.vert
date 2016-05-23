#version 330

//TODO: receive a uniform value
uniform float timeVal;

in vec2 pos;
in vec3 color;
in float speed;

out vec4 smoothColor;

void main()
{

	vec2 myVec;
	myVec.x = pos.x;
	myVec.y = pos.y + (.2 * speed * sin(timeVal));

	gl_Position = vec4(myVec, 0,1);
	smoothColor = vec4(color, 1);
	
	//TODO: make an animation!
	//could use the frag shader also
}