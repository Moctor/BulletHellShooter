#pragma once
#include <SDL.h>
#include <string>
#include <SDL_ttf.h>

class MainMenu
{
	public:

		MainMenu(SDL_Renderer* renderer, SDL_Window* window);
		~MainMenu() = default;

		bool Run();

		enum class MenuOption : uint8_t
		{
			Play = 0,
			Quit = 1
		};

	private:

		TTF_Font* font = nullptr;

		SDL_Renderer* renderer;
		SDL_Window* window;

		MenuOption selectedOption = MenuOption::Play; //option de base
		bool isRunning = true;
		bool launchGame = false; 

		void HandleInput(SDL_Event& input);
		void Render();

		// collision button avec pos et dim
		SDL_Rect playButtonRect{ 412, 300, 200, 60 };
		SDL_Rect quitButtonRect{ 412, 400, 200, 60 };
		
		void DrawButton(const SDL_Rect& rect, const std::string& text, bool isHovered);

};