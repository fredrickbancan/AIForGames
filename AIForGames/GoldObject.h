#pragma once
#include "AABB.h"

/*Class for the gold entity in the level to be stolen*/
class GoldObject
{
private:
	const AABB* playerBox = nullptr;//pointer for keeping track of players hitbox
	AABB goldBox{0,0,0,0};
	bool* pickupBool = nullptr;
public:

	GoldObject(float posX, float posY, const AABB* playerHitbox, bool* playerPickedUp)
	{
		playerBox = playerHitbox;
		goldBox = AABB(posX - 5, posY-5, posX + 5, posY + 5);
		pickupBool = playerPickedUp;
	}

	/*Should be called every tick to detect player pick up*/
	void onTick()
	{
		if (AABB::touching(*playerBox, goldBox))
		{
			*pickupBool = true;
		}
	}
};