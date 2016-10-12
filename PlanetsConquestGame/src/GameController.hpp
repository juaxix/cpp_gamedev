#ifndef __opengl_sample__GameController__
#define __opengl_sample__GameController__

#include "Time.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"
#include "StartScene.hpp"
#include "WinScene.hpp"
#include "LoseScene.hpp"
#include "GameScene.hpp"
#include "ServiceLocator.hpp"
#include <GLFW/glfw3.h>

class Game;
/** @TODO: uncomment to add fireworks
class Fireworks;
*/
class GameController
{
	SceneManager* sceneManager;
	Input* input;
	static GameController *instance;
	Game *game;
	/** @TODO: uncomment to add fireworks
	std::vector<Fireworks*> fireworks;*/
public:
	enum GameStates {
		IDLE = 0, MENU, PLAY, PAUSE, WIN, LOSE, END
	};
	
	GameController::GameController(Game* g);
	static GameController& getInstance();
	void update();
	bool addToGameScene(GameEntity *entity);
	void removeFromGameScene(GameEntity *entity);
	void pushState(GameStates newState);
	void render();
	void getInput();
	void run();
	void destroy();
	inline bool isPaused() const { return state == GameStates::PAUSE; };
private:
	GameStates state;
};

#endif /* defined(__opengl_sample__GameController__) */