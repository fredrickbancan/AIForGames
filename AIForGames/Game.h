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
	class NodeGraph* nodeGraph;//navigation nodes for guards
	AABB escapeTrigger{ 0,0,0,0 };//bounding box for the trigger the player must touch to win game
	AABB goldTrigger{ 0,0,0,0 };//bounding box for the trigger the player must touch to steal the gold
	bool drawDebug = false;//if true, draw vectors and bounding boxes.
	bool closing = false;//will be true when window is being closed
	bool playerHasGold = false;//true when player picks up the gold
	bool gameWon = false;//true if player escapes with gold
	bool gameLost = false;//true if player is captured by guard
	
	int ticksSinceGameEndNeededToRestart;//number of ticks needed to pass before the game restarts after losing/winning
	int ticksSinceGameEnd = 0;//ticks since the game ended, increases each tick after game is won/lost, when this reaches ticksSinceGameEndNeededToRestart, the game will restart.

	std::vector<AABB> levelWallBoxes;//all AABB colliders for level walls
	/*Toggles the provided boolean reference if the provided button bool is true.*/
	void toggleBooleanOnButtonPress(bool button, bool& booleanToToggle);

	/*closes raylib*/
	void closeRaylib();

	/*creates all rectangles and AABB for the levels walls*/
	void buildLevelWalls();

	/*adds provided wall to walls*/
	void addWall(float minX, float minY, float maxX, float maxY);

	/*when called loads all the guards to be new with random positions
	  also resets player and game state*/
	void loadGuardsAndResetGame();

	/*Returns a position vector along the ray closest to the provided point*/
	Vector2 getClosestPointOnRay(struct Ray2D ray, Vector2 testPoint);
	
	/*draws the provided ray, if it hits a wall, draws it up to where it hits the wall.*/
	void drawRayHittingWallsOrPlayer(struct Ray2D ray);

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

	/*Should be called before game loop*/
	void init();

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

	/*returns number of ticks for provided seconds based on ticksandfps*/
	int getNumOfTicksForSeconds(int seconds);

	/*returns true if provided ray hits wall or player*/
	bool doesRayHitWallOrPlayer(struct Ray2D ray, Vector2* hitLocation = nullptr);

	/*returns true if provided ray hits wall*/
	bool doesRayHitWall(struct Ray2D ray, Vector2* hitLocation = nullptr);

	/*returns true if the player has direct line of sight to the provided position
	  without walls in the way.*/
	bool canPlayerSeePos(Vector2 pos);

	Vector2 getPlayerPos();

	AABB getPlayerAABB();

	/*returns true of the provided ray intersects with the provided box. Assigns the hit location on
	  the AABB to hitlocation vector*/
	bool intersecting(struct Ray2D ray, AABB box, Vector2* hitLocation = nullptr);

	/*returns an array of NavNode pointers which should be navigated to in order to reach the 
	  endPos position. count will be assigned the count of nodes in the node pointer array.
	  When path is no longer being used, this pointer array must be de-allocated by the user.*/
	struct NavNode** getPath(Vector2 startPos, Vector2 endPos, int& count);

	/*returns node closest to the provided pos*/
	struct NavNode* getNodeAtPos(Vector2 pos);
};
