/**
 @file Game.cpp
 @brief Main Class of the Simple Pong Game
 @author Juan Belón Pérez
 @date 01/11/2015
*/
#include "Game.h"
/**
@brief Constructor of the Game class. Creates the Render Window, Load the resources and initialize the game.
@return Game
*/
Game::Game(void)
{
	//init flag states
	playerWantToExit = leftKeyPressed = rightKeyPressed = upKeyPressed = downKeyPressed=false;
	window = new sf::RenderWindow(sf::VideoMode(800, 600), "Pong!");
	window->setFramerateLimit(60);
	//load all game resources here
	if(!LoadResources()){
		playerWantToExit = true;
	} else {
		InitializeGame();
	}
}
/**
@brief Load all the resources of the game, if any of the resources fails to load, exits the game.
*/
bool Game::LoadResources()
{
	//Textures
	if(texPad.loadFromFile("GameData/pad.png")==0){
		std::cout<<"Cant load pad texture";
		return false;
	}
	if(texBall.loadFromFile("GameData/ball.png")==0){
		std::cout<<"Cant load ball texture";
		return false;
	}
	if(texBackground.loadFromFile("GameData/background.png")==0){
		std::cout<<"Cant load background texture";
		return false;
	}

	//Sounds
	if(buffHit.loadFromFile("GameData/hit.wav")==0){
		std::cout<<"Cant load hit sound";
		return false;
	} else {
		soundHit.setBuffer(buffHit);
	}

	//Fonts
	if(font.loadFromFile("GameData/ARCADE_R.TTF")==0){
		std::cout<<"Cant load Arcade font";
		return false;
	}

	return true;
}
/**
 @brief Set default values of all the elements of the game and variables. Reset the clock.
*/
void Game::InitializeGame()
{

	//Background
	rBackground.setSize(sf::Vector2f(800,600));
	rBackground.setPosition(0,0);
	rBackground.setTexture(&texBackground);

	//Pad 1
	rPad1.setSize(sf::Vector2f(50,100));
	rPad1.setPosition(50,200);
	rPad1.setTexture(&texPad);

	//Pad 2
	rPad2.setSize(sf::Vector2f(50,100));
	rPad2.setPosition(700,200);
	rPad2.setTexture(&texPad);

	//Ball
	rBall.setSize(sf::Vector2f(50,50));
	rBall.setPosition(400,200);
	rBall.setTexture(&texBall);

	//Texts
	textScore.setFont(font);
	textScore.setCharacterSize(30);
	textScore.setColor(sf::Color::White);
	textScore.setPosition(sf::Vector2f(350,10));
	textScore.setString("0 - 0");
	//Game Vars initializing
	ySpeedPad1 = 0;
	ySpeedPad2 = 0;
	xSpeedBall = ySpeedBall = -SPEED_BALL; //start moving left/up
	lastHitSound = 0;
	goal1 = goal2 = 0;
	//Clock
	clock.restart();
}
/**
@brief Gets and process the input events of the game,modify vars for the logic to handle
*/
void Game::GetAndProcessEvents()
{
	while (window->pollEvent(gameEvents))
    {
		switch(gameEvents.type)
		{
				case sf::Event::EventType::KeyPressed:
					switch(gameEvents.key.code){
						case sf::Keyboard::Left: leftKeyPressed = true; break;
						case sf::Keyboard::Right: rightKeyPressed = true; break;
						case sf::Keyboard::Up: upKeyPressed = true; break;
						case sf::Keyboard::Down: downKeyPressed = true; break;
						case sf::Keyboard::Space: 
							//...
							break;
					}
					break;
				case sf::Event::EventType::KeyReleased: 
					switch(gameEvents.key.code){
						case sf::Keyboard::Escape: playerWantToExit = true; break; //ESC is released?
						case sf::Keyboard::Left: leftKeyPressed = false; break;
						case sf::Keyboard::Right: rightKeyPressed = false; break;
						case sf::Keyboard::Up: upKeyPressed = false; break;
						case sf::Keyboard::Down: downKeyPressed = false; break;
					}
					break;
				case sf::Event::Closed: 
					playerWantToExit = true;
					return;
					break;

		}
    }
}

/**
@brief Handle of game mechanics
*/
void Game::Logic()
{
	////// PAD 1
	//up key?
	if(upKeyPressed){
		ySpeedPad1 = -speedChange;
	} else {
		//or down key?
		if(downKeyPressed){
			ySpeedPad1 = speedChange;
		}
	}
	//both or none
	if((upKeyPressed&&downKeyPressed)||(!upKeyPressed&&!downKeyPressed)) ySpeedPad1 = 0;
	//Move pad1  now
	if(ySpeedPad1!=0){
		rPad1.move(0, ySpeedPad1);
		if(rPad1.getPosition().y<0){
			//fix position (out of screen):
			rPad1.setPosition(50,0);
		} else {
			float lowLimitY = /*window->getSize().y*/ 600 - rPad1.getSize().y;
			if(rPad1.getPosition().y>lowLimitY){
				rPad1.setPosition(50,lowLimitY);
			}
		}
	}

	//////// Pad 2 (AI)
	//Before move the ball : check where the ball is and move the pad2 in Y axis
	if(rBall.getPosition().y < rPad2.getPosition().y)
	{
		ySpeedPad2 = -speedChange/2;
	} else {
		if(rBall.getPosition().y > rPad2.getPosition().y){
		  	ySpeedPad2 = speedChange/2;
		} else {
			ySpeedPad2 = 0;
		}
	}
	//move pad2
	rPad2.move(0, ySpeedPad2);


	//////// Ball
	rBall.move(xSpeedBall, ySpeedBall);
	bool collision = false;
	//check ball collisions with pad1:
	if(rBall.getGlobalBounds().intersects(rPad1.getGlobalBounds())){
		xSpeedBall = std::abs(xSpeedBall); //only can go right
		collision = true;
		if( (rBall.getPosition().y+50)>=rPad1.getPosition().y){
			//rBall.setPosition(100, rBall.getPosition().y-5);
			ySpeedBall = -std::abs(ySpeedBall); //only can go up
		}
	}
	//check ball collisions with pad2:
	if(!collision && rBall.getGlobalBounds().intersects(rPad2.getGlobalBounds()))
	{
		xSpeedBall = -std::abs(xSpeedBall); //can only go left
		collision = true;
	}

	//check position
	if(rBall.getPosition().y<=0){
		//hit top wall
		ySpeedBall = -ySpeedBall;
		collision = true;
	}

	if(rBall.getPosition().y>=550){
		//hit bottom wall
		ySpeedBall = -ySpeedBall;
		collision = true;
	}

	if(rBall.getPosition().x<=0){ //hit left wall: Goal for pad2
		Goal(2);
		collision = true;
	}
	if(rBall.getPosition().x>=750){ //hit right wall: Goal pad1
		Goal(1);
		collision = true;
	}

	if(collision)
	{
		if( soundHit.getStatus()!=soundHit.Playing)
		   soundHit.play();
		
	}

}

void Game::Goal(int nPad)
{
	//invert speed of the ball
	xSpeedBall = -xSpeedBall;

	switch(nPad)
	{
		case 1:
			goal1++;
			break;
		case 2:
			goal2++;
			break;
	}

	//Update Scores
	std::stringstream text;
	text << goal1 << " - " << goal2;
	textScore.setString(text.str());


	//reset ball position
	rBall.setPosition(400,200);
}



/**
@brief Draw all the components of the game in order of display ,from background to pads and ball
*/
void Game::Draw()
{
	//first the background
	window->draw(rBackground);
	window->draw(rPad1);
	window->draw(rPad2);
	window->draw(rBall);
	window->draw(textScore);
}

/**
@brief Main Loop of the game
*/
void Game::Loop()
{	
	//EVENTS:
	this->GetAndProcessEvents();
	//Do we need to exit?
	if(playerWantToExit){
		window->close();
		return;
	}

	//LOGIC
	this->Logic();

	//RENDER
    window->clear();  
	this->Draw();
    window->display();
}

/**
* @brief Destructor of Game class
*/
Game::~Game(void)
{
	//destroy anything
}
