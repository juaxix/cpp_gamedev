#include "StartScene.hpp"
#include <GLFW/glfw3.h>
#include "ServiceLocator.hpp"
#include "SceneManager.hpp"
#include "Time.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "GUIText.hpp"
#include "Plane.hpp"
#include "ResourcesPaths.hpp"
#include "WindowInfo.hpp"
#include "GameController.hpp"
#include "ResourcesPaths.hpp"

StartScene::StartScene()
:_loadTime(0), _waittingTime(1.f), _plane(NULL), _text(NULL), _camera(NULL)
{

}

StartScene::~StartScene()
{
    if (_text) {
        delete _text;
        _text = NULL;
    }
    
    if (_plane) {
        delete _plane;
        _plane = NULL;
    }
    if(_camera)
    {
        delete _camera;
        _camera = NULL;
    }
}

void StartScene::load()
{

    // setup the ortho camera
    float ar = ServiceLocator::get().getWindowInfo().getAspectRatio();
    _camera = new Camera();
    _camera->setOrthoParams(ar*-1.f, ar*1.f, -1.f, 1.f, 2.0f, 0.f);
	_camera->setLookAt(vec3(0.f, 0.f, 1.f), vec3(0.f, 0.f, -1.f), vec3(0.f, 1.0f, 0.f));
    
    // logo into the screen
	_plane = new Plane();
	_plane->setTextureByName(ResourcesPaths::kLogoImg);
	_plane->setPosition(vec3(0.f));
	_planeScale = vec3(0.f);
	_planeRotation = vec3(0.f);
	_planeRotation.x = MathUtils::rand(-2.f, -5.f);
	_plane->setRotation(quat(_planeRotation));
	_plane->setScale(_planeScale);
    
	//TWEEN for logo in scale and rotation in the same Tween
	CDBTweener::CTween* planeTween = new CDBTweener::CTween();
	planeTween->setEquation(&CDBTweener::TWEQ_LINEAR, CDBTweener::EEasing::TWEA_IN, 1.4f);
	planeTween->addValue(&this->_planeScale.x, 2.f);
	planeTween->addValue(&this->_planeScale.y, 0.5f);
	planeTween->addValue(&this->_planeScale.y, 0.5f);
	planeTween->addValue(&this->_planeRotation.x, 0.f);
	_tweens.addTween(planeTween);


    // click to start text
    _text = new GUIText();
    _text->init(10, 10, ResourcesPaths::kTextAtlas);
	
	_textPos = glm::vec3(1.5f, -0.9f, 0.f);
	_text->setPosition(_textPos);
    _text->setScale(glm::vec3(1.25f, 0.5f, 0.f));

	//CTween(CEquation *pEquation, EEasing eEasing, float fDuration, float *fpValue, float fTarget)
	CDBTweener::CTween* textTween = new CDBTweener::CTween();
	textTween->setEquation(&CDBTweener::TWEQ_LINEAR, CDBTweener::EEasing::TWEA_IN, 2.4f);
	textTween->addValue(&this->_textPos.x, -0.75f);
	_tweens.addTween(textTween);


    _plane->setTextureByName(ResourcesPaths::kLogoImg);
	_text->setText("CLICK TO START");
    _loadTime = Time::getTimeSinceAppStart();
	_frameTime = 0;
	//First step for tweens
	_tweens.step(_loadTime);


}

void StartScene::update()
{
	Input& input = ServiceLocator::get().getInput();
    
    if(Time::getTimeSinceAppStart() - _loadTime < _waittingTime) 
    {
        return;
    }
    
	if (input.getMouseButtonBecomesPressed(0))
	{
		GameController::getInstance().pushState(GameController::GameStates::PLAY);
		return;
	}
	//update tweens
	_frameTime += Time::getDeltatime();
	_tweens.step(_frameTime);

	//use the tween values for tweens:
	_plane->setRotation(quat(_planeRotation));
	_plane->setScale(_planeScale);
	_text->setPosition(_textPos);
	
	//another way of doing the same thing,but manually would be:
	/*for (CDBTweener::CTween* tween : _tweens.getTweens())
	{
		if (!tween->isFinished())
		{
			//do animation stuff
		}
	}*/
}

void StartScene::render()
{   
	glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&_camera->getViewProjection()[0][0]);
    
    // render the plane and text
	glMatrixMode(GL_MODELVIEW);
    
    glm::mat4 planeLocalToWorld = _plane->getModel();
	glLoadMatrixf(&planeLocalToWorld[0][0]);
    _plane->render();
    
    glm::mat4 textLocalToWorld = _text->getModel();
	glLoadMatrixf(&textLocalToWorld[0][0]);
    _text->render();

}
