#include "Game.h"
#include <iostream>
#include "../Engine/Logger.h"
#include <SDL.h>
#include "Player.h"
#include "../Engine/Utils.hpp"
#include <Windows.h>
#include <glm.hpp>
#include "Level.h"
#include "Enemies.h"

Game::Game()
{
	Logger::Log("Game constructor called");
	isRunning = false;
}

Game::~Game()
{
	Logger::Log("Game destructor called");
}

bool Game::SetUpSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Logger::Err("Error initializing SDL!");
		return false;
	}

	window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		Logger::Err("Error creating SDL Window!");
		return false;

	}

#ifdef VSYNC
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
#else
	renderer = SDL_CreateRenderer(window, -1, 0);
#endif // VSYNC

	if (!renderer)
	{
		Logger::Err("Error creating SDL Renderer!");
		return false;

	}

	return true;
}

void Game::Start()
{

	


	//Engine setup
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	input = std::make_unique<Input>();
	assetManager = std::make_unique<AssetManager>(renderer);

	//Game setup
	//Keyboard bindings
	input->BindKey(Action::MoveLeft, SDL_SCANCODE_LEFT);
	input->BindKey(Action::MoveRight, SDL_SCANCODE_RIGHT);
	input->BindKey(Action::MoveUp, SDL_SCANCODE_UP);
	input->BindKey(Action::MoveDown, SDL_SCANCODE_DOWN);
	input->BindKey(Action::Shoot, SDL_SCANCODE_SPACE);

	//Gamepad mapping (assuming Xbox-style controller)
	input->BindButton(Action::MoveLeft, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	input->BindButton(Action::MoveRight, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	input->BindButton(Action::MoveUp, SDL_CONTROLLER_BUTTON_DPAD_UP);
	input->BindButton(Action::MoveDown, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	input->BindButton(Action::Shoot, SDL_CONTROLLER_BUTTON_A);

	assetManager->AddTexture("Ship", "./Assets/Images/Ship.tga");
	assetManager->AddTexture("Bullet", "./Assets/Images/Bullet.tga");
	assetManager->AddTexture("Tileset", "./Assets/Images/Tileset.tga");
	assetManager->AddTexture("Enemi1", "./Assets/Images/Enemi1.tga");
	assetManager->AddTexture("Enemi2", "./Assets/Images/Enemi2.tga");
	assetManager->AddTexture("HealthPickUp", "./Assets/Images/HealthPickUp.tga");
	assetManager->AddTexture("PowerPickUp", "./Assets/Images/PowerPickUp.tga");

	audioManager = std::make_unique<AudioManager>();
	//check du systeme audio
	if (!audioManager->SetUpAudio())
	{
		Logger::Err("Audio system failed to start.");
		return;
	}

	//create level
	level = std::make_unique<Level>(assetManager->GetTexture("Tileset"), "./Assets/Maps/Level01.csv", 8, 64);

	//create player
	player = std::make_unique<Player>(	glm::vec2(512 - 32, 900),
										assetManager->GetTexture("Ship"),
										assetManager->GetTexture("Bullet"),
										input);
	player->dim = glm::vec2(64, 64);
	player->speed = 400;
	player->animSpeed = 20;
	player->SetShootPlayer(ShootPlayer::Single);

	//create hud
	hud = std::make_unique<HUD>(renderer, player.get());

	//create enemi manager
	enemyManager = std::make_unique<EnemyManager>(
		assetManager->GetTexture("Enemi1"),   
		assetManager->GetTexture("Enemi2"),   
		assetManager->GetTexture("Bullet"),
		assetManager->GetTexture("HealthPickUp"),
		assetManager->GetTexture("PowerPickUp"),
		player.get(),                         
		10);                               

	isRunning = true;
}

void Game::ProcessInput()
{
	input->ProccesInput();

	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
			case SDL_QUIT: isRunning = false; break;
			case SDL_KEYDOWN: if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) isRunning = false; break;
		}
	}
}

void Game::Update()
{
#ifdef CAP_FPS
	uint64_t timeToWait = MILLISEC_PER_FRAME - (SDL_GetTicks64() - millisecondPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISEC_PER_FRAME)
		SDL_Delay(timeToWait); //yield to other processes
#endif // CAP_FPS

	uint64_t frameStart = SDL_GetTicks64();
	float deltaTime = (SDL_GetTicks64() - millisecondPreviousFrame) / 1000.0f;
	millisecondPreviousFrame = SDL_GetTicks64();

	// check si le joueur est mort si cest le cas arrete le jeu
	if (!player->IsAlive())
	{
		restartFromMenu = true;
		isRunning = false;
		return;
	}

	//perform update
	uint64_t updateStart = SDL_GetTicks64();
	level->Update(deltaTime);
	player->Update(deltaTime);
	enemyManager->Update(deltaTime);
	hud->SetScore(player->GetScore());

	//lie le player aux enemies
	auto activeEnemies = enemyManager->GetActiveEnemies();
	player->SetEnemies(activeEnemies);

	uint64_t updateEnd = SDL_GetTicks64();

	// Stockage pour perf
	float updateTime = (updateEnd - updateStart);
	perfTotalUpdateTime += updateTime;
}

void Game::Render()
{

	uint64_t renderStart = SDL_GetTicks64();

	//clear draw buffer
	SDL_SetRenderDrawColor(renderer, 80, 60, 40, 255);
	SDL_RenderClear(renderer);

	//perform rendering
	level->Render(renderer);
	player->Render(renderer);
	enemyManager->Render(renderer);
	hud->Render(renderer);

	//wait for VSync (or not) and swap buffers -> draw becomes display and vice-versa
	SDL_RenderPresent(renderer);

	uint64_t renderEnd = SDL_GetTicks64();
	float renderTime = (renderEnd - renderStart);

	// Calcul total frame time
	float totalFrameTime = (renderEnd - millisecondPreviousFrame);

	// Stockage pour stats
	perfTotalRenderTime += renderTime;
	perfTotalFrameTime += totalFrameTime;
	perfFrameCount++;

	// Affichage une fois par seconde
	if (SDL_GetTicks64() - perfLastLogTime >= 1000)
	{
		float avgUpdate = perfTotalUpdateTime / perfFrameCount;
		float avgRender = perfTotalRenderTime / perfFrameCount;
		float avgFrame = perfTotalFrameTime / perfFrameCount;
		float fps = 1000.0f / avgFrame;

		std::cout << "\rFPS: " << fps
			<< " | Update: " << avgUpdate << " ms"
			<< " | Render: " << avgRender << " ms"
			<< " | Frame: " << avgFrame << " ms"
			<< std::flush;

		// Reset
		perfLastLogTime = SDL_GetTicks64();
		perfFrameCount = 0;
		perfTotalUpdateTime = 0.0f;
		perfTotalRenderTime = 0.0f;
		perfTotalFrameTime = 0.0f;
	}
}

void Game::Run()
{

	if (!SetUpSDL())
	{
		Logger::Err("Failed to initialize SDL and window/renderer.");
		return;  // Arrête si échec
	}
	
	bool restartFromMenu = true;

	while (restartFromMenu)
	{
		restartFromMenu = false;

		MainMenu menu(renderer, window);
		bool play = menu.Run();

		if (!play)
		{
			Quit();
			return;
		}

		Start();  // charge les assets, initialise audio, etc.

		if (isRunning)
		{
			while (isRunning)
			{
				ProcessInput();
				Update();
				Render();
			}
		}
	}

	Quit();
}
void Game::Quit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	audioManager->StopMusic();
	SDL_Quit();
	Logger::Log("SDL closed !");
}
