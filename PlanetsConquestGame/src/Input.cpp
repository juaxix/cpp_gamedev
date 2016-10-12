#include "Input.hpp"
#include <GLFW/glfw3.h>

#include "WindowInfo.hpp"
#include "ServiceLocator.hpp"
#include "Camera.hpp"

void Input::update()
{
    for (auto& kv : _keys) {
        InputValue& key = kv.second;
        
        key._wasPressed = key._isPressed;
        key._isPressed = false;
    }
    
    for (auto& kv : _mouseKeys) {
        InputValue& key = kv.second;
        
        key._wasPressed = key._isPressed;
        key._isPressed = false;
    }
}

void Input::setKeyPressed(int key)
{
    _keys[key]._isPressed = true;
}

bool Input::getIsKeyPressed(int key)
{
    return _keys[key]._isPressed;
}

void Input::setMouseButtonPressed(int key)
{
    _mouseKeys[key]._isPressed = true;
}

bool Input::getIsMouseButtonPressed(int key)
{
    return _mouseKeys[key]._isPressed;
}

glm::vec2 Input::getCursorPos()
{
	WindowInfo& device = ServiceLocator::get().getWindowInfo();
    
	double x, y;
	glfwGetCursorPos(device.getWindow(), &x, &y);
    
	return glm::vec2((float)x, (float)y);
}

glm::vec3 Input::getCursorWorldPos(const Camera& camera)
{
    // convert mouse position to projection space
	Input& input = ServiceLocator::get().getInput();
	glm::vec2 mousePos = input.getCursorPos();
    
	WindowInfo& device = ServiceLocator::get().getWindowInfo();
	glm::vec2 maxPos(device.getXRes(), device.getYRes());
    
	glm::vec2 projPos = (mousePos / maxPos);
	projPos.y = 1 - projPos.y;
	projPos = (projPos * glm::vec2(2.f, 2.f)) - glm::vec2(1.f, 1.f);
    
    // convert projection space back to world space
	glm::mat4 vp = camera.getViewProjection();
	glm::mat4 vpInv = glm::inverse(vp);
    
	glm::vec4 projPosUniform(projPos, 0.f, 1.f);
	glm::vec4 pointWorldPos = vpInv * projPosUniform;
    
	return glm::vec3(pointWorldPos.x, pointWorldPos.y, pointWorldPos.z);
}

bool Input::getKeyBecomesPressed(int key)
{
	return _keys[key]._wasPressed == false && _keys[key]._isPressed == true;
}

bool Input::getMouseButtonBecomesPressed(int key)
{
	return _mouseKeys[key]._wasPressed == false && _mouseKeys[key]._isPressed == true;
}


