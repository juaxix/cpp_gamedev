#include "Bullet.h"


Bullet::Bullet():
	_speed(0),
	_direction(0.0f,0.0f),
	_position(0.0f,0.0f),
	_uv(0.0f, 0.0f, 1.0f, 1.0f),
	_size(30),
	_lifeTime(1)
{
	_color.set(255, 255, 255, 255);
}

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
	if (isAlife()) {
		_position += _direction * _speed; //should apply gavity and other stuff
		_lifeTime--;
	}
}

