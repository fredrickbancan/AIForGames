#include "EntityLiving.h"

EntityLiving::EntityLiving(float x, float y, float rotation): posX(x), posY(y), rotation(rotation), prevPosX(x), prevPosY(y), prevRotation(rotation)
{
	moveAcel = 1.5F;
}

EntityLiving::EntityLiving() : posX(0), posY(0), rotation(0), prevPosX(0), prevPosY(0), prevRotation(0)
{
	moveAcel = 1.5F;
}

void EntityLiving::onTick()
{
	prevPosX = posX;
	prevPosY = posY;

	
	velX += acelX;
	velY += acelY;
	velX *= moveResistance;
	velY *= moveResistance;
	posX += velX;
	posY += velY;
}