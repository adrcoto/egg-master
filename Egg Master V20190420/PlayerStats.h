#pragma once
#include <map>
#include <string>

class PlayerStats {
private:
	static int lives;
	static int collectedEggs;
	static int brokenEggs;
public:
	PlayerStats();
	~PlayerStats();

	static int Lives();
	static bool hasLives();
	static void healUp();
	static void rockHit();

	static void eggCollected();
	static int eggsCollected();

	static void eggDropped();
	static int  eggsBroken();

	static bool win;
	static bool gameOver;

	static void cleanse(float);

	static bool hasHealed;
	static bool heal;
	static bool canHeal();

};

