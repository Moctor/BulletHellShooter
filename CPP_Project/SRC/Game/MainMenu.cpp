#include "MainMenu.h"
#include "../Engine/CollisionSystem.h"
#include "../Engine/Logger.h"
#include <SDL_ttf.h>
#include <string>
#include <iostream>


MainMenu::MainMenu(SDL_Renderer* renderer, SDL_Window* window)
    : renderer(renderer), window(window)
{
    if (TTF_Init() == -1)
    {
        Logger::Err("TTF_Init: ");
    }

    font = TTF_OpenFont("./Assets/Fonts/arial.ttf", 24);
    if (!font)
    {
        Logger::Err("Failed to load font: ");
    }

    isRunning = true;
    launchGame = false;
}

bool MainMenu::Run()
{
    SDL_Event input;

    while (isRunning)
    {
        while (SDL_PollEvent(&input))
        {
            if (input.type == SDL_QUIT)
            {
                isRunning = false;
            }

            HandleInput(input);
        }

        Render();
    }

    return launchGame;
}

void MainMenu::HandleInput(SDL_Event& e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
    {
        SDL_Point mousePos = { e.button.x, e.button.y };

        if (CollisionSystem::IsPointInsideRect(mousePos, playButtonRect))
        {
            selectedOption = MenuOption::Play;
            launchGame = true;
            isRunning = false;
        }
        else if (CollisionSystem::IsPointInsideRect(mousePos, quitButtonRect))
        {
            selectedOption = MenuOption::Quit;          
            launchGame = false;
            isRunning = false;
        }
    }
}


void MainMenu::Render()
{
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    int mx, my;
    SDL_GetMouseState(&mx, &my);
    SDL_Point mousePoint = { mx, my };

    bool hoveringPlay = CollisionSystem::IsPointInsideRect(mousePoint, playButtonRect);
    bool hoveringQuit = CollisionSystem::IsPointInsideRect(mousePoint, quitButtonRect);

    DrawButton(playButtonRect, "Jouer", hoveringPlay);
    DrawButton(quitButtonRect, "Quitter", hoveringQuit);

    SDL_RenderPresent(renderer);
}


void MainMenu::DrawButton(const SDL_Rect& rect, const std::string& text, bool isHovered)
{
    // Fond bouton
    SDL_SetRenderDrawColor(renderer, isHovered ? 100 : 60, isHovered ? 100 : 60, isHovered ? 255 : 100, 255);
    SDL_RenderFillRect(renderer, &rect);

    if (!font) return; // si pas de police, on quitte

    SDL_Color color = { 255, 255, 255, 255 }; // blanc
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface)
    {
        std::cerr << "TTF_RenderText_Blended error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        std::cerr << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    // Calcule la position pour centrer le texte dans le bouton
    SDL_Rect textRect;
    textRect.w = surface->w;
    textRect.h = surface->h;
    textRect.x = rect.x + (rect.w - textRect.w) / 2;
    textRect.y = rect.y + (rect.h - textRect.h) / 2;

    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_DestroyTexture(texture);
}

