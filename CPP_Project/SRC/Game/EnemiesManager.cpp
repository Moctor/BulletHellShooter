#include "EnemiesManager.h"
#include "../Engine/Logger.h"


EnemyManager::EnemyManager(SDL_Texture* enemyTex1, SDL_Texture* enemyTex2, SDL_Texture* bulletTex, SDL_Texture* pickupTexHeal, SDL_Texture* pickupTexPowerUp, Player* playerRef, size_t maxEnemies)
    : enemyTexture1(enemyTex1), enemyTexture2(enemyTex2), bulletTexture(bulletTex),
    pickupTextureHeal(pickupTexHeal),pickupTexturePowerUp(pickupTexPowerUp),player(playerRef), maxEnemies(maxEnemies)
{
    enemyPool.reserve(maxEnemies); //set le nombre max d'ennemis pour la mémoire

    for (size_t i = 0; i < maxEnemies; ++i) //création du pool ennemi
    {
        // crée les ennemis avec des valeurs par défaut, tu peux adapter la position/vel comme tu veux
        auto enemy = std::make_unique<Enemies>(glm::vec2(0, 0), glm::vec2(0, 0), enemyTexture1, bulletTexture);
        enemy->SetPlayer(player);
        enemy->SetEnemyManager(this);  //pointeur EnemyManager
        enemy->isActive = false; // pas actif au départ
        enemyPool.push_back(std::move(enemy));
    }
    // pool pickup
    pickupPool.reserve(maxEnemies);
    for (int i = 0; i < maxEnemies; ++i)
    {
        pickupPool.emplace_back(glm::vec2(0, 0), glm::vec2(0, 0), nullptr, PickupType::Heal);
        pickupPool.back().Reset();
        pickupPool.back().SetPlayer(player);

    }
}

void EnemyManager::Update(float deltaTime)
{
    // stock le temps écouler
    elapsedTime += deltaTime;

    // update la difficulté en fonction du temps écoulé
    UpdateDifficulty(deltaTime);

    for (auto& enemy : enemyPool)
    {
        if (enemy->isActive) // update les enemis actif
        {
            enemy->Update(deltaTime);
        }
        // désactiver l'ennemi s'il est mort
        if (!enemy->Enemies::isAlive())
        {
            enemy->isActive = false;
        }
    }

    // Update des pickups actifs
    for (auto& pickup : pickupPool)
    {
        if (pickup.IsActive())
        {
            pickup.Update(deltaTime);
        }
    }

    //respawn automatique
    timeSinceLastSpawn += deltaTime;
    if (timeSinceLastSpawn >= spawnCooldown)
    {
        glm::vec2 spawnPos = GetRandomSpawnPosition(); //pos aléatoire dans l'array donner
        glm::vec2 velocity = glm::vec2(0, 100); // vitesse ver le bas
        ShootPattern pattern = GetRandomShootPattern(); //pattern de tir random
        SpawnEnemy(spawnPos, velocity, pattern);

        timeSinceLastSpawn = 0.0f;
    }


}

void EnemyManager::Render(SDL_Renderer* renderer)
{
    for (auto& enemy : enemyPool) //render uniquement les membres actifs
    {
        if (enemy->isActive)
        {
            enemy->Render(renderer);
        }
    }

    // render des pickups actifs
    for (auto& pickup : pickupPool)
    {
        if (pickup.IsActive())
        {
            pickup.Render(renderer);
        }
    }
}

void EnemyManager::SpawnEnemy(glm::vec2 pos, glm::vec2 vel, ShootPattern pattern)
{
    // chosi la texture de l ennemi selon son pattern
    SDL_Texture* chosenTexture;

    if (pattern == ShootPattern::Burst)
    {
        chosenTexture = enemyTexture1;
    }
    else
    {
        chosenTexture = enemyTexture2;
    }
    // cherche si y a un enemi inactif dans le pool
    for (auto& enemy : enemyPool)
    {
        if (!enemy->isActive) //reset l'ennemi et l'active
        {
            enemy->Reset(pos, vel, chosenTexture, bulletTexture, pattern);
            enemy->SetPlayer(player);
            enemy->isActive = true;
            break;
        }
    }
}

glm::vec2 EnemyManager::GetRandomSpawnPosition()
{
    //tire un index entre 0 et la size de spawnPosition
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, static_cast<int>(spawnPositions.size()) - 1);
    return spawnPositions[dist(gen)];
}

ShootPattern EnemyManager::GetRandomShootPattern()
{
    //choisi random entre burst et spread pattern
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1);
    return dist(gen) == 0 ? ShootPattern::Burst : ShootPattern::Spread;
}

std::vector<Enemies*> EnemyManager::GetActiveEnemies()
{
    //retourne la liste d'ennemis actif
    std::vector<Enemies*> activeEnemies;
    for (auto& enemy : enemyPool)
    {
        if (enemy->isActive)
            activeEnemies.push_back(enemy.get());
    }
    return activeEnemies;
}

void EnemyManager::SpawnPickup(glm::vec2 pos, PickupType type)
{
    for (auto& p : pickupPool)
    {
        if (!p.IsActive())
        {
            SDL_Texture* tex = nullptr;
            switch (type)
            {
            case PickupType::Heal:
                tex = pickupTextureHeal;
                break;
            case PickupType::PowerUp:
                tex = pickupTexturePowerUp;
                break;
            }
            p.Setup(pos, glm::vec2(0.f, 100.0f), tex, type);          
            break;
        }
    }
}

void EnemyManager::UpdateDifficulty(float deltaTime)
{
    if (currentState == DifficultyState::Normal && elapsedTime >= 120.0f) // 120 = 2minutes
    {
        currentState = DifficultyState::Hard;
        spawnCooldown = 1.25f; // hard mode
    }
}