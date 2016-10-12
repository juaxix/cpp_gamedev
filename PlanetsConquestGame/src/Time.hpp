#ifndef __opengl_sample__Time__
#define __opengl_sample__Time__

class Time
{
	static bool  _init;
	static float _prevTime;
	static float _currTime;
	static float _dt;
public:
	static double getTimeSinceAppStart();
	static float getDeltatime();
	static void update();
};

#endif /* defined(__opengl_sample__Time__) */
