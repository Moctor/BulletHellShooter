#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>



class AssetManager
{
	public:
		AssetManager(SDL_Renderer* m_renderer);
		~AssetManager();

		void ClearAsset();
		void AddTexture(std::string_view name, std::string_view fileName);
		SDL_Texture* GetTexture(std::string_view name) const;


	private:
		SDL_Renderer* m_renderer;
		std::unordered_map<std::string_view, SDL_Texture*> textures;
		//add map exemple audio, fonts,...


};
