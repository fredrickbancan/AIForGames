#pragma once
#include "raymath.h"

/*custom axis aligned bounding box class*/
struct AABB
{
	Vector2 minBounds{ 0,0 }, maxBounds{ 0,0 };

	AABB(float minX, float minY, float maxX, float maxY)
	{
		minBounds.x = minX;
		minBounds.y = minY;
		maxBounds.x = maxX;
		maxBounds.y = maxY;
	}
	
	/*moves this AABB to the provided coordinate*/
	void translate(Vector2 pos)
	{
		Vector2 distanceFromCenter{(maxBounds.x - minBounds.x) * 0.5F, (maxBounds.y - minBounds.y) * 0.5F};
		minBounds = Vector2Subtract(pos, distanceFromCenter);
		maxBounds = Vector2Add(pos, distanceFromCenter);
	}

	/*returns a vector2 which is the position of the center of this AABB*/
	Vector2 getCenter()
	{
		return Vector2{ minBounds.x + (maxBounds.x - minBounds.x) * 0.5F , minBounds.y + (maxBounds.y - minBounds.y) * 0.5F };
	}
};