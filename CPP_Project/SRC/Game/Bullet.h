#pragma once
#include <SDL.h>
#include <glm.hpp>


//Liste des types d'entité qui existe
enum class BulletOwner {Player, Enemy};

struct BulletParams
{
	glm::vec2 pos = { 0,0 };
	glm::vec2 vel = { 0,0 };
};

union BulletState
{
	BulletParams param;
	class Bullet* next = nullptr; //on met class car il compile la class bullet apres le bullet state ce qui est donc une forward declaration
};

class Bullet //simple class
{
public:

	bool Update(float deltatime); //true when destroyed
	void Render(SDL_Renderer* renderer);

	SDL_Rect CollisionRect() const; //permet de récupérer le rectangle de collision des bullets

	BulletState state;
	bool isActive = false;
	BulletOwner owner;
	inline static SDL_Texture* texture = nullptr; // on met nullptr pour iniatiliser le pointeur

private:
	inline static const glm::vec2 dim = { 8,8 }; //Partager par toutes les instances de bullets
	inline static const SDL_Color playerBulletColor = { 30,60,200 };
	inline static const SDL_Color enemiesBulletColor = { 255,255,255 };

};