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
	AABB translate(Vector2 pos)
	{
		Vector2 distanceFromCenter{(maxBounds.x - minBounds.x) * 0.5F, (maxBounds.y - minBounds.y) * 0.5F};
		minBounds = Vector2Subtract(pos, distanceFromCenter);
		maxBounds = Vector2Add(pos, distanceFromCenter);
		return *this;
	}

	/*moves this by the provided direction*/
	AABB offset(Vector2 pos)
	{
		minBounds = Vector2Add(pos, minBounds);
		maxBounds = Vector2Add(pos, maxBounds);
		return *this;
	}

	/*returns a vector2 which is the position of the center of this AABB*/
	Vector2 getCenter()
	{
		return Vector2{ minBounds.x + (maxBounds.x - minBounds.x) * 0.5F , minBounds.y + (maxBounds.y - minBounds.y) * 0.5F };
	}
	
	float extentX()
	{
		return (maxBounds.x - minBounds.x) * 0.5F;
	}

	float extentY()
	{
		return (maxBounds.y - minBounds.y) * 0.5F;
	}

	/*returns true if this aabb is generally above the other aabb*/
	bool isOnTopOf(AABB other)
	{
		return getCenter().y > other.getCenter().y;
	}

	/*returns true if this aabb is generally to the right of the other aabb*/
	bool isToRightOf(AABB other)
	{
		return getCenter().x > other.getCenter().x;
	}

	/*returns true if this aabb is generally below the other aabb*/
	bool isBelowOf(AABB other)
	{
		return getCenter().y < other.getCenter().y;
	}

	/*returns true if this aabb is generally to the left of the other aabb*/
	bool isToLeftOf(AABB other)
	{
		return getCenter().x < other.getCenter().x;
	}

	/*returns true of the two provided aabb are overlapping in the x dimension*/
	static bool overlappingX(AABB a, AABB b)
	{
		return a.maxBounds.x > b.minBounds.x  && b.maxBounds.x > a.minBounds.x;
	}

	/*returns true of the two provided aabb are overlapping in the y dimension*/
	static bool overlappingY(AABB a, AABB b)
	{
		return a.maxBounds.y > b.minBounds.y && b.maxBounds.y > a.minBounds.y;
	}

	/*returns true if the provided AABB are touching in any way*/
	static bool touching(AABB a, AABB b)
	{
		return overlappingX(a, b) && overlappingY(a, b);
	}
};