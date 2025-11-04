#include "AssetManager.h"
#include "Logger.h"
#include <SDL_image.h>


AssetManager::AssetManager(SDL_Renderer* renderer)
{
	this->m_renderer = renderer;
	Logger::Log("Asset Manager constructor called");
}

AssetManager::~AssetManager()
{
	ClearAsset();
	Logger::Log("Asset Manager destructor called");
}


void AssetManager::ClearAsset()
{
	for (auto [name, texture] : textures) //key value
	{
		SDL_DestroyTexture(texture);
	}
	textures.clear();
}

void AssetManager::AddTexture(std::string_view name, std::string_view fileName)
{
	SDL_Texture* texture = IMG_LoadTexture(m_renderer, fileName.data()); //get the raw const char*
	if (texture)
	{
		textures.emplace(name, texture);
		Logger::Log("New texture added to the assetManager " + static_cast<std::string > (name));
	}
	else
	{
		Logger::Err("Cannot find " + static_cast<std::string > (name));
	}
}

SDL_Texture* AssetManager::GetTexture(std::string_view name) const
{
	return textures.at(name);
}
