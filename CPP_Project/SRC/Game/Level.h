#pragma once
#include <vector>
#include <string>
#include <SDL.h>
#include "../Engine/Logger.h"
#include "../Engine/AudioManager.h"
#include "Game.h"

class Level
{
public:
	Level(SDL_Texture* t, const std::string& mapData, size_t w, size_t h) : tileSetTexture(t), width(w), height(h)
	{
		Logger::Log("Level constructo called");
		ReadTileMapData(mapData);
		AudioManager::PlayMusic("Assets/Audio/SpaceGameLoopMusicLVL1.mp3");
	}
	~Level();

	void Update(float deltaTime);
	void Render(SDL_Renderer* renderer);

private:
	void ReadTileMapData(const std::string& filename);

	SDL_Texture* tileSetTexture;

	static const size_t tileHeight = 128;
	static const size_t tileWidth = 128;

	size_t width;
	size_t height;
	std::vector<uint8_t> tileMap; //assuming we do not have a tileset larger than 256 indices

	//camera stuff (could be in its own class)
	const float initCamPosY = height * tileHeight - DISPLAY_HEIGHT;
	float camPosY = initCamPosY;
	float camSpeed = 50.0f;

	//other data such as enemies or pickups initial pos/time or attack patterns could be stored here
};
