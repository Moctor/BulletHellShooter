#include "HUD.h"
#include "Player.h"
#include <SDL_ttf.h>
#include <sstream>

HUD::HUD(SDL_Renderer* renderer, Player* playerRef)
    : renderer(renderer), player(playerRef)
{
    TTF_Init();
    font = TTF_OpenFont("Assets/Fonts/arial.ttf", 24);
    if (!font)
    {
        SDL_Log("Failed to load font: %s", TTF_GetError());
    }
}

HUD::~HUD()
{
    TTF_CloseFont(font);
    TTF_Quit();
}

void HUD::SetScore(int newScore)
{
    score = newScore;
}

SDL_Texture* HUD::CreateTextTexture(const std::string& text, SDL_Rect& outRect)
{
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    outRect = { 0, 0, surface->w, surface->h };
    SDL_FreeSurface(surface);
    return texture;
}

void HUD::Render(SDL_Renderer* renderer)
{
    if (!player || !font) return;

    // life (haut a gauche)
    const int barWidth = 200;   
    const int barHeight = 15;   
    const int x = 10;           
    const int y = 10;            

    // donne la largeur proportionnel a notre vie
    float healthPercent = player->life / 100.0f;  // /100car la vie max 100
    int healthWidth = static_cast<int>(barWidth * healthPercent);

    // render le fond
    SDL_Rect bgRect = { x, y, barWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    // render la vie & color
    SDL_Rect healthRect = { x, y, healthWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255);//vert
    SDL_RenderFillRect(renderer, &healthRect);

    // fond color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &bgRect);


    // score (haut a droite)
    std::stringstream ssScore;
    ssScore << "Score : " << score;

    SDL_Rect rectScore;
    SDL_Texture* scoreTexture = CreateTextTexture(ssScore.str(), rectScore);

    int windowWidth = 1024; 
    rectScore.x = windowWidth - rectScore.w - 10; // decallage a droite
    rectScore.y = 10;  // decallage en haut

    SDL_RenderCopy(renderer, scoreTexture, nullptr, &rectScore);
    SDL_DestroyTexture(scoreTexture);
}