#ifndef __Player__
#define __Player__

#include "Creature.h"

using namespace std;

class Player : public Creature {
public:
	Player(const char* name, const char* description, Room* location, int health, int attack, int defense);
	~Player();

private:

};

#endif