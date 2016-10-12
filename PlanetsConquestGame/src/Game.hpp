#pragma once
#include <GLFW/glfw3.h>
#include "ServiceLocator.hpp"
#include "Input.hpp"
#include "WindowInfo.hpp"
#include "SceneManager.hpp"
#include "Time.hpp"
#include "GameController.hpp"
class Game
{
	GLFWwindow* window;
	GameController *gameController;
	int xres;
	int yres;
	double lastTime, currentTime;
	int framerate = 60; //by default 60FPS
public:
	static int width, height, bpp;
	Game();
	bool shouldUpdate();
	inline void shouldCloseNow(){ glfwSetWindowShouldClose(window, GL_TRUE); };
	void run();
	~Game();
};

