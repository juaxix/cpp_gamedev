#ifndef __opengl_sample__IslandsMap__
#define __opengl_sample__IslandsMap__

#include "GameEntity.hpp"
#include "Tags.hpp"
#include "MathUtils.hpp"
#include "Island.hpp"
#include "ServiceLocator.hpp"
#include "World.hpp"
#include "Tags.hpp"
#include "IScene.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <stdarg.h>
#include <cstdio>
#include "GameController.hpp"

class IslandsMap : public GameEntity
{	
public:
	typedef std::vector<Island*> Islands;
    
private:
	Islands _islands;
	GameController* _gameController;
    
public:
	IslandsMap();
	void load();
	Island* getTouchedIsland(const glm::vec3& point);
	void setGameController(GameController& controller);
	void getIslandsByOwnerTag(Islands& out, Tag tag);
	void getIslandsByOwnerTags(Islands& out, int nTags, ...);
	void update();
	int getUnitsByOwnerTag(Tag tag);
};

#endif /* defined(__opengl_sample__IslandsMap__) */
