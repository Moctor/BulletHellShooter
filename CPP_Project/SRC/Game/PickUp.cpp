#include "Pickup.h"
#include "Player.h"
#include "../Engine/Logger.h"
#include "../Engine/CollisionSystem.h"


Pickup::Pickup(glm::vec2 p, glm::vec2 v, SDL_Texture* tex, PickupType pType)
    : Entity(p, v, tex), pickupType(pType), isActive(true)
{
    dim = glm::vec2(32, 32); // redefinit la taille
}

void Pickup::Render(SDL_Renderer* renderer)
{
    if (!isActive || !texture) // si il est pas actif ou qu'il a pas de texture ne le render pas
    {
        return;
    }
    //source rectangle for blit
    SDL_Rect src = { 0, 0, 32, 32 };

    //dest rectangle
    SDL_FRect dest = { pos.x, pos.y, dim.x, dim.y };


    //blit
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);//SDL_BLENDMODE_BLEND = alphablend
    SDL_RenderCopyF(renderer, texture, &src, &dest);
}

void Pickup::Update(float deltatime)
{
    if (!isActive) // stop si il est pas actif
    {
        return;
    }

    // fais descendre le pick up vers le bas pour donner l'effet qu'il descend avec le scrolling de la map
    pos += vel * deltatime;

    if (pos.y > 1024) // si il sort de l'écran on le reset
    {
        Reset();
    }

    // Vérifie la collision avec le joueur
    if (CollisionSystem::IsColliding(this->CollisionRect(), player->CollisionRect()))
    {
        PickUpEffect(*player);
    }

}

SDL_Rect Pickup::CollisionRect() const//renvoi un SDLRect pour la collision avec le player
{
    return SDL_Rect{
        static_cast<int>(pos.x),
        static_cast<int>(pos.y),
        static_cast<int>(dim.x),
        static_cast<int>(dim.y)
    };
}

void Pickup::Reset()
{
    isActive = false;
    toBeDestroyed = true;
}

void Pickup::Setup(glm::vec2 p, glm::vec2 v, SDL_Texture* tex, PickupType pType)
{
    pos = p;
    vel = v;
    texture = tex;
    pickupType = pType;
    isActive = true;
    toBeDestroyed = false;
    dim = glm::vec2(32, 32);
}

void Pickup::PickUpEffect(Player& player)
{
    switch (pickupType)
    {
    case PickupType::Heal:
        player.Heal(30);
        break;

    case PickupType::PowerUp:
        player.SetShootPlayer(ShootPlayer::Burst);
        break;

    default:
        break;
    }

    // Désactive le pickup après usage
    Reset();
}

