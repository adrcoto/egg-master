#pragma once
class PlayerStats {
private:
	static int lives;
	static int lvl;
	static int collectedEggs;
	static int brokenEggs;
	static bool lvledUp;
public:
	PlayerStats();
	~PlayerStats();

	static bool hasLives();
	static void rockHit();
	static int Lives();

	static void levelUp();
	static int level();
	static bool hasLeveledUp();
	static void leveledUp(bool);
	
	static void eggCollected();
	static int eggsCollected();

	static void eggDropped();
	static int  eggsBroken();

	static bool win;
	static bool gameOver;
};

