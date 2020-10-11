#pragma once
#include "EntityLiving.h"
class Player : public EntityLiving
{
private:
	bool hasObjectToSteal = false;//true when player gets the object to steal
public:
	static constexpr float rotationSpeed = 5.0F;
	Player(float posX, float posY, float rotation);

	void onTick() override;
};