#version 330
uniform int mousePressed;
uniform float timeVal;

in vec4 smoothColor;

out vec4 fragColor;

void main()
{
	//TODO: make an animation
	//could use the vertex shader also
	
	fragColor = smoothColor;
	if(mousePressed == 0)
		fragColor = vec4(smoothColor.r  * (0.5*sin(timeVal) + 0.75),
					   smoothColor.g  * (0.5*sin(timeVal) + 0.75),
					   smoothColor.b  * (0.5*sin(timeVal) + 0.75)
					   , 1);
	else{
		fragColor = vec4(1, 1, 1, 1);
	}
}