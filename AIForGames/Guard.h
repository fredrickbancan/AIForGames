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
	int seekTicks = 0;//ticks will increase when lost sight of target to seek.
	int maxSeekTicks = 0;//maximum number of ticks of not having sight of player. when reached guard will switch to wondering state
	Vector2 seekPosition{0,0};
	float detectorRayLength = 1000.0F;//length of front facing ray that if player hits, will trigger guard to chase them.
	enum class GuardState currentState = GuardState::WONDERING;

	/*changes behaviour of this guard depending on its current state and external influences.*/
	void handleState();

	/*does the wondering state, randomly wondering.*/
	void doStateWondering();
public:
	Guard(float x, float y, float rotation);
	Guard();

	void setMaxSeekTicks(int max) { maxSeekTicks = max; };

	void onTick() override;

	struct Ray2D getDetectorRay();
};