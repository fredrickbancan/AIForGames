#pragma once
#include "raymath.h"
struct Ray2D
{
	Vector2 origin;//origin of ray
	Vector2 direction;//direction of ray
	float length;//length of ray

	Ray2D()//default constructor
	{
		origin = { 0,0 };
		direction = { 0,0 };
		length = 0;
	}

	//non vector constructor
	Ray2D(float originX, float originY, float directionX, float directionY)
	{
		origin.x = originX;
		origin.y = originY;
		Vector2 directionVec{ directionX, directionY };
		length = Vector2Length(directionVec);
		direction = Vector2Normalize(directionVec);
	}

	//vector constructor
	Ray2D(Vector2 origin, Vector2 direction)
	{
		this->origin = origin;
		length = Vector2Length(direction);
		this->direction = Vector2Normalize(direction);
	}
};