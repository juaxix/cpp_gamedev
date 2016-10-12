#include "Plane.hpp"

#include "ServiceLocator.hpp"
#include "TextureManager.hpp"
#include <cassert>

void Plane::setTextureByName(const std::string& name)
{
    TextureManager& textureManager = ServiceLocator::get().getTextureManager();
    
    _textureId = textureManager.getTexture(name);
    
    assert(_textureId>0);
}

void Plane::render()
{   
    assert(_textureId>0);
    
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
    
	glBegin(GL_TRIANGLES);
    
    glTexCoord2f(0, 1); glVertex3f(-0.5f, -0.5f, +0.f);
    glTexCoord2f(1, 1); glVertex3f(+0.5f, -0.5f, +0.f);
    glTexCoord2f(0, 0); glVertex3f(-0.5f, +0.5f, +0.f);
    
    glTexCoord2f(0, 0); glVertex3f(-0.5f, +0.5f, +0.f);
    glTexCoord2f(1, 1); glVertex3f(+0.5f, -0.5f, +0.f);
    glTexCoord2f(1, 0); glVertex3f(+0.5f, +0.5f, +0.f);
    
	glEnd();
}
