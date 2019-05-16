#include "Frame.h"
#include <iostream>


int Frame::frame = 0; 
int Frame::FPS = 0;

Frame::Frame(int fps) {
	frameDelay = 1000.0f / fps;
}


Frame::~Frame(){}

int Frame::getFPS() {
	return FPS;
}

void Frame::incrementFrame() {
	frame += 1;
}

void Frame::setCap(int time) {
	if (frameDelay > time) {
		Timer::timeToDelay = frameDelay - time;
		SDL_Delay(Timer::timeToDelay);
	}
}

bool Frame::setInterval(const int interval, int update, int fps) {
	if (update >= interval) {
		FPS = (int)Frame::frame / (fps / 1000.0f);
		return true;
	}

	return false;
}


