#pragma once

/*Singleton game class*/
class Game
{

private:
	static Game* gameInstance;
	class TicksAndFPS* ticksAndFps;
	bool closing = false;//will be true when window is being closed

	/*Toggles the provided boolean reference if the provided button bool is true.*/
	void toggleBooleanOnButtonPress(bool button, bool& booleanToToggle);

	/*closes raylib*/
	void closeRaylib();
public:
	static constexpr int screenWidth = 800;
	static constexpr int screenHeight = 450;
	/*returns the instance of the game*/
	static Game* get();
	
	/*deletes the game instance*/
	static void close();

	Game();
	Game(const Game&) = delete;
	~Game();

	/*called every frame*/
	void onFrame();

	/*renders the level and entities*/
	void drawScene();

	bool isClosing() const { return closing; }
};
