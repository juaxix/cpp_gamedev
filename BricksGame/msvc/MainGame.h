/// MainGame.h
/// Header of the class MainGame
///
#pragma once
#include <xixEngine/Engine.h> ///MainGame uses the Engine class
#include <xixEngine/Updater.h> ///MainGame inherits from Updater
#include <xixEngine/TextureCache.h> //Get Textures from cache
#include <xixEngine/Animator.h>
#include "Board.h" ///we need access to the Board class
#include <functional>
#include <fstream>

///File to store the best score
#define GAME_BEST_SCORE_FILE "highscore.dat"

///Used for rotations
#define GLM_FORCE_RADIANS 

///Total time of a gameplay
#define GAME_TIME 60.f

///Total time of the intro
#define GAME_INTRO_TIME 2.f

///Total time of waiting text (game starts in...)
#define GAME_WAIT_SCREEN_TIME 15.f

///Total time of the Game Enter intro 
#define GAME_ENTER_TIME 3.f

///Possible game states: idle->waiting for play, PLAY->game is running, EXIT->game is going to exit
enum GameStates { IDLE, INTRO, GAME_ENTER, MENU, ANIMATING, PLAY, EXIT }; 
enum GameTextures : unsigned int { ///Set of textures identifiers used for OpenGL and caching
	TEXTURE_BACKGROUND = 0,
	TEXTURE_BLUE = 1,
	TEXTURE_GREEN = 2,
	TEXTURE_PURPLE = 3,
	TEXTURE_RED = 4,
	TEXTURE_YELLOW = 5,
	TEXTURE_MAX = 6,
	TEXTURE_FONT,
	TEXTURE_TARGET,
	TEXTURE_PARTICLE,
	TEXTURE_SCORE_BG,
	TEXTURE_FUSE,
	TEXTURE_MENU_BG,
	TEXTURE_BUTTON_BG
};
enum GameSoundFXs:unsigned int { ///Set of sound fx identifiers used for the sounds cache
	CLICK0 = 0,
	SWAP_FORWARD,
	SWAP_BACK,
	MATCH_3,
	MATCH_4, 
	COUNTDOWN,
	TRANSITION
};

const xixEngine::ColorRGBA8 COLOR_TIME = xixEngine::ColorRGBA8(255, 1, 255, 255);

const xixEngine::ColorRGBA8 COLOR_LABEL = xixEngine::ColorRGBA8{ 1,103,1,255 };

///Declare the Board class before the actual declaration to let the compiler know of it (only works as pointer because of this)
class Board; 
/// class MainGame 
/// It's the main class of the game, connect the different parts of the game
/// Used to get the file paths of assets too
///
class MainGame : public xixEngine::Updater  ///We need to specify a function to be called in each frame by the xixEngine::Engine
{
public:
	//Constants
	/// Directory for all of the game assets
	const static std::string assetsDirectory;

	//Structs
	/*struct Button {
		xixEngine::GLTexture texture;
		xixEngine::Position position;
		std::function<void()> clickFunction;
	};*/


	/// MainGame constructor
	/// <summary>Creates an <c>Engine</c>,a <c>Board</c>,starts the <c>Music</c>,initializes all the systems,load asset files</summary>
	///<param>gameTitle</param>
	///<param>Screen Width</param>
	///<param>Screen Height</param>
	///<param>Max Frame Ticks</param>
	///<param>TextScale: applied to all the texts passed to the xixEngine::Engine if not scale is specified</param>
	MainGame(const char* gameTitle,int ScreenWidth,int ScreenHeight, float MaxFrameTicks, float TextScale);

	///MainGame destructor: it only destroys the things created in this class (i.e.:mBoard)
	~MainGame();

	///Calls the <c>Engine::Start</c>
	void Start();

	///Update function is called everyframe from the <c>xixEngine::Engine</c> and it calls all of the updates functions that needs it.
	void Update();

	///<summary><c>GetTexturePath</c> function returns the texture path for a given identifier </summary>
	///<param>index</param>(unsigned int enumerator value)
	const static std::string GetTexturePath(GameTextures index);

	///Returns the color associated with a Game Texture
	const static xixEngine::ColorRGBA8 GetTextureColor(GameTextures tex);

private:
	///Window Width Resolution
	int mWindowWidth = 800;

	///Window Height Resolution
	int mWindowHeight = 600;

	///Maximum number of frame ticks
	float mMaxFrameTicks = 300.0f;

	///current number of seconds
	int bestScore = 0;

	///seconds passed for each game state
	float seconds = 0.0f;

	///flag to know if the user is pressing the mouse
	bool bMouseDown = false;

	///current state of the game
	GameStates mGameState, mNextGameState;

	///game Engine
	xixEngine::Engine mEngine;

	///pointer to the Board class (pre-defined,and later the compiler will see the implementation)
	Board* mBoard;

	///The game Music
	xixEngine::Music mMusic;
    
    ///TextureCache for mainGame program
    xixEngine::TextureCache textureCache;
    
	xixEngine::GLTexture _backgroundTex;

	//private methods
	///Loads only the textures to be used with methods to draw GL_QUADS and a <c>Sdl_Surface</c>
	void initTextures();

	///Loads all the sounds to be used in the game
	void initSounds();

	///Compute value of seconds
	void calculateSeconds();
	
	///Draw the animation of the intro
	void IntroLoop();

	///Animation of the game just before start
	void GameEnterLoop(); 

	///Draws the main menu with the score
	void MenuLoop();

	///<summary>The <c>xixEngine::Update</c> function whe the state of the game is <c>GameStates::PLAY</c></summary>
	///It will check any operation to be made depending on game conditions.
	void GameLoop();

	int getBestScore();

	void setBestScore(int newScore);
};
