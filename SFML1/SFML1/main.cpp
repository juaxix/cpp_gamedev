//Libraries
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include "Tweener.h"

using tweeners::Tweener;
//Global variables,functions and classes
/**
* @brief Returns the name of a key
*/
const char *getKeyName( const sf::Keyboard::Key key ) {
    switch( key ) {
    default:
    case sf::Keyboard::Unknown:
        return "Unknown";
    case sf::Keyboard::A:
        return "A";
    case sf::Keyboard::B:
        return "B";
    case sf::Keyboard::C:
        return "C";
    case sf::Keyboard::D:
        return "D";
    case sf::Keyboard::E:
        return "E";
    case sf::Keyboard::F:
        return "F";
    case sf::Keyboard::G:
        return "G";
    case sf::Keyboard::H:
        return "H";
    case sf::Keyboard::I:
        return "I";
    case sf::Keyboard::J:
        return "J";
    case sf::Keyboard::K:
        return "K";
    case sf::Keyboard::L:
        return "L";
    case sf::Keyboard::M:
        return "M";
    case sf::Keyboard::N:
        return "N";
    case sf::Keyboard::O:
        return "O";
    case sf::Keyboard::P:
        return "P";
    case sf::Keyboard::Q:
        return "Q";
    case sf::Keyboard::R:
        return "R";
    case sf::Keyboard::S:
        return "S";
    case sf::Keyboard::T:
        return "T";
    case sf::Keyboard::U:
        return "U";
    case sf::Keyboard::V:
        return "V";
    case sf::Keyboard::W:
        return "W";
    case sf::Keyboard::X:
        return "X";
    case sf::Keyboard::Y:
        return "Y";
    case sf::Keyboard::Z:
        return "Z";
    case sf::Keyboard::Num0:
        return "Num0";
    case sf::Keyboard::Num1:
        return "Num1";
    case sf::Keyboard::Num2:
        return "Num2";
    case sf::Keyboard::Num3:
        return "Num3";
    case sf::Keyboard::Num4:
        return "Num4";
    case sf::Keyboard::Num5:
        return "Num5";
    case sf::Keyboard::Num6:
        return "Num6";
    case sf::Keyboard::Num7:
        return "Num7";
    case sf::Keyboard::Num8:
        return "Num8";
    case sf::Keyboard::Num9:
        return "Num9";
    case sf::Keyboard::Escape:
        return "Escape";
    case sf::Keyboard::LControl:
        return "LControl";
    case sf::Keyboard::LShift:
        return "LShift";
    case sf::Keyboard::LAlt:
        return "LAlt";
    case sf::Keyboard::LSystem:
        return "LSystem";
    case sf::Keyboard::RControl:
        return "RControl";
    case sf::Keyboard::RShift:
        return "RShift";
    case sf::Keyboard::RAlt:
        return "RAlt";
    case sf::Keyboard::RSystem:
        return "RSystem";
    case sf::Keyboard::Menu:
        return "Menu";
    case sf::Keyboard::LBracket:
        return "LBracket";
    case sf::Keyboard::RBracket:
        return "RBracket";
    case sf::Keyboard::SemiColon:
        return "SemiColon";
    case sf::Keyboard::Comma:
        return "Comma";
    case sf::Keyboard::Period:
        return "Period";
    case sf::Keyboard::Quote:
        return "Quote";
    case sf::Keyboard::Slash:
        return "Slash";
    case sf::Keyboard::BackSlash:
        return "BackSlash";
    case sf::Keyboard::Tilde:
        return "Tilde";
    case sf::Keyboard::Equal:
        return "Equal";
    case sf::Keyboard::Dash:
        return "Dash";
    case sf::Keyboard::Space:
        return "Space";
    case sf::Keyboard::Return:
        return "Return";
    case sf::Keyboard::BackSpace:
        return "BackSpace";
    case sf::Keyboard::Tab:
        return "Tab";
    case sf::Keyboard::PageUp:
        return "PageUp";
    case sf::Keyboard::PageDown:
        return "PageDown";
    case sf::Keyboard::End:
        return "End";
    case sf::Keyboard::Home:
        return "Home";
    case sf::Keyboard::Insert:
        return "Insert";
    case sf::Keyboard::Delete:
        return "Delete";
    case sf::Keyboard::Add:
        return "Add";
    case sf::Keyboard::Subtract:
        return "Subtract";
    case sf::Keyboard::Multiply:
        return "Multiply";
    case sf::Keyboard::Divide:
        return "Divide";
    case sf::Keyboard::Left:
        return "Left";
    case sf::Keyboard::Right:
        return "Right";
    case sf::Keyboard::Up:
        return "Up";
    case sf::Keyboard::Down:
        return "Down";
    case sf::Keyboard::Numpad0:
        return "Numpad0";
    case sf::Keyboard::Numpad1:
        return "Numpad1";
    case sf::Keyboard::Numpad2:
        return "Numpad2";
    case sf::Keyboard::Numpad3:
        return "Numpad3";
    case sf::Keyboard::Numpad4:
        return "Numpad4";
    case sf::Keyboard::Numpad5:
        return "Numpad5";
    case sf::Keyboard::Numpad6:
        return "Numpad6";
    case sf::Keyboard::Numpad7:
        return "Numpad7";
    case sf::Keyboard::Numpad8:
        return "Numpad8";
    case sf::Keyboard::Numpad9:
        return "Numpad9";
    case sf::Keyboard::F1:
        return "F1";
    case sf::Keyboard::F2:
        return "F2";
    case sf::Keyboard::F3:
        return "F3";
    case sf::Keyboard::F4:
        return "F4";
    case sf::Keyboard::F5:
        return "F5";
    case sf::Keyboard::F6:
        return "F6";
    case sf::Keyboard::F7:
        return "F7";
    case sf::Keyboard::F8:
        return "F8";
    case sf::Keyboard::F9:
        return "F9";
    case sf::Keyboard::F10:
        return "F10";
    case sf::Keyboard::F11:
        return "F11";
    case sf::Keyboard::F12:
        return "F12";
    case sf::Keyboard::F13:
        return "F13";
    case sf::Keyboard::F14:
        return "F14";
    case sf::Keyboard::F15:
        return "F15";
    case sf::Keyboard::Pause:
        return "Pause";
    }
}
inline int clamp(int n, int lower, int upper){ return std::max(lower, std::min(n, upper)); }
struct animRect {
	sf::RectangleShape rect;
	float x;
};

int main()
{
	////STATES VARS
	//loop?
	bool play = true;
	//left?
	bool leftKeyPressed = false, rightKeyPressed = false, upKeyPressed=false, downKeyPressed=false;
	//left click?
	bool Click = false;
	//num of left clicks
	int nClicks = 0;
	int mouseX = 0, mouseY = 0;
	//Create the window
	sf::RenderWindow window(sf::VideoMode(800,600), "SFML Game");
	//Configure
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	//Create event handler
	sf::Event event;

	//Text Label
	sf::Font font;
	sf::String stringMousePos;
	if(!font.loadFromFile("Fonts/ARCADE_R.TTF")){
		std::cout << "Font ARCADE_R.TTF could not be loaded";
		return 1;
	}
	sf::Text text = sf::Text("",font);
	text.setColor(sf::Color::Red);
	text.setCharacterSize(24);

	//Render shapes
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(200,200));
	rectangle.setFillColor(sf::Color::Cyan);

	//Player variables
	sf::RectangleShape rPlayer;
	rPlayer.setSize(sf::Vector2f(50,90));
	rPlayer.setPosition(100,500);

	float timeJump = 0, jumpOffSet=0;
	bool jumping   = false;

	//decorations
	sf::RectangleShape background;
	printf("%d,%d",window.getSize().x,window.getSize().y);
	background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	
	std::list<animRect> clouds;
	for(int i=0; i<3; i++)
	{
		animRect animCloud;
		sf::RectangleShape _cloud(sf::Vector2f(std::rand()%100+100,std::rand()%100+50));
		_cloud.setPosition(std::rand()%window.getSize().x/1.2f+_cloud.getSize().x,std::rand()%(window.getSize().y/2)+_cloud.getSize().y);
		animCloud.rect = _cloud;
		animCloud.x    = _cloud.getPosition().x;
		clouds.push_back(animCloud);
	}
	sf::CircleShape circle;
	circle.setRadius(50);
	circle.setFillColor(sf::Color::Blue);
	
	//Textures Images
	sf::Texture paw, backgroundImage,cloud;
	if(paw.loadFromFile("Textures/paw.png")==0){
		std::cout << "\nPaw texture could not be loaded";
		return 1;
	}
	if(cloud.loadFromFile("Textures/cloud.png")==0){
		std::cout << "\nCloud texture could not be loaded";
		return 1;
	}

	if(backgroundImage.loadFromFile("Textures/background.png")==0){
		std::cout << "\nBackground texture could not be loaded";
		return 1;
	}

	sf::Music music;
	if(music.openFromFile("Sounds/Contact.ogg")){
		music.setLoop(true);
		//music.play();
	}

	background.setTexture(&backgroundImage);
	//background.setTextureRect(sf::IntRect(0,0,window.getSize().x, window.getSize().y));
	
	//assign the textures
	rectangle.setTexture(&paw);
	for(std::list<animRect>::iterator it = clouds.begin(); it!=clouds.end(); it++){
		(*it).rect.setTexture(&cloud);
	}

	//Sounds
	sf::SoundBuffer airBuffer, explosionBuffer;
	if(airBuffer.loadFromFile("Sounds/air.wav")==0 || explosionBuffer.loadFromFile("Sounds/explosion.wav")==0){
		std::cout << "\nSounds could not be loaded";
		return 1;
	}

	sf::Sound airSound, explosionSound;
	airSound.setBuffer(airBuffer);
	explosionSound.setBuffer(explosionBuffer);

	//Animation
	sf::Clock clock;
	Tweener tweener;
	//Circle animation
	float cxOffSet=0, cyOffSet=0;
	tweeners::TweenerParam cParams(2000, tweeners::BOUNCE);
	cParams.addProperty(&cxOffSet, 100);
	cParams.addProperty(&cyOffSet, 50);
	tweener.addTween(cParams);
	//Rectangle animation
	float rSx=200,rSy=200;
	tweeners::TweenerParam rParams(3000, tweeners::EASE_IN_OUT);
	rParams.addProperty(&rSx, 300);
	rParams.addProperty(&rSy, 66);
	tweener.addTween(rParams);
	//tween.addListener(new tweeners::TweenerListener());
	for(std::list<animRect>::iterator it = clouds.begin(); it!=clouds.end(); it++)
	{
		//animation for each cloud
		tweeners::TweenerParam acParam(std::rand()%2000+10000, tweeners::SINE);
		acParam.addProperty(&(*it).x, 0);
		tweener.addTween(acParam);
	}
	
	text.setPosition(0 /*window.getView().getCenter().x - text.getString().getSize()*/, 0); // window.getView().getCenter().y);

	//Game Loop (sixty times a second)
	while(play)
	{
		////EVENTS (seperate from logic)
		while(window.pollEvent(event)) //take the first event from the queue
		{
			//each event has its type (code)
			switch(event.type)
			{
				case sf::Event::EventType::KeyPressed:
					//std::cout<< "Key pressed: "<< getKeyName(event.key.code) << "\n";
					switch(event.key.code){
						case sf::Keyboard::Left: leftKeyPressed = true; break;
						case sf::Keyboard::Right: rightKeyPressed = true; break;
						case sf::Keyboard::Up: upKeyPressed = true; break;
						case sf::Keyboard::Down: downKeyPressed = true; break;
						case sf::Keyboard::Space: 
							if(!jumping && (timeJump==0 || clock.getElapsedTime().asSeconds() - timeJump > 0))
							{
								jumping=true; 
								//add tween
								tweeners::TweenerParam jParam(900, tweeners::LINEAR);
								jumpOffSet = rPlayer.getPosition().y;
								jParam.addProperty(&jumpOffSet, jumpOffSet-100);
								tweener.addTween(jParam);
								if(airSound.getStatus()!=airSound.Playing){
									//std::cout<<"Playing sound air";
									airSound.play();
								}
								//save time
								timeJump = clock.getElapsedTime().asSeconds() + 1;
							}
							
							break;
					}
					break;
				case sf::Event::EventType::KeyReleased: 
					//ESC is released?
					switch(event.key.code){
						case sf::Keyboard::Escape: play = false; break;
						case sf::Keyboard::Left: leftKeyPressed = false; break;
						case sf::Keyboard::Right: rightKeyPressed = false; break;
						case sf::Keyboard::Up: upKeyPressed = false; break;
						case sf::Keyboard::Down: downKeyPressed = false; break;
					}
					break;
				case sf::Event::Closed: 
					//Window closed
					play = false;
					break;
				case sf::Event::MouseButtonPressed:
					Click = true;
					if(event.mouseButton.button == sf::Mouse::Left)
					{
						nClicks++;
					} else {
						if(event.mouseButton.button == sf::Mouse::Right) {
							nClicks = std::max(0, nClicks-1);
						}
					}
					break;
				case sf::Event::MouseMoved:
					mouseX = event.mouseMove.x;
					mouseY = event.mouseMove.y;
					break;
			}
		}
		/////////////////////
		////LOGIC
		////////////////////
		float speedX=0, speedY=0;
		if(leftKeyPressed)
		{
			if(rightKeyPressed){
				//std::cout << "\n\tPlayer wants to go to the left and right at the same time\n";
				speedX = 1;
			} else {
				//std::cout << "\n\tPlayer wants to go to the left\n";
				speedX = -1;
			}
		} else if(rightKeyPressed) {
			speedX = 1;
		}
		if(speedX!=0){
			rPlayer.move(speedX,0);
			//Check collisions:
			if(rPlayer.getGlobalBounds().intersects(circle.getGlobalBounds()))
			{
				rPlayer.move(-speedX,0);
				//Player rect collides with sphere
				//explosionSound.play();
			}
		}
		
		if(upKeyPressed){
			speedY = -1;
		} else {
			if(downKeyPressed){
				speedY = 1;
			}
		}
		if(speedY!=0){
			rPlayer.move(0,speedY);
			//Check collisions:
			if(rPlayer.getGlobalBounds().intersects(circle.getGlobalBounds()))
			{
				rPlayer.move(0,-speedY);
				//Player rect collides with sphere
				//explosionSound.play();
			}
		}

		if(Click)
		{
			std::cout << "\n\tPlayer clicked left button " << nClicks << " times";
			std::cout << "\n\tMouse position: " << mouseX << "," << mouseY << "\n";
			Click = false;
		}

		//Update tweens
		tweener.step(clock.getElapsedTime().asMilliseconds());
		


		//Now we can translate shapes with the calculated values in the login part
		//background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y)); //?? neccessary?

		//move player
		if(jumping){
			if(clock.getElapsedTime().asSeconds()-timeJump>0){
				//std::cout<<"End of jump"<<clock.getElapsedTime().asSeconds()-timeJump;
				jumping=false;
				jumpOffSet = 0;
			} else {
				rPlayer.setPosition(rPlayer.getPosition().x, jumpOffSet);
			}
		}


		//move decoratoins: clouds
		for(std::list<animRect>::iterator it = clouds.begin(); it!=clouds.end(); it++)
		{
			(*it).rect.setPosition((*it).x, (*it).rect.getPosition().y);
		}

		rectangle.setPosition(window.getView().getCenter().x-200, window.getView().getCenter().y-200);
		rectangle.setSize(sf::Vector2f(rSx,rSy));
		circle.setPosition(
			cxOffSet + rectangle.getPosition().x + circle.getRadius() + rectangle.getSize().x, 
			cyOffSet + rectangle.getPosition().y + circle.getRadius() + rectangle.getSize().y
		);

		


		//Generate Texts
		std::ostringstream sMousePos;
		sMousePos << "X:" << mouseX << ",Y: " << mouseY;
		stringMousePos = sMousePos.str();
		text.setString(stringMousePos);
		//text.setPosition(window.getView().getCenter().x - text.getString().getSize(), window.getView().getCenter().y);

		

		//-----------
		////RENDERING
		//-----------
		//Clear screen
		window.clear();

		//draw background
		window.draw(background);
		//draw all the clouds
		for(std::list<animRect>::iterator it = clouds.begin(); it!=clouds.end(); it++)
		{
			window.draw((*it).rect);
		}

		//draw rectangle & circle
		window.draw(rectangle);
		window.draw(circle);

		//draw player
		window.draw(rPlayer);

		//draw text
		window.draw(text);

		//Display
		window.display();
	}

	////////////////

	//Clean up
	window.close();

	return 0;
}

int defaultWindowWithAcircle()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
