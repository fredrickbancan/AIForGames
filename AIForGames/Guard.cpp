#include "Guard.h"
#include <random>
#include "Ray2D.h"


Guard::Guard(float x, float y, float rotation) : EntityLiving(x,y,rotation)
{
}

Guard::Guard()
{
}

void Guard::onTick()
{
	//do stuff here

	handleState();
	
	EntityLiving::onTick();
}

Ray2D Guard::getDetectorRay()
{
	return Ray2D(pos, Vector2Scale(frontVector, detectorRayLength));
}

void Guard::handleState()
{
	switch (currentState)
	{
	case GuardState::WONDERING:
		doStateWondering();

		break;
	default:
		break;
	}
}
void Guard::doStateWondering()
{
	if (rand() % 200 == 0)
	{
		walkingFowards = true;
	}

	if (walkingFowards && rand() % 20 == 0)
	{
		walkingFowards = false;
	}
	if (walkingFowards)
	{
		turningLeft = turningRight = false;
	}
	else
	{
		if (rand() % 100 == 0)
		{
			turningLeft = true;
		}
		if (turningLeft && rand() % 10 == 0)
		{
			turningLeft = false;
		}
		if (rand() % 100 == 0)
		{
			turningRight = true;
		}
		if (turningRight && rand() % 10 == 0)
		{
			turningRight = false;
		}
	}
	if (walkingFowards)
	{
		walkFowards();
	}

	if (turningLeft)
	{
		rotation -= rotationSpeed;
	}

	if (turningRight)
	{
		rotation += rotationSpeed;
	}
}
