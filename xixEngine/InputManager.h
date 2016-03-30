#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
//behaves the same as map but uses a hashtable (built on top of it) and makes it faster unless it uses more space than a map because the order 
namespace xixEngine 
{
	class InputManager
	{
	private:
		std::unordered_map<unsigned int, bool> _keyMap;
		glm::vec2 _mouseCoords;
	public:
		InputManager();
		~InputManager();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);
		void setMouseCoords(const float x,const float y);
		const glm::vec2 InputManager::getMouseCoords() const;
	};
}
