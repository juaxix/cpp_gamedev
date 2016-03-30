#pragma once
#include <GL/glew.h>
#include "VertexData.h"
#include "GLTexture.h"
#include "ResourceManager.h"
#include <cstddef>
#include <string>
namespace xixEngine {
	class Sprite
	{
	private:
		float _x, _y, _width, _height;
		//guarantee the amount of correct size:
		//for the vertex outside buffer
		GLuint _vboID;
		GLTexture _texture;
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);
		void draw();
	};

}
