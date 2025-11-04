#include "Game.h"
#include "Player.h"
#include "Enemies.h"
#include "../Engine/Utils.hpp"
#include "../Engine/CollisionSystem.h"
#include <iostream>
#include "../Engine/Logger.h"


SDL_Rect Player::CollisionRect() const //Fonction qui me renvoi un SDLRect pour les bullets
{
	return SDL_Rect
	{
		static_cast<int>(pos.x),
		static_cast<int>(pos.y),
		static_cast<int>(dim.x),
		static_cast<int>(dim.y)
	};
}

void Player::TakeDamage(uint8_t damage)
{
	life -= damage;
	if (life < 0)
	{
		life = 0;
		Death();
	}

}

void Player::Shoot(vec2 pos, vec2 vel)
{
	if (!isAlive) //si le player est mort empeche le code d'aller plus loin grace au return
	{
		return;
	}
	switch (currentShootPlayer)
	{
		case ShootPlayer::Single:
		{
			if (head)
			{
				Bullet* bullet = head;
				head = bullet->state.next;

				bullet->state.param.pos = pos;
				bullet->state.param.vel = vel;
				bullet->owner = BulletOwner::Player;
				bullet->isActive = true;
			}
		}break;
		case ShootPlayer::Burst:
		{
			int bulletCount = 3;
			float spreadAngle = 30; // total spread
			float angleStart = -90.f -spreadAngle/ 2.0f; //-90 pour changer d'ou parte les tirs
			float angleStep = spreadAngle / (bulletCount - 1);
			float speed = 250.f;

			for (int i = 0; i < bulletCount; ++i)
			{
				if (!head) break; // plus de bullets dispo

				float angle = angleStart + i * angleStep;
				glm::vec2 direction = Utils::AngleToVec(angle);
				glm::vec2 finalVel = direction * speed;

				Bullet* bullet = head;
				head = bullet->state.next;

				bullet->state.param.pos = pos;
				bullet->state.param.vel = finalVel;
				bullet->owner = BulletOwner::Player;
				bullet->isActive = true;
			}
		}
	}
}

void Player::Update(float deltatime)
{
	if (!isAlive) //si le player est mort empeche le code d'aller plus loin grace au return
	{
		return;
	}

	if (input->IsActionHeld(Action::MoveLeft))
	{
		pos.x -= speed * deltatime;
	}
	if (input->IsActionHeld(Action::MoveRight))
	{
		pos.x += speed * deltatime;
	}
	if (input->IsActionHeld(Action::MoveUp))
	{
		pos.y -= speed * deltatime;
	}
	if (input->IsActionHeld(Action::MoveDown))
	{
		pos.y += speed * deltatime;
	}
	if (input->IsActionHeld(Action::Shoot))
	{
		fireRate -= deltatime;
		if (fireRate <= 0.0f)
		{
			Shoot(vec2(pos + vec2(28, 0)), vec2(0, -500));
			fireRate = fireCd; // reset le timer
		}
	}

	pos.x += input->GetLeftStickX() * speed * deltatime;
	pos.y += input->GetLeftStickY() * speed * deltatime;

	//Clamp position to border so he doesnt go out of the screen
	const float rightBorder = DISPLAY_WIDTH - dim.x;
	const float bottomBorder = DISPLAY_HEIGHT - dim.y;
	if (pos.x >= rightBorder)
	{
		pos.x = rightBorder;
	}
	else if (pos.x <= 0)
	{
		pos.x = 0;
	}

	if (pos.y >= bottomBorder)
	{
		pos.y = bottomBorder;
	}
	else if (pos.y <= 0)
	{
		pos.y = 0;
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

	//check de collision avec lennemis
	for (Enemies* enemy : enemies)
	{
		if (!enemy) continue;

		for (size_t i = 0; i < enemy->GetPoolSize(); i++)
		{
			Bullet& b = enemy->GetBullets(i);
			if (b.isActive && b.owner == BulletOwner::Enemy)
			{
				if (CollisionSystem::IsColliding(this->CollisionRect(), b.CollisionRect()))
				{
					TakeDamage(enemy->damage);
					b.isActive = false;
					b.state.next = enemy->GetHead();
					enemy->SetHead(&b);
				}
			}
		}
	}

	// check du timer pour le burst shoot
	if (currentShootPlayer == ShootPlayer::Burst)
	{
		auto now = std::chrono::steady_clock::now();
		if (now >= powerUpEndTime)
		{
			SetShootPlayer(ShootPlayer::Single);
		}
	}




#ifndef NDEBUG //Permet d'ajouter des informations seulement en mode debug
	//std::cout << "Num of bullets: " << bullets.size() << std::endl;
#endif // !NDEBUG

}

void Player::Render(SDL_Renderer* renderer)
{
	//Render bullet first (so player is always on top)
	for (size_t i = 0; i < poolSize; i++)
	{
		if (bullets[i].isActive)
		{
			bullets[i].Render(renderer);
		}
	}

	//computer animation frame
	uint32_t animFrame = (SDL_GetTicks64() * animSpeed / 1000) % 2 ;// on divise par 1000 pour avoir en millisecond car le tick est en milli et %2 car on a deux anim pour le vaiseau

	//source rectangle for blit
	SDL_Rect src = { animFrame * 64,0,64,64 };

	//dest rectangle
	SDL_FRect dest = { pos.x,pos.y, dim.x, dim.y };

	//blit
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);//SDL_BLENDMODE_BLEND = alphablend
	SDL_RenderCopyF(renderer, texture, &src, &dest);
}

void Player::Death()
{
	isAlive = false;
	Logger::Log("Player is dead");
}

void Player::AddScore(int score)
{
	playerScore += score;
}

void Player::Heal(int amount)
{
	life += amount;
	if (life > 100)
		life = 100;
}

