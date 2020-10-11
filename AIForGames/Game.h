#pragma once
#include <vector>
#include "AABB.h"
/*Singleton game class*/
class Game
{

private:
	static Game* gameInstance;
	class TicksAndFPS* ticksAndFps;
	class Player* thePlayer;//the player
	class Guard* guards;//array
	class PlayerController* playerController;//player controller instance
	bool closing = false;//will be true when window is being closed
	std::vector<AABB> levelWallBoxes;//all AABB colliders for level walls
	/*Toggles the provided boolean reference if the provided button bool is true.*/
	void toggleBooleanOnButtonPress(bool button, bool& booleanToToggle);

	/*closes raylib*/
	void closeRaylib();

	/*creates all rectangles and AABB for the levels walls*/
	void buildLevelWalls();

	/*adds provided wall to walls*/
	void addWall(float minX, float minY, float maxX, float maxY);

	float radians(float degrees);
	Game();
public:
	static constexpr int screenWidth = 800;
	static constexpr int screenHeight = 600;
	static constexpr int guardCount = 12;
	/*returns the instance of the game*/
	static Game* get();
	
	/*deletes the game instance*/
	static void close();

	Game(const Game&) = delete;
	~Game();

	/*called every frame*/
	void onFrame();

	/*called every tick*/
	void onTick();

	/*renders the level and entities*/
	void drawScene();

	/*draws aabb as a wire box, if wall is true, will draw as a solid rectangle.*/
	void drawAABB(AABB box, bool wall);

	bool isClosing() const { return closing; }

	/*can be called to move an entity with respect to all AABB in the level
	  this will also modify the velocity vector so it does not increase while
	  colliding with walls*/
	void tryToMoveEntity(class EntityLiving* ent, Vector2& vel);

	/*linear interpolates the given values by the ticksandfps percentage to next tick*/
	float lerp(float start, float dest);
};
