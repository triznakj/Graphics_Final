#ifndef __MODEL
#define __MODEL

#include <vector>
#include "glm/glm.hpp"
#include "objload/objLoader.h"
using namespace std; //makes using vectors easy

class Model
{
public:


	void init()
	{
		initTexVertices();

		min = computeMinBound();
		max = computeMaxBound();
		center = computeCentroid();
		dim = computeDimension();
	}

	vector<GLfloat> const getPosition() const
	{ return positions; }

	vector<GLfloat> const getColor() const
	{ return colors; }

	vector<GLuint> const getElements() const
	{ return elements; }

	size_t getVertexCount() const
	{ return positions.size()/3; }

	size_t getPositionBytes() const
	{ return positions.size()*sizeof(GLfloat); }

	size_t getColorBytes() const
	{ return colors.size()*sizeof(GLfloat); }

	size_t getElementBytes() const
	{ return elements.size()*sizeof(GLuint); }

	glm::vec3 getMinBound()
	{ return min; }

	glm::vec3 getMaxBound()
	{ return max; }

	glm::vec3 getCentroid()
	{ return center; }

	glm::vec3 getDimension()
	{ return dim; }

private:

	void initTexVertices()
	{
		//black quad
		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(0);
		addBlackColor();
		positions.push_back(0);
		positions.push_back(10);
		positions.push_back(0);
		addBlackColor();
		positions.push_back(10);
		positions.push_back(10);
		positions.push_back(0);
		addBlackColor();

		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(0);
		addBlackColor();
		positions.push_back(10);
		positions.push_back(10);
		positions.push_back(0);
		addBlackColor();
		positions.push_back(10);
		positions.push_back(0);
		positions.push_back(0);
		addBlackColor();

		//red quad
		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(-3);
		addRedColor();
		positions.push_back(6);
		positions.push_back(0);
		positions.push_back(-3);
		addRedColor();
		positions.push_back(6);
		positions.push_back(6);
		positions.push_back(-3);
		addRedColor();

		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(-3);
		addRedColor();
		positions.push_back(6);
		positions.push_back(6);
		positions.push_back(-3);
		addRedColor();
		positions.push_back(0);
		positions.push_back(6);
		positions.push_back(-3);
		addRedColor();

		//green quad
		positions.push_back(2);
		positions.push_back(2);
		positions.push_back(-5);
		addGreenColor();
		positions.push_back(8);
		positions.push_back(2);
		positions.push_back(-5);
		addGreenColor();
		positions.push_back(8);
		positions.push_back(8);
		positions.push_back(-5);
		addGreenColor();

		positions.push_back(2);
		positions.push_back(2);
		positions.push_back(-5);
		addGreenColor();
		positions.push_back(8);
		positions.push_back(8);
		positions.push_back(-5);
		addGreenColor();
		positions.push_back(2);
		positions.push_back(8);
		positions.push_back(-5);
		addGreenColor();

		//green quad
		positions.push_back(4);
		positions.push_back(4);
		positions.push_back(-7);
		addBlueColor();
		positions.push_back(10);
		positions.push_back(4);
		positions.push_back(-7);
		addBlueColor();
		positions.push_back(10);
		positions.push_back(10);
		positions.push_back(-7);
		addBlueColor();

		positions.push_back(4);
		positions.push_back(4);
		positions.push_back(-7);
		addBlueColor();
		positions.push_back(10);
		positions.push_back(10);
		positions.push_back(-7);
		addBlueColor();
		positions.push_back(4);
		positions.push_back(10);
		positions.push_back(-7);
		addBlueColor();
	}

	glm::vec3 computeMinBound()
	{
		glm::vec3 bound;

		for(int c=0; c<3; c++)
			bound[c] = std::numeric_limits<float>::max();

		for(int i=0; i<positions.size(); i+=3)
		{
			for(int c=0; c<3; c++)
			{
				if(positions[i+c] < bound[c])
					bound[c] = positions[i+c];
			}
		}

		return bound;
	}

	glm::vec3 computeMaxBound()
	{
		glm::vec3 bound;

		for(int c=0; c<3; c++)
			bound[c] = -std::numeric_limits<float>::max();

		for(int i=0; i<positions.size(); i+=3)
		{
			for(int c=0; c<3; c++)
			{
				if(positions[i+c] > bound[c])
					bound[c] = positions[i+c];
			}
		}

		return bound;
	}

	glm::vec3 computeCentroid()
	{
		glm::vec3 center = glm::vec3(0);
		center = (max - min)/3.0f;

		return center;
	}

	glm::vec3 computeDimension()
	{
		glm::vec3 max = getMaxBound();
		glm::vec3 min = getMinBound();
		glm::vec3 dim = max - min;
		return dim;
	}

	template<typename Container, typename Value>
	void addAttr(Container & c, Value v)
	{
		c.push_back(v);
	}

	template<typename Container, typename Value, typename... Args>
	void addAttr(Container & c, Value v, Args... args)
	{
		c.push_back(v);
		addAttr(c, args...);
	}

	void addColor(float const & r, float const & g, float const & b, float const & a)
	{
		colors.push_back(r);
		colors.push_back(g);
		colors.push_back(b);
		colors.push_back(a);
	}

	void addBlackColor()
	{ addColor(0,0,0,0.5); }

	void addRedColor()
	{ addColor(1,0,0,0.3333); }

	void addGreenColor()
	{ addColor(0,1,0,0.5); }

	void addBlueColor()
	{ addColor(0,0,1,1); }
	
	vector<GLfloat> positions;
	vector<GLfloat> colors;
	vector<GLuint> elements;
	size_t objectCount;

	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 dim;
	glm::vec3 center;
};

#endif
