#ifndef __opengl_sample__MathUtils__
#define __opengl_sample__MathUtils__

class MathUtils
{
public:
	static float rand(float min, float max);
    static float min(float a, float b);
	static float max(float a, float b);
	static int min(int a, int b);
	static int max(int a, int b);
	static int abs(int a);
	static float abs(float a);
	static float lerp(float a, float b,float t);
};

#endif /* defined(__opengl_sample__MathUtils__) */
