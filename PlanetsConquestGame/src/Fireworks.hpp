#ifndef __opengl_sample__FireWorks__
#define __opengl_sample__FireWorks__
#include <iostream>
#include <ctime>
#include <time.h>   // Needed to use random numbers
#include <windows.h>
#include <GL/GL.h>
#include "Game.hpp"


using namespace std;

#ifndef FIREWORKS_MAX
#define FIREWORKS_MAX 30 //Max number of fireworks
#endif
#ifndef FIREWORKS_PARTICLES
#define FIREWORKS_PARTICLES 50 // Number of particles per firework
#endif

class Fireworks
{
private:
	bool initialized = false;
	
public:
	GLfloat x[FIREWORKS_PARTICLES];
	GLfloat y[FIREWORKS_PARTICLES];
	GLfloat xSpeed[FIREWORKS_PARTICLES];
	GLfloat ySpeed[FIREWORKS_PARTICLES];

	GLfloat red;
	GLfloat blue;
	GLfloat green;
	GLfloat alpha;

	GLint framesUntilLaunch;

	GLfloat particleSize;
	GLboolean hasExploded;

	static const GLfloat baselineYSpeed;
	static const GLfloat maxYSpeed;
	static const GLfloat GRAVITY;
	static int count; //count initialized fireworks
	Fireworks(); // Constructor declaration
	void initialize();
	void move();
	void explode();
	void render();
	~Fireworks();
};

#endif /* defined(__opengl_sample__FireWorks__) */