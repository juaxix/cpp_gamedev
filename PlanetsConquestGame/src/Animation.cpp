#include "Animation.hpp"
#include "ServiceLocator.hpp"
#include "TextureManager.hpp"
#include <cassert>
#include "Time.hpp"


void Animation::init(int sizeTileX, int sizeTileY, int rows, int columns, const std::string& textureName, float secondsPerFrame)
{
	_sizeTileX = sizeTileX;
	_sizeTileY = sizeTileY;
	_rows = rows;
	_columns = columns;
	_gridSizeX = sizeTileX*columns;
	_gridSizeY = sizeTileY*rows;
	_textureId = ServiceLocator::get().getTextureManager().getTexture(textureName);
	_frameSpeed = 1;
	_frameTimeLimit = secondsPerFrame;
	_frameTime = 0;
	_totalFrames = rows*columns;
	_currentFrame = 1;
	loops = 0;
}

void Animation::update()
{
	_frameTime -= Time::getDeltatime();
	if (_frameTime <= 0)
	{
		_frameTime = _frameTimeLimit;
		_currentFrame += _frameSpeed;
		if (_currentFrame>_totalFrames)
		{
			_currentFrame = 1;
			loops++;
		}
	}
}


void Animation::renderQuad(const glm::vec2& posMin, const glm::vec2& posMax, const glm::vec2& uvMin, const glm::vec2& uvMax)
{
	assert(_textureId > 0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glBegin(GL_TRIANGLES);

	glTexCoord2f(uvMin.x, uvMax.y); glVertex3f(posMin.x, posMin.y, 0.f);
	glTexCoord2f(uvMax.x, uvMax.y); glVertex3f(posMax.x, posMin.y, 0.f);
	glTexCoord2f(uvMin.x, uvMin.y); glVertex3f(posMin.x, posMax.y, 0.f);

	glTexCoord2f(uvMin.x, uvMin.y); glVertex3f(posMin.x, posMax.y, 0.f);
	glTexCoord2f(uvMax.x, uvMax.y); glVertex3f(posMax.x, posMin.y, 0.f);
	glTexCoord2f(uvMax.x, uvMin.y); glVertex3f(posMax.x, posMax.y, 0.f);

	glEnd();
	
}

void Animation::render()
{
	glm::vec3 size = getScale();
	
	int x = _currentFrame / _columns;
	int y = _currentFrame / _rows;

	float uZise = (1.f / (float)_rows);
	float vZise = (1.f / (float)_columns);

	float uMin = uZise * (float)x;
	float uMax = uMin + uZise;

	float vMin = vZise * (float)y;
	float vMax = vMin + vZise;

	glm::vec2 posMin(0.f);
	glm::vec2 posMax(size.x, size.y);

	glm::vec2 uvMin(uMin, vMin);
	glm::vec2 uvMax(uMax, vMax);

	renderQuad(posMin, posMax, uvMin, uvMax);
	//std::cout << "Frame " << _currentFrame << "; x: " << x << "; y: " << y << "\n";
}

