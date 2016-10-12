#ifndef __opengl_sample__EntityManager__
#define __opengl_sample__EntityManager__

#include <iostream>
#include <vector>
#include "GameScene.hpp"

class GameEntity;

class World
{
    typedef std::vector<GameEntity*> Entities;
    Entities _entities;
    Entities _entitiesToAdd;
    Entities _entitiesToRemove;
	
public:
	
	~World();
	void load();
    
    void update();
    void render();
    
    void addEntity(GameEntity* entity);
    void removeEntity(GameEntity* entity);
private:
    void removePendingEntities();
    void addPendingEntities();
    void updateEntities();
};

#endif /* defined(__opengl_sample__EntityManager__) */
