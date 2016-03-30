#include "Window.h"

namespace xixEngine {

	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		Uint32 flags = SDL_WINDOW_OPENGL;
		if (currentFlags&INVISIBLE)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}
		else {
			flags |= SDL_WINDOW_SHOWN;
		}
		if (currentFlags&FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		if (currentFlags&BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		_sdlWindow = SDL_CreateWindow(
			windowName.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			_screenWidth,
			_screenHeight,
			flags
			);
		if (_sdlWindow == nullptr)
		{
			fatalError("SDL Window could not be created");
			return 0;
		}
		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr)
		{
			fatalError("SDL GL Context could not be created");
			return 0;
		}
		//Avoid weird errors (optional)
		glewExperimental = true;
		//init GLU extensions
		GLenum errorGL = glewInit();
		if (errorGL != GLEW_OK)
		{
			fatalError("Could not initialized glew");
			return 0;
		}

		//Set the color for clean the screen
		glClearColor(0.1f, 0.3f, 0.5f, 1.0f);

		//Try to sync with the frame rate activating vsync?
		SDL_GL_SetSwapInterval(0); // 0 = Off

								   //Check the OpenGL version, just in case the vsync is available with the current hardware
		std::printf("\t* OpenGL Version %s \n", glGetString(GL_VERSION /* macro */));

		//Enable alpha blend for the textures with transparency
		glEnable(GL_BLEND);

		//Tell OGL what function is to blend (where in the textures? -> PNG: spots with alpha = no color)
		glBlendFunc(/*source: how it will affect*/GL_SRC_ALPHA, /* mult color: what portion of the color to draw on top of the background color */ GL_ONE_MINUS_SRC_ALPHA  // 1 - zero*originalcolor=all
		);

		return 1;
	}
}
