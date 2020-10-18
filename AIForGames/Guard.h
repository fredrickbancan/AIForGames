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
	int seekCantSeePlayerTicks = 0;//ticks will increase when lost sight of target to seek.
	int maxSeekCantSeePlayerTicks = 0;//maximum number of ticks of not having sight of player. when reached guard will switch to wondering state
	Vector2 seekPosition{0,0};
	float detectorRayLength = 150.0F;//length of front facing ray that if player hits, will trigger guard to chase them.
	enum class GuardState currentState = GuardState::WONDERING;
	struct NavNode** currentPath = nullptr;//the current path this guard is following
	int currentPathCount = 0;//the count of nodes in the current path
	int currentPathProgress = 0;//the number of nodes the guard has covered in its current path, acts as an index into the path node array.
	/*changes behaviour of this guard depending on its current state and external influences.*/
	void handleState();

	/*returns true if the detector ray is hitting the player aabb*/
	bool doesDetectorRayHitPlayer();

	/*does the wondering state, randomly wondering. If the detector ray hits the player AABB,
	  switch to seeking state.*/
	void doStateWondering();

	/*Does the seeking state, If has direct line of sight to player, walk directly to them.
	  Otherwise, use navigation nodes to go to player closest node. If the player has been out of line
	  of sight longer than this guards maximum seek ticks, then go to wondering state.*/
	void doStateSeeking();

	/*makes this guard seek to the provided position.*/
	void seekToPos(Vector2 pos);

	void deleteCurrentPath();

public:
	Guard(float x, float y, float rotation);
	Guard();
	~Guard() { deleteCurrentPath(); };

	void setMaxSeekCantSeePlayerTicks(int max) { maxSeekCantSeePlayerTicks = max; };

	void onTick() override;

	/*returns the ray from the front of this guard for detecting the player*/
	struct Ray2D getDetectorRay();

	/*returns true if this guard is navigating a path*/
	bool hasPath() { return currentPath != nullptr; };

	/*draws the current nav path as a series of lines*/
	void drawPath();
};