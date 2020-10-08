#pragma once

/*Singleton game class*/
class Game
{
private:
	static Game* gameInstance;
	class TicksAndFPS* ticksAndFps;
	
	/*renders the level and entities*/
	void drawScene();
public:

	/*returns the instance of the game*/
	static Game* get();
	
	/*deletes the game instance*/
	static void close();

	Game();
	Game(const Game&) = delete;
	~Game();

	/*called every frame*/
	void onFrame();
};
