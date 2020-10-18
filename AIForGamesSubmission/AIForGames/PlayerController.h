#pragma once
#include "raylib.h"
enum class PlayerAction
{
	WALKFOWARDS,
	WALKBACKWARDS,
	ROTATELEFT,
	ROTATERIGHT,
	ENUM_COUNT = 4//count of all enums
};
class PlayerController
{
private:
	static PlayerController* instance;//singleton instance
	
	/*actions requested by player*/
	bool actions[(int)PlayerAction::ENUM_COUNT];

	/*called when user requests action via keyboard input*/
	void addAction(PlayerAction act);

	/*resets the playercontroller so it is ready for more input*/
	void reset();

	/*if the key is down, the action will be set true in list of actions*/
	void addActIfKeyDown(KeyboardKey key, PlayerAction act);

	PlayerController();
public:
	static PlayerController* get();//returns the singleton instance
	static void close();//deletes instance

	PlayerController(const PlayerController& ) = delete;

	/*called every frame to update input*/
	void update();

	/*returns true if the provided action has been requested by the user*/
	bool doingAction(PlayerAction act);
};