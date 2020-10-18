#include "Guard.h"
#include <random>
#include "Ray2D.h"
#include "Game.h"
#include "NodeGraph.h"
#include "raylib.h"
void Guard::doStateSeeking()
{
	if (Game::get()->canPlayerSeePos(pos))//if guard has line of sight to player
	{
		rotationSpeed = 5.0F;
		//seek directly to player
		seekToPos(Game::get()->getPlayerPos());
		seekCantSeePlayerTicks = 0;

		deleteCurrentPath();
	}
	else
	{
		rotationSpeed = 25.0F;
		//follow dijkstras path of nav nodes
		if (currentPath != nullptr)//if the current path exists
		{
			if (Game::get()->getNodeAtPos(pos) == currentPath[currentPathProgress])//if this guard as reached it current node position
			{
				currentPathProgress++;
			}
			if (currentPathProgress >= currentPathCount)//if this guard has reached the end of the path
			{
				//get a new path
				deleteCurrentPath();
				currentPath = Game::get()->getShortestPath(pos, Game::get()->getPlayerPos(), currentPathCount);
			}
				seekToPos(currentPath[currentPathProgress]->pos);
		}
		else
		{
			//get a path to the last node the player was seen at
			currentPath = Game::get()->getShortestPath(pos, Game::get()->getPlayerPos(), currentPathCount);
		}
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

void Guard::deleteCurrentPath()
{
	if (currentPath != nullptr)//remove the current path if it exists
	{
		delete[] currentPath;
		currentPath = nullptr;
		currentPathProgress = 0;
	}
}

Guard::Guard(float x, float y, float rotation) : EntityLiving(x,y,rotation)
{
	moveAcel = .5F;
	
	rotationSpeed = 5.0F;
	moveResistance = 0.15f;
}

Guard::Guard()
{
	moveAcel = .5F;
	rotationSpeed = 5.0F;
	moveResistance = 0.15f;
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

void Guard::drawPath()
{
	if (currentPath == nullptr) return;
	for (int i = currentPathCount; i > currentPathProgress + 1; i--)
	{
		//draw line from node to its parent
		NavNode* current = currentPath[i - 1];
		NavNode* next = currentPath[i - 2];
		DrawLine(current->pos.x, current->pos.y, next->pos.x, next->pos.y, RED);
	}
	NavNode* end = currentPath[currentPathCount - 1];
	DrawRectangle(end->pos.x - 2, end->pos.y -2, 4, 4, RED);
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
			rotationSpeed = 5.0F;
			deleteCurrentPath();
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
