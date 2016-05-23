#ifndef __TRACKBALL
#define __TRACKBALL

#include "glm/glm.hpp"
#define  SQRT2 1.4142136f

#define TRACKBALLSIZE (0.8f)          // trackball size in percentage of window

class TrackBall
{
public:
	TrackBall()
	{
		xsize = 0;
		ysize = 0;
	}
	
	void setSize(int const & xsize, int const & ysize)
	{
		this->xsize = xsize;
		this->ysize = ysize;
	}
	
	void getRotation(float & angle, glm::vec3 & axis, glm::ivec2 const & oldPos, glm::ivec2 const & newPos) const
	{
		update_rotate(angle, axis, oldPos.x, oldPos.y, newPos.x, newPos.y);
	}
	
private:
	int xsize;
	int ysize;
	
	// find the z coordinate corresponding to the mouse position 
	float
	project_to_sphere(float r, float x, float y) const
	{
		float d, t, z;
		
		d = sqrt(x*x + y*y);
		t = r / SQRT2;
		if (d < t) {       // Inside sphere
			z = sqrt(r*r - d*d);
		}
		else {             // On hyperbola
			z = t*t / d;
		}
		
		return z;
	}
	
	// get the axis of rotation and angle
	void
	update_rotate(float & angle, glm::vec3 & gaxis, int x1, int y1, int x2, int y2) const
	{
		float axis[3], p1[3], p2[3];
		float phi, t;
		
		if (x1 == x2 && y1 == y2) { // if there's no movement, no rotation
			axis[0] = 1;
			axis[1] = 0;
			axis[2] = 0;
			phi = 0;
		}
		else {
			// first vector
			p1[0] = (2.0f*x1)/(float)xsize - 1.0f;
			p1[1] = 1.0f - (2.0f*y1)/(float)ysize;
			p1[2] = project_to_sphere((float)TRACKBALLSIZE, p1[0], p1[1]);
			// second vector
			p2[0] = (2.0f*x2)/(float)xsize - 1.0f;
			p2[1] = 1.0f - (2.0f*y2)/(float)ysize;
			p2[2] = project_to_sphere((float)TRACKBALLSIZE, p2[0], p2[1]);
			
			// the axis of rotation is given by the cross product of the first
			// and second vectors
			axis[0] = p1[1]*p2[2] - p1[2]*p2[1];
			axis[1] = p1[2]*p2[0] - p1[0]*p2[2];
			axis[2] = p1[0]*p2[1] - p1[1]*p2[0];
			t = axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2];
			axis[0] /= t;
			axis[1] /= t;
			axis[2] /= t;
			
			// the amount of rotation is proportional to the magnitude of the
			// difference between the vectors
			t = sqrt((p1[0]-p2[0])*(p1[0]-p2[0]) + (p1[1]-p2[1])*(p1[1]-p2[1]) +
					 (p1[2]-p2[2])*(p1[2]-p2[2]))/(2.0f*TRACKBALLSIZE);
			
			if (t > 1.0f) {
				t = 1.0f;
			}
			if (t < -1.0f) {
				t = -1.0f;
			}
			phi = asin(t);
			//phi = phi * 0.002;
		}
		
		gaxis = glm::vec3(axis[0], axis[1], axis[2]);
		angle = phi;
	}
};

#endif