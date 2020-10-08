#pragma once

/*Singleton game class*/
class Game
{
private:
	static Game* gameInstance;
public:

	/*returns the instance of the game*/
	static Game* get();
	Game() = delete;
	Game(const Game&) = delete;
	~Game();
};