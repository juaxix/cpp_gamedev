#pragma once
#include "TextureCache.h"
#include <string>
namespace xixEngine {
	class ResourceManager
	{
	private:
		static TextureCache _textureCache;
	public:
		static GLTexture getTexture(std::string texturePath);

	};

}
