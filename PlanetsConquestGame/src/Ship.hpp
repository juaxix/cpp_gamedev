#ifndef __opengl_sample__Ship__
#define __opengl_sample__Ship__
#include "GameEntity.hpp"
#include "Tags.hpp"
#include "Time.hpp"
#include "Plane.hpp"
#include "ResourcesPaths.hpp"
#include "MathUtils.hpp"
#include "ServiceLocator.hpp"
#include "Animation.hpp"
#include <sstream>
class Island;

class ShipIsLandedDelegate
{
public:
	void OnShipLanded();
};

enum class ShipState
{
	NEUTRAL = 0,
	FLYING,
	EXPLODING,
	DISABLED
};

class Ship : public GameEntity
{
	
	float accel;
	glm::vec3 _speed, _destiny;
	glm::vec3 _rotation;
	ShipState state;
	Plane* _plane;
	Tag _ownerTag;
	Island *islandDestiny;
	Animation explosion;
	static int nShips;
public:
	Ship();
	Ship(Tag tag);
	inline Tag getOwnerTag() const { return this->_ownerTag; };
	void load();
	inline void setIslandDestiny(Island* island){ this->islandDestiny = island; }
	void update();
	void render();
	void explode();
	void fly(const glm::vec3 &position, const glm::vec3 &rotation);
	~Ship();
private:
	void nameThisShip();
};

#endif /* defined(__opengl_sample__Ship__) */