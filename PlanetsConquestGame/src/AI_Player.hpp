#ifndef __opengl_sample__AI_Player__
#define __opengl_sample__AI_Player__

#include "GameEntity.hpp"
#include "Actor.hpp"
#include "Ship.hpp"
#include "IslandsMap.hpp"
#include "Island.hpp"
#include "Tags.hpp"
#include <vector>
#include <cassert>
#include "Time.hpp"

class AI_Player : public GameEntity,public Actor
{
	typedef std::vector<Island*> VIslands;

	VIslands _islandsSelected;
	IslandsMap* _islandMap;
	IslandsMap::Islands ai_islands, target_islands;
	float lastAttackTime; //seconds
	int myUnits;
	const float timeBetweenAttacks = 0.777f; //seconds
	Island* lastIslandAttacking;
	GUIText* _guiText;
public:
	AI_Player();
	~AI_Player();
	void load();
	void render();
	void update();
	void setIslandMap(IslandsMap& islandMap);
	

private:
	void clearSelectedIslands();
	void addSelectedIsland(Island& island);
};

#endif /* defined(__opengl_sample__AI_Player__) */
