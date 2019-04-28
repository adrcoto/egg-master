#pragma once
#include "SDL.h"
#include "Timer.h"

class Frame {
private:
	static int frame;
	static int FPS;
	int frameDelay;
public:

	Frame(int);
	~Frame();

	 void setCap(int);
	 bool setInterval(const int, int, int);

	static int getFPS();

	void incrementFrame();
};

