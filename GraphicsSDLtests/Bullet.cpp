#include "Bullet.h"
/*
Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float speed, const std::string& texturePath, int lifeTime) 
{
	this->_position = position;
	this->_direction = direction;
	this->_speed = speed;
	this->_texture = xixEngine::ResourceManager::getTexture(texturePath);
	this->_lifeTime = lifeTime;
}

Bullet::~Bullet() {}

void Bullet::draw(xixEngine::SpriteBatch& spriteBatch)
{
	spriteBatch.draw(
		//Pos and size :
		glm::vec4(_position.x, _position.y, _size, _size),
		_uv, _texture.id, 0.0f, _color
	);
}

void Bullet::update()
{
	
		_position += _direction * _speed; //should apply gavity and other stuff
		_lifeTime--;
	
}

*/

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime)
{
	_lifeTime = lifeTime;
	_position = pos;
	_direction = dir;
	_speed = speed;
}


Bullet::~Bullet()
{
}

void Bullet::draw(xixEngine::SpriteBatch& spriteBatch) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static xixEngine::GLTexture texture = xixEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/Bullet.png");
	xixEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30, 30);

	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);
}

bool Bullet::update() {
	_position += _direction * _speed;
	_lifeTime--;
	if (_lifeTime == 0) {
		return true;
	}
	return false;
}
