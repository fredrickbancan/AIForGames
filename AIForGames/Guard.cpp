#include "Guard.h"

Guard::Guard(float x, float y, float rotation) : EntityLiving(x,y,rotation)
{
}

Guard::Guard()
{
}

void Guard::onTick()
{
	//do stuff here
	EntityLiving::onTick();
}
