#pragma once

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

	/*Toggles the provided boolean reference if the provided button bool is true.*/
	void toggleBooleanOnButtonPress(bool button, bool& booleanToToggle);

	/*closes raylib*/
	void closeRaylib();

	float radians(float degrees);
	Game();
public:
	static constexpr int screenWidth = 800;
	static constexpr int screenHeight = 450;
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

	bool isClosing() const { return closing; }

	/*linear interpolates the given values by the ticksandfps percentage to next tick*/
	float lerp(float start, float dest);
};
