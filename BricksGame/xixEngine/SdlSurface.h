/// SDL_Surface.h
/// Header of the class SDL_Surface
#pragma once
#include <memory>

struct SDL_Surface;

namespace xixEngine {
	class SdlSurface {
	public:
		SdlSurface(const char* filename);
		operator SDL_Surface*();
		void Bind();
		int Width() const;
		int Height() const;
		const unsigned int& ID() { return *mTextureId; }
	private:
		static void DeleteTexture(unsigned int* textureId);
		std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> mSurface;
		std::unique_ptr<unsigned int,void(*)(unsigned int*)> mTextureId;
	};
}
