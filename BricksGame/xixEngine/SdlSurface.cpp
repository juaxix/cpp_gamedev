/// SdlSurface.cpp
/// Implementation of the class SdlSurface

#include "SdlSurface.h"
#include <stdexcept>
#include <string>
#include <glew/glew.h>
#include <sdl/SDL.h>
#include <sdl/SDL_image.h>

namespace xixEngine {
	SdlSurface::SdlSurface(const char* filename)
		: mSurface(IMG_Load(filename), SDL_FreeSurface)
		, mTextureId(new unsigned int, &DeleteTexture) {
		if (mSurface == nullptr) {
			throw std::runtime_error(std::string("Unable to load texture ") + filename);
		}
		glGenTextures(1, mTextureId.get());
		glBindTexture(GL_TEXTURE_2D, *mTextureId);
		int mode;
		switch (mSurface->format->BytesPerPixel) {
		case 4:
			mode = GL_RGBA;
			break;
		case 3:
			mode = GL_RGB;
			break;
		case 1:
			mode = GL_LUMINANCE_ALPHA;
			break;
		default:
			throw std::runtime_error("Image with unknown channel profile");
			break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, mode, mSurface->w, mSurface->h, 0, mode, GL_UNSIGNED_BYTE, mSurface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void SdlSurface::DeleteTexture(unsigned int* textureId) {
		glDeleteTextures(1, textureId);
	}

	void SdlSurface::Bind() {
		glBindTexture(GL_TEXTURE_2D, *mTextureId);
	}

	int SdlSurface::Width() const {
		return mSurface->w;
	}

	int SdlSurface::Height() const {
		return mSurface->h;
	}

	SdlSurface::operator SDL_Surface*() {
		return mSurface.get();
	}
}
