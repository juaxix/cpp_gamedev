#include "GameEntity.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/gtc/type_ptr.hpp> 

int GameEntity::entityCounter = 0;

GameEntity::~GameEntity()
{
}

GameEntity::GameEntity(Tag tag)
{
	_tag = tag;
    
	_pos = glm::vec3(0.f);
	_scale = glm::vec3(1.f);
	_rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
    
	_model = glm::mat4(1.f);
    
	_id = GameEntity::entityCounter++;
}

GameEntity::GameEntity()
:GameEntity(Tag::UNDEFINED)
{
}

void GameEntity::setTag(Tag tag)
{
	_tag = tag;
}

Tag GameEntity::getTag()
{
	return _tag;
}

int GameEntity::getId()
{
    return _id;
}

void GameEntity::setPosition(const glm::vec3 &pos)
{
    _pos = pos;
    
    buildModel();
}

glm::vec3 GameEntity::getPosition() const
{
    return _pos;
}

void GameEntity::setScale(const glm::vec3 &newScale)
{
    _scale = newScale;
    
	buildModel();
}

const glm::vec3& GameEntity::getScale() const
{
	return _scale;
}

void GameEntity::setRotation(const glm::quat &newRot)
{
	_rotation = newRot;
}

const glm::quat& GameEntity::getRotation() const
{
	return _rotation;
}

void GameEntity::buildModel()
{
	//Order: scale * rotation * translation
	glm::mat4 mScale = glm::scale(_scale);
	glm::mat4 mRot = glm::mat4_cast(_rotation);
	glm::mat4 mPos = glm::translate(_pos);
	
	_model = mPos * mRot * mScale;
}

void GameEntity::move(const glm::vec3 &pos)
{
	setPosition(_pos + pos);
}

const glm::mat4& GameEntity::getModel()
{
	return _model;
}

void GameEntity::setName(const std::string& name)
{
	_name = name;
}

const std::string& GameEntity::getName()
{
	return _name;
}
