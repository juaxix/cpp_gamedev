#include "TextureCache.h"
namespace xixEngine {
	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(const std::string& texturePath)
	{
		/*std::map<std::string,GLTexture>::iterator */
		/* is the same as auto ,it needs the initialization to be valid*/
		auto it = _textureMap.find(texturePath); //search
		if (it == _textureMap.end())
		{
			//it is the same as the end of the map, so it's not in it
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);
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

}