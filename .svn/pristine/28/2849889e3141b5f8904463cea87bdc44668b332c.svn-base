#version 330

uniform mat4 P;
uniform mat4 M;
uniform uvec2 mapSize;
uniform sampler2DRect posTexture;

layout(location = 0) in uint idVal;

out vec4 pos;
out vec4 vel;

void main()
{
	uint x =uint(gl_VertexID)%mapSize.x;
	uint y =uint(gl_VertexID)/mapSize.x;
	vec4 p = texture(posTexture, vec2(x,y));

	pos = p;
	
	gl_Position = P*M*p;
}
