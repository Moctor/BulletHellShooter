#pragma once
#include "../Engine/Entity.h"
#include "Bullet.h"
#include "../Engine/Input.h"
#include <memory>
#include <chrono>


class Enemies;


enum class ShootPlayer {Single, Burst };


class Player : public Entity
{
	public:
		float speed;
		int animSpeed; //frame per second
		std::unique_ptr<Input>& input;

		Player(vec2 p, SDL_Texture* t,SDL_Texture* bt, std::unique_ptr<Input>& i ) : Entity(p, vec2(0,0), t), bulletTexture (bt), input(i)
		{
			Bullet::texture = bulletTexture;
			head = &bullets[0];
			for (int i = 0; i < poolSize - 1; i++)
			{
				bullets[i].state.next = &bullets[i + 1]; // head
			}
			bullets[poolSize - 1].state.next = nullptr; //tail
		}
		~Player() = default;

		void Update(float deltatime) override;
		void Render(SDL_Renderer* renderer) override;
		void TakeDamage(uint8_t damage);

		void Shoot(vec2 pos, vec2 vel);
		void SetShootPlayer(ShootPlayer shooting)
		{
			currentShootPlayer = shooting;
			if (shooting == ShootPlayer::Burst)
			{
				powerUpEndTime = std::chrono::steady_clock::now() + std::chrono::seconds(5);
			}
		}

		void Death();
		bool IsAlive()const
		{
			return isAlive;
		}

		void SetEnemies(const std::vector<Enemies*>& enemiesList)
		{
			enemies = enemiesList;
		}

		SDL_Rect CollisionRect() const; //permet de récupérer le rectangle de collision du player

		uint32_t GetPoolSize() const //recup poolsize des bullets du player
		{
			return poolSize;
		}
		Bullet& GetBullets(size_t i) //recup bulelts du player
		{
			return bullets[i];
		}
		Bullet* GetHead() //recup head du player
		{
			return head;
		}
		void SetHead(Bullet* b) //set le head du player
		{
			head = b;
		}

		int life = 100;
		uint8_t damage = 10;

		int GetLife() const //permet de recup la life du player(pour l'hud principalement)
		{
			return life; 
		}

		int GetScore()const
		{
			return playerScore;
		}

		void AddScore(int score);
		void Heal(int amount);

	private:
		bool isAlive = true; // état de base en vie
		int playerScore = 0; //de base a 0

		Bullet* head;
		SDL_Texture* bulletTexture;
		static const uint32_t poolSize = 300;
		Bullet bullets[poolSize];

		float fireRate = 0.0f;
		float fireCd = 0.25f;
		ShootPlayer currentShootPlayer = ShootPlayer::Single; //etat par défaut
		std::chrono::steady_clock::time_point powerUpEndTime;

		std::vector<Enemies*> enemies;

};

