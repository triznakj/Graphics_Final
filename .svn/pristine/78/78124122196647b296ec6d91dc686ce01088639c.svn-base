#version 330

uniform mat4 P;  //projection matrix
uniform mat4 C;  //camera matrix
uniform mat4 M;  //modelview matrix: M = C * mR * mT
uniform mat3 N;  //inverse transpose of upperleft 3x3 of M
uniform mat4 L;  //light rotation matrix

uniform float elapsedTime;        //time in seconds since simulation began
uniform float near;               //near rendering plane
uniform float far;                //far rendering plane
uniform float fov;                //field of view
uniform float cursorScrollAmount; //delta scroll amount
uniform vec2 resolution;          //view resolution in pixels
uniform vec2 cursorAbsolutePos;   //absolute cursor position in pixels
uniform vec2 cursorDragAmount;    //tracks amount of x,y change while the mouse button is down (i.e. amount of drag)
uniform vec2 lastClickPos;        //where the last mouse click happened
uniform vec2 lastFrameDragPos;    //where the cursor was on the previous frame while the mouse button is down
uniform int  mouseButtonDown;     //tracks if the mouse button is down

uniform vec3 modelCenter; //center of the model (might be useful for rotations)
uniform vec3 lookAtPos;   //point in 3D space the camera is point towards
uniform vec3 cameraUp;    //camera up vector
uniform vec4 lightPos;    //light position
uniform vec4 camPos;      //camera position

uniform sampler2D texId;
//uniform sampler2DRect texId;

smooth in vec4 smoothColor;

//out vec4 fragColor;
layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = smoothColor;
	vec2 res = vec2(resolution);
	vec2 fragCoord = gl_FragCoord.xy;
	vec2 texCoord = fragCoord/res;
	fragColor = vec4(texCoord, 1, 1);


}
