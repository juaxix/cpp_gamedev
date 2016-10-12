#include "Player.hpp"


Player::Player() :GameEntity(Tag::PLAYER), _islandMap(NULL), _camera(NULL), Actor(Tag::PLAYER_SHIP), _guiText(NULL)
{

}

void Player::load(){
	_guiText = new GUIText();
	_guiText->init(10, 10, ResourcesPaths::kTextAtlas);
	_guiText->setPosition(glm::vec3(39, 89, 0));
	_guiText->setScale(glm::vec3(9.f, 3.4f, 1.f));
	_guiText->setText("0");
}


void Player::render(){
	//draw player units objects:
	glm::mat4 textLocalToWorld = _guiText->getModel();
	glLoadMatrixf(&textLocalToWorld[0][0]);
	_guiText->render();
}

Player::~Player(){
	if (_guiText){
		delete _guiText;
	}
}

void Player::setIslandMap(IslandsMap& islandMap)
{
	_islandMap = &islandMap;
}

void Player::setCamera(Camera& camera)
{
	_camera = &camera;
}

void Player::update()
{
	Input& input = ServiceLocator::get().getInput();
    
	if (input.getMouseButtonBecomesPressed(0))
	{
		glm::vec3 cursorWorldPos = input.getCursorWorldPos(*_camera);
        
		assert(_islandMap);
		Island* island = _islandMap->getTouchedIsland(cursorWorldPos);
        
		if (island)
		{
			onIslandTouched(*island);
		}
		else
		{
			clearSelectedIslands();
		}
	}
	//get players units
	//update my units count
	std::stringstream s;
	s << "Player: " << this->_islandMap->getUnitsByOwnerTag(getTag());
	_guiText->setText(s.str());
}


void Player::addSelectedIsland(Island& island)
{
	if (!isIslandSelectedByPlayer(island))
    {
        _islandsSelected.push_back(&island);
    }
}

void Player::clearSelectedIslands()
{
	for (Island* island: _islandsSelected)
	{
		island->setIsSelected(false);
	}
    
	_islandsSelected.clear();
}


void Player::launchSelectedIslandsShips(Island& island)
{
	for (VIslands::iterator it = _islandsSelected.begin(); it != _islandsSelected.end(); ++it)
	{
		(*it)->launchShipUnits(
			(*it)->getUnits(),&island,this
		);
	}
	clearSelectedIslands();
}

void Player::onIslandTouched(Island& island)
{
	const Tag pTag = getTag();
	Tag iTag = island.getOwnerTag();
	if(0 == _islandsSelected.size())
    {
        if(pTag == iTag)
        {
            setTag(island.getOwnerTag());
            island.setIsSelected(true);
            addSelectedIsland(island);
        }
    }
	else {
		switch (iTag)
		{
		case Tag::PLAYER:
			island.setIsSelected(true);
			addSelectedIsland(island);
			break;
		case Tag::AI:
		case Tag::NEUTRAL:
			//launch ships
			launchSelectedIslandsShips(island);
			
			break;
		}
	}
	
}

bool Player::isIslandSelectedByPlayer(Island& island)
{
	VIslands::iterator it = std::find(_islandsSelected.begin(), _islandsSelected.end(), &island);

	return (_islandsSelected.end() != it);
}
