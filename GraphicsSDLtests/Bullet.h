#pragma once
#include <glm/glm.hpp>
#include <xixEngine/SpriteBatch.h>
#include <xixEngine/ResourceManager.h>
#include <string>
class Bullet
{
private:
	float _speed;
	glm::vec2 _direction; //combined with speed we get the velocity 
	glm::vec2 _position;
	glm::vec4 _uv;
	float _size;
	int _lifeTime;

	xixEngine::Color _color;
	xixEngine::GLTexture _texture;

public:
	Bullet();
	Bullet(glm::vec2 position, glm::vec2 direction, float speed, const std::string& texturePath, int lifeTime);
	~Bullet();
	
	void setPosition(glm::vec2 position) { _position = position; }
	const glm::vec2 getPosition()const { return _position; };
	void setDirection(glm::vec2 direction) { _direction = direction; }
	const glm::vec2 getDirection()const { return _direction; };
	void setSize(const float size) { _size = size; }
	const float getSize() const { return _size; }
	void setColor(xixEngine::Color color) { _color = color; }
	const xixEngine::Color getColor() const { return _color; }
	const float getSpeed() const { return _speed; }
	const xixEngine::GLTexture getTexture() const { return _texture;  }
	void draw(xixEngine::SpriteBatch& spriteBatch);
	void update();
	bool isAlife() { return (_lifeTime > 0.0f); }
};

