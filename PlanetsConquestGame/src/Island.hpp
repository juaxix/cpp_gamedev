#ifndef __opengl_sample__Island__
#define __opengl_sample__Island__

#include "Tags.hpp"
#include "GameEntity.hpp"
#include "Actor.hpp"
#include "ServiceLocator.hpp"
#include "Input.hpp"
#include "IScene.hpp"
#include "Plane.hpp"
#include "GUIText.hpp"
#include "Time.hpp"
#include "MathUtils.hpp"
#include "ResourcesPaths.hpp"
#include "MathUtils.hpp"
#include "Actor.hpp"
#include "Ship.hpp"
#include <sstream>

//#include "GameController.hpp"


class Plane;
class Island;
class GUIText;
//class GameController;

enum class IslandState
{
	NEUTRAL = 0,
	PLAYER,
	SELECTED_BY_PLAYER,
	AI,
    SELECTED_BY_AI
};

class IslandTouchDelegate
{
public:
	virtual void onIslandTouched(Island& island) = 0;
};

class Island : public GameEntity
{
	Plane* _plane;
    Tag _ownerTag;
	GUIText* _guiText;
	//GameController* _gameController;
	IslandState state;
	bool _isSelected;
	int _maxUnits;
	int _units=0;
	float _lastTimeUnitsIncremented;
	float _unitsIncrementStep;
	float _lastTimeAILaunch;
	bool initialized = false;
	const float _timeLaunchDelay = 0.5f;
	static int nIslands;
public:
    Island();
    ~Island();
    
    void load();
    void update();
    void render();
    
	bool isPointInside(const glm::vec3& point);
    
	void setOwnerByTag(Tag owner);
	Tag getOwnerTag();
	inline IslandState getIslandState() const { return state; };
	inline bool isInitialized() const { return initialized; };
	inline bool isTimeForAILaunch(){ return _lastTimeAILaunch <= 0; };
	void setIsSelected(bool isSelected);
	inline int getUnits(){ return _units; };
	//void setGameController(GameController& gameController);
	void onShipLands(Ship *ship);
	void launchShipUnits(int nShips, Island* destinyIsland, Actor *actor);
private:
	void updateView();
	void updateText();
};


#endif /* defined(__opengl_sample__Island__) */
