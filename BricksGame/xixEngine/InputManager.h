/// InputManager.h
/// Header of the class InputManager
#pragma once
#include <unordered_map>
#include <glm/glm.hpp>

namespace xixEngine
{
	class InputManager
	{
	public:
		///Default constructor: just set mouse coords to 0
		InputManager();
		///Default destructor (empty)
		~InputManager();
		/// Needed to keep track of pressed / released keys
		void update();
		///add a keyID to the pressed keymap
		void pressKey(unsigned int keyID);
		///set false to the pressed keymap with the given id
		void releaseKey(unsigned int keyID);
		///is the keyID down?
		bool isKeyDown(unsigned int keyID) const;
		///event pressed of a keyId
		bool isKeyPressed(unsigned int keyID) const;
		///set the current mouse position
		void setMouseCoords(const float x, const float y);
		///get the current mouse position
		const glm::vec2 getMouseCoords() const;
	private:
		//an unordered_map behaves the same as map but uses a hashtable (built on top of it) and makes it faster unless it uses more space than a map because the order 
		typedef std::unordered_map<unsigned int, bool> KeyMap;
		///our key maps, one for track current pressed and just released (not pressed not but was before)
		KeyMap _keyMap, _previousKeyMap;
		glm::vec2 _mouseCoords;
		///internal function to keep track of previous keys pressed
		bool wasKeyDown(unsigned int keyID) const;
	};
}
