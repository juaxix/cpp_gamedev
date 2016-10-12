#ifndef __opengl_sample__Animation__
#define __opengl_sample__Animation__
#include "GameEntity.hpp"
#include <string>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Animation: public GameEntity
{
	int _currentFrame, _frameSpeed, _totalFrames, _sizeTileX, _sizeTileY, _rows, _columns, _gridSizeX, _gridSizeY,loops=0;
	GLuint _textureId;
	float _frameTimeLimit, _frameTime;
public:
	inline int getLoops() const { return loops; };
	void init(int sizeTileX, int sizeTileY, int rows, int columns, const std::string& textureName, float secondsPerFrame = 0.1f);
	void update();
	void render();
private:
	void renderQuad(const glm::vec2& posMin, const glm::vec2& posMax, const glm::vec2& uvMin, const glm::vec2& uvMax);
	
	
};


#endif /* defined(__opengl_sample__Animation__) */
