#include "WindowInfo.hpp"

void WindowInfo::init(GLFWwindow* window, int xres, int yres)
{
	_window = window;
	_xres = xres;
	_yres = yres;
}

int WindowInfo::getXRes()
{
	return _xres;
}

int WindowInfo::getYRes()
{
	return _yres;
}

float WindowInfo::getAspectRatio()
{
    return (float)_xres/(float)_yres;
}

GLFWwindow* WindowInfo::getWindow()
{
	return _window;
}
