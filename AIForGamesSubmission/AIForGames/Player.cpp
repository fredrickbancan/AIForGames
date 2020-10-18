#include "Player.h"
#include "PlayerController.h"
Player::Player(float posX, float posY, float rotation) : EntityLiving(posX, posY, rotation)
{
	moveAcel = 0.5F;
}

void Player::onTick()
{
	//do stuff here

	//MOVEMENT
	PlayerController* pc = PlayerController::get();
	//steering
	if (pc->doingAction(PlayerAction::ROTATERIGHT))
	{
		rotation += rotationSpeed;
	}
	if (pc->doingAction(PlayerAction::ROTATELEFT))
	{
		rotation -= rotationSpeed;
	}
	//front and backwards movement
	float frontBackMove = (int)pc->doingAction(PlayerAction::WALKFOWARDS) - (int)pc->doingAction(PlayerAction::WALKBACKWARDS);	
	acel.y += frontVector.y * frontBackMove * moveAcel;
	acel.x += frontVector.x * frontBackMove * moveAcel;
	//END MOVEMENT

	EntityLiving::onTick();
}
