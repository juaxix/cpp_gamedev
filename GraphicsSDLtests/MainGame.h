#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <SDL.h>
#include <GL/glew.h>
#undef main //fix for SDL
#include <xixEngine/Errors.h>
#include <xixEngine/GLTexture.h>
#include <xixEngine/ResourceManager.h>
#include <xixEngine/SpriteBatch.h>
#include <xixEngine/GLSLProgram.h>
#include <xixEngine/Window.h>
#include <xixEngine/xixEngine.h>
#include <xixEngine/Camera2D.h>
#include <xixEngine/InputManager.h>
#include <xixEngine/Timing.h>
#include "Bullet.h"
#include <glm/glm.hpp>

enum class GameState {IDLE, PLAY, EXIT};

class MainGame
{
private:
	xixEngine::Window _window;
	int _screenWidth, _screenHeight;
	GameState _gameState;
	float _time, _maxFPS;
	//std::vector<xixEngine::Sprite*> _sprites; //allocate in the heap to avoid the sprites to be deleted when relocated inside so they also were glDeleteBuffers in their destructors
	xixEngine::GLSLProgram _colorProgram;
	//Game entities
	xixEngine::Camera2D _camera;
	xixEngine::SpriteBatch _spriteBatch;
	xixEngine::InputManager _inputManager;
	xixEngine::FpsLimiter _fpsLimiter;
	std::vector<Bullet> _bullets;
	//private methods
	bool initShaders();
	bool initSystems();
	void processInput();
	void drawGame();
	void gameLoop();
public:
	MainGame();
	~MainGame();
	void run();
	

};

