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

smooth in vec4 smoothColor;
smooth in vec3 smoothPos;
smooth in vec3 smoothNorm;

layout(location = 0) out vec4 fragColor;

vec4 toon_edges = vec4(0.1, 0.5, 0.8, 0.8);
float outline_edge = 0.3;

float toon_refl(float refl)
{
	vec4 rv = vec4(refl);
	vec4 edges = step(toon_edges, rv)*toon_edges;
	float r = max( edges.x, max(edges.y, max(edges.z, edges.w)));
	return r;
}

vec4 toon(in vec3 pos, in vec3 norm, in vec3 colorIn)
{
	float alpha = 10;
	vec4 p = vec4(pos, 1);
	vec4 lp = lightPos;
	vec4 n = normalize(vec4(norm,0));
	vec4 c = vec4(0);
	vec4 Li = vec4(1);
	vec4 ka = vec4(0.05);
	vec4 kd = vec4(0.2, 0.2, 0.8, 1);
	vec4 ks = vec4(1);

	lp = C*L*lp;
	p = M*p;
	n = vec4(N*n.xyz, 0);

	// diffuse coefficent
	vec4 l = normalize(lp - p);
	float d = clamp(dot(l,n), 0, 1);
	d = toon_refl(d);

	// specular coefficient
	vec4 v = normalize(c-p);
	vec4 r = normalize(reflect(-l,n));
	float s = clamp(dot(v,r), 0, 1);
	s = pow(s, alpha);
	s = toon_refl(s);

	//outline
	float t = dot(n, v);
	if(t < outline_edge)
		return vec4(0);
	
	return ka*Li + kd*d*Li + ks*s*Li;
}

void main()
{
	fragColor = toon(smoothPos, smoothNorm, smoothColor.xyz);
}
