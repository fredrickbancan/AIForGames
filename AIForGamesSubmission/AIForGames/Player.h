#pragma once
#include "EntityLiving.h"
class Player : public EntityLiving
{
public:
	Player(float posX, float posY, float rotation);

	void onTick() override;
};