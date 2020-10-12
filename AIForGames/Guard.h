#pragma once
#include "EntityLiving.h"

enum class GuardState
{
	WONDERING,
	SEEKING
};

class Guard : public EntityLiving
{
private:
	bool canSeePlayer = false;//true if guard has line of sight of player.
	bool walkingFowards = false;
	bool turningLeft = false;
	bool turningRight = false;
	enum class GuardState currentState = GuardState::WONDERING;
public:
	Guard(float x, float y, float rotation);
	Guard();

	void onTick() override;
};