#include "MathUtils.hpp"
#include <glm/gtx/random.hpp>

float MathUtils::rand(float min, float max)
{
	return glm::linearRand(min, max);
}

float MathUtils::min(float a, float b)
{
    return a < b ? a : b;
}

float MathUtils::max(float a, float b)
{
	return a > b ? a : b;
}


int MathUtils::min(int a, int b)
{
	return a < b ? a : b;
}

int MathUtils::max(int a, int b)
{
	return a > b ? a : b;
}

int MathUtils::abs(int a)
{
	if (a >= 0) return a;
	else return a*(-1);
}

float MathUtils::abs(float a)
{
	if (a >= 0.0f) return a;
	else return a*(-1.0f);
}

float MathUtils::lerp(float a,float b, float t)
{
	return (1 - t)*a + t*b;
}