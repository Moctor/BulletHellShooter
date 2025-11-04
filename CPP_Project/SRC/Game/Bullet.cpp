#include "Bullet.h"
#include "Game.h"
#include "Enemies.h"
#include "Player.h"
#include "../Engine/CollisionSystem.h"


bool Bullet::Update(float deltatime)
{
	const float rightBorder = DISPLAY_WIDTH - dim.x;
	const float bottomBorder = DISPLAY_HEIGHT - dim.y;

	state.param.pos += state.param.vel * deltatime;

	if (state.param.pos.x >= rightBorder)
	{
		return true;
		// ajouter collision
	}
	else if (state.param.pos.x <= 0)
	{
		return true;
	}

	if (state.param.pos.y >= bottomBorder)
	{
		return true;
	}
	else if (state.param.pos.y <= 0)
	{
		return true;
	}
	return false;
}


void Bullet::Render(SDL_Renderer* renderer)
{
	// source rectangle for blit
	static const SDL_Rect src = { 0,0,8,8 }; // on le met en const car on va pas le modifer, static pour le faire créer qu'une fois et l'itérer sur l'ensemble des bullets créer

	//dest rectangle
	SDL_FRect dest = { state.param.pos.x,state.param.pos.y,dim.x,dim.y };

	//blit
	//ajouter un if qui change le settexture pour la couleur en fonction de l'owner si cest un player ou ennemies
	if (owner == BulletOwner::Player)
	{
		SDL_SetTextureColorMod(texture, playerBulletColor.r, playerBulletColor.g, playerBulletColor.b);
	}
	else 
	{
		SDL_SetTextureColorMod(texture, enemiesBulletColor.r, enemiesBulletColor.g, enemiesBulletColor.b);
	}
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopyF(renderer, texture, &src, &dest);
}


SDL_Rect Bullet::CollisionRect() const //Fonction qui me renvoi un SDLRect pour les bullets
{
	return SDL_Rect
	{
		static_cast<int>(state.param.pos.x),
		static_cast<int>(state.param.pos.y),
		static_cast<int>(dim.x),
		static_cast<int>(dim.y)
	};
}