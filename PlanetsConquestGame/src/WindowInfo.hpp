#ifndef __opengl_sample__RenderDevice__
#define __opengl_sample__RenderDevice__

#include <GLFW/glfw3.h>

class WindowInfo
{
	GLFWwindow* _window;
	int _xres, _yres;
    
public:
	void init(GLFWwindow*, int xres, int yres);
    
	int getXRes();
	int getYRes();
    float getAspectRatio();
    
	GLFWwindow* getWindow();
};


#endif /* defined(__opengl_sample__RenderDevice__) */
