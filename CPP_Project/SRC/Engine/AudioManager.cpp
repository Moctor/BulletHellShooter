#include "AudioManager.h"
#include "Logger.h"

Mix_Music* AudioManager::currentMusic = nullptr; //definie la variable currentmusic en static

bool AudioManager::SetUpAudio()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		Logger::Err("SDL_Mixer failed to start" + std::string(Mix_GetError()));
		return false;
	}
	return true;
}

void AudioManager::PlayMusic(const std::string& path, int loop)
{
	//libére la musique jouer avant si il y en avait une
	if (currentMusic)
	{
		Mix_FreeMusic(currentMusic);
		currentMusic = nullptr;
	}

	//charge une music
	currentMusic = Mix_LoadMUS(path.c_str());
	if (!currentMusic)
	{
		Logger::Err("Failed to load music: " + path + " | SDL_mixer Error: " + std::string(Mix_GetError()));
		return;
	}

	//music loop
	if (Mix_PlayMusic(currentMusic, loop) == -1)
	{
		Logger::Err("Failed to play music: " + std::string(Mix_GetError()));
	}
}

void AudioManager::StopMusic()
{
	Mix_HaltMusic();
}

void AudioManager::CleanUp()
{
	if (currentMusic)
	{
		Mix_FreeMusic(currentMusic);
		currentMusic = nullptr;
	}
	Mix_CloseAudio;
}