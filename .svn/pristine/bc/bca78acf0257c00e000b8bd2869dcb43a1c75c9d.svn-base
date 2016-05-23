#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H

#define NUM_TRACKED_FRAMES 10

class WorldState
{
private:
	float frameTimes[NUM_TRACKED_FRAMES];
	float currentTime;
	bool running;

public:
	WorldState()
	{
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = 0.0f;
		running = true;
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
	
	void setRunning(bool r)
	{
		running = r;
	}

	bool isRunning() const
	{
		return running;
	}

	float getCurrentTime() const
	{ return this->currentTime; }

	void timeStep(float t)
	{
		this->currentTime = t;
	}
};

#endif
