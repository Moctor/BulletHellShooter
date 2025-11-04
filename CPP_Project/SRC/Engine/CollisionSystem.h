#pragma once
#include <SDL.h>



class CollisionSystem
{
	public:
		static bool IsColliding(const SDL_Rect& a, const SDL_Rect& b);
		static bool IsPointInsideRect(const SDL_Point& point, const SDL_Rect& rect);
	
	private:

};