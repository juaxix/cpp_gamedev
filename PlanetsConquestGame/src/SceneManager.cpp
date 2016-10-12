#include "SceneManager.hpp"
#include "IScene.hpp"
#include <iostream>

SceneManager::SceneManager()
:_current(NULL), _nextScene(NULL)
{
}

SceneManager::~SceneManager()
{
    if(_current)
    {
        delete _current;
        _current = NULL;
    }
    if(_nextScene)
    {
        delete _nextScene;
        _nextScene = NULL;
    }
}

void SceneManager::update()
{
	if (_nextScene)
	{
		if (_current)
		{
			delete _current;
		}
        
		_current = _nextScene;
		_nextScene = NULL;
		
		_current->load();
	}
    
	if (_current)
	{
		_current->update();
	}
}

void SceneManager::render()
{
	if (_current)
	{
		_current->render();
	}
}

void SceneManager::loadScene(IScene* scene)
{
	_nextScene = scene;
}

IScene* SceneManager::getCurrent()
{
	return _current;
}
