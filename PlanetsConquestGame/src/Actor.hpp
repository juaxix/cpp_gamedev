#ifndef __opengl_sample__Actor__
#define __opengl_sample__Actor__

#include "Ship.hpp"
#include "ServiceLocator.hpp"
#include "IScene.hpp"
//#include "Island.hpp"
class Island;

class Actor
{
	Tag _shipsTag;
public:
	Actor(Tag shipTags);
	Ship *createShip(const glm::vec3 &origin, const glm::vec3 &destiny, const glm::vec3 &rotation, const glm::vec3 &endRotation,Island* island);
	~Actor();
};

#endif /* defined( __opengl_sample__Actor__) */