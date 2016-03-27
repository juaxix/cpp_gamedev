#include "MainGame.h"

MainGame::MainGame(): 
	_window(nullptr), 
	_screenWidth(800),
	_screenHeight(640), 
	_gameState(GameState::IDLE),
	_time(0),
	_fps(0),
	_frameTime(0),
	_maxFPS(60)
{
}

MainGame::~MainGame()
{
}

bool MainGame::initShaders() 
{
	//Pass the shaders
	if (!_colorProgram.compileShaders(
		"Shaders/colorShading.vert", 
		"Shaders/colorShading.frag"
		)
		) 
	{
		return false;
	}
	//bind attributes
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	//link program
	if (!_colorProgram.linkShaders()) {
		return false;
	}

	return true;
}

bool MainGame::initSystems() 
{
	_screenWidth = 800;
	_screenHeight = 640;
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow(
		"Graphics SDL tests", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		_screenWidth,
		_screenHeight,
		SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN
	);
	if (_window == nullptr) 
	{
		fatalError("SDL Window could not be created");
		return false;
	}
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr)
	{
		fatalError("SDL GL Context could not be created");
		return false;
	}
	//Avoid weird errors (optional)
	glewExperimental = true;
	//init GLU extensions
	GLenum errorGL = glewInit();
	if (errorGL != GLEW_OK)
	{
		fatalError("Could not initialized glew");
		return false;
	}
	//We want to have 2 windows, one for drawing and another for buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//Set the color for clean the screen
	glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
	initShaders();
	return true;
}

void MainGame::run()
{
	if (initSystems()) {
		//change the game state:
		_gameState = GameState::PLAY;
		//let's initialize the sprite
		_sprite.init(-1.0f, -1.0f, 1.0f, 1.0f, "Textures/jimmyJump_pack/CharacterRight_Standing.png"); //from the left bottom corner to the center

		//more sprites
		_sprites.push_back(new Sprite());
		_sprites.back()->init(0, -1, 1, 1, "Textures/jimmyJump_pack/CharacterRight_Standing.png");
		_sprites.push_back(new Sprite());
		_sprites.back()->init(0, 0, 1, 1, "Textures/jimmyJump_pack/CharacterRight_Standing.png");
		_sprites.push_back(new Sprite());
		_sprites.back()->init(-1, 0, 1, 1, "Textures/jimmyJump_pack/CharacterRight_Standing.png");
		//Start the loop
		this->gameLoop();
	}
}

void MainGame::processInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		switch (e.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			return;
			break;
		case SDL_MOUSEMOTION:
			//std::cout << e.motion.x << "," << e.motion.y<<std::endl;
			break;
		default:
			break;
		}
	}
}


void MainGame::drawGame() 
{
	glClearDepth(1.0); 
	//clear color and depth buffer
	glClear(
		GL_COLOR_BUFFER_BIT | 
		GL_DEPTH_BUFFER_BIT
	);

	//Use the GLSL programs
	_colorProgram.use();
	//Activate the first texture so we can bind it to the sprite
	glActiveTexture(GL_TEXTURE0);
	//the player texture is binded
	
	GLint textureLocation = _colorProgram.getUniformLocation("tSampler");
	glUniform1i(textureLocation, 0);

	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	//Set time to the GPU
	glUniform1f(timeLocation, _time);

	//Example: Draw a polygon
	/*
	glEnableClientState(GL_COLOR_ARRAY);
	glBegin(GL_TRIANGLES);
		//Color:
		glColor3f(0.7f,0.2f,0.1f);
		glVertex2f(0, 0);
		glVertex2f(0, 1);
		glVertex2f(1, 1);
	glEnd();
	*/

	//Draw a sprite
	_sprite.draw();
	//More sprites draw
	for (int i = 0; i < _sprites.size(); i++)
	{
		_sprites[i]->draw();
	}
		
	_colorProgram.unUse();
	//unbind the player texture
	glBindTexture(GL_TEXTURE_2D, 0);
	//Now we swap the buffers and draw everything to the screen
	SDL_GL_SwapWindow(_window);
	
}

void MainGame::calculateFPS()
{
	//vars that only can be changed here:
	static const int NUM_SAMPLES = 100;
	static float frameTimes[NUM_SAMPLES];
	static float prevTicks = SDL_GetTicks();
	static int currentFrame = 0;
	//get current ticks and compare with previous ticks
	float currentTicks = SDL_GetTicks();
	
	_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame%NUM_SAMPLES] = _frameTime;
	prevTicks = currentTicks;
	int count;  //number of measures
	currentFrame++; //first frame 1:avoid division by zero
	if (currentFrame < NUM_SAMPLES) 
	{
		count = currentFrame;
	} else {
		count = NUM_SAMPLES;
	}
	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++)
	{
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;
	if (frameTimeAverage > 0)
	{
		_fps = 1000.0f / frameTimeAverage;
	} else {
		_fps = 60.0f;
	}
	
	//Print the result once 100 frames only
	if (fmodf(currentFrame, 100.0f) == 0)
		std::cout << _fps << "(" << _frameTime << ")" << std::endl;
	
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT) 
	{
		//use for frame time measuring
		float startTicks = SDL_GetTicks();
		
		processInput();
		_time += 0.01f;
		drawGame();
		calculateFPS();

		//Limit the FPS:
		float frameTicks = SDL_GetTicks() - startTicks;
		if (1000.0f /*millisec*/ / _maxFPS > frameTicks)
		{
			//wait a little longer, delay the programs execution
			SDL_Delay(1000.0f / _maxFPS - frameTicks);//some windows does not accept a minimum number to delay programs
		}
	}
}

