#pragma once
#include "../Engine/Entity.h"
#include "Player.h"
#include <SDL.h>
#include <glm.hpp>


enum class PickupType 
{
    Heal,
    PowerUp
};

class Pickup : public Entity
{
    public:

        Pickup(glm::vec2 pos, glm::vec2 vel, SDL_Texture* tex, PickupType type);
        ~Pickup() = default;

        void Render(SDL_Renderer* renderer);
        void Update(float deltatime);

        SDL_Rect CollisionRect() const; //permet de récupérer le rectangle de collision des ennemies

        void Reset();
        bool IsActive() const 
        { 
            return isActive;
        }

        void Setup(glm::vec2 p, glm::vec2 v, SDL_Texture* tex, PickupType pType);
        void PickUpEffect(Player& player);
        void SetPlayer(Player* p) 
        { 
            player = p; 
        }



    private:
        // taille du pickup
        bool isActive = false;
        PickupType pickupType;
        Player* player = nullptr;

};
