#pragma once
#include <glm.hpp>
#include <cstdint>
#include <SDL.h>


enum class EntityType;//forward declaration on le déclare ici met le définis ailleurs (Entities.h)

class Entity
{
	public:
		using vec2 = glm::vec2; // permet que dans ce scope entity et ses "enfants" que vec2 est glm::vec2 ce qui permet de pas tout le temps ecrire glm::vec2

		vec2 pos;
		vec2 vel;
		vec2 dim = { 8,8 }; //width & height

		EntityType type;
		SDL_Texture* texture;
		bool toBeDestroyed = false;

		//vec2 p et vec2 v sont des parametes a passer dans la fonction et pos(p) , vel(v) initialise les parametres
		Entity(vec2 p, vec2 v, SDL_Texture* tx) : pos(p), vel(v), texture(tx) {}
		virtual ~Entity(){}

		virtual void Update (float deltaTime) = 0;
		virtual void Render (SDL_Renderer* render) = 0;
		

	private:

};

