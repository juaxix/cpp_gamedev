/// Audio.h
/// Header of the classes: Audio, SoundEffect and Music
#pragma once
#include <SDL/SDL_mixer.h>
#include <string>
#include <iostream>
#include <map>
namespace xixEngine 
{
	///<summary>SoundEffect stores information about a Mix_Chunk, is friend with Audio</summary>
	class SoundEffect {
	public :
		friend class Audio;
		
		///<summary>plays the sounds the number of loops
		///param int loops</summary>
		///<param>loops</param> 
		///0 = one, 1 = two...,and -1 = loop
		void play(int loops=0);
	private :
		///<summary>Pointer of the chunk for the sound once loaded</summary>
		Mix_Chunk* m_chunk = nullptr;
	};
	///<summary>SoundEffect stores information about a Mix_Music, is friend with Audio. We need to make pause, stop and resume functions static because SDL_Mix will apply it globally (to the current music playing)</summary>
	class Music {
	public:
		friend class Audio;
		///<summary>Plays the music the number of loops
		///<param>loops</param> 1 = one ,-1 = loop
		void play(int loops=-1);
		///<summary>Pause all the music</summary>
		static void pause();
		///<summary>Stop all the music</summary>
		static void stop();
		///<summary>Resume all the music</summary>
		static void resume();
	private:
		///<summary>Pointer to the music buffer</summary>
		Mix_Music* m_music = nullptr;
	};
	///<summary>Audio class initializes once, can load sound effects and music</summary>
	class Audio
	{
	public:
		///Constructor
		Audio();
		///Destructor
		~Audio();
		///Initialize audio
		void init();
		///Free memory
		void destroy();
		///<summary>Load a sound effect from a file and save it in the cache of its class SoundEffect</summary>
		///<param>index</param> unsigned int used to index the file in m_soundsMap
		///<param>soundPath</param> relative path to the file
		///<return>SoundEffect</return>
		SoundEffect loadSoundEffect(unsigned int index, const std::string& soundPath);
		void playSoundEffect(unsigned int soundID, int loops);
		///<summary>Load a music file and save the buffer in the cache of its class Music</summary>
		///<param>index</param> unsigned int used to index the file in m_musicMap
		///<param>soundPath</param> relative path to the file
		///<return>Music</return>
		Music loadMusic(const std::string& musicPath);
	private:
		///<param>Flag to know if the class is initialized</param>
		bool m_Initialized = false;
		///<param>Cache the files with datafile pointers for sounds</param>
		std::map<unsigned int, SoundEffect> m_soundsMap;
		///<param>Cache the files with datafile pointers for musics</param>
		std::map<std::string, Mix_Music*> m_musicMap;
	};


}
