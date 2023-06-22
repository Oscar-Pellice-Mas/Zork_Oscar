#ifndef __Item__
#define __Item__

#include <string>
#include "Entity.h"

class Room;

using namespace std;

class Item : public Entity {
public:
	Item(const char* name, const char* description, bool equipable, int attack, int defense);
	~Item();

	int getAttack() const;
	int getDefense() const;
	bool isEquipable() const;

private:
	bool equipable;
	int attack;
	int defense;
};

#endif