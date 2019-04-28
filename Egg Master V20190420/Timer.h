#pragma once
#include <SDL.h>

class Timer {
private:
	static float delta;

	//The clock time when the timer started
	int startTicks;

	//The ticks stored when the timer was paused
	int pausedTicks;

	//The timer status
	bool paused;
	bool started;

public:
	//Initializes variables
	Timer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	int get_ticks();

	static float deltaTime();
	static void setDelta(Timer&);

	static int timeToDelay;
	//Checks the status of the timer
	bool is_started();
	bool is_paused();
};