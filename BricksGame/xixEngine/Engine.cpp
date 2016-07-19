/// Engine.cpp
/// Implementation of the class Engine
#include "Engine.h"

namespace xixEngine
{
	///Struct to hide the implementation of the engine from outside Engine
	struct Engine::EngineImplementation
	{
		Sdl mSdl;
		SdlWindow mSdlWindow;
		GlContext mGlContext;
		float mElapsedTicks;
		float mMaxFrameTicks;
		float mTextScale;
		float mLastFrameSeconds;
		float mElapsedSeconds;
		Updater* mUpdater;
		bool mQuit;
		std::unordered_map<Texture, std::unique_ptr<SdlSurface>> mSdlSurfaceContainer;
		std::unique_ptr<SdlSurface> mFontSdlSurface;
		SpriteBatch spriteBatch;
		GLSLProgram glslProgram;
		InputManager inputManager;
		Camera2D camera;
		Audio audio;
		ParticleSystem particleSystem;
		EngineImplementation(const char* WindowTitle, int WindowWidth,
			int WindowHeight, float MaxFrameTicks, float TextScale)
			: mSdl(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE | SDL_INIT_AUDIO)
			, mSdlWindow(WindowTitle, WindowWidth, WindowHeight)
			, mGlContext(mSdlWindow)
			, mMaxFrameTicks(MaxFrameTicks)
			, mTextScale(TextScale)
			, mElapsedTicks(static_cast<float>(SDL_GetTicks()))
			, mLastFrameSeconds(1.0f / 60.0f)
			, mElapsedSeconds(0.f)
			, mUpdater(nullptr)
			, mQuit(false)
		{
			//Init the camera
			camera.init(WindowWidth, WindowHeight);

		}
		void initSystems();
		void initShaders();
		void Start();
		void ParseEvents();
	};

	void Engine::EngineImplementation::initSystems()
	{
		//init the shaders
		initShaders();
		//init the sprite batch
		spriteBatch.init();
		
		//init the audio
		audio.init();
	}



	void Engine::EngineImplementation::initShaders()
	{
		if (!glslProgram.compileShaders(
			"./shaders/colorShading.vert",
			"./shaders/colorShading.frag"
			)
			)
		{
			throw std::runtime_error("Error reading and compiling shaders");
		}
		//bind attributes
		glslProgram.addAttribute("vertexPosition");
		glslProgram.addAttribute("vertexColor");
		glslProgram.addAttribute("vertexUV");
		//link program
		if (!glslProgram.linkShaders()) {
			throw std::runtime_error("Error linking shaders");
		}
	}

	Engine::Engine(const char* windowTitle, int windowWidth, int windowHeight, float maxFrameTicks, float textScale)
		:
		_WindowWidth(windowWidth),
		_WindowHeight(windowHeight),
		_MaxFrameTicks(maxFrameTicks),
		m_TextScale(textScale),
		mPimpl(new EngineImplementation(windowTitle, windowWidth, windowHeight, maxFrameTicks, textScale))
	{
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetSwapInterval(0); //immediate update

		glEnable(GL_TEXTURE_2D);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0f, _WindowWidth, _WindowHeight, 0.0f, -1.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
	}

	void Engine::loadTexture(Texture index, const std::string& texturePath)
	{
		//Texture cache method: using std::unique_ptr destroys the object currently managed by the unique_ptr(if any) and takes ownership
		//the deleter of std::unique_ptr should delete the object when it exit the scope where it was declared, that is to say destroy it (as you can assume it should be), then free the memory used if needed.
		//But we need to add more textures from outside, we use an unordered_map to store the texture and it string
		auto it = mPimpl->mSdlSurfaceContainer.find(index);
		if (it != mPimpl->mSdlSurfaceContainer.end())
		{
			//does exist
			mPimpl->mSdlSurfaceContainer[index].reset(new SdlSurface(texturePath.c_str()));
		}
		else {
			//create space for it:
			mPimpl->mSdlSurfaceContainer[index] = 
				std::unique_ptr<SdlSurface>(new SdlSurface(texturePath.c_str()));
		}
	}

	void Engine::loadFontTexture(const std::string& texturePath)
	{
		mPimpl->mFontSdlSurface.reset(new SdlSurface(texturePath.c_str()));
	}

	void Engine::loadSoundEffect(unsigned int soundID, const std::string& soundPath)
	{
		mPimpl->audio.loadSoundEffect(soundID, soundPath);
	}

	void Engine::playSoundEffect(unsigned int soundID, int loops)
	{
		mPimpl->audio.playSoundEffect(soundID, loops);
	}

	Music Engine::loadMusic(const std::string& musicPath)
	{
		return mPimpl->audio.loadMusic(musicPath);
	}

	Engine::~Engine() {

	}

	float Engine::GetLastFrameSeconds() const {
		return mPimpl->mLastFrameSeconds;
	}

	float Engine::GetElapsedTimeSeconds() const {
		return mPimpl->mElapsedSeconds;
	}

	float Engine::GetMouseX() const {
		return mPimpl->inputManager.getMouseCoords().x;
	}

	float Engine::GetMouseY() const {
		return mPimpl->inputManager.getMouseCoords().y;
	}

	glm::vec2 Engine::GetMouseCoords() const {
		return mPimpl->inputManager.getMouseCoords();
	}

	const glm::vec2 Engine::GetMouseWorldCoords() const
	{
		return mPimpl->camera.getScreenToWorldCoords(
			mPimpl->inputManager.getMouseCoords()
		);
	}

	bool Engine::GetLeftMouseButtonDown() const {
		return mPimpl->inputManager.isKeyDown(SDL_BUTTON_LEFT);
	}

	bool Engine::GetRightMouseButtonPressed() const {
		return mPimpl->inputManager.isKeyPressed(SDL_BUTTON_RIGHT);
	}
	bool Engine::GetRightMouseButtonDown() const {
		return mPimpl->inputManager.isKeyDown(SDL_BUTTON_RIGHT);
	}

	bool Engine::GetLeftMouseButtonPressed() const {
		return mPimpl->inputManager.isKeyPressed(SDL_BUTTON_LEFT);
	}
	bool Engine::GetKeyPressed(unsigned int keyID) const {
		return mPimpl->inputManager.isKeyPressed(keyID);
	}

	bool Engine::GetKeyDown(unsigned int keyID) const {
		return mPimpl->inputManager.isKeyDown(keyID);
	}

	void Engine::Quit() {
		mPimpl->mQuit = true;
	}

	void Engine::Start(Updater& updater) 
	{
		mPimpl->mUpdater = &updater;
		mPimpl->initSystems();

		mPimpl->mSdlWindow.Show();
		mPimpl->Start();
	}

	/*int Engine::GetTextureHeight(Texture texture) const {
		return mPimpl->mSdlSurfaceContainer[texture]->Height();
	}

	int Engine::GetTextureWidth(Texture texture) const {
		return mPimpl->mSdlSurfaceContainer[texture]->Width();
	}*/

    void Engine::RenderSurfaceBatch(GLTexture gTexture, Texture texture, float x, float y, float rotation)
    {
		auto surfIt = mPimpl->mSdlSurfaceContainer.find(texture);
		if (surfIt == mPimpl->mSdlSurfaceContainer.end()) {
			std::cout << "You need to load the textures before render them";
			return;
		}
        //same uv for all sprites
        static const glm::vec4 sUV(0.0f, 0.0f, 1.0f, 1.0f);
        SdlSurface& surface = *mPimpl->mSdlSurfaceContainer[texture];
        glm::vec2 pos = mPimpl->camera.getScreenToWorldCoords(glm::vec2(x,y));
        this->mPimpl->spriteBatch.draw(
           glm::vec4(
            pos.x,pos.y,
            surface.Width(), surface.Height()
           ),
           sUV, gTexture.id,
           0.0f /* same depth */, 
           COLOR_WHITE, //right now we are not tinting the sprites
           rotation
        );
    }
    
	void Engine::Render(Engine::Texture texture, const glm::mat4& transform)
    {
		auto surfIt = mPimpl->mSdlSurfaceContainer.find(texture);
		if (surfIt== mPimpl->mSdlSurfaceContainer.end()) {
			std::cout << "You need to load the textures before render them";
			return;
		}
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));
		SdlSurface& surface = *mPimpl->mSdlSurfaceContainer[texture];
		surface.Bind();

		glBegin(GL_QUADS);
		glTexCoord2i(0, 1); glVertex2i(0, surface.Height());
		glTexCoord2i(1, 1); glVertex2i(surface.Width(), surface.Height());
		glTexCoord2i(1, 0); glVertex2i(surface.Width(), 0);
		glTexCoord2i(0, 0); glVertex2i(0, 0);
		glEnd();
        
		
		/** @TODO: Change GL_QUADS for GL_TRIANGLES */
		/*
		glBegin(GL_TRIANGLES);
		//First triangle of the quad
		//right up
		glVertex2f(surface.Width(), surface.Height());
		glTexCoord2i(1, 1);
		//up
		glVertex2f(0.0f,surface.Height());
		glTexCoord2i(0, 1);
		//left bottom
		glVertex2f(0.0f, 0.0f);
		glTexCoord2i(0,0);
		//Second triangle of the quad (same,but reversed order)
		glVertex2f(0.0f, 0.0f);
		glTexCoord2i(0, 0);
		glVertex2f(surface.Width(), 0.0f);
		glTexCoord2i(1, 0);
		glVertex2f(surface.Width(), surface.Height());
		glTexCoord2i(1, 1);
		glEnd();*/
	}

	void Engine::Render(Texture texture, float x, float y, float rotation) 
	{
		glm::mat4 transformation;
		transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
		if (rotation) {
			transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		Render(texture, transformation);
	}
	
	void Engine::RenderSprite(Texture texture, const float x,const float y, const float sx,const float sy, const float angle)
	{
        //same uv for all sprites
		static const glm::vec4 sUV(0.0f, 0.0f, 1.0f, 1.0f);
		glm::vec2 pos = mPimpl->camera.getScreenToWorldCoords(glm::vec2(x,y));
		this->mPimpl->spriteBatch.draw(
			glm::vec4(
				pos.x,pos.y,
				sx,sy
			),
			sUV, texture, 
			0.0f /* same depth */, 
			COLOR_WHITE, //right now we are not tinting the sprites
			angle
		);
	}

	void Engine::Write(const char* text, const glm::mat4& transform,const float textScale, ColorRGBA8 color) 
	{
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));
		int advance = 0;
		float _textScale = textScale == -1.0f ? this->m_TextScale:textScale;
		for (; *text;++text) {
			FontData& g = FindFontChar(*text);
			float fontTexWidth  = static_cast<float>(mPimpl->mFontSdlSurface->Width());
			float fontTexHeight = static_cast<float>(mPimpl->mFontSdlSurface->Height());

			float uvLeft = static_cast<float>(g.x) / fontTexWidth;
			float uvRight = static_cast<float>(g.x + g.width) / fontTexWidth;
			float uvBottom = static_cast<float>(g.y) / fontTexHeight;
			float uvTop = static_cast<float>(g.y + g.height) / fontTexHeight;

			float worldLeft = static_cast<float>(g.xoffset + advance);
			float worldRight = static_cast<float>(g.xoffset + g.width + advance);
			float worldBottom = static_cast<float>(g.yoffset);
			float worldTop = static_cast<float>(g.yoffset + g.height);

			mPimpl->mFontSdlSurface->Bind();

			glBegin(GL_QUADS);
			glColor4f(
				static_cast<float>(color.r)/255.0f, 
				static_cast<float>(color.g)/255.0f,
				static_cast<float>(color.b)/255.0f,
				static_cast<float>(color.a)/255.0f
			);
			
			glTexCoord2f(uvLeft / 2.0f, uvTop / 2.0f); glVertex2f(worldLeft * _textScale, worldTop * _textScale);
			glTexCoord2f(uvRight / 2.0f, uvTop / 2.0f); glVertex2f(worldRight * _textScale, worldTop * _textScale);
			glTexCoord2f(uvRight / 2.0f, uvBottom / 2.0f); glVertex2f(worldRight * _textScale, worldBottom * _textScale);
			glTexCoord2f(uvLeft / 2.0f, uvBottom / 2.0f); glVertex2f(worldLeft * _textScale, worldBottom * _textScale);
			glEnd();
			glColor4f(1.f, 1.f, 1.f,1.f);
			advance += g.advance;
		}
	}

	void Engine::Write(const char* text, float x, float y, float rotation,ColorRGBA8 color,float scale) 
	{
		//position
		glm::mat4 transformation;
		transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
		//rotation
		transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		transformation = glm::translate(
			transformation, glm::vec3(-xixEngine::CalculateStringWidth(
				text, scale == -1.0f ? m_TextScale : scale
				),
				-20.0f, 0.0f)
		);
		
		Write(text, transformation, scale == -1.0f ? m_TextScale : scale, color);
	
	}

	void Engine::RenderParticleSystems() 
	{
		mPimpl->particleSystem.draw(&mPimpl->spriteBatch);
	}

	void Engine::AddParticleBatch(ParticleBatch* particleBatch)
	{
		mPimpl->particleSystem.addParticleBatch(particleBatch);
	}

	int Engine::GetWidth() const {
		return _WindowWidth;
	}

	int Engine::GetHeight() const {
		return _WindowHeight;
	}

	void Engine::spriteBatchingBegin(float time) 
	{
		//Use the GLSL program
		mPimpl->glslProgram.use();
		//Activate the first texture so we can bind it to the sprite
		glActiveTexture(GL_TEXTURE0);
		GLint textureLocation = mPimpl-> glslProgram.getUniformLocation("tSampler");
		glUniform1i(textureLocation, 0);
		GLuint timeLocation = mPimpl->glslProgram.getUniformLocation("time");
		// time to the GPU
		glUniform1f(timeLocation, time);
		mPimpl->spriteBatch.begin();
		GLuint camPositionLocation = mPimpl->glslProgram.getUniformLocation("camPosition");
		glm::mat4 cameraMatrix = mPimpl->camera.getCamMatrix();
		glUniformMatrix4fv(camPositionLocation, 1, GL_FALSE, &(cameraMatrix[0][0])// address of the first element 
		);
	}
	


	void Engine::spriteBatchingEnd()
	{
		mPimpl->spriteBatch.end();
		mPimpl->spriteBatch.renderBatch();

		//unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);
		mPimpl->glslProgram.unUse();
	}

	void Engine::EngineImplementation::Start() {
		while (!mQuit) 
		{
			glClearDepth(1.0);
            
			//clear color and depth buffer
			glClear(
				GL_COLOR_BUFFER_BIT |
				GL_DEPTH_BUFFER_BIT
			);
			//Get input
			ParseEvents();
			
			//manage frames
			float currentTicks = static_cast<float>(SDL_GetTicks());
			float lastFrameTicks = currentTicks - mElapsedTicks;
			mElapsedTicks = currentTicks;

			lastFrameTicks = std::min(lastFrameTicks, mMaxFrameTicks);
			mLastFrameSeconds = lastFrameTicks * 0.001f;
			mElapsedSeconds += mLastFrameSeconds;
			//Update the camera
			camera.update();
			
			//Update the particles
			particleSystem.update(mLastFrameSeconds);

			//Call the user update function
			if (mUpdater) 
			{
				mUpdater->Update();
			}
			//update input
			inputManager.update();
			//Swap the buffer and draw content in screen:
			SDL_GL_SwapWindow(mSdlWindow);
            
		}
	}

	void Engine::EngineImplementation::ParseEvents() {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				mQuit = true;
				break;
			case SDL_KEYDOWN:
				inputManager.pressKey(e.key.keysym.sym);
				break;
			case SDL_KEYUP:
				inputManager.releaseKey(e.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				inputManager.pressKey(e.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				inputManager.releaseKey(e.button.button);
				break;
			case SDL_MOUSEMOTION:
				inputManager.setMouseCoords(
					static_cast<float>(e.motion.x), 
					static_cast<float>(e.motion.y)
				);
				break;
			default:
				break;
			}
		}
	}
	void Engine::MoveCamera(float x, float y)
	{
		mPimpl->camera.setPosition(
			mPimpl->camera.getPosition() +
			glm::vec2(x, y)
		);
	}
}
