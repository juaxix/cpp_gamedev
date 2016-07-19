/// InputManager.cpp
/// Implementation of the class InputManager
#include "InputManager.h"

namespace xixEngine
{
	InputManager::InputManager() :
		_mouseCoords(0.0f, 0.0f)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		//look for previous keys (we could use a foreach with a for(auto& it): _keyMap
		for (auto it = _keyMap.begin(); it != _keyMap.end(); it++)
		{
			//store in previous map
			_previousKeyMap[it->first] = it->second;
		}

	}


	void InputManager::pressKey(unsigned int keyID) {
		_keyMap[keyID] = true; //the unorder map creates the key if it does not exists by itself
	}

	void InputManager::releaseKey(unsigned int keyID) {
		_keyMap[keyID] = false;
	}

	bool InputManager::isKeyDown(unsigned int keyID) const {
		//we need to check if the keyID exists or else it will create it with a random bool as value!.
		KeyMap::const_iterator it = _keyMap.find(keyID);
		if (it != _keyMap.end())
		{
			//does exist
			return it->second; //return bool value
		}
		else {
			return false;
		}
	}

	bool InputManager::isKeyPressed(unsigned int keyID) const {
		if (isKeyDown(keyID) == true && wasKeyDown(keyID)==false)
		{
			return true;
		}
		return false;
	}

	void InputManager::setMouseCoords(const float x, const float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}

	const glm::vec2 InputManager::getMouseCoords() const {
		return _mouseCoords;
	}
	
	bool InputManager::wasKeyDown(unsigned int keyID) const
	{
		KeyMap::const_iterator it = _previousKeyMap.find(keyID);
		if (it != _previousKeyMap.end())
		{
			return it->second;
		}
		else {
			return false;
		}
	}
}