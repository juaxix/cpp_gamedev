#include "World.hpp"
#include "GameEntity.hpp"
#include "Island.hpp"
#include "GameController.hpp"

World::~World() 
{
    removePendingEntities();
    
    for (GameEntity* entity : _entitiesToAdd) {
        delete entity;
    }
    _entitiesToAdd.clear();
    
    for (GameEntity* entity : _entities) {
        delete entity;
    }
    _entities.clear();
}

void World::load()
{
    for (GameEntity* entity : _entities) {
        entity->update();
    }
}

void World::update()
{
    removePendingEntities();
    addPendingEntities();
    updateEntities();
}

void World::updateEntities()
{
	
    for (GameEntity* entity : _entities) 
	{
        entity->update();
		
    }
	
}

void World::addPendingEntities()
{
    for (GameEntity* newEntity : _entitiesToAdd) 
    {
        _entities.push_back(newEntity);
    }
    _entitiesToAdd.clear();
}

void World::removePendingEntities()
{
    for (GameEntity* removedEntity : _entitiesToRemove) 
    {
        Entities::iterator it = std::find(_entities.begin(), _entities.end(), removedEntity);
        
        assert(it!=_entities.end());
        if(it != _entities.end())
        {
            _entities.erase(it);
            delete removedEntity;
        }
    }
    _entitiesToRemove.clear();
}

void World::render()
{       
    for (GameEntity* entity : _entities) {
        entity->render();
    }
}

void World::addEntity(GameEntity* entity)
{
    assert(entity);
    entity->load();
    
    _entitiesToAdd.push_back(entity);
}

void World::removeEntity(GameEntity* entity)
{
    assert(entity);
    entity->onDestroy();
    
    _entitiesToRemove.push_back(entity);
}

