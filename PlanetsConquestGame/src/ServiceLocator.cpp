#include "ServiceLocator.hpp"

#include "Input.hpp"
#include "TextureManager.hpp"
#include "World.hpp"
#include "WindowInfo.hpp"
#include "SceneManager.hpp"
#include "Audio.hpp"
ServiceLocator* ServiceLocator::_instance = NULL;


ServiceLocator::ServiceLocator()
{
}

ServiceLocator& ServiceLocator::get()
{
    if(NULL == ServiceLocator::_instance)
    {
        _instance = new ServiceLocator();
        _instance->load();
    }
    
    return *_instance;
}

void ServiceLocator::load()
{
    _input = new Input();
	_audio = new Audio();
    _textureManager = new TextureManager();
	_windowInfo = new WindowInfo();
	_sceneMng = new SceneManager();	
}

void ServiceLocator::destroy()
{
	if (_instance)
	{
		_instance->doDestroy();
        delete _instance;
	}
    
    _instance = NULL;
}

void ServiceLocator::doDestroy()
{
    if (_sceneMng)
	{
		delete _sceneMng;
		_sceneMng = NULL;
	}
    if (_windowInfo)
	{
		delete _windowInfo;
		_windowInfo = NULL;
	}
	if (_textureManager)
	{
		delete _textureManager;
		_textureManager = NULL;
	}
    if (_input)
	{
		delete _input;
		_input = NULL;
	}
	if (_audio)
	{
		delete _audio;
		_audio = NULL;
	}
}

Input& ServiceLocator::getInput()
{
    return *_input;
}

Audio& ServiceLocator::getAudio()
{
	return *_audio;
}

TextureManager& ServiceLocator::getTextureManager()
{
    return *_textureManager;
}

IScene& ServiceLocator::getCurrentScene()
{
    return *getSceneManager().getCurrent();
}

WindowInfo& ServiceLocator::getWindowInfo()
{
	return *_windowInfo;
}

SceneManager& ServiceLocator::getSceneManager()
{
	return *_sceneMng;
}
