/// SdlWindow.h
/// Header of the class SdlWindow 
#pragma once
#include <memory>
#include <glew/glew.h>
struct SDL_Window;

namespace xixEngine 
{
	///Class to store the SDL_Window
	class SdlWindow 
	{
	public:
		///Constructor: it will create the SDL Window 
		SdlWindow(const char* windowTitle,unsigned int width, unsigned int height);
		///return the SDL_Window for glContext
		operator SDL_Window*();
		///Show the SDL_Window
		void Show();

	private:
		///Store the SDL_Window
		std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> mSDLWindow;
	};
}