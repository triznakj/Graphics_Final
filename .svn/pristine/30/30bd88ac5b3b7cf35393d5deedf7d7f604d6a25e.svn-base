#ifndef __MODEL
#define __MODEL

#include <vector>
#include "glm/glm.hpp"
using namespace std; //makes using vectors easy

class Model
{
public:
	
	Model()
	{
		GLfloat pos[] = {
			0.0, 0.0, 0.0,
			2.0, 0.0, 0.0,
			2.0, 2.0, 0.0,
			0.0, 2.0, 0.0,
			0.0, 0.0, 2.0,
			2.0, 0.0, 2.0,
			2.0, 2.0, 2.0,
			0.0, 2.0, 2.0};
		
		positions.insert(positions.begin(), pos, pos+3*8);
		
		for(int i=0; i<positions.size(); i++)
			positions[i] = positions[i] * 0.5f;
		
		GLfloat col[] = {
			0.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
			1.0, 1.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0,
			1.0, 0.0, 1.0,
			1.0, 1.0, 1.0,
			0.0, 1.0, 1.0};
		
		colors.insert(colors.begin(), col, col+3*8);
		
		GLuint ele[] = {
			1, 0, 3,
			1, 3, 2,
			3, 7, 6,
			3, 6, 2,
			7, 3, 0,
			7, 0, 4,
			2, 6, 5,
			2, 5, 1,
			4, 5, 6,
			4, 6, 7,
			5, 4, 0,
			5, 0, 1,
		};
		
		elements.insert(elements.begin(), ele, ele+3*12);
		
	}
	
	GLfloat const * getPosition() const
	{ return &positions[0]; }
	
	GLfloat const * getColor() const
	{ return &colors[0]; }
	
	GLuint const * getElements() const
	{ return &elements[0]; }
	
	size_t getVertexCount() const
	{ return positions.size()/3; }
	
	size_t getPositionBytes() const
	{ return positions.size()*sizeof(GLfloat); }
	
	size_t getColorBytes() const
	{ return colors.size()*sizeof(GLfloat); }
	
	size_t getElementBytes() const
	{ return elements.size()*sizeof(GLuint); }
	
	glm::vec3 getCentroid()
	{
		glm::vec3 center = glm::vec3(0);
		float positionCount = 1.0f/(positions.size()/3.0f);
		
		for(int i=0; i<positions.size(); i+=3)
		{
			center[0] += positions[i] * positionCount;
			center[1] += positions[i+1] * positionCount;
			center[2] += positions[i+2] * positionCount;
		}
		
		return center;
	}
	
private:
	vector<GLfloat> positions;
	vector<GLfloat> colors;
	vector<GLuint> elements;
	size_t objectCount;
};

#endif