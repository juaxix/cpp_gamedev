/// Sdl.cpp
/// Implementation of the class Sdl: a wrapper

#include "Sdl.h"
#if __APPLE_
#include <OpenGL/OpenGL.h>
#endif
#include <sdl/SDL.h>
#include <stdexcept>
#include <sdl/SDL_image.h>
namespace xixEngine {
	Sdl::Sdl(int flags)
    {
        if (SDL_Init(flags) != 0) {
            throw std::runtime_error("Failed to init SDL");
        }
#if __APPLE_	
        CGLPixelFormatAttribute attribs[13] = {
            kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute)kCGLOGLPVersion_3_2_Core, // This sets the context to 3.2
            kCGLPFAColorSize,     (CGLPixelFormatAttribute)24,
            kCGLPFAAlphaSize,     (CGLPixelFormatAttribute)8,
            kCGLPFAAccelerated,
            kCGLPFADoubleBuffer,
            kCGLPFASampleBuffers, (CGLPixelFormatAttribute)1,
            kCGLPFASamples,       (CGLPixelFormatAttribute)4,
            (CGLPixelFormatAttribute)0
        };
        
        CGLPixelFormatObj pix;
        GLint npix;
        CGLChoosePixelFormat(attribs, &pix, &npix);
        
        CGLContextObj ctx;
        CGLCreateContext(pix, 0, &ctx);
        
        CGLSetCurrentContext(ctx);
        CGLLockContext(ctx);
        
        //  glutInitDisplayMode(GLUT_1_3_CORE_PROFILE);
		
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
        
	}
	Sdl::~Sdl() {
		IMG_Quit();
		SDL_Quit();
	}
}
