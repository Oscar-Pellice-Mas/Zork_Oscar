#ifndef __Creature__
#define __Creature__

#include <string>
#include "Entity.h"

class Room;
class Item;

using namespace std;

class Creature : public Entity {
public:
	Creature(const char* name, const char* description, Room* location, int health, int attack, int defense);
	~Creature();

    Room* getLocation() const;
	void setLocation(Room* room);
    int getHealth() const;
	int getAttack() const; 
	int getDefense() const;

private:
	Room* location;

	int health;
	int attack;
	int defense;
};
#endif