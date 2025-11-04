#include "Level.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm.hpp>

Level::~Level()
{
	Logger::Log("Level destructor called");
}

void Level::ReadTileMapData(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) Logger::Err("Error: Could not open map file!");

	tileMap.reserve(width * height);
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream lineStream(line);
		std::string cell;

		while (std::getline(lineStream, cell, ',')) //get substring (cell) delimited by ','
		{
			uint8_t value = static_cast<uint8_t>(std::stoi(cell)); //convert to int and cast to byte
			tileMap.emplace_back(value);
		}
	}
	file.close();
}

void Level::Update(float deltaTime)
{
	camPosY -= camSpeed * deltaTime;
	if (camPosY < tileHeight) camPosY = initCamPosY;
}

void Level::Render(SDL_Renderer* renderer)
{
	size_t tileOffset = (size_t)camPosY / tileHeight;
	for (size_t y = 0; y < 9; y++)
		for (size_t x = 0; x < 8; x++)
		{
			//source rectangle for the blit
			uint8_t tileSetIndex = tileMap[x + (y + tileOffset) * width];
			glm::ivec2 srcPos = { (tileSetIndex % 16) * tileWidth , (tileSetIndex / 16) * tileHeight };
			SDL_Rect src = { srcPos.x, srcPos.y, tileWidth, tileHeight };

			//dest rectangle for the blit
			float integral;
			float frac = modf(camPosY, &integral);
			float pixelOffset = frac + (int)integral % tileHeight;
			SDL_FRect dst = { x * tileWidth, y * tileHeight - pixelOffset, tileWidth + .5, tileHeight + .5 };

			//blit
			SDL_RenderCopyF(renderer, tileSetTexture, &src, &dst);
		}
}
