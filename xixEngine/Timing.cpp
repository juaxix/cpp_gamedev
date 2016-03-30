#include "Timing.h"
namespace xixEngine
{
	FpsLimiter::FpsLimiter()
	{

	}

	void FpsLimiter::calculateFPS()
	{
		//vars that only can be changed here:
		static const int NUM_SAMPLES = 100;
		static float frameTimes[NUM_SAMPLES];
		static float prevTicks = SDL_GetTicks();
		static int currentFrame = 0;
		//get current ticks and compare with previous ticks
		float currentTicks = SDL_GetTicks();

		_frameTime = currentTicks - prevTicks;
		frameTimes[currentFrame%NUM_SAMPLES] = _frameTime;
		prevTicks = currentTicks;
		int count;  //number of measures
		currentFrame++; //first frame 1:avoid division by zero
		if (currentFrame < NUM_SAMPLES)
		{
			count = currentFrame;
		}
		else {
			count = NUM_SAMPLES;
		}
		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++)
		{
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;
		if (frameTimeAverage > 0)
		{
			_fps = 1000.0f / frameTimeAverage;
		}
		else {
			_fps = 60.0f;
		}

	}


	void FpsLimiter::init(float MaxFPS)
	{
		setMaxFPS(MaxFPS);
	}

	void FpsLimiter::setMaxFPS(float MaxFPS)
	{
		_maxFPS = MaxFPS;
	}

	void FpsLimiter::begin()
	{
		_startTicks = SDL_GetTicks();
	}

	void FpsLimiter::end()
	{
		calculateFPS();
		//Limit the FPS:
		float frameTicks = SDL_GetTicks() - _startTicks;
		if (1000.0f /*millisec*/ / _maxFPS > frameTicks)
		{
			//wait a little longer, delay the programs execution
			SDL_Delay(1000.0f / _maxFPS - frameTicks);//some windows does not accept a minimum number to delay programs
		}
	}
	float FpsLimiter::getFPS()
	{
		return _fps;
	}
}
