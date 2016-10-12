#ifndef __opengl_sample__Input__
#define __opengl_sample__Input__

#include <iostream>
#include <map>
#include <glm/glm.hpp>


class Camera;

struct InputValue
{
	bool _wasPressed;
	bool _isPressed;
    
	InputValue()
    :_wasPressed(false), _isPressed(false)
	{
	}
    
	InputValue(bool wasPressed, bool isPressed)
    :_wasPressed(wasPressed), _isPressed(isPressed)
	{
	}
};

class Input
{
	typedef std::map<int, InputValue> Keymap;
    
	Keymap _keys;
    Keymap _mouseKeys;
    
public:
    void update();
    
    void setKeyPressed(int key);
	bool getIsKeyPressed(int key);
	bool getKeyBecomesPressed(int key);
    
    void setMouseButtonPressed(int key);
	bool getIsMouseButtonPressed(int key);
	bool getMouseButtonBecomesPressed(int key);
    
	glm::vec2 getCursorPos();
	glm::vec3 getCursorWorldPos(const Camera& camera);
};

#endif /* defined(__opengl_sample__Input__) */
