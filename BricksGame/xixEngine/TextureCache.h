/// TextureCache.h
/// Header of the class TextureCache
#pragma once
#include "VertexData.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <sdl/SDL_image.h>
namespace xixEngine {

	class TextureCache
	{
	public:
		enum ImageType {
			JPG,
			PNG
		};
		///<summary>TextureCache Constructor</summary>
		TextureCache();

		///<summary>TextureCache Destructor</summary>
		~TextureCache() {};

		///<summary>Returns a GLTexture from a file path (JPG or PNG)</summary>
		///<param>file path</param>
		///<return>texture as GLTexture</return>
		GLTexture getTexture(const std::string& texturePath, TextureCache::ImageType = ImageType::PNG);
	private:
		///<summary>RedBlack tree to store all our textures paths and data</summary>
		std::map<std::string /*path*/, GLTexture> _textureMap;
		///<summary>Function to load a png file from a file path and returns a GLTexture</summary>
		///<param>file path</param>
		///<return>GLTexture</return>
		GLTexture loadImage(std::string filePath, TextureCache::ImageType imageType = ImageType::PNG);
	};
}
