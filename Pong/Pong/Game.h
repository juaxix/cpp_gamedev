#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Tweener.h"
#include <sstream>
class Game
{
private:
	sf::Clock clock;
	bool leftKeyPressed;
	bool rightKeyPressed;
	bool upKeyPressed;
	bool downKeyPressed;
	//Render window
	sf::RenderWindow *window;

	//Events
    sf::Event gameEvents;

	//Textures
	sf::Texture texPad;
	sf::Texture texBall;
	sf::Texture texBackground;

	//Sounds
	sf::SoundBuffer buffHit;
	sf::Sound soundHit;
	float lastHitSound;
	//Font
	sf::Font font;

	//Shapes for layers
	sf::RectangleShape rBackground;
	sf::RectangleShape rPad1, rPad2;
	sf::RectangleShape rBall;


	//Texts
	sf::Text textScore;

	//Goals
	int goal1,goal2;

	////Physics
	//Pad 1
	int ySpeedPad1;
	//Pad 2
	int ySpeedPad2;
	//Ball
	int xSpeedBall, ySpeedBall;

	//Private Methods
	bool LoadResources();
	void InitializeGame();
	void GetAndProcessEvents();
	void Logic();
	void Goal(int nPad);
	void Draw();
	
public:
	bool playerWantToExit;
	const static int speedChange = 5;
	const static int SPEED_BALL = 4;
	//Public methods
	Game(void);
	void Loop();
	virtual ~Game(void);
};

