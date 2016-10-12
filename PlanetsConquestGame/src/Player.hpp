#ifndef __opengl_sample__Player__
#define __opengl_sample__Player__

#include "GameEntity.hpp"
#include "Island.hpp"
#include <vector>
#include "Actor.hpp"
#include "Ship.hpp"
#include "Tags.hpp"
#include "ServiceLocator.hpp"
#include "Input.hpp"
#include "IslandsMap.hpp"
#include "Camera.hpp"
#include <cassert>

class Camera;

class Player : public GameEntity, public IslandTouchDelegate, public Actor
{
	typedef std::vector<Island*> VIslands;
	
    VIslands _islandsSelected;
	IslandsMap* _islandMap;
    Camera* _camera;
	GUIText* _guiText;
public:
	Player();
	~Player();
	void update();
	void load();
	void render();
	void setIslandMap(IslandsMap& islandMap);
    void setCamera(Camera& camera);
	bool isIslandSelectedByPlayer(Island& island);
private:
	void onIslandTouched(Island& island);
	void clearSelectedIslands();
    void addSelectedIsland(Island& island);
	void launchSelectedIslandsShips(Island& island);
};


#endif /* defined(__opengl_sample__Player__) */
