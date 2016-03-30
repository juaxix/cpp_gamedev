#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <string>
#include "Errors.h"
namespace xixEngine {
	enum WindowFlags {
		INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4
	};
	class Window
	{
	private:
		SDL_Window* _sdlWindow;
		int _screenWidth, _screenHeight;
	public:
		Window();
		~Window();
		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
		int getScreenWidth() { return _screenWidth; };
		int getScreenHeight() { return _screenHeight; }
		void swapBuffers() {
			SDL_GL_SwapWindow(_sdlWindow);
		}
	};

}
