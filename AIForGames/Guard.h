#pragma once
#include "EntityLiving.h"

class Guard : public EntityLiving
{
private:
	bool canSeePlayer = false;//true if guard has line of sight of player.
public:
	Guard(float x, float y, float rotation);
	Guard();

};