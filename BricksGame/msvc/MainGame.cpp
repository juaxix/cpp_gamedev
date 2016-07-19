/// MainGame.cpp
/// Implementation of the class MainGame
///
#include "MainGame.h"
///Public static constant to know the main asset directory of the game
const std::string MainGame::assetsDirectory = "./assets";

MainGame::MainGame(const char* gameTitle,int ScreenWidth,int ScreenHeight,float MaxFrameTicks, float TextScale)
	: mWindowWidth(ScreenWidth)
	, mWindowHeight (ScreenHeight)
	, mMaxFrameTicks(MaxFrameTicks)
	, seconds(0.f)
	, bMouseDown(false)
	, mGameState(GameStates::IDLE)
	, mEngine(gameTitle, ScreenWidth, ScreenHeight, MaxFrameTicks, TextScale)
	, mBoard(new Board(mEngine))
{
	//Load the textures to be used
	initTextures();
	//Load sounds and music
	initSounds();
	//Configure play button
	//mButtonPlay.position.x = 301;
	//mButtonPlay.position.y = 514;
	bestScore = getBestScore();
	setBestScore(bestScore); //save in case it's the first time
	
}


MainGame::~MainGame()
{
	delete mBoard;
}
///this function could be done with a static array too
const std::string MainGame::GetTexturePath(GameTextures index)
{
	switch (index)
	{
	case TEXTURE_BACKGROUND:
		return assetsDirectory + "/textures/BackGround.jpg";
		break;
	case TEXTURE_BLUE:
		return assetsDirectory + "/textures/Blue.png";
		break;
	case TEXTURE_GREEN:
		return assetsDirectory + "/textures/Green.png";
		break;
	case TEXTURE_PURPLE:
		return assetsDirectory + "/textures/Purple.png";
		break;
	case TEXTURE_RED:
		return assetsDirectory + "/textures/Red.png";
		break;
	default: case TEXTURE_YELLOW:
		return assetsDirectory + "/textures/Yellow.png";
		break;
	case TEXTURE_FONT:
		return assetsDirectory + "/textures/berlin_sans_demi_72_0.png";
		break;
	case TEXTURE_TARGET:
		return assetsDirectory + "/textures/target.png";
		break;
	case TEXTURE_PARTICLE:
		return assetsDirectory + "/textures/particle.png";
		break;
	case TEXTURE_SCORE_BG:
		return assetsDirectory + "/textures/scorebg.png";
		break;
	case TEXTURE_FUSE:
		return assetsDirectory + "/textures/fuse.png";
		break;
	case TEXTURE_MENU_BG:
		return assetsDirectory + "/textures/titlebg.png";
		break;
	case TEXTURE_BUTTON_BG:
		return assetsDirectory + "/textures/bluebtn.png";
		break;
	}
}

const xixEngine::ColorRGBA8 MainGame::GetTextureColor(GameTextures tex)
{
	switch (tex)
	{
	case TEXTURE_BLUE:
		return xixEngine::ColorRGBA8(49,110,250,255);
		break;
	case TEXTURE_GREEN:
		return xixEngine::ColorRGBA8(97, 229, 109, 255);
		break;
	case TEXTURE_PURPLE:
		return xixEngine::ColorRGBA8(163, 33, 141, 255);
		break;
	case TEXTURE_RED:
		return xixEngine::ColorRGBA8(255, 57, 67, 255);
		break;
	case TEXTURE_YELLOW:
		return xixEngine::ColorRGBA8(255, 248, 155, 255);
		break;
	default:
		return xixEngine::COLOR_WHITE;
		break;
	}
}

void MainGame::initTextures()
{
	//To use with the engine sprite batch from this class,we use textureCache:
	_backgroundTex = textureCache.getTexture(
		GetTexturePath(TEXTURE_BACKGROUND),
		xixEngine::TextureCache::ImageType::JPG
	);
	
	//if we want to use SdlSurfaces: load all the textures here
	mEngine.loadTexture(TEXTURE_BACKGROUND,
		GetTexturePath(TEXTURE_BACKGROUND)
	);
	mEngine.loadTexture(TEXTURE_MENU_BG,
		GetTexturePath(TEXTURE_MENU_BG)
	);
	mEngine.loadTexture(TEXTURE_BUTTON_BG,
		GetTexturePath(TEXTURE_BUTTON_BG)
	);
	//Load the font texture:
	mEngine.loadFontTexture(
		GetTexturePath(TEXTURE_FONT)
	);

	//The board pieces (gems) textures will use the TextureCache method (via class)
	/*mEngine.loadTexture(TEXTURE_BLUE,
		GetTexturePath(TEXTURE_BLUE)
	);
	mEngine.loadTexture(TEXTURE_GREEN,
		GetTexturePath(TEXTURE_GREEN)
	);
	mEngine.loadTexture(TEXTURE_PURPLE,
		GetTexturePath(TEXTURE_PURPLE)
	);
	mEngine.loadTexture(TEXTURE_RED,
		GetTexturePath(TEXTURE_RED)
	);
	mEngine.loadTexture(TEXTURE_YELLOW,
		GetTexturePath(TEXTURE_YELLOW)
	); 
	mEngine.loadTexture(TEXTURE_SCORE_BG,
		GetTexturePath(TEXTURE_SCORE_BG)
	);
	*/
    
}

void MainGame::initSounds()
{
	mEngine.loadSoundEffect(GameSoundFXs::CLICK0,
		assetsDirectory + "/sounds/click0.ogg"
	);
	mEngine.loadSoundEffect(GameSoundFXs::SWAP_FORWARD,
		assetsDirectory + "/sounds/swapfwd.ogg"
	);
	mEngine.loadSoundEffect(GameSoundFXs::SWAP_BACK,
		assetsDirectory + "/sounds/swapback.ogg"
	);
	mEngine.loadSoundEffect(GameSoundFXs::MATCH_3,
		assetsDirectory + "/sounds/match3.ogg"
	);
	mEngine.loadSoundEffect(GameSoundFXs::MATCH_4,
		assetsDirectory + "/sounds/match4.ogg"
	);
	mEngine.loadSoundEffect(GameSoundFXs::COUNTDOWN,
		assetsDirectory + "/sounds/countdown.ogg"
	);
	mEngine.loadSoundEffect(GameSoundFXs::TRANSITION,
		assetsDirectory + "/sounds/transition.ogg"
	);
	mMusic = mEngine.loadMusic("assets/music/game.ogg");
	mMusic.play();
}

void MainGame::Start() 
{
	seconds = 0.0f;
	mEngine.playSoundEffect(TRANSITION);
	mGameState = GameStates::INTRO;
	mEngine.Start(*this);
}

void MainGame::IntroLoop()
{
	//Use a sprite batch and passing the time>0 to let know the glsl shader that we want to apply an "arcade" filter to the sprites render with it
	mEngine.spriteBatchingBegin(seconds);
	mEngine.RenderSprite(_backgroundTex.id, 0, _backgroundTex.height, _backgroundTex.width, _backgroundTex.height);
	mEngine.spriteBatchingEnd();
	
	float s = seconds / GAME_INTRO_TIME;
	mEngine.Render(
		TEXTURE_MENU_BG, 53.f,
		xixEngine::lerp(-500.f, 4.f, s),
		xixEngine::lerp(0.4f, 0.f, s)
		);
	calculateSeconds();
	if (seconds > GAME_INTRO_TIME)
	{
		seconds = 0.0f;
		
		mGameState = GameStates::MENU;
	}
}

void MainGame::GameEnterLoop()
{
	mEngine.Render(TEXTURE_BACKGROUND, 0.0f, 0.0f);
	float s = seconds / GAME_ENTER_TIME;
	mEngine.Render(
		TEXTURE_MENU_BG, 53.f,
		xixEngine::lerp(5, -500.f, s*2),
		xixEngine::lerp(0.f, 0.6f, s)
	);
	mEngine.Write(
		std::to_string(
			(int)(std::max(0.f, GAME_ENTER_TIME + 1 - seconds))
		).c_str(),
		this->mWindowWidth / 2.f, this->mWindowHeight / 2.f,
		0.f,
		xixEngine::COLOR_RED, seconds
	);
	calculateSeconds();
	if (seconds > GAME_ENTER_TIME)
	{
		seconds = 0.0f;
		this->mBoard->init();
		mGameState = GameStates::PLAY;
	}
}

void MainGame::MenuLoop()
{
    //mEngine.spriteBatchingBegin();
	//draw background
	mEngine.Render(TEXTURE_BACKGROUND, 0.0f, 0.0f);
    
    //static xixEngine::GLTexture tBackground = textureCache.getTexture(
    //    GetTexturePath(TEXTURE_BACKGROUND)
    //);
    //mEngine.RenderSprite(tBackground.id, 0.f, tBackground.height, tBackground.width, tBackground.height);
    //mEngine.spriteBatchingEnd();
	//draw board
	mEngine.Render(TEXTURE_MENU_BG, 53.f, 4.f);
	//draw button play
	mEngine.Render(TEXTURE_BUTTON_BG, 290.f, 505.f);
	mEngine.Write("BLOCKS - MATCH 3 GAME", this->mWindowWidth / 1.2f, this->mWindowHeight / 2.f,0.0f,xixEngine::COLOR_BLACK,0.6f);
	mEngine.Write("Made by: JUAN BELON PEREZ", this->mWindowWidth / 1.2f, (this->mWindowHeight / 2.f) + 60.f, 0.0f, xixEngine::COLOR_BLUE, 0.4f);
	mEngine.Write(std::string("Best score:" + std::to_string(bestScore)).c_str(), this->mWindowWidth / 1.2f, 450.f, 0.f, xixEngine::COLOR_WHITE, 0.33f);
	mEngine.Write(std::string("Game starts in " +
		std::to_string(
			(int)(std::max(0.f, GAME_WAIT_SCREEN_TIME - seconds))
			)).c_str(),
		this->mWindowWidth/2.f + 60.f, 486.f, 0.f,
		xixEngine::COLOR_BLACK, 0.37f
	);
	mEngine.Write("PLAY", 417.f, 545.f);
	calculateSeconds();
	if (seconds >= GAME_WAIT_SCREEN_TIME ||
		//User want to play?
		mEngine.GetLeftMouseButtonDown()) 
	{
		seconds = 0.0f; 
		mEngine.playSoundEffect(COUNTDOWN);
		this->mGameState = GameStates::GAME_ENTER;
		return;
	}
	
}

void MainGame::GameLoop()
{
	//Draw everything in the screen depending on the game state
	//from back to top, first the background:Sprites with the old QUAD method
	mEngine.Render(TEXTURE_BACKGROUND, 0.0f, 0.0f);
	
	//now the time
	mEngine.Write("Time: ", 102.f, 416.f, 0.0f, COLOR_LABEL, 0.3f);
	mEngine.Write(std::to_string(
		(int)(std::max(0.f, GAME_TIME - seconds))
	).c_str(), 133.0f, 450.f, 0.0f, COLOR_TIME,0.8f);
	
	//Prepare for drawing sprites batches from game entities
	mEngine.spriteBatchingBegin(
		//pass the time to the glshader used in the engine
		//to apply an effect,only in certain times
		(!mBoard->isInitialized() ||
		mBoard->GetState()==Board::BoardStates::IDLE||
		mBoard->GetState()==Board::BoardStates::GAME_OVER)?
		mEngine.GetElapsedTimeSeconds()
		:0.0f
	);
	mBoard->draw();
	mEngine.spriteBatchingEnd();
	mBoard->drawTexts();
	if (mEngine.GetKeyDown(SDLK_SPACE)) {
		std::cout << mEngine.GetMouseX() << "," << mEngine.GetMouseY()<<std::endl;
	}
	 
	//Get Mouse Coords from input manager --> engine implementation 
	glm::vec2 mouseCoords = mEngine.GetMouseCoords();
	//Debug: draw mouse coords in screen
	/*mEngine.Write(std::string(std::to_string(
		(int)mouseCoords.x) + "  " + 
		std::to_string((int)mouseCoords.y)
	).c_str(), 105.f, 50.f);
	*/
	if (bMouseDown) {
		if (!mEngine.GetLeftMouseButtonDown()) {
			mBoard->onMouseUp(
				mouseCoords.x, mouseCoords.y
			);
			bMouseDown = false;
		}
	}
	else {
		if (mEngine.GetLeftMouseButtonDown())
		{
			mBoard->onMouseDown(
				mouseCoords.x, mouseCoords.y
			);
			bMouseDown = true;
		}
	}
	

	//update the game board
	mBoard->update();

	//get the amount of time spent in play mode
	if(mBoard->isInitialized())
		calculateSeconds();

	if (seconds >= GAME_TIME )
	{
		//time up!
		mBoard->stop();
		if (mBoard->getScore() > bestScore) {
			setBestScore(mBoard->getScore());
		}
		seconds = 0.f;
		mGameState = GameStates::INTRO;
		mEngine.playSoundEffect(TRANSITION);
		return;
	}
}

void MainGame::calculateSeconds()
{
	seconds += mEngine.GetLastFrameSeconds();
}

void MainGame::Update() 
{
	if (mEngine.GetKeyPressed(SDLK_ESCAPE))
	{
		mGameState = GameStates::EXIT;
		mMusic.stop();
		mEngine.Quit(); 
		return;
	}
	switch (mGameState)
	{
	case GameStates::IDLE: //game is loading
		break;
	case GameStates::INTRO:
		IntroLoop();
		break;
	case GameStates::MENU: //main menu
		MenuLoop();
		break;
	case GameStates::GAME_ENTER:
		GameEnterLoop();
		break;
	case GameStates::PLAY:
		GameLoop();
		break;
	case GameStates::EXIT:
		break;
	default:
		break;
	}
	
}

int MainGame::getBestScore()
{
	int score;
	std::ifstream f(std::string(assetsDirectory +
		"/"+GAME_BEST_SCORE_FILE), std::ios::binary);
	if (f.fail())
	{
		std::cout<<"Could not open the best score data file to read, creating a new one..."<<std::endl;
		return 0;
	}
	f.seekg(0, std::ios::end); //go to the ending
	int fSize = //and now get the file size
		f.tellg(); 
	if (fSize != sizeof(int)) //how many bytes has we passed with seek? it must be 4, the size of an int
		return 0; //the file has no data or more than an int
	f.seekg(0, std::ios::beg); 
	//go to the beginning of  the file again and read the int
	f.read(reinterpret_cast<char*>(&score), sizeof(int)/*4*/);
	f.close();
	return score;
}

void MainGame::setBestScore(int score)
{
	bestScore = score;
	std::ofstream f(std::string(assetsDirectory +
		"/" + GAME_BEST_SCORE_FILE), std::ios::binary);
	if (f.fail()) {
		throw std::runtime_error("Could not open the best score data file to write");
		return;
	}
	f.write(reinterpret_cast<const char*>(&score), sizeof(score));
	f.close();
}
