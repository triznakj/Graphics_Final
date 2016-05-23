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

int kernelSize = 27;  //larger kernels will blur more, but run slower
int halfSize = kernelSize / 2;

vec2 res = vec2(resolution);
vec2 fragCoord = gl_FragCoord.xy;
vec2 texCoord = fragCoord/res;

vec4 pinch()
{
	float range = 60.0;
	float holeSize = 30.0;

	float lookupRange = range - holeSize;
	vec2 toFrag = fragCoord - cursorAbsolutePos;
	float dis = length(toFrag);
	if(dis>range)
		return texture(texId, texCoord);
	vec2 dir = normalize(toFrag);
	float scale = lookupRange/range*dis + holeSize;
	dir = dir * scale;
	return texture(texId, (cursorAbsolutePos+dir)/res);
}

vec4 swirl()
{
	float range = 60.0;

	vec2 toFrag = fragCoord - cursorAbsolutePos;
	float dis = length(toFrag);
	if(dis>range)
		return texture(texId, texCoord);
	float scale = ( pow(1-dis/range,3))*5;

	mat2 rotZ = mat2(cos(scale), sin(scale), -sin(scale), cos(scale));
	vec2 swlCoord = rotZ * toFrag;
	return texture(texId, (cursorAbsolutePos+swlCoord)/res);
}

vec4 blur()
{
	//blur kernel
	vec4 blurColor = vec4(0);
	int kernelSize = 5;
	float scale = pow(kernelSize*2, 2);
	for(int x=-kernelSize; x<=kernelSize; x++)
	{
		for(int y=-kernelSize; y<=kernelSize; y++)
		{
			vec2 o = vec2(x,y);
			vec2 kCoord = (fragCoord+o)/res;
			blurColor += texture(texId, kCoord);
		}
	}
	blurColor = blurColor / scale;
	return blurColor;
}

vec4 spotLight()
{
	int spotSize = 100;
	vec2 toFrag = fragCoord - cursorAbsolutePos;
	float dis = length(toFrag);
	float scale = 1- dis/spotSize;

	if(dis > spotSize)
		scale = 0;

	return texture(texId, texCoord)*scale;
}

void main()
{
	//this is needed on some computers
	res = vec2(resolution);
	fragCoord = gl_FragCoord.xy;
	texCoord = fragCoord/res;

	fragColor = smoothColor;
	fragColor = vec4(texCoord, 1, 1);

	fragColor = texture(texId, texCoord);

	if(mouseButtonDown == 1) {
		//fragColor = pinch();
		//fragColor = swirl();
		//fragColor = blur();
		//fragColor = spotLight();
	}

}