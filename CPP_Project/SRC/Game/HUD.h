#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Player; // Forward declaration

class HUD
{
public:
    HUD(SDL_Renderer* renderer, Player* playerRef);
    ~HUD();

    void Render(SDL_Renderer* renderer);

    void SetScore(int newScore);

private:
    SDL_Renderer* renderer;
    Player* player;

    TTF_Font* font;
    SDL_Color color = { 255, 255, 255, 255 };

    int score = 0;

    SDL_Texture* CreateTextTexture(const std::string& text, SDL_Rect& outRect);
};