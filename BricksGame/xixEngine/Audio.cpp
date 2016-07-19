/// Audio.cpp
/// Implementation of the classes: Audio, SoundEffect and Music

#include "Audio.h"

namespace xixEngine {

	Audio::Audio()
	{
	}


	Audio::~Audio()
	{
		//assert the audio is closed (can be called twice thanks to the m_Initialized flag)
		destroy();
	}

	void SoundEffect::play(int loops /*=0*/) 
	{
		if (Mix_PlayChannel(
			-1 /*pick the first open channel */
			, m_chunk
			, loops
			)==-1)
		{
			//Try another channel
			if (Mix_PlayChannel(0, m_chunk, loops) == -1)
			{
				/*throw std::runtime_error("Mix_PlayChannel error playing the sound " + std::string(Mix_GetError()));*/
				
			}
		}
	}

	void Music::play(int loops /* =-1 */)
	{
		if (Mix_PlayMusic(m_music, loops)==-1) {
			throw std::runtime_error("Mix_PlayMusic error playing " + std::string(Mix_GetError()));
		}
	}

	//Static functions for music
	void Music::pause()
	{
		Mix_PauseMusic();
	}

	void Music::stop()
	{
		Mix_HaltMusic();
	}

	void Music::resume()
	{
		Mix_ResumeMusic();
	}

	//Audio methods
	void Audio::init() 
	{
		if (m_Initialized) return;
		//Parameters can be a bitwise combination of MIX_INIT_FAC ,MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG (wav by default)
		if (Mix_Init(MIX_INIT_MP3|MIX_INIT_OGG)==0) 
		{
			throw std::runtime_error("Could not initialize the audio engine");
		}
		Mix_OpenAudio(
			MIX_DEFAULT_FREQUENCY*2 
			/* 22050 * 2 = best quality*/
			, MIX_DEFAULT_FORMAT
			,2 //2 channels
			,1024 //default chunk
		);
		m_Initialized = true;
	}

	void Audio::destroy()
	{
		if (!m_Initialized) return;
		m_Initialized = false;
		for (auto it = m_soundsMap.begin(); it != m_soundsMap.end(); it++) 
		{
			Mix_FreeChunk(it->second.m_chunk);
		}
		m_soundsMap.clear();
		//New C++11 iteration mode (foreach)
		for (auto &it : m_musicMap) { //as long as there is a begin and end you can iterate
			Mix_FreeMusic(it.second);
		}
		m_musicMap.clear();
		Mix_CloseAudio();
		Mix_Quit();
	}

	//Sound effect load function
	SoundEffect Audio::loadSoundEffect(unsigned int index,const std::string& soundPath)
	{
		if (!m_Initialized) this->init();
		SoundEffect soundEffect;
		//Exists?
		auto it = m_soundsMap.find(index);
		if (it == m_soundsMap.end())
		{
			//not found: load file with loadwav (load all file extensions)
			Mix_Chunk* chunk = Mix_LoadWAV(soundPath.c_str());
			//Check errors:
			if (chunk == nullptr)
			{
				throw std::runtime_error("Sound not found: " + soundPath + " " + std::string(Mix_GetError()));
			}
			//Create the SoundEffect
			SoundEffect soundEffect;
			soundEffect.m_chunk = chunk;
			m_soundsMap[index] = soundEffect; //creates the new node in the map
			return soundEffect;
		} else {
			//found
			//soundEffect.m_chunk = it->second.m_chunk;
			return it->second;
		}
		
	}
	void Audio::playSoundEffect(unsigned int soundID, int loops)
	{
		//search it
		auto it = m_soundsMap.find(soundID);
		if (it == m_soundsMap.end())
		{
			return;
		} else {
			it->second.play(loops);
		}
	}

	//Music load function
	Music Audio::loadMusic(const std::string& musicPath)
	{
		if (!m_Initialized) this->init();
		Music music;
		//Exists?
		auto it = m_musicMap.find(musicPath);
		if (it == m_musicMap.end())
		{
			//not found: load the music
			Mix_Music* mixmus = Mix_LoadMUS(musicPath.c_str());
			//Check errors:
			if (mixmus == nullptr)
			{
				throw std::runtime_error("Sound not found: " + musicPath);
			}
			m_musicMap[musicPath] = mixmus; //creates the new node in the map
											//we can access the private because we are friends
			music.m_music = mixmus;
			m_musicMap[musicPath] = mixmus;
		}
		else {
			//found
			music.m_music = it->second;
		}
		return music;
	}
}