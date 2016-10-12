#include "Fireworks.hpp"

const GLfloat Fireworks::GRAVITY = 0.05f;
const GLfloat Fireworks::baselineYSpeed = -4.0f;
const GLfloat Fireworks::maxYSpeed = -4.0f;
int Fireworks::count = 0;

Fireworks::Fireworks()
{
	initialized = false;
	initialize();
	count++;

}

void Fireworks::initialize()
{
	// Pick an initial x location and  random x/y speeds
	float xLoc = (rand() / (float)RAND_MAX) * Game::width;
	float xSpeedVal = -2 + (rand() / (float)RAND_MAX) * 4.0f;
	float ySpeedVal = baselineYSpeed + ((float)rand() / (float)RAND_MAX) * maxYSpeed;

	// Set initial x/y location and speeds
	for (int loop = 0; loop < FIREWORKS_PARTICLES; loop++)
	{
		x[loop] = xLoc;
		y[loop] = Game::height + 10.0f; // Push the particle location down off the bottom of the screen
		xSpeed[loop] = xSpeedVal;
		ySpeed[loop] = ySpeedVal;
	}

	// Assign a random colour and full alpha (i.e. particle is completely opaque)
	red = (rand() / (float)RAND_MAX);
	green = (rand() / (float)RAND_MAX);
	blue = (rand() / (float)RAND_MAX);
	alpha = 1.0f;

	// Firework will launch after a random amount of frames between 0 and 400
	framesUntilLaunch = ((int)rand() % 400);

	// Size of the particle (as thrown to glPointSize) - range is 1.0f to 4.0f
	particleSize = 1.0f + ((float)rand() / (float)RAND_MAX) * 3.0f;

	// Flag to keep trackof whether the firework has exploded or not
	hasExploded = false;
	if (!initialized)
		std::cout << "Fireworks " << count << " initialized \n ";
	initialized = true;
}

void Fireworks::move()
{
	if (!initialized) return;
	for (int loop = 0; loop < FIREWORKS_PARTICLES; loop++)
	{
		// Once the firework is ready to launch start moving the particles
		if (framesUntilLaunch <= 0)
		{
			x[loop] += xSpeed[loop];

			y[loop] += ySpeed[loop];

			ySpeed[loop] += Fireworks::GRAVITY;
		}
	}
	framesUntilLaunch--;

	// Once a fireworks speed turns positive (i.e. at top of arc) - blow it up!
	if (ySpeed[0] > 0.0f)
	{
		for (int loop2 = 0; loop2 < FIREWORKS_PARTICLES; loop2++)
		{
			// Set a random x and y speed beteen -4 and + 4
			xSpeed[loop2] = -4 + (rand() / (float)RAND_MAX) * 8;
			ySpeed[loop2] = -4 + (rand() / (float)RAND_MAX) * 8;
		}

		//cout << "Boom!" << endl;
		hasExploded = true;
	}
}

void Fireworks::explode()
{
	if (!initialized) return;
	for (int loop = 0; loop <FIREWORKS_PARTICLES; loop++)
	{
		// Dampen the horizontal speed by 1% per frame
		xSpeed[loop] *= 0.99;
		
		// Move the particle
		x[loop] += xSpeed[loop];
		y[loop] += ySpeed[loop];

		// Apply gravity to the particle's speed
		ySpeed[loop] += Fireworks::GRAVITY;
	}

	// Fade out the particles (alpha is stored per firework, not per particle)
	if (alpha > 0.0f)
	{
		alpha -= 0.01f;
	}
	else // Once the alpha hits zero reset the firework
	{
		initialize();
	}
}


void Fireworks::render()
{
	if (!initialized) return;
	
	
	// Setup our viewport to be the entire size of the window
	glViewport(0, 0, (GLsizei)Game::width, (GLsizei)Game::height);

	// Change to the projection matrix and reset the matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Change to orthographic (2D) projection and switch to the modelview matrix
	glOrtho(0, Game::width, Game::height, 0, 0, 1);
	// Set ModelView matrix mode and reset to the default identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Displacement trick for exact pixelisation
	glTranslatef(0.375, 0.375, 0);

	// Draw fireworks
	//cout << "Firework count is: " << Fireworks::count << endl;
	float ar = ServiceLocator::get().getWindowInfo().getAspectRatio();

	for (int particleLoop = 0; particleLoop < FIREWORKS_PARTICLES; particleLoop++)
	{
		// Set the point size of the firework particles (this needs to be called BEFORE opening the glBegin(GL_POINTS) section!)
		glPointSize(particleSize);

		glBegin(GL_POINTS);

		// Set colour to yellow on way up, then whatever colour firework should be when exploded
		if (hasExploded == false)
		{
			glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{
			glColor4f(red, green,blue, alpha);
		}

		// Draw the point
		glVertex2f(x[particleLoop] * ar, y[particleLoop]*ar);
		glEnd();
	}

	// Move the firework appropriately depending on its explosion state
	if (hasExploded == false)
	{
		move();
	}
	else
	{
		explode();
	}
	
	//glAccum(GL_ACCUM, 0.9f);

	// Return clean state
	// swap buffers
}

Fireworks::~Fireworks()
{
	//destroy every particle, avoid mem leaks...
	
}