#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <SDL.h>
#include <GL/glew.h>
#undef main //fix for sdl
#include "Errors.h"
#include "Sprite.h"
#include "GLSLProgram.h"

enum class GameState {IDLE, PLAY, EXIT};

class MainGame
{
private:
	SDL_Window* _window;
	int _screenWidth, _screenHeight;
	GameState _gameState;
	float _time,_fps,_frameTime,_maxFPS;
	
	std::vector<Sprite*> _sprites; //allocate in the heap to avoid the sprites to be deleted when relocated inside so they also were glDeleteBuffers in their destructors
	Sprite _sprite;
	GLSLProgram _colorProgram;
	
	bool initShaders();
	bool initSystems();
	void gameLoop();
	void processInput();
	void drawGame();
	void calculateFPS();
public:
	MainGame();
	~MainGame();
	void run();
	

};

