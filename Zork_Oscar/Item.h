#ifndef __Item__
#define __Item__

#include <string>
#include "Entity.h"

class Room;

using namespace std;

enum ItemType {
	COMMON,
	WEAPON,
	ARMOUR
};

class Item : public Entity {
public:
	Item(const char* name, const char* description, ItemType itemType);
	~Item();

private:
	ItemType itemType;
};

#endif