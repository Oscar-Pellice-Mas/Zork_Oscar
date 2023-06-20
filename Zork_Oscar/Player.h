#ifndef __Player__
#define __Player__

#include "Creature.h"

using namespace std;

class Player : public Creature {
public:
	Player(const string& name, const string& description, Room* location);
	~Player();

private:

};

#endif