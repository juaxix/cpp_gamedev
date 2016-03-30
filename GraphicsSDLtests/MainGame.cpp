#include "MainGame.h"

MainGame::MainGame(): 
	_screenWidth(800),
	_screenHeight(640), 
	_gameState(GameState::IDLE),
	_time(0),
	_maxFPS(60)
{
	//Init the camera
	_camera.init(_screenWidth, _screenHeight);
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
	xixEngine::init();
	if (_window.create("Graphics SDL tests", _screenWidth, _screenHeight, 0))
	{
		//Load and compile all the shaders:
		initShaders();

		//Now we can init the sprite batch
		_spriteBatch.init();

		//Limit the fps
		_fpsLimiter.init(_maxFPS);
		return true;
	}
	else {
		return false;
	}
	
}

void MainGame::run()
{
	if (initSystems()) {
		//change the game state:
		_gameState = GameState::PLAY;
		//let's initialize the sprites: old method with sprites directly, now we use SpriteBatch
		
		//from the left bottom corner to the center
		//_sprites.push_back(new xixEngine::Sprite());
		//_sprites.back()->init(0.0f, 0.0f, _screenWidth/2, _screenWidth / 2, "Textures/jimmyJump_pack/CharacterRight_Standing.png");
		//_sprites.push_back(new xixEngine::Sprite());
		//_sprites.back()->init(_screenWidth / 2, 0.0f, _screenWidth / 2, _screenWidth / 2, "Textures/jimmyJump_pack/CharacterRight_Standing.png");
		

		//Start the loop
		this->gameLoop();
	}
}

void MainGame::processInput()
{
	SDL_Event e;
	const float CAMERA_SPEED = 10.0f;
	while (SDL_PollEvent(&e)) 
	{
		switch (e.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			return;
			break;
		
		case SDL_KEYDOWN:
			_inputManager.pressKey(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(e.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(e.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(e.button.button);
			break;
		case SDL_MOUSEMOTION:
			//std::cout << e.motion.x << "," << e.motion.y<<std::endl;
			_inputManager.setMouseCoords(e.motion.x, e.motion.y);
			break;
		}
	}
	
	if (_inputManager.isKeyPressed(SDLK_w))
		_camera.setPosition(
			_camera.getPosition() +
			glm::vec2(0.0, -CAMERA_SPEED)
	);
			
	if (_inputManager.isKeyPressed(SDLK_a))
		_camera.setPosition(
			_camera.getPosition() +
			glm::vec2(CAMERA_SPEED, 0.0)
	);
			
	if (_inputManager.isKeyPressed(SDLK_d))
		_camera.setPosition(
			_camera.getPosition() +
			glm::vec2(-CAMERA_SPEED, 0.0)
	);
			
	if (_inputManager.isKeyPressed(SDLK_s))
		_camera.setPosition(
			_camera.getPosition() +
			glm::vec2(0.0, CAMERA_SPEED)
	);
			
	if (_inputManager.isKeyPressed(SDLK_q))
		_camera.setScale(
			_camera.getScale() + 0.1f
	);
			
	if (_inputManager.isKeyPressed(SDLK_e))
		_camera.setScale(
			_camera.getScale() - 0.1f
	);
	//SHOOT
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) 
	{
		glm::vec2 mouseWCoords = _camera.getScreenToWorldCoords(
			_inputManager.getMouseCoords()
		);
		
		glm::vec2 playerPosition(0.0);
		glm::vec2 shootDirection = mouseWCoords - playerPosition;
		//normalize direction
		shootDirection = glm::normalize(shootDirection);
		//std::cout<< "Bullet is going in dir:" << shootDirection.x << "," << shootDirection.y <<std::endl;
		//Create bullet
		_bullets.emplace_back(
			playerPosition, 
			shootDirection, 
			5.0f, 
			//"Textures/jimmyJump_pack/Bullet.png",
			100
		);
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

	//GLuint timeLocation = _colorProgram.getUniformLocation("time");
	//Set time to the GPU
	//glUniform1f(timeLocation, _time);
	
	GLuint camPositionLocation = _colorProgram.getUniformLocation("camPosition");
	glm::mat4 cameraMatrix = _camera.getCamMatrix();
	glUniformMatrix4fv(camPositionLocation, 1, GL_FALSE, &(cameraMatrix[0][0])// address of the first element 
	);
	
	//Example: Draw a polygon
	
	//glEnableClientState(GL_COLOR_ARRAY);
	//glBegin(GL_TRIANGLES);
	//	//Color:
	//	glColor3f(0.7f,0.2f,0.1f);
	//	glVertex2f(0, 0);
	//	glVertex2f(0, 1);
	//	glVertex2f(1, 1);
	//glEnd();
	

	//Draw sprites from game entities: first player
	_spriteBatch.begin();
	//Sprite info
	glm::vec4 sPos(0.0, 0.0, 50.0, 50.0);
	glm::vec4 sUV(0.0f, 0.0f, 1.0f, 1.0f);
	static xixEngine::GLTexture sTexture = xixEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/CharacterRight_Standing.png");
	xixEngine::Color sColor;
	sColor.r = sColor.g = sColor.b = sColor.a = 255;
	
	_spriteBatch.draw(
		sPos, sUV, sTexture.id, 0.0f, sColor
	);
	
	//now bullets
	for (int i = 0; i < _bullets.size(); i++)
	{
		_bullets[i].draw(_spriteBatch);
		//_spriteBatch.draw(
		//	glm::vec4(
		//		_bullets[i].getPosition().x, _bullets[i].getPosition//().y,_bullets[i].getSize(), _bullets[i].getSize()
		//	), sUV, _bullets[i].getTexture().id, 0.0f, _bullets//[i].getColor()
		//);
	}
	
	_spriteBatch.end();
	_spriteBatch.renderBatch();
	
	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//disable the shader
	_colorProgram.unUse();

	//Now we swap the buffers and draw everything to the screen
	_window.swapBuffers();
}



void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		//use for frame time measuring
		_fpsLimiter.begin();

		processInput();
		_time += 0.1f;
		//Update the camera
		_camera.update();

		//update all the bullets
		for (int i = 0; i < _bullets.size();)
		{
			if(_bullets[i].update()) {
				//destroy by swapping
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else {
				i++;
			}
		}

		drawGame();
		
		//Print the result once 100 frames only
		static unsigned int currentFrame = 0;
		currentFrame++;

		//End of the FPS measures
		_fpsLimiter.end();
		//Show FPS only each 1k frames
		if (currentFrame == 1000) 
		{
			std::cout << _fpsLimiter.getFPS() << " FPS" << std::endl;
			currentFrame = 0;
		}
	}
}
