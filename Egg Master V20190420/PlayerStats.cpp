#include "PlayerStats.h"
#include "Game.h"

 int PlayerStats::lives = 3;
 int PlayerStats::collectedEggs = 0;
 int PlayerStats::brokenEggs = 0;

 bool PlayerStats::win = false;
 bool PlayerStats::gameOver = false;

 bool PlayerStats::heal = false;
 bool PlayerStats::hasHealed = false;

 PlayerStats::PlayerStats(){}
 PlayerStats::~PlayerStats(){}

void PlayerStats::healUp() {
		lives += 1;
}

bool PlayerStats::hasLives() {
	return (lives > 0);
}


void PlayerStats::rockHit() {
	if (collectedEggs < 10) {
		lives -= 1;
	}
	else
	collectedEggs -= 10;
}

int PlayerStats::Lives() {
	return lives;
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

void PlayerStats::cleanse(float number) {
	brokenEggs = floor(brokenEggs / number);
}

bool PlayerStats::canHeal() {
	return (0 < lives && lives < 3);
}





