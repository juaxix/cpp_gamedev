#include "Ship.hpp"
#include "Island.hpp"
#include "GameController.hpp"
#include <GLFW/glfw3.h>

//static counter of ships to use as part of the game entity name
int Ship::nShips = 0;
//Empty constructor
Ship::Ship() : GameEntity(Tag::NEUTRAL), _ownerTag(Tag::NEUTRAL), _plane(NULL), state(ShipState::NEUTRAL)
{
	_speed = glm::vec3(0, 0, 0);
	_destiny = glm::vec3(0, 0, 0);
	nameThisShip();
}
//Constructor with Tag
Ship::Ship(Tag tag)
{
	_speed = glm::vec3(0, 0, 0);
	_destiny = glm::vec3(0, 0, 0);
	this->setTag(tag);
	if (tag == Tag::AI_SHIP){
		this->_ownerTag = Tag::AI;
	}
	else {
		if (tag == Tag::PLAYER_SHIP){
			this->_ownerTag = Tag::PLAYER;
		}
		else {
			this->_ownerTag = Tag::NEUTRAL;
		}
	}
	state = ShipState::NEUTRAL;
	nameThisShip();
}

void Ship::nameThisShip(){
	nShips++;
	std::stringstream s;
	s << "Ship " << nShips;
	this->setName(s.str());
	//std::cout << this->getName() << "\n";
}

void Ship::fly(const glm::vec3 &position,const glm::vec3 &rotation)
{
	const glm::vec3 p = getPosition();
	accel = MathUtils::rand(15, 25);
	if (p.x > position.x)
		_speed.x = -1;
	else
		_speed.x = 1;
	
	if (p.y > position.y)
		_speed.y = -1;
	else
		_speed.y = 1;
	
	_speed.z = 0;
	
	_destiny = position;
	_rotation = rotation;
	
	//add rotation
	state = ShipState::FLYING;
	//add to the scene
	if (!GameController::getInstance().addToGameScene(this)){
		delete this;
	}
}

void Ship::load()
{
	_plane = new Plane();
	_plane->load();
	float scaler = MathUtils::rand(0.777f, 1.333f);
	_plane->setScale(glm::vec3(20.0f*scaler, 15.0f*scaler, 1));
	/*_guiText = new GUIText();
	_guiText->init(10, 10, ResourcesPaths::kTextAtlas);
	_guiText->setPosition(glm::vec3(-0.48f, -0.6f, 0.f));
	_guiText->setScale(glm::vec3(1.f));*/
	
	std::string textureName = "";

	if (_ownerTag == Tag::AI)
	{
		textureName = ResourcesPaths::kEnemyShipImg;
	}
	if (_ownerTag == Tag::PLAYER)
	{
		textureName = ResourcesPaths::kPlayerShipImg;
	}

	if (_plane && textureName!="")
	{
		_plane->setTextureByName(textureName);
	}
}

void Ship::update()
{
	switch (state)
	{
	case ShipState::FLYING:{
		if (islandDestiny == NULL)
		{
			std::cout << "NO ISLAND!!!";
			state = ShipState::DISABLED;
			GameController::getInstance().removeFromGameScene(this);
			return;
		}
		//get current position of the ship
		glm::vec3 p = getPosition();
		float dx = MathUtils::abs(_destiny.x - p.x);
		float dy = MathUtils::abs(_destiny.y - p.y);
		float lim = MathUtils::min(islandDestiny->getScale().x, this->getScale().x);
		
		if (dx > lim){
			if (p.x > _destiny.x)
				_speed.x = -1;
			else
				_speed.x = 1;
		}
		else {
			_speed.x = 0;
		}
		if (dy > lim){
			if (p.y > _destiny.y)
				_speed.y = -1;
			else
				_speed.y = 1;
		}
		else {
			_speed.y = 0;
		}

		//move ship with the speed and acceleration in X,Y:
		move(_speed*Time::getDeltatime()*accel);

		//get new position
		p = getPosition();
		//compute new distances to goal
		dx = MathUtils::abs(_destiny.x - p.x);
		dy = MathUtils::abs(_destiny.y - p.y);

		//rotate ship
		glm::quat rot = getRotation();
		rot.x = MathUtils::lerp(rot.x, _rotation.x, Time::getDeltatime() * 6);
		rot.y = MathUtils::lerp(rot.y, _rotation.y, Time::getDeltatime() * 6);
		rot.z = MathUtils::lerp(rot.z, _rotation.z, Time::getDeltatime() * 6);
		setRotation(rot);

		//Does the ship reached its destiny Island?
		if (dx < lim && dy < lim)
		{
			//notify island
			islandDestiny->onShipLands(this);
			this->explode();
			//wait until the planet has space orbitting the planet?
			/*
			double c = 360.0;
			float angle = std::modf(Time::getTimeSinceAppStart(), &c);
			setPosition(glm::vec3(
			std::cos(angle)*(lim*10),
			std::sin(angle)*(lim*10),
			0
			));
			*/
		}
	}
		break;
	case ShipState::EXPLODING:
	{
		{
			//We use the state of the animation to deactivate this ship
			if (explosion.getLoops() == 1)
			{
				//deactivate this ship
				state = ShipState::DISABLED;
				GameController::getInstance().removeFromGameScene(this);
			}
			else {
				explosion.update();
				/*if (this->getName() == "Ship 2"){
					std::cout << "x:" << p.x << " ,y:" << p.y << "; r:" << lim << "; dx:" <<
					dx << "; dy: " << dy << "dex:" << _destiny.x << ", dey : " << _destiny.y
					<< " \n";
					}*/
				//x:79.45, y : 28.2763; r:2; d:2.62407dex : 81.4286, dey : 30
			}
		}
	}
	}
}

void Ship::explode()
{
	
	//create explosion!
	explosion.init(96, 96, 5, 3, ResourcesPaths::kExplosionAnimationImg, 0.05f);
	explosion.setPosition(getPosition() + glm::vec3(MathUtils::rand(-0.2f, 0.2f), MathUtils::rand(-0.1f, 0.1f),0.f));
	//explosion.setScale(glm::vec3(0.3f, 0.3f, 1.f));
	explosion.setScale(getScale()/2.f);
	/**@TODO add fireworks */
	
	//add explosion && sound
	//...


	//change ship state for next frame
	state = ShipState::EXPLODING;
}

void Ship::render()
{
	switch (state)
	{
	case ShipState::FLYING:
	{
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glm::mat4 planeLocalToWorld = getModel() * _plane->getModel();
		glLoadMatrixf(&planeLocalToWorld[0][0]);
		_plane->render();
		//Life amount with gui:
		/*glm::mat4 textLocalToWorld = getModel() * _guiText->getModel();
		glLoadMatrixf(&textLocalToWorld[0][0]);
		_guiText->render();*/
		break;
	}
	case ShipState::EXPLODING:
	{
		glm::mat4 planeLocalToWorld = getModel() * _plane->getModel();
		glLoadMatrixf(&planeLocalToWorld[0][0]);
		explosion.render();
	}
		break;
	
	}
}

Ship::~Ship()
{
	if (_plane)
	{
		delete _plane;
	}

	/*if (_guiText) //add healthbars
	{
		delete _guiText;
	}*/

	
	
}
