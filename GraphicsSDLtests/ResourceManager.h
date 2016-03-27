#pragma once
#include "TextureCache.h"
#include <string>
class ResourceManager
{
private:
	static TextureCache _textureCache;
public:
	static GLTexture getTexture(std::string texturePath);
	
};

