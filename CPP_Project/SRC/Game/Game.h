#pragma once
#include <SDL.h>
#include "../Engine/Entity.h"
#include "../Engine/AssetManager.h"
#include "../Engine/AudioManager.h"
#include "../Engine/Input.h"
#include <vector>
#include "Player.h"
#include "EnemiesManager.h"
#include "HUD.h"
#include "MainMenu.h"


#define VSYNC //FPS will be determined by the display refresh rate
//#define CAP_FPS //If defined, this will limit to the set frame rate below. Otherwise, only the VSYNC will be the limiting factor.

const uint32_t FPS = 30;//Only used if CAP_FPS
const uint32_t MILLISEC_PER_FRAME = 1000 / FPS;

const uint32_t DISPLAY_WIDTH = 1024;
const uint32_t DISPLAY_HEIGHT = 1024;

enum class Action { MoveLeft, MoveRight, MoveUp, MoveDown, Shoot };

class Level;

class Game
{
	public:
		Game();
		~Game();
		
		bool SetUpSDL();
		void Start();
		void Run();
		void Quit();

	private:
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		std::unique_ptr<AssetManager> assetManager;
		std::unique_ptr<Input> input;
		std::unique_ptr<Player> player;
		std::unique_ptr<EnemyManager> enemyManager;
		std::unique_ptr<HUD> hud;
		std::unique_ptr<AudioManager> audioManager;	
		std::unique_ptr<MainMenu> mainMenu;

		MainMenu::MenuOption menuChoice = MainMenu::MenuOption::Quit;
		bool restartFromMenu = false;

		bool isRunning;
		uint64_t millisecondPreviousFrame = 0;

		//Affichage des perfs
		uint64_t perfLastLogTime = 0;
		uint64_t perfFrameCount = 0;
		float perfTotalUpdateTime = 0.0f;
		float perfTotalRenderTime = 0.0f;
		float perfTotalFrameTime = 0.0f;

		//Level data
		std::unique_ptr<Level> level; //this could be a vector of levels

		void ProcessInput();
		void Update();
		void Render();

};
