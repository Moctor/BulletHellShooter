#include <glm.hpp>
#include <SDL.h>
#include <cstdint>
#include <random>

class Utils
{
	public:

		static glm::vec2 RndVec(float min, float max) //peremt de generer une position aléatoire
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dist(min, max);
			return glm::vec2(dist(gen), dist(gen));
		}

		static SDL_Color RndCol() // permet de generer une couleur aléatoire
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dist(0, 255);

			SDL_Color color;
			color.r = static_cast<uint8_t>(dist(gen));
			color.g = static_cast<uint8_t>(dist(gen));
			color.b = static_cast<uint8_t>(dist(gen));
			color.a = 255;

			return color;
		}

		static glm::vec2 AngleToVec(float angleDegrees) 
		{
			float radians = glm::radians(angleDegrees);
			return glm::vec2(std::cos(radians), std::sin(radians));
		}

		static int RandomInt(int min, int max) //permet de generer un int aléatoire
		{
			static std::random_device rd;
			static std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dist(min, max);
			return dist(gen);
		}

	private:

};