#include "PlayerController.h"

PlayerController* PlayerController::instance = nullptr;
PlayerController* PlayerController::get()
{
	if (instance == nullptr)
	{
		instance = new PlayerController();
	}
	return instance;
}

void PlayerController::close()
{
	delete instance;
}

void PlayerController::addAction(PlayerAction act)
{
	actions[(int)act] = true;
}

void PlayerController::addActIfKeyDown(KeyboardKey key, PlayerAction act)
{
	if (IsKeyDown(key))
	{
		addAction(act);
	}
}

PlayerController::PlayerController()
{
	reset();
}

void PlayerController::reset()
{
	for (int i = 0; i < (int)PlayerAction::ENUM_COUNT; i++)
	{
		actions[i] = false;
	}
}

void PlayerController::update()
{
	reset();
	addActIfKeyDown(KEY_UP, PlayerAction::WALKFOWARDS);
	addActIfKeyDown(KEY_DOWN, PlayerAction::WALKBACKWARDS);
	addActIfKeyDown(KEY_LEFT, PlayerAction::ROTATELEFT);
	addActIfKeyDown(KEY_RIGHT, PlayerAction::ROTATERIGHT);
}

bool PlayerController::doingAction(PlayerAction act)
{
	return actions[(int)act];
}
