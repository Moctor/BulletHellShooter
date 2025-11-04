#include "Game.h"
#include "Enemies.h"
#include "Player.h"
#include "../Engine/CollisionSystem.h"
#include "../Engine/Utils.hpp"
#include <iostream>
#include "../Engine/Logger.h"



void Enemies::Shoot(vec2 pos, vec2 vel) 
{
	switch (currentShootPattern)
	{
		case ShootPattern::Burst:
		{
			int bulletCount = 4;
			float spreadAngle = 90; // total spread
			float angleStart = 45; 
			float angleStep = spreadAngle / (bulletCount - 1);
			float speed = 250.f;
			for (int i = 0; i < bulletCount; i++)
			{
				if (head) //creation de la bullet avec ses parametres
				{
					float angle = angleStart + i * angleStep;
					glm::vec2 direction = Utils::AngleToVec(angle);
					glm::vec2 finalVel = direction * speed;

					Bullet* bullet = head;
					head = bullet->state.next;

					bullet->state.param.pos = pos;
					bullet->state.param.vel = finalVel;
					bullet->owner = BulletOwner::Enemy;
					bullet->isActive = true;
				}
			}
		}break;

		case ShootPattern::Spread:
		{
			int bulletCount = 8;
			float spreadAngle = 360; // total spread
			float angleStart = 1;
			float angleStep = spreadAngle / (bulletCount - 1);
			float speed = 250.0f;
			for (int i = 0; i < bulletCount; i++)
			{
				if (head) //creation de la bullet avec ses parametres
				{
					float angle = angleStart + i * angleStep;
					glm::vec2 direction = Utils::AngleToVec(angle);
					glm::vec2 finalVel = direction * speed;

					Bullet* bullet = head;
					head = bullet->state.next;

					bullet->state.param.pos = pos;
					bullet->state.param.vel = finalVel;
					bullet->owner = BulletOwner::Enemy;
					bullet->isActive = true;
				}
			}
		}break;
	}

}

void Enemies::Reset(vec2 p, vec2 v, SDL_Texture* tex, SDL_Texture* btex, ShootPattern pattern)
{
	pos = p;
	vel = v;
	texture = tex;
	bulletTexture = btex;
	life = 50;
	isActive = true;
	fireRate = 0.0f;
	currentShootPattern = pattern;
	dim = vec2(64, 64);
	scoreAdded = false;

	head = &bullets[0];
	for (int i = 0; i < poolSize - 1; i++)
	{
		bullets[i].isActive = false;
		bullets[i].state.next = &bullets[i + 1];
	}
	bullets[poolSize - 1].isActive = false;
	bullets[poolSize - 1].state.next = nullptr;

	Bullet::texture = bulletTexture;
}

void Enemies::Update(float deltatime)
{

	// verif si l ennemi est mort
	if (!isAlive())
	{
		isActive = false;
		return;
	}

	//si il est sortie de l'écran le rend inactif
	if (pos.y > 1024)
	{
		isActive = false;
		return;
	}

	//Deplacement vers le bas
	pos.y += vel.y * deltatime;

	//tir
	fireRate -= deltatime;
	if (fireRate <= 0.0f)
	{
		Shoot(vec2(pos + vec2(28, 32)), vec2(-500, 500) + Utils::RndVec(-100, 300));
		fireRate = fireCd; // reset le timer
	}

	//bullets update
	for (size_t i = 0; i < poolSize; i++) // pour chaque element dans le pool
	{
		if (bullets[i].isActive)//si elle est active
		{
			if (bullets[i].Update(deltatime))
			{
				bullets[i].isActive = false;
				bullets[i].state.next = head;
				head = &bullets[i];
			}
		}
	}

	//check de collision avec le player
	if (player)
	{
		for (size_t i = 0; i < player->GetPoolSize(); i++)
		{
			Bullet& b = player->GetBullets(i);
			if (b.isActive && b.owner == BulletOwner::Player)
			{
				if (CollisionSystem::IsColliding(this->CollisionRect(), b.CollisionRect()))
				{
					TakeDamage(player->damage);

					b.isActive = false;
					b.state.next = player->GetHead();
					player->SetHead(&b);
				}
			}
		}
	}
}


void Enemies::Render(SDL_Renderer* renderer)
{
	//Render bullet first (so ennemies is always on top)
	for (size_t i = 0; i < poolSize; i++)
	{
		if (bullets[i].isActive)
		{
			bullets[i].Render(renderer);
		}
	}

	//source rectangle for blit
	SDL_Rect src = { 0,0,64,64 };

	//dest rectangle
	SDL_FRect dest = { pos.x,pos.y, dim.x, dim.y };

	//blit
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);//SDL_BLENDMODE_BLEND = alphablend
	SDL_RenderCopyF(renderer, texture, &src, &dest);
}


SDL_Rect Enemies::CollisionRect() const // renvoi un SDLRect pour les bullets
{
	return SDL_Rect
	{
		static_cast<int>(pos.x),
		static_cast<int>(pos.y),
		static_cast<int>(dim.x),
		static_cast<int>(dim.y)
	};
}

void Enemies::TakeDamage(uint8_t damage)
{
	life -= damage;
	if (life <= 0)
	{
		life = 0;
		Death();
	}
}

bool Enemies::isAlive() const
{
	return life > 0;
}

void Enemies::Death()
{
	if (!scoreAdded)
	{
		// ajout du score au player
		if (player)
		{
			player->AddScore(score);
		}
		scoreAdded = true;

		isActive = false;

		// 10% chance to spawn a pickup
		if (enemyManager && Utils::RandomInt(0, 99) < 10)
		{
			PickupType type;
			int randomValue = Utils::RandomInt(0, 1);
			if (randomValue == 0)
			{
				type = PickupType::Heal;
			}
			else
			{
				type = PickupType::PowerUp;
			}
			enemyManager->SpawnPickup(pos, type);
		}
	}
}

