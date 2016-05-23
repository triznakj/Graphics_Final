#ifndef __MODEL
#define __MODEL

#include <vector>
#ifndef M_PI
#define M_PI 3.14159
#endif

#include "glm/glm.hpp"

#define COMP_COUNT 2

using namespace std; //makes using vectors easy

class Model
{
public:
	
	Model()
	{
		
		//TODO build a clock outline centered at the origin with radius 0.9
		//for each 2D vertex you insert, add a addOutlineColor() entry
		
		//Clock obj
		beginObject();
		for (int i = 0; i < 360; i++){
			positions.push_back(.9*cos(i*3.14/180));
			positions.push_back(.9*sin(i*3.14/180));
			addOutlineColor();
		}
		
		
		//single tick mark
		beginObject();
		positions.push_back(0);
		positions.push_back(0);
		addTickColor();
		positions.push_back(0.2);
		positions.push_back(0);
		addTickColor();
		
		//small hand
		beginObject();
		positions.push_back(0);
		positions.push_back(0);
		addSmallHandColor();
		positions.push_back(0.4);
		positions.push_back(0);
		addSmallHandColor();
		
		//large hand
		beginObject();
		positions.push_back(0);
		positions.push_back(0);
		addLargeHandColor();
		positions.push_back(0.8);
		positions.push_back(0);
		addLargeHandColor();
	}
	
	GLfloat const * getPositions() const
	{ return &positions[0]; }
	
	size_t getPositionBytes() const
	{ return positions.size()*sizeof(GLfloat); }
	
	GLfloat const * getColors() const
	{ return &colors[0]; }
	
	size_t getColorBytes() const
	{ return colors.size()*sizeof(GLfloat); }
	
	size_t getVertexCount() const
	{ return positions.size(); }
	
	glm::vec3 getCenter()
	{
		glm::vec3 min = getMinBound();
		glm::vec3 max = getMaxBound();
		glm::vec3 center;
		
		for(int c=0; c<2; c++)
			center = (min+max) * 0.5f;
		
		return center;
	}
	
	glm::vec3 getMinBound()
	{
		glm::vec3 bound;
		bound = glm::vec3(9999999.0);
		unsigned int numComp = 3;
		
		for(unsigned int i=0; i<this->positions.size()/numComp; i++)
		{
			for(int c=0; c<numComp; c++)
			{
				if(positions[i*numComp+c] < bound[c])
					bound[c] = positions[i*numComp+c];
			}
		}
		
		return bound;
	}
	
	glm::vec3 getMaxBound()
	{
		glm::vec3 bound;
		bound = glm::vec3(-9999999.0);
		unsigned int numComp = 3;
		
		for(unsigned int i=0; i<this->positions.size()/numComp; i++)
		{
			for(int c=0; c<numComp; c++)
			{
				if(positions[i*numComp+c] > bound[c])
					bound[c] = (positions[i*numComp+c]);
			}
		}
		
		return bound;
	}
	
	size_t getObjectCount() const
	{ return objects.size(); }
	
	size_t getObjectStart(size_t id) const
	{ return objects.at(id)/COMP_COUNT; }
	
	size_t getObjectEnd(size_t id) const
	{
		if(id == objects.size()-1)
			return positions.size()/COMP_COUNT;
		return objects.at(id+1)/COMP_COUNT;
	}
	
	size_t getObjectSize(size_t id) const
	{
		size_t start = getObjectStart(id);
		size_t end = getObjectEnd(id);
		return end - start;
	}
	
private:
	vector<GLfloat> positions;
	vector<GLfloat> colors;
	vector<size_t> objects;
	
	void addColor(float const & r, float const & g, float const & b, float const & a)
	{
		colors.push_back(r);
		colors.push_back(g);
		colors.push_back(b);
		colors.push_back(a);
	}

	void addOutlineColor()
	{ addColor(1,1,1,1); }
	
	void addTickColor()
	{ addColor(0,1,0,0); }
	
	void addSmallHandColor()
	{ addColor(1,0,1,1); }
	
	void addLargeHandColor()
	{ addColor(0,1,1,1); }
	
	void beginObject()
	{
		objects.push_back(positions.size());
	}
};

#endif
