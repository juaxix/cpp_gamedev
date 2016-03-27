#pragma once
#include <map>
#include "GLTexture.h"
#include "ImageLoader.h"
#include <iostream>
class TextureCache
{
private:
	//RedBlack tree to store all our textures paths and data
	std::map<std::string /*path*/, GLTexture> _textureMap;
	
public:
	TextureCache();
	~TextureCache();
	GLTexture getTexture(const std::string& texturePath);
};

