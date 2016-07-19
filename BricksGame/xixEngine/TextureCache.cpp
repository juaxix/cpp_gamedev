/// TextureCache.cpp
/// Implementation of the class TextureCache

#pragma once
#include "TextureCache.h"


namespace xixEngine 
{

	///<summary>TextureCache Constructor. Initializes SDL_image for loading PNG and JPG images</summary>
	TextureCache::TextureCache() {
		int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags)) {
			throw std::runtime_error("Could not initilize SDL_Image flags");
		}
	}

	GLTexture TextureCache::getTexture(const std::string& texturePath, TextureCache::ImageType imageType)
	{
		//std::map<std::string,GLTexture>::iterator 
		// is the same as auto ,it needs the initialization to be valid
		auto it = _textureMap.find(texturePath); //search
		if (it == _textureMap.end())
		{
			//it is the same as the end of the map, so it's not in it
			GLTexture newTexture = loadImage(texturePath, imageType);
			//if it fails, it exit with the failure function
			//add to the map
			//we can use this pair:
			//std::pair<std::string, GLTexture> data(texturePath, newTexture);
			//or the function make_pair
			_textureMap.insert(make_pair(texturePath, newTexture));
			std::cout << "Loaded Texture in cache " + texturePath + "\n";
			return newTexture;
		}
		return it->second; //second element is the GLTexture
	}

	GLTexture TextureCache::loadImage(std::string filePath , TextureCache::ImageType imageType)
	{
		GLTexture texture = {}; //initialize the structure
		// Create storage space for the texture
		SDL_Surface *TextureImage[1];
		TextureImage[0] = IMG_Load(filePath.c_str());

		if (!TextureImage[0]) {
			throw std::runtime_error("Image could not be loaded : " + filePath); //convert int to string
			return texture;
		}
		//Create the texture
		glGenTextures(1, &(texture.id));
		//Bind the texture
		glBindTexture(GL_TEXTURE_2D, texture.id);
		texture.width = TextureImage[0]->w;
		texture.height = TextureImage[0]->h;
		switch (imageType)
		{
		case xixEngine::TextureCache::JPG: {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage[0]->w, TextureImage[0]->h, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			break;
		case xixEngine::TextureCache::PNG:
		default: {
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA,
				texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				TextureImage[0]->pixels
				); //we have a head in the png ,we need to specify the position[0]
			glGetError();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			//mipmapping: filter the texture with a linear interpolation (Linear filtering)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
			}
			break;
		}
		

		//unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);
		//Free up any memory we may have used 
		SDL_FreeSurface(TextureImage[0]);
		return texture;
	}


	
};
