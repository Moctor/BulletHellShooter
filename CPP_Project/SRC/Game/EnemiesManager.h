#pragma once
#include "Enemies.h"
#include <vector>
#include <glm.hpp>
#include <random>
#include <array>

enum class DifficultyState
{
    Normal,
    Hard
};

class EnemyManager
{
public:
    EnemyManager(SDL_Texture* enemyTex1, SDL_Texture* enemyTex2, SDL_Texture* pickupTexHeal, SDL_Texture* pickupTexPowerUp, SDL_Texture* bulletTex, Player* playerRef, size_t maxEnemies);
    ~EnemyManager() = default;

    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);

    void SpawnEnemy(glm::vec2 pos, glm::vec2 vel, ShootPattern pattern);
    std::vector<Enemies*> GetActiveEnemies();

    void SpawnPickup(glm::vec2 pos, PickupType type);


private:

    std::vector<std::unique_ptr<Enemies>> enemyPool;
    std::vector<Pickup> pickupPool; 
    size_t maxEnemies;

    SDL_Texture* enemyTexture1 = nullptr;
    SDL_Texture* enemyTexture2 = nullptr;
    SDL_Texture* bulletTexture = nullptr;
    SDL_Texture* pickupTextureHeal = nullptr;     
    SDL_Texture* pickupTexturePowerUp = nullptr;  

    Player* player = nullptr;

    float spawnCooldown = 2.0f;     // cd entre deux ennmies
    float timeSinceLastSpawn = 0.0f;

    DifficultyState currentState = DifficultyState::Normal;
    float elapsedTime = 0.0f;

    std::array<glm::vec2, 5> spawnPositions =  //permet d'avoir une liste de position que j'ai choisi pour le spawn des enemies
    {
        glm::vec2(100, -100),
        glm::vec2(300, -150),
        glm::vec2(500, -200),
        glm::vec2(700, -120),
        glm::vec2(900, -80)
    };

    glm::vec2 GetRandomSpawnPosition(); //prend une position random de la liste donner
    ShootPattern GetRandomShootPattern(); //prend un pattern de shootrandom comme ça ça permet d'alterner les ennemis car ont fait leur différence aux pattern de shoot
    void UpdateDifficulty(float deltaTime); //permet de switch de state pour augmenter la difficulter
};
