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
		objLoader loader;
		//loader.load("resources/cube.obj");
		//loader.load("resources/sphere.obj");
		//loader.load("resources/teapot.obj");
		loader.load("resources/teapot_flake_floor.obj");
		
		for(size_t i=0; i<loader.vertexCount; i++) {
			positions.push_back(loader.vertexList[i]->e[0]);
			positions.push_back(loader.vertexList[i]->e[1]);
			positions.push_back(loader.vertexList[i]->e[2]);
			//printf("v%zu: %f %f %f\n", i, positions[i*3+0], positions[i*3+1], positions[i*3+2]);
		}
		
		for(size_t i=0; i<loader.faceCount; i++) {
			if(loader.faceList[i]->vertex_count != 3) {
				fprintf(stderr, "Skipping non-triangle face %zu.\n", i);
				continue;
				//exit(1);
			}
			
			elements.push_back(loader.faceList[i]->vertex_index[0]);
			elements.push_back(loader.faceList[i]->vertex_index[1]);
			elements.push_back(loader.faceList[i]->vertex_index[2]);
			//printf("f%zu: %i %i %i\n", i, elements[i*3+0], elements[i*3+1], elements[i*3+2]);
		}
		
		
		vector<glm::vec3> vertexNormals;
		vertexNormals.resize(positions.size());
		for(size_t i=0; i<vertexNormals.size(); i++)
			vertexNormals[i] = glm::vec3(0.0f);
		
		
		//TODO compute the vertex normals by averaging the face normals
		for(size_t i=0; i<elements.size(); i+=3) {
			size_t vertexId[3];
			glm::vec3 vertexPos[3];
			
			for(size_t v=0; v<3; v++)
				vertexId[v] = elements[i+v];
			
			for(size_t v=0; v<3; v++)
			{
				vertexPos[v][0] = positions[ vertexId[v]*3 + 0 ];
				vertexPos[v][1] = positions[ vertexId[v]*3 + 1 ];
				vertexPos[v][2] = positions[ vertexId[v]*3 + 2 ];
			}
			//printf("\nf%lu \n", i/3);
			//for(size_t v=0; v<3; v++)
			//    printf("%zu %.2f %.2f %.2f\n", vertexId[v], vertexPos[v][0], vertexPos[v][1], vertexPos[v][2]);
			
			glm::vec3 a = vertexPos[1] - vertexPos[0];
			glm::vec3 b = vertexPos[2] - vertexPos[1];
			//printf("%.2f %.2f %.2f X %.2f %.2f %.2f\n", a[0], a[1], a[2], b[0], b[1], b[2]);
			glm::vec3 faceNormal = glm::normalize(glm::cross(a, b));
			
			for(size_t v=0; v<3; v++) {
				vertexNormals[ vertexId[v] ] += faceNormal;
				//printf("%zu %.2f %.2f %.2f\n", vertexId[v], faceNormal[0], faceNormal[1], faceNormal[2]);
			}
		}
		
		for(size_t i=0; i<positions.size(); i++) {
			vertexNormals[i] = glm::normalize(vertexNormals[i]);
			//printf("%.2f %.2f %.2f\n", vertexNormals[i][0], vertexNormals[i][1], vertexNormals[i][2]);
		}
		
		for(size_t i=0; i<positions.size(); i++) {
			colors.push_back( (vertexNormals[i][0] + 1.0f) * 0.5f);
			colors.push_back( (vertexNormals[i][1] + 1.0f) * 0.5f);
			colors.push_back( (vertexNormals[i][2] + 1.0f) * 0.5f);
		}
		
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
	
	glm::vec3 getMinBound() const
	{ return min; }
	
	glm::vec3 getMaxBound() const
	{ return max; }
	
	glm::vec3 getCentroid() const
	{ return center; }
	
	glm::vec3 getDimension() const
	{ return dim; }
	
private:
	
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
		float positionCount = 1.0f/(positions.size()/3.0f);
		
		for(int i=0; i<positions.size(); i+=3)
		{
			center[0] += positions[i] * positionCount;
			center[1] += positions[i+1] * positionCount;
			center[2] += positions[i+2] * positionCount;
		}
		
		return center;
	}
	
	glm::vec3 computeDimension()
	{
		glm::vec3 max = getMaxBound();
		glm::vec3 min = getMinBound();
		glm::vec3 dim = max - min;
		return dim;
	}
	
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