#ifndef __opengl_sample__GUIText__
#define __opengl_sample__GUIText__

#include "GameEntity.hpp"
#include <string>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class GUIText : public GameEntity
{
	int _gridX;
	int _gridY;
    
	GLuint _font;
	std::string _text;
    
public:
	GUIText();
    
	void init(int gridX, int gridY, const std::string& atlasName);
	void setText(const std::string& text);
	void render();
    
	static void renderQuad(const glm::vec2& posMin, const glm::vec2& posMax, const glm::vec2& uvMin, const glm::vec2& uvMax, GLuint textureId);
};

#endif /* defined(__opengl_sample__GUIText__) */
