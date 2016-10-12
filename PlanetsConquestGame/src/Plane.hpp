#ifndef __opengl_sample__File__
#define __opengl_sample__File__


#include <GLFW/glfw3.h>
#include "GameEntity.hpp"
#include <string>

class Plane : public GameEntity
{
    GLuint _textureId;
    
public:
    
    void setTextureByName(const std::string& name);
    
    void render();
};

#endif /* defined(__opengl_sample__File__) */
