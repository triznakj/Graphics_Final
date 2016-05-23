#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H
#include "Model.h"

#define NUM_TRACKED_FRAMES 10

class WorldState
{
private:
	float frameTimes[NUM_TRACKED_FRAMES];
	float currentTime;
	bool running;
	Model model;
	
	bool scale;
	bool translate;
	bool rotY;
	bool rotZ;
	
	float xPos;
	float yPos;
	
	float xVel;
	float yVel;

public:
	WorldState()
	{
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = 0.0f;
		running = true;
		model = Model();
		
		scale = false;
		translate = false;
		rotY = false;
		rotZ = false;
		
		xPos = 0;
		yPos = 0;
		
		xVel = 0.4;
		yVel = 0.7;
	}
	
	void updateFrameTime(float timeAsSeconds)
	{
		for(size_t i=1; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = frameTimes[i-1];
		frameTimes[0] = timeAsSeconds;
	}
	
	void printFPS() const
	{
		float sum = 0.0f;
		float avg = 0.0f;
		float fps = 0.0f;
		
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			sum += frameTimes[i];
		
		avg = sum / NUM_TRACKED_FRAMES;
		fps = 1.0f / avg;
		printf("fps %f\n", fps);
	}
	
	Model const & getModel() const
	{ return model; }
	
	void setRunning(bool r)
	{ running = r; }

	bool isRunning() const
	{ return running; }

	float getCurrentTime() const
	{ return this->currentTime; }

	void timeStep(float t)
	{
		float elapsed = t - this->currentTime;
		
		//TODO compute new position and velocity
		//be sure to account for the elapsed time
		
		if (this->translateEnabled()){
			this->xPos += (elapsed*this->xVel);
			this->yPos += (elapsed*this->yVel);
			if (this->xPos < -1 || this->xPos > 1){
				this->xVel *= -1;
			}
			if (this->yPos < -1 || this->yPos > 1){
				this->yVel *= -1;
			}
		}
		
		this->currentTime = t;
	}
	
	void toggleScale()
	{ this->scale = !this->scale; }
	void toggleTranslate()
	{ this->translate = !this->translate; }
	void toggleRotZ()
	{ this->rotZ = !this->rotZ; }
	void toggleRotY()
	{ this->rotY = !this->rotY; }
	
	bool scaleEnabled() const
	{ return this->scale; }
	bool translateEnabled() const
	{ return this->translate; }
	bool rotZEnabled() const
	{ return this->rotZ; }
	bool rotYEnabled() const
	{ return this->rotY; }
	
	float getXPos() const
	{ return this->xPos; }
	
	float getYPos() const
	{ return this->yPos; }
};

#endif
