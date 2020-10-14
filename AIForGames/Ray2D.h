#pragma once
#include "raymath.h"
struct Ray2D
{
	Vector2 origin;
	Vector2 direction;
	float length;
	Ray2D()
	{
		origin = { 0,0 };
		direction = { 0,0 };
		length = 0;
	}
	Ray2D(float originX, float originY, float directionX, float directionY)
	{
		origin.x = originX;
		origin.y = originY;
		Vector2 directionVec{ directionX, directionY };
		length = Vector2Length(directionVec);
		direction = Vector2Normalize(directionVec);
	}

	Ray2D(Vector2 origin, Vector2 direction)
	{
		this->origin = origin;
		length = Vector2Length(direction);
		this->direction = Vector2Normalize(direction);
	}
};