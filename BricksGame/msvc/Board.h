/// Board.h
/// Header of the class Board

#pragma once
#include "MainGame.h"
#include <random>
#include <xixEngine/VertexData.h>
#include <xixEngine/TextureCache.h>
#include <xixEngine/Animator.h>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <time.h>
#include <glm/gtx/rotate_vector.hpp>

///<summary>Declare the GameTextures enum to let the compiler know it before the final declaration in MainGame</summary>
enum GameTextures:unsigned int;

///
/// <summary>class Board used to represent the gems game grid</summary>
///
class Board
{
public:
#pragma region Public constants
	///<summary> of COLUMNS of the Board</summary>
	static const unsigned int COLUMNS = 8;

	///<summary>Number of ROWS of the Board</summary>
	static const unsigned int ROWS = 8;
#pragma endregion
#pragma region Public structures and types
	///<summary>A set of States for the Board to control the flow of the game</summary>
	enum BoardStates
	{
		IDLE,
		PLAYING,
		SWAPPING_FORWARD,
		SWAPPING_BACK,
		REMOVING,
		FALLING,
		GAME_OVER
	};

	///<summary>Structure to be used in the array grid of bricks</summary>
	struct Brick 
	{
		///<summary><c>xixEngine::GLTexture</c> which</summary>
		xixEngine::GLTexture texture;
		///GameTextures enum to use it  for comparations (color)
		GameTextures gameTexture;

		///<summary>Different states of a Brick for the different behaviours it can perceive</summary>
		enum States {
			PICKABLE,
			SWAPPING,
			REMOVING,
			FALLING ,
			EMPTY 
		};
		///<summary>Stores its Brick state</summary>
		States state;
		
		/*enum Types { //Type of brick to make explosions
			NORMAL,
			HORIZONTAL,
			VERTICAL,
			BOMB
		};
		Types type;*/
	};

	///<summary>Used for the target, the player select a brick and this structure store all the info related</summary>
	struct SwapTarget 
	{
		///<summary>Texture of the target, to draw image, and know its information</summary>
		xixEngine::GLTexture texture;
		///<summary>Current position of the target</summary>
		glm::vec2 position;
		///<summary>Current angle of the target</summary>
		float angle;
		///<summary>Indexes of bricks to swap</summary>
		unsigned int fromI, fromJ, toI, toJ;
		///<summary>If false, it will not be drawn</summary>
		bool active = false;
	};
#pragma endregion

#pragma region Public Methods
	/// Board constructor
	/// <summary>Board constructor</summary>
	/// 
	Board(xixEngine::Engine& engine);

	///<summary>Builds initialize board with random values</summary>
	/// It uses the new C++ mt19937 random generator
	/// It will try to generate a board with no matches using a simple
	/// comparation operation.
	/// <param>engine</param> is the <c>xixEngine::Engine</c> from <c>MainGame</c>
	void init();
	///Stops the board, the game is Over
	void stop();
	///
	/// <summary>Board destructor</summary>
	///
	~Board();

	///Draw all the bricks using the engine SpriteBatch
	void drawGrid();
	
	///Draw SwapTarget target
	void drawTarget();
	
	///Draw the fuse sprite using SpriteBatch
	void drawFuse();	
	
	///
	/// Draw method
	/// <summary>Draw all the items of the board depending on the internal State
	/// including the target sprite</summary>
	///
	void draw();

	///Draw texts of the board
	void drawTexts();

	///
	/// <summary>Updates the Board state using the MatchSet data and user interaction aswell as the MainGame input.</summary>
	///
	void update();

	///
	/// <summary>Handles the event when the mouse is being pressed in a screen position</summary>
	/// <param>float x</param>
	/// <param>float y</param>
	///
	void onMouseDown(float x, float y);

	///
	/// <summary>Handles the event when the mouse is released in a screen position</summary>
	/// <param>float x</param>
	/// <param>float y</param>
	///
	void onMouseUp(float x, float y);

	///<summary>Returns the score of the last played game</summary>
	///<return>int score</return>
	inline const int getScore() const { return _score; };
	BoardStates GetState() const { return state; }
	bool isInitialized() const { return _initialized; }
#pragma endregion
private:
#pragma region Private constants
	///<summary>Min number of gems to be considered as a valid match</summary>
	const unsigned int MIN_MATCH = 3;

	///<summary>X value of the left border of the board</summary>
	const unsigned int LEFT_BORDER = 326;

	///<summary>Y value of the top border of the board</summary>
	const unsigned int TOP_BORDER = 138;

	///<summary>X and Y space to use as space for click collision test calculation and separate each gem brick between them</summary>
	const unsigned int BRICK_SPACE = 43;

	///<summary>Time in seconds for the single swap animation of two gems (*2 for invalid swap)</summary>
	const float BRICK_SWAP_TIME = 0.13f;

	///<summary>Time in seconds to make the bricks falling</summary>
	const float BRICK_FALL_TIME = 0.16f;

	///<summary>Time in seconds to make the scale animation when there is a match that involve a brick removal</summary>
	const float BRICK_SCALE_TIME = 0.23f;

#pragma endregion
#pragma region Private variables,types and structures
	//Vars to generate random numbers in the C++11 style
	std::random_device rseed;
	std::mt19937 rgen;
	std::uniform_int_distribution<int> idist;

	///<summary>Grid of bricks</summary>
	Brick _bricks[ROWS][COLUMNS];

	///<summary>The xixEngine Engine used to draw,get input,and information related with the SDL/OGL</summary>
	xixEngine::Engine& mEngine;

	///<summary>The Texture cache used to cache all the textures (images) in the Board</summary>
	xixEngine::TextureCache textureCache;

    xixEngine::SpriteBatch spriteBatch;

	xixEngine::ParticleBatch* particleBatch;

	///<summary>Target structure used to store information for swapping gems</summary>
	SwapTarget target;
	
	//is the game over (time)
	bool _gameOver = false,
		_initialized = false;
	
	//Seconds of game
	float _totalTime = 0.f;

	//Score
	int _score = 0;
	xixEngine::GLTexture _scoreBackgroundTex, _particleTex;
	const xixEngine::ColorRGBA8 COLOR_SCORES{ 255,227,0,255 };
    struct ScoreText {
		std::string text;
		glm::vec2 position;
		xixEngine::ColorRGBA8 color;
		float lifeTime;
		float totalLifeTime;
		float lifeDecayRate;
		glm::vec2 speed;
		ScoreText(std::string _text, glm::vec2 _position, xixEngine::ColorRGBA8 _color, float _lifeTime, float _lifeDecayRate, glm::vec2 _speed)
		{
			text = _text;
			position = _position;
			color = _color;
			totalLifeTime = lifeTime = _lifeTime;
			lifeDecayRate = _lifeDecayRate;
			speed = _speed;
		}
	};
    
	std::list<ScoreText> _scoreTexts;

	//Fuse
	#define NUM_ROPE_POSITIONS 5
	const float FUSE_CHANGE_POSITION_TIME = 60 / NUM_ROPE_POSITIONS;
	struct Fuse {
		bool active;
		xixEngine::GLTexture texture;
		unsigned int positionIndex;
		float rotation;
		float lastPositionChange;
		const xixEngine::Position ropePositions[NUM_ROPE_POSITIONS] = {
			{ 232.f , 616.f },
			{ 190.f, 550.f },
			{ 188.f, 450.f },
			{ 171.f, 400.f },
			{ 80.f, 385.f }

		};
	} _fuse;
	

	//Matches data structures info
	///<summary>Set of rows and columns used to access the grid of bricks <c>Brick</c></param>
	typedef std::set<std::pair<unsigned int, unsigned int>> MatchSet;


	///<summary>Array of sets(i,j) to store all matches in vertical lines</summary>
	std::vector<MatchSet> mVerticalMatches;

	///<summary>Array of sets(i,j) to store all matches in horizontal lines</summary>
	std::vector<MatchSet> mHorizontalMatches;

	///<summary>Array of all matches combined: once analyzed mVerticalMatches (the vertical) and mHorizontalMatches (the horizontal) matches, they mix together into mMatchesFound</summary>
	std::vector<MatchSet> mMatchesFound;
	
	///<summary>The current state of the Board, PLAYING by default</summary>
	BoardStates state = BoardStates::PLAYING;

	///<summary>Current number of matches for each frame loop of PLAYING Board state</summary>
	unsigned int numMatches = 0;

	///<summary>The current time of the current animation</summary>
	float animationTime = 0;
#pragma endregion
#pragma region Private Methods
	///Remove possible matches
	void makeBoardPlayable();

	void initTextures();

	void initParticles();

	///Init the board with random bricks
	void initBoardGrid();
	
	///Init the swaptarget used for user brick swapping
	void initTarget();
	
	///init the fuse animation
	void initFuse();

	///<summary><c>CheckMatches</c> will find the matches in the board and stores them in the mVerticalMatches and mHorizontalMatches arrays</summary>
	void checkMatches();

	///<summary>Look for vertical matches using the function searchVerticalMatches to get new sets for the mVerticalMatches array</summary>
	void checkVerticalMatches();

	///<summary>Used by checkVerticalMatches, generate a set of matches in m with the input params</summary>
	///<param>i</param> the row to look
	///<param>star</param> the starting column 
	///<param>m</param> the set to add the search results
	void searchVerticalMatches(unsigned int i, unsigned int start, MatchSet &m);


	///<summary>Look for horizontal matches using the function searchHorizontalMatches to get new sets for the mHorizontalMatches array</summary>
	void checkHorizontalMatches();

	///<summary>Used by checkVerticalMatches, generate a set of matches in m with the input params
	///<param>j</param> the column to look into
	///<param>star</param> the starting row 
	///<param>m</param> the set to add the search results
	void searchHorizontalMatches(unsigned int j, unsigned int start, MatchSet &m);


	///<summary>Prints debug information about the matches</summary>
	void printFoundMatches() const;

	///<summary>Deletes the matches bricks and start the fall animations</summary>
	void executeFoundMatches();

	
	///<summary>Remove duplicated sets in a vector of MatchSet</summary>
	void removeMatchSetDuplicates(std::vector<MatchSet> &m);


	///Inline function to get the current number of matches, the sum of mVerticalMatches and mHorizontalMatches
	inline const int getNumMatches() const{return mVerticalMatches.size() + mHorizontalMatches.size();};

	///<summary>Find a Brick in the grid of bricks with the coordinates passed as parameters and save the info in the reference parms</summary>
	///<param>x</param> X Mouse coord
	///<param>x</param> Y Mouse coord
	///<param>brickX</param> store the brick X in screen coords
	///<param>brickY</param> store the brick Y in screen coords
	///<param>I</param> store the row of the bricks grid
	///<param>J</param> store the column of the bricks grid
	///<return>bool</return> if true, there is a brick in the mouse coords
	bool findBrickInScreenCoords(const float x, const float y,float& brickX,float& brickY,unsigned int& I,unsigned int &J) const;


	///<summary>The player click the board</summary>
	void clickBoard(const float x, const float y);

	///<summary>Remove all the matched bricks and configure them to be animated, at the end of the animation it will call <c>freeRemovedBricks</c> to make bricks on top to be in FREE state ones and then make them fall down</summary>
	void removeMatches();


	///<summary>Search removed bricks, per column a brick on top of it ,replaces it and then make it fall to the free position, the found position is now free, then it calls <c>refillEmptyBricks</c></summary>
	void freeRemovedBricks();

	///<summary>Create new bricks in the empty positions</summary>
	///<return>true if there is new objects falling in from the top to fill the board</return>
	bool refillEmptyBricks();

	///<summary>Swap a bricks[toI][toJ] with another bricks[target.fromI][target.fromJ] and make Board to go the next BoardStates and returns the result</summary>
	///<param>toI</param> row for the origin brick
	///<param>toJ</param> column for the origin brick
	///<param>nextState</param> next BoardStates if swap is valid
	///<return>bool</return> true if the swap is taking place (is valid)
	bool OnSwapBrickWith(unsigned int toI, unsigned int toJ, BoardStates nextState = BoardStates::SWAPPING_FORWARD);

	///<summary></summary>
	void addExplosion(const glm::vec2& position, int numParticles, xixEngine::ColorRGBA8 color=xixEngine::COLOR_WHITE);

	void addScore(int score, float x, float y);

#pragma endregion
};

