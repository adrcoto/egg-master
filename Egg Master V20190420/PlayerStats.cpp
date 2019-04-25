#include "PlayerStats.h"
#include "Game.h"

 int PlayerStats::lives = 3;
 int PlayerStats::lvl = 1;
 int PlayerStats::collectedEggs = 0;
 int PlayerStats::brokenEggs = 0;
 bool PlayerStats::lvledUp = false;

 bool PlayerStats::win = false;
 bool PlayerStats::gameOver = false;

PlayerStats::PlayerStats(){}


PlayerStats::~PlayerStats(){}


void PlayerStats::healUp() {
	if (lives < 3)
		lives += 1;
}

bool PlayerStats::hasLives() {
	return (lives > 0);
}


void PlayerStats::rockHit() {
	if (collectedEggs < 10)
		lives -= 1;
	else
	collectedEggs -= 10;
}

int PlayerStats::Lives() {
	return lives;
}

void PlayerStats::levelUp() {
	lvl += 1;
}

int PlayerStats::level() {
	return lvl;
}

void PlayerStats::leveledUp(bool p) {
	lvledUp = p;
}

bool PlayerStats::hasLeveledUp() {
	return lvledUp;
}

void PlayerStats::eggCollected() {
	collectedEggs += 1;
}

int PlayerStats::eggsCollected() {
	return collectedEggs;
}


void PlayerStats::eggDropped() {
	brokenEggs += 1;
}

int PlayerStats::eggsBroken() {
	return brokenEggs;
}
