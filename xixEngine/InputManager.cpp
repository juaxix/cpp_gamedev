#include "InputManager.h"

namespace xixEngine 
{
	InputManager::InputManager():
		_mouseCoords(0.0f,0.0f)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::pressKey(unsigned int keyID) {
		_keyMap[keyID] = true; //the unorder map creates the key if it does not exists by itself
	}

	void InputManager::releaseKey(unsigned int keyID) {
		_keyMap[keyID] = false;
	}

	bool InputManager::isKeyPressed(unsigned int keyID) {
		//we need to check if the keyID exists or else it will create it with a random bool as value!.
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end())
		{
			//does exist
			return it->second; //return bool value
		}
		else {
			return false;
		}
	}

	void InputManager::setMouseCoords(const float x, const float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}

	const glm::vec2 InputManager::getMouseCoords() const {
		return _mouseCoords;
	}
}