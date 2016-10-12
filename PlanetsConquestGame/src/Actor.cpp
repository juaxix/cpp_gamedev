#include "Actor.hpp"


Actor::Actor(Tag shipsTag)
{
	this->_shipsTag = shipsTag;
}

Ship *Actor::createShip(
	const glm::vec3 &origin, const glm::vec3 &destiny,
	const glm::vec3 &rotation, const glm::vec3 &endRotation,
	Island* island
){
	Ship* ship = new Ship(this->_shipsTag);
	ship->load();
	ship->setPosition(origin);
	ship->setRotation(glm::quat(rotation));
	ship->setIslandDestiny(island);
	ship->fly(destiny, endRotation);
	return ship;
}
Actor::~Actor()
{
}
