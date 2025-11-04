#include "CollisionSystem.h"


bool CollisionSystem::IsColliding(const SDL_Rect& a, const SDL_Rect& b)
{
	if (SDL_HasIntersection(&a, &b))
	{
		return true;
	}
	return false;
};

bool CollisionSystem::IsPointInsideRect(const SDL_Point& point, const SDL_Rect& rect)
{
	return SDL_PointInRect(&point, &rect);
}