#include "Guard.h"
#include <random>

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
	if (rand() % 300 == 0)
	{
		walkingFowards = true;
	}

	if (walkingFowards && rand() % 50 == 0)
	{
		walkingFowards = false;
	}
	if (walkingFowards)
	{
		turningLeft = turningRight = false;
	}
	else
	{
		if (rand() % 300 == 0)
		{
			turningLeft = true;
		}
		if (turningLeft && rand() % 10 == 0)
		{
			turningLeft = false;
		}
		if (rand() % 300 == 0)
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
