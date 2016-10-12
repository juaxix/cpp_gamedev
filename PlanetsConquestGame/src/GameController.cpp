#include "GameController.hpp"
#include "Game.hpp"
#include "GameScene.hpp"
#include "Fireworks.hpp"

GameController* GameController::instance = NULL;

GameController::GameController(Game *g)
{
	game = g;
	// stores the input events catched from glfw callbacks, every frame all the key events are cleared
	input = (&ServiceLocator::get().getInput());
	sceneManager = (&ServiceLocator::get().getSceneManager());
	this->pushState(GameController::GameStates::IDLE);
	srand(time(NULL));
	//Create fireworks
	/* @TODO: uncomment to add fireworks
	std::cout << "Creating fireworks,...\n";
	for (int i = 0; i < FIREWORKS_MAX; i++)
		fireworks.push_back(new Fireworks());
		*/
	instance = this;
}

GameController& GameController::getInstance()
{
	if (NULL == GameController::instance)
	{
		instance = new GameController(NULL);
	}

	return *instance;
}

void GameController::update()
{
	Time::update();
	if (input->getIsKeyPressed(GLFW_KEY_ESCAPE)){
		switch (state){
		case GameStates::MENU:
			this->pushState(GameStates::END);
			break;
		case GameStates::PAUSE:
			this->pushState(GameStates::PLAY);
			break;
		case GameStates::PLAY:
			this->pushState(GameStates::PAUSE);
			break;
		default:
			//nothing for idle or end states (win,lose included)
			break;
		}
		
	}
	

	//Update scene with the updated state
	sceneManager->update();
}


bool GameController::addToGameScene(GameEntity *entity)
{
	if (state == GameController::GameStates::PLAY || state == GameController::GameStates::PAUSE){
		ServiceLocator::get().getCurrentScene().addEntity(entity);
		return true;
	}
	else {
		return false;
	}

}

void GameController::removeFromGameScene(GameEntity *entity)
{
	
	ServiceLocator::get().getCurrentScene().removeEntity(entity);
	
}

void GameController::pushState(GameStates newState)
{
	switch (newState)
	{
	case GameStates::IDLE:
		break;
	case GameStates::MENU:
		sceneManager->loadScene(new StartScene);
		break;
	case GameStates::PLAY:
		sceneManager->loadScene(new GameScene());
		break;
	case GameStates::PAUSE:

		break;
	case GameStates::END:
		if(game!=NULL) game->shouldCloseNow();
		break;
	case GameStates::WIN:
		sceneManager->loadScene(new WinScene());
		break;
	case GameStates::LOSE:
		sceneManager->loadScene(new LoseScene());
		break;
	default:
		break;
	}

	state = newState;
}

void GameController::render()
{
	//render current scene 
	sceneManager->render();
	//draw FXs on top of the scene:
	/* @TODO: uncomment to render
	for (int i = 0; i < Fireworks::count; i++)
	{
		fireworks[i]->render();
	}*/
}

void GameController::getInput()
{
	input->update(); // reset input events
}


void GameController::destroy()
{
	// destroy all game services
	ServiceLocator::get().destroy();
	if (instance)
	{
		delete instance;
	}

	/** @TODO: uncomment to add fireworks
	if (fireworks.size()>0){
		for (int i = 0; i < Fireworks::count; i++){
			delete &fireworks[i];
		}
	}*/
	
}
