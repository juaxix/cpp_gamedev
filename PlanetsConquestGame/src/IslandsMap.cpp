#include "IslandsMap.hpp"

IslandsMap::IslandsMap():_islands(NULL) {
	_gameController = &GameController::getInstance();
}

void IslandsMap::load()
{
	IScene& scene = ServiceLocator::get().getCurrentScene();
    
	glm::vec2 grid(7, 7);
    
	glm::vec2 grid_size = glm::vec2(180, 180);
	glm::vec2 halfgrid_size = grid_size / 2.0f;
    
	glm::vec2 block_size = grid_size / grid;
	glm::vec2 block_halfSize = block_size / 1.5f;
	
	glm::vec2 minPos = -halfgrid_size + block_halfSize;
    
	Island* island = NULL;
	Island* islandAI = NULL;
    
	int counter = 0;
	for (int i = 0; i < grid.x; i++)
	{
		for (int j = 0; j < grid.y; j++)
		{
			bool skip = MathUtils::rand(0.f, 1.f) < 0.65f;
			if (skip)
			{
				continue;
			}
            
			island = new Island();
            
			if (0 == counter)
			{
				islandAI = island;
			}
			counter++;
            
			glm::vec2 pos(block_size.x*i, block_size.y*j);
            
			glm::vec2 finalPos = minPos + pos;
            
			glm::vec3 scale(MathUtils::rand(block_size.x / 3.f, block_size.x));
            
			island->setPosition(glm::vec3(finalPos.x, finalPos.y, 0.f));
			island->setScale(scale);
			//island->setGameController(*_gameController);
            
			_islands.push_back(island);
            
			scene.addEntity(island);
		}
	}
    
	island->setOwnerByTag(Tag::PLAYER);
	islandAI->setOwnerByTag(Tag::AI);
}

Island* IslandsMap::getTouchedIsland(const glm::vec3& point)
{
	for (Island* island: _islands)
	{
		bool touched = island->isPointInside(point);
        
		if (touched)
		{
			return island;
		}
	}
    
	return NULL;
}

void IslandsMap::setGameController(GameController& controller)
{
	_gameController = &controller;
}


void IslandsMap::getIslandsByOwnerTag(Islands& out, Tag tag)
{
	for (Island* island : _islands)
	{
		if (tag == island->getOwnerTag())
		{
			out.push_back(island);
		}
	}
}

void IslandsMap::getIslandsByOwnerTags(Islands& out, int nTags, ...)
{
	va_list tagsList;
	for (Island* island : _islands)
	{
		int iTag = (int) island->getOwnerTag();
		va_start(tagsList, nTags); //Requires the last fixed parameter (to get the address)
		for (int j = 0; j < nTags; j++)
		{
			int tag = va_arg(tagsList, int); //Requires the type to cast to. Increments ap to the next argument.
			if (tag == iTag)
			{
				out.push_back(island);
				break; //add island and go for next one (already contains a tag in the list)
			}
		}
		va_end(tagsList);
	}
	
}


void IslandsMap::update()
{
	int nIslands = 0;
	int nAI_Islands = 0;
	int nPlayer_Islands = 0;
	for (Island* island : this->_islands)
	{
		//still an island?
		if (island->getTag() == Tag::ISLAND)
		{
			//is already loaded?
			if (!island->isInitialized())
			{
				continue;
			}
			//who is the owner?
			switch (island->getOwnerTag())
			{
			case Tag::AI:
				nAI_Islands++;
				break;
			case Tag::PLAYER:
				nPlayer_Islands++;
				break;
			case Tag::NEUTRAL:
				nIslands++;
				break;
			default:
				break; //undefined->not initialized yet
			}
		}
	}
	
	if (_gameController != NULL && nIslands == 0 && (
		(nPlayer_Islands == 0 && nAI_Islands>0) ||
		(nPlayer_Islands>0 && nAI_Islands == 0)
		))
	{
		//All islands are conquered!
		if (nAI_Islands == 0){
			//Player Wins
			_gameController->pushState(GameController::GameStates::WIN);
		}
		else {
			if (nPlayer_Islands == 0)
			{
				//Player Loses
				_gameController->pushState(GameController::GameStates::LOSE);
			}
		}
	}
}

int IslandsMap::getUnitsByOwnerTag(Tag tag)
{
	int total = 0;
	for (Island* island : _islands)
	{
		if (tag == island->getOwnerTag())
		{
			total += island->getUnits();
		}
	}
	return total;
}
