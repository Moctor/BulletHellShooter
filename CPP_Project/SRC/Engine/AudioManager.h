#pragma once
#include <SDL_mixer.h>
#include <string>


class AudioManager
{
	public:
		
		static bool SetUpAudio();

		
		static void PlayMusic(const std::string& path, int loop = -1); //loop -1 permet de boucler la musique/le son

		static void StopMusic();

		static void CleanUp();


	private:

		static Mix_Music* currentMusic; //pointeur vers la musique joué

};
