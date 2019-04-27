// SoundManager.h
//
// Zach Elko
// 2010
//
// A simple sound manager for SDL.
//
#ifndef _SOUNDMANAGER_H
#define	_SOUNDMANAGER_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <iostream>

namespace soundmanager {

   class SoundManager {
   public:
	
      // Singleton pattern
	   Mix_Chunk* fx = nullptr;
      static SoundManager* getInstance() {
         if (instance == 0) {
            instance = new SoundManager;
            SoundManager::initAudioDevice();
         }
         return instance;
      }

      void playMusic(const std::string& fileName);
      void pauseMusic();
      void stopMusic();
      void loadFX(const std::string& fileName) const;
	  void playFX();

      bool isPaused() const
      {
         return currentState == PAUSED;
      }

      bool isStopped() const
      {
         return currentState == STOPPED;
      }

      bool isPlaying() const
      {
         return currentState == PLAYING;
      }

      bool inErrorState() const
      {
         return currentState == ERROR;
      }

   private:
      static std::string m_sCurrentMusicFilename;

      static SoundManager* instance;

      enum AudioState
      {
         ERROR = 0,
         WAITING,
         PAUSED,
         STOPPED,
         PLAYING
      };
      static AudioState currentState;

      static void initAudioDevice()
      {
         if (SDL_Init(SDL_INIT_AUDIO) != -1)
         {
            if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
            {
               std::cerr << "Error initializing audio device...\n";
               currentState = ERROR;
            }
            else
            {
               currentState = WAITING;
            }
         }
         else
         {
            std::cerr << "Error initializing SDL audio subsystem...\n";
            currentState = ERROR;
         }
      }

      // All of these are private due to the Singleton pattern

      SoundManager()
      {
      }

      SoundManager(const SoundManager&)
      {
      }

      ~SoundManager()
      {
         Mix_CloseAudio();
      }

      SoundManager & operator=(const SoundManager&)
      {
      }

   };

}

#endif

