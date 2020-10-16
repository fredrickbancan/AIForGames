#include "Guard.h"
#include <random>
#include "Ray2D.h"
#include "Game.h"
#include <iostream>

void Guard::doStateSeeking()
{
	if (Game::get()->canPlayerSeePos(pos))//if guard has line of sight to player
	{
		//seek directly to player
		seekToPos(Game::get()->getPlayerPos());
		seekCantSeePlayerTicks = 0;
	}
	else
	{//TODO: impliment
		//follow dijkstras path of nav nodes
		seekCantSeePlayerTicks++;
	}

}

void Guard::seekToPos(Vector2 target)
{
	//rads to degrees = radians * (180.0F / 3.141592653589F)
	Vector2 facingVec = Vector2Subtract(target, pos);
	float facingRotation = (atan2(facingVec.y, facingVec.x) * (180.0F / 3.141592653589F)) + 90.0F;
	
	//rotate towards facing rotation at a max speed of rotationSpeed
	////

	//make rotation 0 to 360
	if (facingRotation < 0)
	{
		facingRotation = 360 + facingRotation;
	}
	
	//rotate towards facingRotation
	float rotDiff = facingRotation - rotation;
	
	//ensure not to rotate full 360 degrees
	if (rotDiff < -180 || rotDiff > 180)
	{
		rotDiff = -rotDiff;
	}
	rotation += Clamp(rotDiff, -rotationSpeed, rotationSpeed);
	////

	walkFowards();
}

Guard::Guard(float x, float y, float rotation) : EntityLiving(x,y,rotation)
{
	moveAcel = .25F;
	rotationSpeed = 3.0F;
	moveResistance = 0.05f;
}

Guard::Guard()
{
	moveAcel = .25F;
	rotationSpeed = 3.0F;
	moveResistance = 0.05f;
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
		if (doesDetectorRayHitPlayer())
		{
			currentState = GuardState::SEEKING;
		}
		break;
	case GuardState::SEEKING:
		doStateSeeking();
		if (seekCantSeePlayerTicks >= maxSeekCantSeePlayerTicks)
		{
			currentState = GuardState::WONDERING;
			seekCantSeePlayerTicks = 0;
		}
		break;
	default:
		break;
	}
}

bool Guard::doesDetectorRayHitPlayer()
{
	Vector2 playerHitPos{};
	Vector2 nearestWallHitPos{};//nearest wall hit pos is needed to test if wall obscures player
	if (Game::get()->intersecting(getDetectorRay(), Game::get()->getPlayerAABB(), &playerHitPos))
	{
		if (Game::get()->doesRayHitWall(getDetectorRay(), &nearestWallHitPos))
		{
			//if the wall hit pos is closer than the players, then the wall is in the way. So return false.
			if (Vector2Distance(pos, nearestWallHitPos) < Vector2Distance(pos, playerHitPos))
			{
				return false;
			}
		}
		return true;
	}
	return false;
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
