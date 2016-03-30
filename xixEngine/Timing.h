#pragma once
#include <SDL.h>
#include <iostream>

namespace xixEngine 
{
	class FpsLimiter 
	{
	private:
		float _maxFPS, _fps, _frameTime;
		unsigned int _startTicks;

		void calculateFPS();


	public:
		FpsLimiter();
		void init(float MaxFPS);
		void setMaxFPS(float MaxFPS);
		void begin();
		void end();
		float getFPS();
	};

}
