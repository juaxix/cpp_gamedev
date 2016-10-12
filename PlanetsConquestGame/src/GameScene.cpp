#include "GameScene.hpp"

#include <GLFW/glfw3.h>
#include "ServiceLocator.hpp"
#include "World.hpp"
#include "Input.hpp"
#include "IslandsMap.hpp"
#include "Player.hpp"
#include "AI_Player.hpp"
#include "Camera.hpp"
#include "Plane.hpp"
#include "ResourcesPaths.hpp"
#include "WindowInfo.hpp"
#include "GameController.hpp"
GameScene::GameScene()
	:_world(NULL), _camera(NULL), _background(NULL), _pauseBackground(NULL), _pauseText(NULL)
{
}

void GameScene::load()
{
    // saves all the entities to be updated and rendered
	_world = new World();
    
    // setup the ortho camera
    _camera = new Camera();
    float ar = ServiceLocator::get().getWindowInfo().getAspectRatio();
    _camera->setOrthoParams(ar*-100.f, ar*100.f, -100.f, 100.f, -100.0f, 100.f);
	_camera->setLookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.0f, 0.f));
    
    // procedural creation of islands
	IslandsMap* islandsMap = new IslandsMap();
	addEntity(islandsMap);
    
    // player logic
	Player* player = new Player();
	player->setIslandMap(*islandsMap);
    player->setCamera(*_camera);
	addEntity(player);
    
	//AI logic
	AI_Player *aiPlayer = new AI_Player();
	aiPlayer->setIslandMap(*islandsMap);
	addEntity(aiPlayer);

    // background screen
	_background = new Plane();
	_background->setTextureByName(ResourcesPaths::kBackgroundImg);
	_background->setPosition(glm::vec3(0.f));
	_background->setScale(glm::vec3(200.f*ar, 200.f, 1.f));

	//pause background
	_pauseBackground = new Plane();
	_pauseBackground->setTextureByName(ResourcesPaths::kLogoImg);
	_pauseBackground->setPosition(glm::vec3(0.f));
	_pauseBackground->setScale(glm::vec3(150.f*ar, 50.f*ar, 1.f));
	

	// pause text
	_pauseText = new GUIText();
	_pauseText->init(10, 10, ResourcesPaths::kTextAtlas);
	_pauseText->setPosition(glm::vec3(-12,-32,0));
	_pauseText->setScale(glm::vec3(6.f, 3.f, 1.f));
	
	_pauseText->setText("PAUSED");
}

GameScene::~GameScene()
{
    if(_camera)
    {     
        delete _camera; 
        _camera = NULL;  
    }
    if (_world) {
        delete _world;
        _camera = NULL;
    }
	if (_background){
		delete _background;
		_background = NULL;
	}
	if (_pauseBackground){
		delete _pauseBackground;
		_pauseBackground = NULL;
	}
	if (_pauseText){
		delete _pauseText;
		_pauseText = NULL;
	}
}

void GameScene::update()
{
	Input& input = ServiceLocator::get().getInput();
	if (!GameController::getInstance().isPaused()){
		_world->update();
	}
	input.update();
}

void GameScene::requestGameFinish(bool Win)
{
	if (Win){
		GameController::getInstance().pushState(GameController::GameStates::WIN);
	}
	else {
		GameController::getInstance().pushState(GameController::GameStates::LOSE);
	}
}

void GameScene::render()
{  
	// set camera view projection matrix mode
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&_camera->getViewProjection()[0][0]);
	
	// change to model view mode
	glMatrixMode(GL_MODELVIEW);

	if (GameController::getInstance().isPaused())
	{
		glm::mat4 planeLocalToWorld = _pauseBackground->getModel();
		glLoadMatrixf(&planeLocalToWorld[0][0]);
		_pauseBackground->render();
		
		glm::mat4 textLocalToWorld = _pauseText->getModel();
		glLoadMatrixf(&textLocalToWorld[0][0]);
		_pauseText->render();
	}
	else 
	{
		glm::mat4 planeLocalToWorld = _background->getModel();
		glLoadMatrixf(&planeLocalToWorld[0][0]);
		_background->render();
		_world->render();
	}

}

void GameScene::addEntity(GameEntity* entity)
{
    assert(entity);
	_world->addEntity(entity);
}

void GameScene::removeEntity(GameEntity* entity)
{
	assert(entity);
	_world->removeEntity(entity);
}
