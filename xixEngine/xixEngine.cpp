#include "xixEngine.h"
namespace xixEngine {
	int init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		//We want to have 2 buffers, one for drawing and another for changes
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		return 0;
	}
}