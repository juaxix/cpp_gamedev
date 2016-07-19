/// Engine.h
/// Header of the class xixEngine

#pragma once
#define GLM_FORCE_RADIANS
#include <glm/fwd.hpp>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <sdl/Sdl.h>
#include "Font.h"
#include "GlContext.h"
#include "Sdl.h"
#include "SdlWindow.h"
#include "SdlSurface.h"
#include "Updater.h"
#include "Camera2D.h"
#include "GLSLProgram.h"
#include "InputManager.h"
#include "SpriteBatch.h"
#include "Audio.h"
#include "ParticleSystem.h"

namespace xixEngine {
	class Updater;
	class Engine {
	public:
		///Used to store id's of the Gl binding textures
		typedef unsigned int Texture;
		///Constructor
		Engine(const char* windowTitle, int windowWidth, int windowHeight,float maxFrameTicks,float textScale);
		///Destructor
		~Engine();
		///Loads a texture from a path creating a SdlSurface and storing it 
		void loadTexture(Texture index, const std::string& texturePath);
		///Loads a texture from the given path to be used to render texts
		void loadFontTexture(const std::string& texturePath);
		///Load a sound file from the path to be cached and be able to be used later with playSoundEffect
		void loadSoundEffect(unsigned int soundID, const std::string& soundPath);
		///Play a sound effect if it is previously loaded 
		void playSoundEffect(unsigned int soundID,int loops=0);
		///Load Music file and store it in the cache 
		Music loadMusic(const std::string& musicPath);
		
		///Returns the time of the last frame
		float GetLastFrameSeconds() const;
		///Returns the total application running time
		float GetElapsedTimeSeconds() const;
		///Get the current mouse position in X
		float GetMouseX() const;
		///Get the current mouse position in Y
		float GetMouseY() const;
		///Get the current mouse position
		glm::vec2 GetMouseCoords() const;
		///Get the current mouse position in World Coords
		const glm::vec2 GetMouseWorldCoords() const;
		///Is the Left Mouse pressed down?
		bool GetLeftMouseButtonDown() const;
		///Is the Left Mouse Clicked?
		bool GetLeftMouseButtonPressed() const;
		///Is the Right Mouse pressed down?
		bool GetRightMouseButtonDown() const;
		///Is the Right Mouse Clicked?
		bool GetRightMouseButtonPressed() const;
		///Is the keyId pressed?
		bool GetKeyPressed(unsigned int keyID) const;
		///Is the keyId down?
		bool GetKeyDown(unsigned int keyID) const;
		///Start function to init the engine
		void Start(Updater& updater);
		///Method to quit
		void Quit();
		///Get the texture 
		//int GetTextureHeight(Texture texture) const;
		//int GetTextureWidth(Texture texture) const;

		///Render a given texture with a transformation
		void Render(Texture texture, const glm::mat4& transform);
		///Render a texture using a position and rotation
		void Render(Texture texture, float x, float y, float rotation = 0.0f);
        ///Render a texture from a surface (previously loaded) using a the engine spritebatch, with the given parms
        void RenderSurfaceBatch(GLTexture gTexture, Texture texture, float x, float y, float rotation = 0.0f);
        ///Render a sprite with the engine spritebatch
		void RenderSprite(Texture texture, const float x,const float y, const float sx, const float sy, const float angle =0.0f);
		///Begin of the spriteBatch process
		///Time given is passed to the glslProgram
		void spriteBatchingBegin(float time =0.f);
		///Ends of the spriteBatch process
		void spriteBatchingEnd();

		///Renders a text in the given coordinates with a rotation,color and scale. If no scale given or -1 it will use the default TextScale, if no color given it will use COLOR_WHITE
		void Write(const char* text, const glm::mat4& transform,const float textScale=-1.f, ColorRGBA8 color=COLOR_WHITE);

		///Renders a text in the given coordinates with a rotation,color and scale. If no scale given or -1 it will use the default TextScale, if no color given it will use COLOR_WHITE
		void Write(const char* text, float x, float y, float rotation = 0.0f, ColorRGBA8 color = COLOR_WHITE,float scale=-1.f);
		///Adds a ParticleBatch to the engine Particle System
		void AddParticleBatch(ParticleBatch* particleBatch);
		///Renders all the ParticleBatchs in the ParticleSystem
		void RenderParticleSystems();
		///Get the window With
		int GetWidth() const;
		///Get the window Height
		int GetHeight() const;
		///Move the camera x,y using the Camera2D and glslProgram
		void MoveCamera(float, float y);
	private:
		///Hidden EngineImplementation struct with all the classes needed 
		struct EngineImplementation;
		int _WindowWidth;
		int _WindowHeight;
		float _MaxFrameTicks;
		float m_TextScale;
		///Pointer to the engine implementation struct
		std::unique_ptr<EngineImplementation> mPimpl;
	};
}