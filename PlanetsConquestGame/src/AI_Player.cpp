#include "AI_Player.hpp"

AI_Player::AI_Player() :GameEntity(Tag::AI), _islandMap(NULL), Actor(Tag::AI_SHIP), _guiText(NULL)
{
	this->ai_islands = IslandsMap::Islands();
	this->target_islands = IslandsMap::Islands();
	myUnits = 0;
	lastAttackTime = 0.0f;
	lastIslandAttacking = NULL;
}

void AI_Player::load()
{
	_guiText = new GUIText();
	_guiText->init(10, 10, ResourcesPaths::kTextAtlas);
	_guiText->setPosition(glm::vec3(-133, -93, 0));
	_guiText->setScale(glm::vec3(9.f, 3.4f, 1.f));
	_guiText->setText("AI Ships: 1");
}


void AI_Player::update()
{
	//update ships information:
	ai_islands.clear();
	target_islands.clear();
	_islandMap->getIslandsByOwnerTags(target_islands, 2, (int)Tag::NEUTRAL, (int)Tag::PLAYER);
	myUnits = 0;
	this->_islandMap->getIslandsByOwnerTag(ai_islands, Tag::AI);
	
	//update attack time
	lastAttackTime -= Time::getDeltatime();
	/* @TODO: Add logic to choose the best neighbour based on balanced trees or redblacktree */
	//Do we have islands to move to?
	if (ai_islands.size() > 0 && target_islands.size() > 0)
	{
		float min_distance = 9999;
		int max_units = 0;
		Island *best_attacker_island = NULL;
		int withUnits = 0;
		glm::vec3 p = getPosition();
		myUnits = 0;
		for (Island* ai_island : ai_islands)
		{
			int nUnits = ai_island->getUnits();
			myUnits += nUnits;

			/*if (ai_island != lastIslandAttacking && ai_island->isTimeForLaunch() && lastAttackTime <= 0 && nUnits > 0)
			{
				lastIslandAttacking = ai_island;
			}
			else {
				break;
			}*/
			//consider this island as next island attacker neighbours?
			if (
				((ai_islands.size()>1 && ai_island != lastIslandAttacking ) || ai_islands.size()==1) && 
				ai_island->isTimeForAILaunch() && nUnits > 0)
			{
				const glm::vec3 dest = ai_island->getPosition();
				float d = std::sqrt( //get distance to this target island from the ai island
					((dest.x - p.x)*(dest.x - p.x)) +
					((dest.y - p.y)*(dest.y - p.y))
					);
				if (d < min_distance && nUnits > max_units){
					min_distance = d;
					best_attacker_island = ai_island;
					withUnits = nUnits;
				}
			}
		}
		
		if (lastAttackTime <= 0 && best_attacker_island != NULL){
			lastIslandAttacking = best_attacker_island;
			target_islands.clear();
			_islandMap->getIslandsByOwnerTags(target_islands, 2, (int)Tag::NEUTRAL, (int)Tag::PLAYER);
			lastAttackTime = timeBetweenAttacks;

			if (target_islands.size() > 0){
				best_attacker_island->launchShipUnits(withUnits, target_islands[
					(int)MathUtils::rand(0, (int) target_islands.size())
				], this);
				//std::cout << "AI:: Sending " << nUnits << "\n";
			}
		}
		
	}
	//std::cout << ai_islands.size() << " : " << target_islands.size() << "\n";
	//update AI units count
	std::stringstream s;
	s << "AI Units: " << myUnits;
	_guiText->setText(s.str());
}


void AI_Player::render(){
	//render ai units objects:
	glm::mat4 textLocalToWorld = _guiText->getModel();
	glLoadMatrixf(&textLocalToWorld[0][0]);
	_guiText->render();
}

void AI_Player::setIslandMap(IslandsMap& islandMap){
	this->_islandMap = &islandMap;
}

AI_Player::~AI_Player()
{
	//delete ai_islands;
	if (_guiText){
		delete _guiText;
	}
}
