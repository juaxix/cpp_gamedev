#include "WinScene.hpp"

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

WinScene::WinScene()
	:_loadTime(0), _waittingTime(1.f), _plane(NULL), _text(NULL), _camera(NULL)
{

}

WinScene::~WinScene()
{
	if (_text) {
		delete _text;
		_text = NULL;
	}

	if (_plane) {
		delete _plane;
		_plane = NULL;
	}
	if (_camera)
	{
		delete _camera;
		_camera = NULL;
	}
}

void WinScene::load()
{
	// setup the ortho camera
	float ar = ServiceLocator::get().getWindowInfo().getAspectRatio();
	_camera = new Camera();
	_camera->setOrthoParams(ar*-1.f, ar*1.f, -1.f, 1.f, 2.0f, 0.f);
	_camera->setLookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.0f, 0.f));

	// logo image
	_plane = new Plane();
	_plane->setTextureByName(ResourcesPaths::kLogoImg);
	_plane->setPosition(glm::vec3(0.f));
	_plane->setScale(glm::vec3(2.f, 0.5f, 1.f));

	// click to start text
	_text = new GUIText();
	_text->init(10, 10, ResourcesPaths::kTextAtlas);
	_text->setPosition(glm::vec3(-0.75f, -0.9f, 0.f));
	_text->setScale(glm::vec3(1.25f, 0.5f, 0.f));
	_plane->setTextureByName(ResourcesPaths::kLogoImg);
	_text->setText("YOU WIN");

	_loadTime = Time::getTimeSinceAppStart();
}

void WinScene::update()
{
	Input& input = ServiceLocator::get().getInput();

	if (Time::getTimeSinceAppStart() - _loadTime < _waittingTime)
	{
		return;
	}

	if (input.getMouseButtonBecomesPressed(0))
	{
		GameController::getInstance().pushState(GameController::GameStates::MENU);
	}
}

void WinScene::render()
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

