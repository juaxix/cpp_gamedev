#include "Island.hpp"

int Island::nIslands = 0;

Island::Island()
:GameEntity(Tag::ISLAND), _ownerTag(Tag::NEUTRAL), _plane(NULL), _guiText(NULL)
, _lastTimeUnitsIncremented(0.f), _unitsIncrementStep(0.5f), _units(0), _isSelected(false)
{
	nIslands++;
	std::stringstream s;
	s << "Island " << nIslands;
	this->setName(s.str());
	_lastTimeAILaunch = 2; //2 seconds delay at the start
	initialized = false;
}

Island::~Island()
{
    if(_plane)
    {
        delete _plane;
    }
    
    if(_guiText)
    {
        delete _guiText;
    }
	state = IslandState::NEUTRAL;
}

void Island::load()
{
	_plane = new Plane();
	_plane->load();
    
	_guiText = new GUIText();
	_guiText->init(10, 10, ResourcesPaths::kTextAtlas);
    _guiText->setPosition(glm::vec3(-0.48f, -0.6f, 0.f));
    _guiText->setScale(glm::vec3(1.f));
	_maxUnits = (int) getScale().x;
	_units = 0;
	updateView();
	initialized = true;
}

void Island::update()
{
	if (getOwnerTag() == Tag::AI || getOwnerTag() == Tag::PLAYER)
	{
		_lastTimeUnitsIncremented -= Time::getDeltatime();
		if (_lastTimeUnitsIncremented <= 0)
		{
			//add an unit
			_units = MathUtils::min(_units + 1, _maxUnits);
			updateView();
			//reset time
			_lastTimeUnitsIncremented = _unitsIncrementStep;
		}
	}
	if (_lastTimeAILaunch>0 && getOwnerTag() == Tag::AI)
		_lastTimeAILaunch -= Time::getDeltatime();
}

void Island::onShipLands(Ship *ship)
{
	//std::cout << ship->getName() << " Arrived to " << this->getName()<< ":" << ((int)this->getOwnerTag()) << "\n";
	
	if (getOwnerTag() == Tag::NEUTRAL){
		this->_units++;
		this->setOwnerByTag(ship->getOwnerTag());
	}
	else {
		if (ship->getOwnerTag() != getOwnerTag())
		{
			this->_units--;
		}

		if (this->_units <= 0){
			this->setOwnerByTag(ship->getOwnerTag());
			this->_units = MathUtils::abs(this->_units);
		}
	}

	updateView();
}

void Island::launchShipUnits(int nShips, Island* destinyIsland, Actor *actor)
{
	nShips = MathUtils::min(nShips, this->_units);
	this->_units -= nShips;
	for (int i = 0; i < nShips; i++)
	{
		//Instantiate ship
		Ship *ship = actor->createShip(
			this->getPosition(), destinyIsland->getPosition(),
			glm::vec3(0.0f), glm::vec3(0.f, 0.f, MathUtils::rand(-0.6f,0.6f)),
			destinyIsland
		);
		
		//std::cout << ship->getName() <<  " Go to " << destinyIsland->getName()<< ":" << ((int)destinyIsland->getOwnerTag()) << "\n";
		
	}
	_lastTimeAILaunch = _timeLaunchDelay;
	updateView();
	
}

bool Island::isPointInside(const glm::vec3& point)
{
	glm::vec3 cursorDir = point - getPosition();
    
	float distToPoint = glm::length(cursorDir);
    
    // check whether the point is inside the cicle inside the island's square (side x and y are the same value)
	if (distToPoint <= getScale().x)
	{
		return true;
	}
    
	return false;
}

void Island::setOwnerByTag(Tag newOwer)
{
	_ownerTag = newOwer;
	this->setIsSelected(this->_isSelected);
    updateView();
}

Tag Island::getOwnerTag()
{
	return _ownerTag;
}

void Island::updateView()
{
	std::string textureName = ResourcesPaths::kNeutralIslandImg;
    
	if (_ownerTag == Tag::AI)
	{
        if (_isSelected)
		{
			textureName = ResourcesPaths::kEnemyIslandSelectedImg;
		}
		else {
			textureName = ResourcesPaths::kEnemyIslandImg;
		}
	}
	if (_ownerTag == Tag::PLAYER)
	{
		if (_isSelected)
		{
			textureName = ResourcesPaths::kPlayerIslandSelectedImg;
		}
		else {
			textureName = ResourcesPaths::kPlayerIslandImg;
		}
	}
    
    if(_plane)
    {
        _plane->setTextureByName(textureName);
    }
    
    updateText();
}

void Island::updateText()
{
	char buffer[256];
	sprintf_s(buffer, "%d", _units);
	_guiText->setText(buffer);
}

void Island::render()
{   
    // be sure the matrix model is set to modelview to not override GL_PROJECTION (camera settings)
	glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glm::mat4 planeLocalToWorld = getModel() * _plane->getModel();
	glLoadMatrixf(&planeLocalToWorld[0][0]);
    _plane->render();
    
    glm::mat4 textLocalToWorld = getModel() * _guiText->getModel();
	glLoadMatrixf(&textLocalToWorld[0][0]);
    _guiText->render();
}

void Island::setIsSelected(bool isSelected)
{
	_isSelected = isSelected;
	if (_isSelected){
		if (getOwnerTag() == Tag::AI){
			state = IslandState::SELECTED_BY_AI;
		}
		else {
			if (getOwnerTag() == Tag::PLAYER)
			{
				state = IslandState::SELECTED_BY_PLAYER;
			}
			else {
				state = IslandState::NEUTRAL;
			}
		}
	}
	else {
		if (getOwnerTag() == Tag::AI){
			state = IslandState::AI;
		}
		else {
			if (getOwnerTag() == Tag::PLAYER)
			{
				state = IslandState::PLAYER;
			}
			else {
				state = IslandState::NEUTRAL;
			}
		}
	}
	updateView();
}
/*
void Island::setGameController(GameController& gameController)
{
	_gameController = &gameController;
}
*/