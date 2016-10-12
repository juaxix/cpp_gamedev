#include "Game.hpp"
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Audio.hpp"

//Input Associations
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input& input = ServiceLocator::get().getInput();

	/*if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}*/

	if (action == GLFW_PRESS)
	{
		input.setKeyPressed(key);
	}
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	Input& input = ServiceLocator::get().getInput();

	if (action == GLFW_PRESS)
	{
		input.setMouseButtonPressed(button);
	}
}
//Game config:
int Game::width = 800;
int Game::height = 600;
int Game::bpp = 32;

Game::Game()
{
	xres = Game::width;
	yres = Game::height;
	// create a OpenGL window with GLFW library
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(xres, yres, "Planets Conquest", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	glfwSetMouseButtonCallback(window, mouse_callback);

	// save window info to be used in the game
	ServiceLocator::get().getWindowInfo().init(window, xres, yres);
	//Create Game Controller
	gameController = new GameController(this);
	gameController->pushState(GameController::GameStates::MENU);
	lastTime = currentTime = glfwGetTime();
	//Initialize audio
	ServiceLocator::get().getAudio().initialize();
	

}

bool Game::shouldUpdate()
{
	currentTime = glfwGetTime();

	if (currentTime - lastTime >= 1.0 / framerate)
	{
		lastTime = currentTime;
		return true;
	}
	return false;
}

/**
 @brief Main Game loop
*/
void Game::run()
{
	while (!glfwWindowShouldClose(window))
	{
		//60 FPS LIMIT by Default
		if (this->shouldUpdate())
		{
			//----
			//GLFW
			//----
			glfwGetFramebufferSize(window, &width, &height);
			//----
			//  GL
			//----
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glMatrixMode(GL_PROJECTION);

			glLoadIdentity();
			//-------
			//	LOGIC
			//-------
			gameController->update();
			//-------
			//	DRAW
			//-------
			gameController->render();
			//-------
			//	INPUT
			//-------
			gameController->getInput();
			//Display:
			glfwSwapBuffers(window);
			//Poll events
			glfwPollEvents();
		}
	}

}


Game::~Game()
{
	//delete game controller
	gameController->destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
	
	std::cout << "Exiting...";
}
