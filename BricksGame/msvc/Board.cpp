/// Board.cpp
/// Implementation of the class Board
///
#include "Board.h"
///--------------------------
#pragma region Construction of the Board
//--------------------------
Board::Board(xixEngine::Engine& engine)
	:mEngine(engine),
	idist(1, TEXTURE_MAX - 1),
	state(BoardStates::IDLE)
{
	initTextures();
	initParticles();
}

void Board::initTextures() {
	//load the score background texture
	_scoreBackgroundTex = textureCache.getTexture(
		MainGame::GetTexturePath(GameTextures::TEXTURE_SCORE_BG)
		);
	_fuse.texture = textureCache.getTexture(
		MainGame::GetTexturePath(GameTextures::TEXTURE_FUSE)
		);
	_particleTex = textureCache.getTexture(
		MainGame::GetTexturePath(GameTextures::TEXTURE_PARTICLE)
		);
}

void Board::initParticles()
{
	//init the particle batch 
	particleBatch = new xixEngine::ParticleBatch();
	particleBatch->init(1500, 0.6f,
		textureCache.getTexture(
			MainGame::GetTexturePath(GameTextures::TEXTURE_PARTICLE)
			),
		[](xixEngine::Particle& particle, float deltaTime)
	{
		//Code of each particle:
		particle.position += particle.velocity * deltaTime;
		particle.color.a = (GLubyte)(particle.lifeTime * 255.0f);
	}
	);
	//add the particle batch to the particle system:
	mEngine.AddParticleBatch(particleBatch);
}

void Board::init()
{
	_gameOver = _initialized = false;
	_totalTime = 0.f;
	_score = 0;
	animationTime = 0.f;
	state = BoardStates::IDLE; //loading
	srand(time(NULL));
	rgen.seed(rseed());
	numMatches = 0;
	mHorizontalMatches.clear();
	mVerticalMatches.clear();
	mMatchesFound.clear();
	_scoreTexts.clear();
	initBoardGrid();
	initTarget();
	initFuse();
}

void Board::makeBoardPlayable()
{
	//Check initial matches before let the player do the first move
	checkMatches();
	//Debug information to know how many matches in the creation of the board
	printFoundMatches();
	unsigned int i, j, r;
	int n = getNumMatches();
	if (n > 0)
	{
		for (auto &m : mMatchesFound)
		{
			for (auto &d : m)
			{
				i = d.first;
				j = d.second;
				r = idist(rgen);
				//change the color for other one
				while (r == static_cast<unsigned int>(_bricks[i][j].texture.id))
					r = idist(rgen);
				_bricks[i][j].texture = textureCache.getTexture(
					MainGame::GetTexturePath(
						static_cast<GameTextures>(r)
						)
					);
				_bricks[i][j].gameTexture = static_cast<GameTextures>(r);
			}
		}
		checkMatches();
		printFoundMatches();
		executeFoundMatches();
		n = mMatchesFound.size();
		std::cout << " Matches cleaned: " << n << std::endl;
	} else {
		//Must assure that there is a movement
		//makeSureMoveIsPossible();
		//start game
		if(!_gameOver)
		state = BoardStates::PLAYING;
	}
}

void Board::initBoardGrid() {
	unsigned int i, j, r = idist(rgen), prev_r = 0;
	for (i = 0; i < ROWS; i++)
	{
		while (r == prev_r) r = idist(rgen); //diferent gems to avoid generation with too many same-colored bricks
		for (j = 0; j < COLUMNS; j++)
		{
			while (r == prev_r)r = idist(rgen);
			prev_r = r;
			_bricks[i][j].state = Brick::States::PICKABLE;
			_bricks[i][j].texture = textureCache.getTexture(
				MainGame::GetTexturePath(static_cast<GameTextures>(r))
				);
			_bricks[i][j].gameTexture = static_cast<GameTextures>(r);
		}
	}
	//Remove possible matches
	makeBoardPlayable();
}

void Board::initTarget() {
	target.texture = textureCache.getTexture(
		MainGame::GetTexturePath(
			TEXTURE_TARGET
			)
		);
	target.position = glm::vec2(0, 0);
	target.fromI = target.fromJ = 0;
	target.active = false;
}

void Board::initFuse() {
	_fuse.positionIndex = 0;
	_fuse.rotation = 0.0f;
	_fuse.lastPositionChange = 0.f;
	_fuse.active = true;
}
#pragma endregion 
///-----------
///Destructor
///-----------
Board::~Board()
{
}
///----------------------------
#pragma region Drawing everything in the board
//-----------------------

void Board::drawGrid()
{
	if (_gameOver) return;
	//From back to front ,draw all the sprites
	mEngine.RenderSprite(_scoreBackgroundTex.id, 3.f, _scoreBackgroundTex.height - 30.f, _scoreBackgroundTex.width, _scoreBackgroundTex.height);
	unsigned int i, j; //indexes for loops
	float x, y, sx, sy; //position and scale
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLUMNS; j++)
		{
			sx = _bricks[i][j].texture.width;
			sy = _bricks[i][j].texture.height;
			switch (_bricks[i][j].state) 
			{
			case Brick::States::EMPTY:
				continue;
				break;
			case Brick::States::SWAPPING:
				// animationTime
				if (target.toI == i&&target.toJ == j)
				{
					x = xixEngine::lerp(i, target.fromI,
						animationTime / BRICK_SWAP_TIME);
					y = xixEngine::lerp(j, target.fromJ,
						animationTime / BRICK_SWAP_TIME);
				}
				else {
					x = xixEngine::lerp(i, target.toI,
						animationTime / BRICK_SWAP_TIME);
					y = xixEngine::lerp(j, target.toJ,
						animationTime / BRICK_SWAP_TIME);
				}

				x = static_cast<const float>(
					LEFT_BORDER + x*BRICK_SPACE
					);
				y = static_cast<const float>(
					TOP_BORDER + y*BRICK_SPACE
					);
					
				break;
			case Brick::States::REMOVING:
				//it's being removed, animate!
				x = static_cast<const float>(
					LEFT_BORDER + i*BRICK_SPACE
					);
				y = static_cast<const float>(
					TOP_BORDER + j*BRICK_SPACE
					);
				//interpolate to zero in scale
				sx = xixEngine::lerp( 
					sx,0.0f,animationTime / BRICK_SCALE_TIME);
				sy = xixEngine::lerp( 
					sy,0.0f,animationTime / BRICK_SCALE_TIME);
				
				break;
			case Brick::States::FALLING:
			{
				x = static_cast<float>(LEFT_BORDER + (i*BRICK_SPACE));
				y = static_cast<float>(
					TOP_BORDER + (
						xixEngine::lerp(
							(float)(j - 1), (float)j,
							animationTime / BRICK_FALL_TIME
							)
						)*BRICK_SPACE
					);
			}
				break;
			default:
				x = static_cast<const float>(
					LEFT_BORDER + (i * BRICK_SPACE)
					);
				y = static_cast<const float>(
					TOP_BORDER + (j * BRICK_SPACE)
					);
				break;
			}

			/*if (_bricks[i][j].state != Brick::States::EMPTY && _bricks[i][j].type == Brick::Types::BOMB)
			{
				mEngine.RenderSprite(_particleTex.id, 
					x - _particleTex.width, 
					y -  _particleTex.width, 
					_particleTex.width,_particleTex.height);
			}*/
			mEngine.RenderSprite(
				_bricks[i][j].texture.id,
				x,y,sx,sy
			);
			
			//old way of drawing a sprite without batching and using quads instead of triangles:
			/*mEngine.Render(
			_bricks[i][j].texture,
			static_cast<const float>(LEFT_BORDER + (i * GEM_SPACE)),
			static_cast<const float>(100 + (j * GEM_SPACE))
			);*/
		}
	}
}

void Board::drawTarget()
{
	if (target.active)
	{
		mEngine.RenderSprite(
			target.texture.id,
			target.position.x, target.position.y,
			target.texture.width, target.texture.height,
			target.angle
		);
	}
}

void Board::drawFuse()
{
	//draw fuse along the rope positions
	if (_fuse.active)
	{
		float fx = _fuse.ropePositions[_fuse.positionIndex].x;
		float fy = _fuse.ropePositions[_fuse.positionIndex].y;
		//unsigned int nPositions = (sizeof(fuse.ropePositions) / sizeof(fuse.ropePositions[0]));
		if (_fuse.positionIndex < NUM_ROPE_POSITIONS - 1 && _totalTime>0.f)
		{
			//move with an interpolation:
			fx = xixEngine::lerp(fx, _fuse.ropePositions[_fuse.positionIndex + 1].x, (_totalTime - _fuse.lastPositionChange) / FUSE_CHANGE_POSITION_TIME);
			fy = xixEngine::lerp(fy, _fuse.ropePositions[_fuse.positionIndex + 1].y, (_totalTime - _fuse.lastPositionChange) / FUSE_CHANGE_POSITION_TIME);
		}
		mEngine.RenderSprite(
			_fuse.texture.id, fx, fy,
			_fuse.texture.width, _fuse.texture.height,
			_fuse.rotation
			);
	}
}
	
void Board::draw()
{
	switch(state){
		case BoardStates::IDLE:
		case BoardStates::PLAYING:
		case BoardStates::SWAPPING_FORWARD:
		case BoardStates::SWAPPING_BACK:
		case BoardStates::REMOVING:
		case BoardStates::FALLING:
		{
			drawGrid();
			drawTarget();
			drawFuse();
			//draw particles
			mEngine.RenderParticleSystems();
		}
		break;
		case BoardStates::GAME_OVER:
		{
			drawFuse();
			mEngine.RenderParticleSystems();
		}
		break;
	}
}

void Board::drawTexts()
{
	//Draw score text
	mEngine.Write( "Score: " , 150.f, 113.f);
	mEngine.Write( std::to_string(_score).c_str(), 175.f, 143.f);
	//Draw little scores
	for (auto &st : _scoreTexts)
	{
		mEngine.Write(
			st.text.c_str(), 
			st.position.x, st.position.y, 
			0.0f, st.color, 0.22f
		);
		
	}
}

#pragma endregion


///----------------------------
#pragma region MATCHES ALGORITHMS
///---------------------------------------------

void Board::searchVerticalMatches(unsigned int i, unsigned int start, MatchSet &m)
{
	while (true)
	{
		if (i >= ROWS || start >= COLUMNS)
			return;
		Brick* brickA = &_bricks[i][start];
		if (brickA == nullptr)
		{
			return;
		}
		if (brickA->state != Brick::States::PICKABLE)
		{
			return;
		}
		//Found match, add:
		m.insert(std::make_pair(i, start));
		//Check limits:
		if ((start + 1) >= COLUMNS)
		{
			return;
		}
		Brick* brickB = &_bricks[i][start + 1];
		if (brickB == nullptr)
		{
			return;
		}
		if (brickB->state != Brick::States::PICKABLE)
		{
			return;
		}

		if (brickB->gameTexture == brickA->gameTexture)
		{
			start++;
		}
		else {
			return;
		}
	}
}


void Board::searchHorizontalMatches(unsigned int j, unsigned int start, MatchSet &m)
{
	while (true)
	{
		//check limits
		if (j >= COLUMNS || start >= ROWS)
		{
			return;
		}
		
		//try to get the brick in the position
		Brick* brickA = &_bricks[start][j];
		if (brickA == nullptr)
		{
			return;
		}

		//only if this brick is available
		if (brickA->state != Brick::States::PICKABLE)
		{
			return;
		}

		//Found match, add:
		m.insert(std::make_pair(start, j));

		//Check limits:
		if ((start + 1) >= ROWS)
		{
			return;
		}

		//Try to get the next brick
		Brick* brickB = &_bricks[start + 1][j];
		if (brickB == nullptr)
		{
			return;
		}

		//only if it is available
		if (brickB->state != Brick::States::PICKABLE)
		{
			return;
		}

		//are them of the same color?
		if (brickB->gameTexture == brickA->gameTexture)
		{
			start++;
		}
		else {
			return;
		}
	}
}

void Board::removeMatchSetDuplicates(std::vector<MatchSet> &m)
{
	std::vector<MatchSet>::iterator it;
	it = std::unique(m.begin(), m.end());
	m.resize(std::distance(m.begin(), it));
}


void Board::checkVerticalMatches()
{
	int i = 0;
	//Aux var to store sets of matches(pairs of indexes to use with the bricks array)
	MatchSet matches;
	//Search for vertical matches 
	mVerticalMatches.clear();
	int startRow = 0;
	for (startRow = 0; startRow < ROWS; startRow++)
	{
		i = 0;
		while (i < ROWS)
		{
			matches.clear();
			searchVerticalMatches(i, startRow, matches);
			if (matches.size() >= MIN_MATCH)
			{
				bool bStop = false;
				//search for any row with the same set in mVerticalMatches
				//std::vector<MatchSet>::iterator
				for (auto rowIt = mVerticalMatches.begin();
				rowIt != mVerticalMatches.end() && !bStop;
					) 
				{
					bool deleteFlag = false;
					//MatchSet::iterator
					for (auto pairIt = matches.begin();
					pairIt != matches.end();
						) {
						deleteFlag = false;
						MatchSet::iterator f = rowIt->find(*pairIt);
						//is the set in the row of the vector?
						if (f != rowIt->end())
						{
							//found a set in matches that appears in a row
							//is the row bigger than this new set?
							if (rowIt->size() > matches.size())
							{
								//we dont carry this new set
								matches.clear();
								bStop = true;
							}
							else {
								//remove the whole row (it's a set)
								//and go to the next row
								rowIt = mVerticalMatches.erase(rowIt);
								deleteFlag = true;
							}
							break;
						}
						else {
							pairIt++;
						}
					}
					if (!deleteFlag) {
						++rowIt;
					}

				}
				if (matches.size() > 0)
				{
					mVerticalMatches.push_back(matches);
					i += (matches.size() - 1);
				}
			}
			i++;
		}
	}
}

void Board::checkHorizontalMatches()
{
	int i = 0, startRow = 0;
	MatchSet matches;
	//Search for horizontal matches
	mHorizontalMatches.clear();
	matches.clear();
	for (i = 0; i < COLUMNS; i++)
	{
		startRow = 0;
		while (startRow < ROWS)
		{
			matches.clear();
			searchHorizontalMatches(i, startRow, matches);
			if (matches.size() >= MIN_MATCH)
			{
				mHorizontalMatches.push_back(matches);
				startRow += (matches.size() - 1);
			}
			startRow++;
		}
	}
	if (mHorizontalMatches.size()>1)
		removeMatchSetDuplicates(mHorizontalMatches);
}

void Board::checkMatches()
{
	//different methods to search and reorder the sets
	//using a comprobation for the best set to be in the array for a given index:
	checkVerticalMatches();
	//or adding all the sets found in an index and then removeMatchSetDuplicates:
	checkHorizontalMatches();
}

//debug info for matches
void Board::printFoundMatches() const
{
	std::cout << std::endl << "===========" << std::endl;
	std::cout << mVerticalMatches.size() << " vertical matches and " << mHorizontalMatches.size() << "  horizontal " << std::endl;
	if (mVerticalMatches.size() > 0)
	{
		std::cout << "-----" << std::endl;
		for (auto &vertSols : mVerticalMatches)
		{
			std::cout << "Vert.Sol Set: ";
			for (auto &sol : vertSols)
				std::cout << sol.first << "," << sol.second << "; ";
			std::cout << std::endl;
		}
	}
	if (mHorizontalMatches.size() > 0)
	{
		std::cout << "-----" << std::endl;
		for (auto &horizSols : mHorizontalMatches)
		{
			std::cout << "Horiz.Sol Set: ";
			for (auto &sol : horizSols)
				std::cout << sol.first << "," << sol.second << "; ";
			std::cout << std::endl;
		}
	}
}

void Board::executeFoundMatches()
{
	mMatchesFound.clear();
	if (mVerticalMatches.size() == 0 && mHorizontalMatches.size() == 0) return;
	if (mVerticalMatches.size() > 0)
	{
		//move vertical matches to the shared array
		mMatchesFound = std::move(mVerticalMatches);
	}
	if (mHorizontalMatches.size() > 0)
	{
		//copy horizontal matches in the shared array
		mMatchesFound.insert(
			mMatchesFound.end(),
			mHorizontalMatches.begin(),
			mHorizontalMatches.end()
		);
		//empty the horizontal matches
		mHorizontalMatches.clear();
	}
	if (mMatchesFound.size() > 0)
	{
		//removing of matched bricks with animations:
		removeMatches();
	}
}

void Board::removeMatches()
{
	if (mMatchesFound.size() == 0) return;
	for (auto &_set : mMatchesFound)
	{
		bool explosion = false;
		for (auto &index : _set) 
		{
			if (_initialized && state != BoardStates::IDLE&&!_gameOver)
			{
				//add score
				addScore(
					50 + _set.size() * 10,
					340 + index.first*BRICK_SPACE,
					150 + index.second*BRICK_SPACE
				);
			}
			if (!explosion)
			{
				explosion = true;
				addExplosion(
					glm::vec2(
						LEFT_BORDER + 
						(index.first* BRICK_SPACE),
						TOP_BORDER + 
						(index.second * BRICK_SPACE) - 
						BRICK_SPACE
					),
					rand() % 60, 
					MainGame::GetTextureColor(
					 static_cast<GameTextures>(
						_bricks[index.first][index.second].gameTexture
					 )
					)
				);
				if (_set.size() > 3) {
					mEngine.playSoundEffect(
						GameSoundFXs::MATCH_4);
				}
				else {
					mEngine.playSoundEffect(
						GameSoundFXs::MATCH_3);
				}
				/*if (_set.size()>3 && _bricks[index.first][index.second].type == Brick::Types::NORMAL)
				{
					_bricks[index.first][index.second].state = Brick::States::FALLING;
					_bricks[index.first][index.second].type = Brick::Types::BOMB;
				}*/
			} else {
				/*_bricks[index.first][index.second].state = Brick::States::REMOVING;*/
			}
			_bricks[index.first][index.second].state = Brick::States::REMOVING;
		}
	}
	animationTime = 0.0f;
	//Start the removing process
	if(!_gameOver)
		state = BoardStates::REMOVING;
}


void Board::freeRemovedBricks()
{
	bool fall = false;
	//The Falling bricks can only be pickable after animations
	//so now we are setting them to pickable:
	for (unsigned int i = 0; i < ROWS; i++)
	{
		for (unsigned int j = 0; j < COLUMNS; j++)
		{
			if (_bricks[i][j].state == Brick::States::FALLING)
			{
				_bricks[i][j].state = Brick::States::PICKABLE;
			}
		}
	}
	//search for any removing or empty brick 
	//and then search for a brick above them to make this fall
	for (unsigned int i = 0; i < ROWS; i++)
	{
		for (unsigned int j = COLUMNS - 1; j != 0;  j--)
		{
			if (_bricks[i][j].state == Brick::States::REMOVING || _bricks[i][j].state== Brick::States::EMPTY)
			{
				//check for the above position
				if (j > 0 && _bricks[i][j - 1].state == Brick::States::PICKABLE) 
					{
						//Found one, swap:
						Brick temp = _bricks[i][j - 1];
						_bricks[i][j] = temp;
						_bricks[i][j - 1] = _bricks[i][j];
						//now, configure the above swapped brick to fall
						_bricks[i][j].state = Brick::States::FALLING;
						//and configure the removed brick will be just empty
						_bricks[i][j - 1].state = Brick::States::EMPTY;
						//fall flag to do start the animations
						fall = true;
						
				} else {
					_bricks[i][j].state = Brick::States::EMPTY;
				}
			}
		}
	}
	
	if (fall) {
		animationTime = 0.0f;
		if(!_gameOver)
		state = BoardStates::FALLING;
		
	} else {
		//Looping to keep removing matches and falling
		if (refillEmptyBricks()) {
			//There are more bricks falling
			animationTime = 0.0f;
			if(!_gameOver)
				state = BoardStates::FALLING;
		}
		else {
			checkMatches();
			if (getNumMatches() > 0) {
				executeFoundMatches();
			}
			else {
				if (!_gameOver)
					state = BoardStates::PLAYING;
			}
		}
	}
}

bool Board::refillEmptyBricks()
{
	bool newFall = false;
	unsigned int r;
	for (unsigned int i = 0; i < ROWS; i++)
	{
		for (unsigned int j = 0; j < COLUMNS; j++)
		{
			if (_bricks[i][j].state == Brick::States::EMPTY || _bricks[i][j].state == Brick::States::REMOVING
			) 
			{
				//generate random texture
				r = idist(rgen);
				_bricks[i][j].texture = textureCache.getTexture(MainGame::GetTexturePath(static_cast<GameTextures>(r)));
				_bricks[i][j].gameTexture = static_cast<GameTextures>(r);
				if (j == 0) 
				{
					_bricks[i][j].state = Brick::States::PICKABLE;
				} else {
					_bricks[i][j].state = Brick::States::FALLING;
				}
				
				newFall = true;
			}
		}
	}
	return newFall;
}
#pragma endregion


///----------------------------
#pragma region BOARD UPDATE AND ANIMATIONS
///-----------------------------
void Board::update()
{
	if(_initialized && !_gameOver)
		_totalTime += mEngine.GetLastFrameSeconds();
	
	if (target.active)
	{
		target.angle += mEngine.GetLastFrameSeconds()*2.5f;
		if (target.angle > 360.f) target.angle = 0.f;
	}
	//update fuse rotation
	_fuse.rotation += mEngine.GetLastFrameSeconds();
	if (_fuse.rotation > 360.0f) _fuse.rotation = 0.0f;
	//update fuse position
	
	if (_totalTime - _fuse.lastPositionChange > FUSE_CHANGE_POSITION_TIME)
	{
		//go to the next position in the rope: maybe use a bezier curve to make it smoother?
		_fuse.positionIndex++;
		if (_fuse.positionIndex>NUM_ROPE_POSITIONS - 1) {
			_fuse.positionIndex = NUM_ROPE_POSITIONS - 1;
			_fuse.active = false; //final position: goal reached: bomb
		} else {
			//update the time
			_fuse.lastPositionChange = _totalTime;
		}
	} 
	//remove dead score texts:
	_scoreTexts.remove_if([](ScoreText& st) {
		return st.lifeTime <= 0.0f;
	});
	//update score text positions
	for (auto &st : _scoreTexts)
	{
		//update text position
		st.position += st.speed;
		//update text color alpha
		st.color.a = (GLubyte)(
			(st.totalLifeTime*st.lifeTime)
			/ 255.0f
		);
		//update text life
		st.lifeTime -= st.lifeDecayRate;
	}

	switch (state)
	{
	case BoardStates::PLAYING:
		if (!_initialized) _initialized = true;
		break;
	case BoardStates::FALLING:
		//move falling bricks
		if (animationTime >= BRICK_FALL_TIME)
		{
			//Remove all removed bricks until no more above filling can be done
			freeRemovedBricks();
		} else {
			animationTime += mEngine.GetLastFrameSeconds();
		}
		break;
	case BoardStates::SWAPPING_FORWARD:
		if (animationTime >= BRICK_SWAP_TIME)
		{
			//end of animation
			Brick aux = _bricks[target.toI][target.toJ];
			_bricks[target.toI][target.toJ] = _bricks[target.fromI][target.fromJ];
			_bricks[target.fromI][target.fromJ] = aux;
			_bricks[target.toI][target.toJ].state = _bricks[target.fromI][target.fromJ].state = Brick::States::PICKABLE;
			checkMatches();
			int newNumMatches = getNumMatches();
			if (newNumMatches == 0 || numMatches >= newNumMatches)
			{
				//Bad move, do the swap back
				unsigned int i2 = target.fromI, j2 = target.fromJ;
				target.fromI = target.toI; target.fromJ = target.toJ;
				OnSwapBrickWith(i2, j2, SWAPPING_BACK);
			}
			else {
				if (!_gameOver) {
					state = BoardStates::PLAYING;
					checkMatches();
					printFoundMatches();
					executeFoundMatches();
				}
			}

		}
		else {
			animationTime += mEngine.GetLastFrameSeconds();
		}

		break;
	case BoardStates::SWAPPING_BACK:
		if (animationTime >= BRICK_SWAP_TIME)
		{
			//end of animation
			Brick aux = _bricks[target.toI][target.toJ];
			_bricks[target.toI][target.toJ] = _bricks[target.fromI][target.fromJ];
			_bricks[target.fromI][target.fromJ] = aux;
			checkMatches();
			_bricks[target.toI][target.toJ].state = _bricks[target.fromI][target.fromJ].state = Brick::States::PICKABLE;
			if(!_gameOver)
			state = BoardStates::PLAYING;
		}
		else {
			animationTime += mEngine.GetLastFrameSeconds();
		}
		break;
	case BoardStates::REMOVING:
		if (animationTime >= BRICK_SCALE_TIME)
		{
			//Mark all the bricks as free now and make the falling
			freeRemovedBricks();
		}
		else {
			animationTime += mEngine.GetLastFrameSeconds();
		}
		break;
	default:
		break;
	}
}

#pragma endregion

///----------------------------
#pragma region EVENTS
///-----------------------------

///Stops the board 
void Board::stop()
{
	//change the game state
	state = BoardStates::GAME_OVER;
	
	//avoid state changing from game over to any other
	_gameOver = true;
}

bool Board::OnSwapBrickWith(unsigned int toI, unsigned int toJ, BoardStates nextState)
{
	//no diagonals
	if (target.fromI != toI&&target.fromJ != toJ) return false;
	//only adjacents 
	if (target.fromI != toI && glm::distance((float)target.fromI, (float)toI) > 1) return false;
	if (target.fromJ != toJ && glm::distance((float)target.fromJ, (float)toJ) > 1) return false;
	//ok,swap animation can be executed
	checkMatches();
	//Store the num of matches before the change to compare after the swap
	numMatches = getNumMatches();
	//nextState will change the game the gameloop runs
	state = nextState;
	animationTime = 0;
	//now the swap starts in the next frame, but first,copy data:
	std::cout << "Swapping " << target.fromI << "," << target.fromJ << " with: " << toI << "," << toJ << " tex:" << _bricks[target.fromI][target.fromJ].gameTexture << std::endl;
	//swap sound
	if (nextState == BoardStates::SWAPPING_FORWARD)
		mEngine.playSoundEffect(GameSoundFXs::SWAP_FORWARD);
	else
		mEngine.playSoundEffect(GameSoundFXs::SWAP_BACK);
	//change the state of the bricks to know if they are swapping so we can know where are them in the draw function
	_bricks[toI][toJ].state = _bricks[target.fromI][target.fromJ].state = Brick::States::SWAPPING;
	//store the indexes of the destiny to make the swap animation
	target.toI = toI;
	target.toJ = toJ;

	return true;
}

void Board::clickBoard(const float x, const float y)
{
	float bx = 0.f, by = 0.f;
	unsigned int i = 0, j = 0;
	if (findBrickInScreenCoords(x, y, bx, by, i, j))
	{
		if (!target.active)
		{
			std::cout << "Touching: (" << i << ","
				<< j << ") tex:" << _bricks[i][j].gameTexture
				<< std::endl
				;
			target.position.x = bx - 3.0f;
			target.position.y = by + target.texture.height + 5;
			target.fromI = i;
			target.fromJ = j;
			target.active = true;
		}
		else {
			if (target.fromI != i || target.fromJ != j)
			{
				if (glm::distance(
					static_cast<float>(target.fromI),
					static_cast<float>(i)
					)>1 || glm::distance(
						static_cast<float>(target.fromJ),
						static_cast<float>(j)
						)> 1)
				{
					//too far, change target
					target.position.x =
						LEFT_BORDER +
						(i * BRICK_SPACE)
						- 3.0f;
					target.position.y =
						TOP_BORDER +
						(j * BRICK_SPACE) - BRICK_SPACE
						+ target.texture.height
						+ 5;
					target.fromI = i;
					target.fromJ = j;
				}
				//ready for swapping
			}
			else {
				//click on the same gem: remove target
				target.active = false;
			}
		}
	}
	else {

	}
}


void Board::onMouseDown(float x, float y)
{
	switch (state)
	{
	case Board::PLAYING:
		clickBoard(x, y);
		break;
	default://Only check for buttons
		break;
	}
	
}


void Board::onMouseUp(float x, float y)
{
	switch (state)
	{
	case Board::PLAYING:
		if (target.active)//Swap?
		{
			float bx = 0.f, by = 0.f;
			unsigned int i = 0, j = 0;
			bool isBrick = findBrickInScreenCoords(x, y, bx, by, i, j);
			if (isBrick)
			{
				if (i != target.fromI || j != target.fromJ)
				{
					if (OnSwapBrickWith(i, j))
						target.active = false;
				}

			} else {
				//Find the direction of the mouse using
				//the target position to check 
				//if the mouse moved to a side and there is
				//an adjacent brick there, swap too

			}
		}
		break;
	default:
		break;
	}
	
}
#pragma endregion

///----------------------------
#pragma region  UTILS
///-----------------

void Board::addScore(int score, float x, float y)
{
	this->_score += score;
	this->_scoreTexts.emplace_back(
		ScoreText(
			std::to_string(score), //score value
			glm::vec2(x,y), //position
			COLOR_SCORES, //Color
			238.f, //life
			1.f, //life decay rate-->applied to color too
			glm::vec2(0.0f,-0.04f) //speed (go up)
		)
	);
}

void Board::addExplosion(const glm::vec2& position, int numParticles, xixEngine::ColorRGBA8 color /*=COLOR_WHITE*/)
{
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);
	
	glm::vec2 vel(32.0f, 30+rand()%6);
	glm::mat4 transformation;

	for (int i = 0; i < numParticles; i++) 
	{
		particleBatch->addParticle(
			glm::vec2(
				position.x-mEngine.GetWidth()/2, 
				-position.y+mEngine.GetHeight()/2
			),
			glm::rotate(vel,static_cast<float>(randAngle(rgen))), 
			color, 30.0f
		);
	}
	
}


///
bool Board::findBrickInScreenCoords(const float x, const float y, float& brickX, float& brickY, unsigned int &I, unsigned int &J) const
{
	glm::vec2 wc(x, y);
	unsigned int i, j;
	float bx, by;
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLUMNS; j++)
		{
			//check collision with a gem (click) using each corner
			bx = static_cast<float>( LEFT_BORDER + (i * BRICK_SPACE));
			by = static_cast<float>(TOP_BORDER + (j * BRICK_SPACE) - BRICK_SPACE);
			if (wc.x < bx + 35 &&
				wc.x + 5 > bx  &&
				wc.y < by + 35 &&
				5 + wc.y > by
				)
			{
				if (_bricks[i][j].state == Brick::States::PICKABLE)
				{
					brickX = bx;
					brickY = by;
					I = i;
					J = j;
					return true;
				}
			}
		}
	}
	return false;
}

#pragma endregion
