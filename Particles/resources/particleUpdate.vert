#version 330

uniform mat4 P;
uniform mat4 M;
uniform uvec2 mapSize;
uniform sampler2DRect posTexture;
uniform sampler2DRect velTexture;

layout(location = 0) in uint idVal;

out vec4 pos;
out vec4 vel;

void main()
{
	uint x = uint(gl_VertexID)%mapSize.x;
	uint y = uint(gl_VertexID)/mapSize.x;
	uvec2 texCoord = uvec2(x,y);

	pos = texture(posTexture, texCoord);
	vel = texture(velTexture, texCoord);
	
	vec4 pos2D = vec4(vec2(float(x)+0.5,float(y)+0.5)/mapSize*2-1, 0, 1);
	gl_Position = pos2D;

}
