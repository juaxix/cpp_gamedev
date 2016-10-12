#include "TextureManager.hpp"
#include "FileHelper.h"
#include "SOIL.h"
#include <cassert>

GLuint TextureManager::getTexture(const std::string &name)
{
    MTextures::iterator it = _textures.find(name);
    
    GLuint textureId = -1;
    
    if(it == _textures.end()) {
        createTexture(name);
        
        it = _textures.find(name);
    }
    
    textureId = it->second;
    
    assert(textureId > 0);
    
    return textureId;
}

GLuint TextureManager::createTexture(const std::string& name)
{
    const std::string &dir = FileHelper::resourcesDirectory();
    
	const char* formats[] = { ".dds", ".bmp", ".png", ".tga", ".jpg" };
    
	GLuint textureId = -1;
	std::string imgPath = "";
    
	for (const char* format: formats)
	{
		imgPath = dir + "resources/" + name + format;
        
		textureId = SOIL_load_OGL_texture(imgPath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
		std::cout << "'" << SOIL_last_result() << "'" << std::endl;
        
		if (textureId > 0)
		{
			break;
		}
	}
    
    assert(textureId > 0);
    
    _textures[name] = textureId;
    
    return textureId;
}
