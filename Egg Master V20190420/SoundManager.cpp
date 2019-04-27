// SoundManager.cpp
//
// Zach Elko
// 2010
//
// A simple sound manager for SDL.
//
#include "SoundManager.h"
#include <string>

using namespace soundmanager;

// Initialize our static variables
SoundManager* SoundManager::instance = 0;
SoundManager::AudioState SoundManager::currentState = ERROR;
std::string SoundManager::m_sCurrentMusicFilename = "";

void SoundManager::playMusic(const std::string& fileName) {
	if (currentState != ERROR) {
		// If no music is playing, play it
		if (Mix_PlayingMusic() == 0)
		{
			// Load music

			Mix_Music* music = Mix_LoadMUS(fileName.c_str());
			//Play music
			Mix_PlayMusic(music, -1);
			currentState = PLAYING;
			m_sCurrentMusicFilename = fileName.c_str();
		}

	}
	else {
		// If music is playing, pause it
		this->pauseMusic();
	}
}

void SoundManager::pauseMusic()
{
   if (currentstate != error)
   {
      // if music is playing, handle the pause request
      if (Mix_PlayingMusic() == 1)
      {
         if (Mix_PausedMusic() == 1)
         {
            // If we receive a pause request and the music is already paused, resume it.
            Mix_ResumeMusic();
            currentState = PLAYING;
         }
         else
         {
            // Otherwise, pause the music
            Mix_PauseMusic();
            currentState = PAUSED;
         }
      }
   }
}

void SoundManager::stopMusic()
{
   if (currentState != ERROR)
   {
      Mix_HaltMusic();
      currentState = STOPPED;
    
      m_sCurrentMusicFilename = "";
   }
}

void SoundManager::loadFX(const std::string& fileName) const
{
   if (currentState != ERROR) {
      // TODO: Alter this to work for mp3 as well      
		  fx = Mix_LoadWAV(fileName.c_str());     
   }
}

void SoundManager::playFX() {
	Mix_PlayChannel(-1, fx, 0);
}

SoundManager::SoundManager()
{
}


SoundManager::~SoundManager()
{
}
