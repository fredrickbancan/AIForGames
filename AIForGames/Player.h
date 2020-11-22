#pragma once
#include "EntityLiving.h"
//simple player class which just extends entityliving
class Player : public EntityLiving
{
public:
	Player(float posX, float posY, float rotation);

	void onTick() override;
};