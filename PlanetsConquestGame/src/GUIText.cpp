#include "GUIText.hpp"
#include "ServiceLocator.hpp"
#include "TextureManager.hpp"
#include <cassert>

GUIText::GUIText()
:_text(""), _gridX(10), _gridY(10)
{
    
}

void GUIText::init(int gridX, int gridY, const std::string& atlasName)
{
    _gridX = gridX;
    _gridY = gridY;
    
    GLuint fontTexture = ServiceLocator::get().getTextureManager().getTexture(atlasName);
	_font = fontTexture;
}

void GUIText::render()
{
    glm::vec3 size = getScale();
    
	size_t numChars = _text.size();
	float charSizeX = size.x / ((float)numChars);
	float charPosX = 0.f;
    
	for (char c: _text)
	{
		int pos = c - ' ';
        
		int x = pos%_gridX;
		int y = pos/_gridX;
        
		float uZise = (1.f / (float)_gridX);
		float vZise = (1.f / (float)_gridY);
        
		float uMin = uZise * (float)x;
		float uMax = uMin + uZise;
        
		float vMin = vZise * (float)y;
		float vMax = vMin + vZise;
        
		glm::vec2 charPosMin(charPosX, 0.f);
		glm::vec2 charPosMax(charPosX + charSizeX, size.y);
		
		glm::vec2 uvMin(uMin, vMin);
		glm::vec2 uvMax(uMax, vMax);
		
		renderQuad(charPosMin, charPosMax, uvMin, uvMax, _font);
		charPosX += charSizeX;
	}
}

void GUIText::setText(const std::string& text)
{
	_text = text;
}

void GUIText::renderQuad(const glm::vec2& posMin, const glm::vec2& posMax, const glm::vec2& uvMin, const glm::vec2& uvMax, GLuint textureId)
{
	assert(textureId > 0);
    
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
    
	glBegin(GL_TRIANGLES);
    
	glTexCoord2f(uvMin.x, uvMax.y); glVertex3f(posMin.x, posMin.y, 0.f);
	glTexCoord2f(uvMax.x, uvMax.y); glVertex3f(posMax.x, posMin.y, 0.f);
	glTexCoord2f(uvMin.x, uvMin.y); glVertex3f(posMin.x, posMax.y, 0.f);
    
	glTexCoord2f(uvMin.x, uvMin.y); glVertex3f(posMin.x, posMax.y, 0.f);
	glTexCoord2f(uvMax.x, uvMax.y); glVertex3f(posMax.x, posMin.y, 0.f);
	glTexCoord2f(uvMax.x, uvMin.y); glVertex3f(posMax.x, posMax.y, 0.f);
    
	glEnd();
}
