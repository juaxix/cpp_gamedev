/// SdlWindow.cpp
/// Implementation of the class SdlWindow

#include "SdlWindow.h"
#include <stdexcept>
#include <string>
#include <sdl/SDL.h>

namespace xixEngine {
	SdlWindow::SdlWindow(const char *windowTitle,unsigned int width, unsigned int height)
		: mSDLWindow(
			SDL_CreateWindow(windowTitle
			, SDL_WINDOWPOS_UNDEFINED
			, SDL_WINDOWPOS_UNDEFINED
			, width
			, height
			, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL
		)
		, SDL_DestroyWindow) {
		if (mSDLWindow == nullptr /* changed from false to nullptr in OSX XCode*/) {
			throw std::runtime_error(std::string("Error creating window: ") + SDL_GetError());
		}
		SDL_GLContext glContext = SDL_GL_CreateContext(
			mSDLWindow.get()
		);
		if (glContext == nullptr)
		{
			throw std::runtime_error("SDL GL Context could not be created");
		}
		//Avoid weird errors (optional)
		glewExperimental = true;
		//init GLU extensions
		GLenum errorGL = glewInit();
		if (errorGL != GLEW_OK)
		{
			throw std::runtime_error("Could not initialized glew");
		}
	}
	void SdlWindow::Show() {
		SDL_ShowWindow(mSDLWindow.get());
	}
	SdlWindow::operator SDL_Window*() {
		return mSDLWindow.get();
	}
}