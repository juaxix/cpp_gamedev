#ifndef __opengl_sample__TextureManager__
#define __opengl_sample__TextureManager__

#include <iostream>
#include <map>
#include <string>

#include <GLFW/glfw3.h>

class TextureManager
{
    typedef std::map<std::string, GLuint> MTextures;
    
    MTextures _textures;
    
public:
    GLuint getTexture(const std::string& name);
    
private:
    GLuint createTexture(const std::string& name);
};

#endif /* defined(__opengl_sample__TextureManager__) */
