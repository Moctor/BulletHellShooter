#pragma once
#include "../Engine/Entity.h"
#include "Bullet.h"
#include "PickUp.h"
#include <memory>
#include <vector>


enum class ShootPattern{Spread, Burst};

class Player;
class EnemyManager;



class Enemies : public Entity
{
	public:

		Enemies(vec2 p, vec2 v, SDL_Texture* t, SDL_Texture* bt)
			: Entity(p, v, t), bulletTexture(bt)
		{
			//bullet
			Bullet::texture = bulletTexture;
			head = &bullets[0];
			for (int i = 0; i < poolSize - 1; i++)
			{
				bullets[i].state.next = &bullets[i + 1];
			}
			bullets[poolSize - 1].state.next = nullptr;
		}
		~Enemies() = default;

		void Update(float deltatime) override;
		void Render(SDL_Renderer* renderer) override;
		void TakeDamage(uint8_t damage);

		bool isAlive()const;

		void Shoot(vec2 pos, vec2 vel);

		void SetShootPattern(ShootPattern pattern)
		{
			currentShootPattern = pattern;
		}

		SDL_Rect CollisionRect() const; //permet de récupérer le rectangle de collision des ennemies

		void SetPlayer(Player* pl)
		{ 
			player = pl; 
		}


		uint32_t GetPoolSize() const //recup poolsize de ennemi
		{
			return poolSize;
		}
		Bullet& GetBullets(size_t i) //recup bullet enemi
		{ 
			return bullets[i];
		}
		Bullet* GetHead() //recup head ennemi
		{ 
			return head;
		}
		void SetHead(Bullet* b)//set head ennemi 
		{ 
			head = b; 
		}

		bool isActive = false;
		void Reset(vec2 p, vec2 v, SDL_Texture* tex, SDL_Texture* btex, ShootPattern pattern);

		int life = 50;
		uint8_t damage = 10;

		uint8_t GetScoreValue() const 
		{ 
			return score; 
		}
		bool scoreAdded = false;//etat de base en false

		void Death();

		void SetPickupPool(std::vector<Pickup>* pool) 
		{ 
			pickupPool = pool; 
		}

		void SetEnemyManager(EnemyManager* em) 
		{ 
			enemyManager = em; 
		}

	private:
		float fireRate = 0.0f;
		float fireCd = 1.0f;

		int score = 100;

		ShootPattern currentShootPattern = ShootPattern::Burst; //etat par défaut

		//pool de bullet
		Bullet* head;
		SDL_Texture* bulletTexture;
		static const uint32_t poolSize = 1000;
		Bullet bullets[poolSize];

		Player* player = nullptr;

		//pool pickup
		std::vector<Pickup>* pickupPool = nullptr;
		SDL_Texture* pickupTexture = nullptr;

		EnemyManager* enemyManager = nullptr;
};
