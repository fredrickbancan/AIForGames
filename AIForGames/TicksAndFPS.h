#pragma once

/*Class for controlling ticks and getting FPS*/
class TicksAndFPS
{
private:
	long long currentFrameTime = 0;//time in UTC miliseconds of the current frame
	long long lastFrameTime = 0;//time in UTC miliseconds of the previous frame
	double deltaFrameTime = 0;//Time in miliseconds that has passed since previous frame.
	const int ticksPerSecond = 0;//ticks per second
	const double msPerTick = 0.0;//miliseconds per tick
	long long applicationTime; //last updated UTC time of application. When ticking game, needs to be ticked untill application catches up with real time.
	double percentToNextTick; //a decimal value between 0 and 1 which can be used as a percentage of progress towards next tick, usefull for interpolation.
	bool paused = false; //true when game is paused
	int frames = 0;//frames in the current second
	int FPS = 0;//frames per second
	double timer = 0.0;//used to tell when a second has passed to calculate FPS

	/*returns the current time since epoch in miliseconds*/
	long long getRealTimeMS();

public:
	TicksAndFPS(int tps);
	~TicksAndFPS() {};

	/*Updates the calcualtion of fps*/
	void updateFPS();

	/*runs the provided onTick function repeatidly untill application has synced with realtime*/
	void doOnTickUntillRealtimeSync(void(* onTickFunc)());

	void pause() { paused = true; }
	void unPause() { paused = false; }

	float getPercentageToNextTick() const { return (float)percentToNextTick; }

	float getNumOfTicksForSeconds(float seconds) const {return (float)(seconds * ticksPerSecond);}
};