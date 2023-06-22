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

	void addItem(Item* item);
	bool removeItem(Item* item);
	list<Item*> getItems() const;
	
    int getHealth() const;
	int getAttack() const; 
	int getDefense() const;

	bool equipItem(Item* item);
	void unequipItem(Item* item);
	list<Item*> getEquippedIems();

	void takeDamage(int damage);
	void makeAttack(Creature* target);

private:
	Room* location;

	list<Item*> inventory;
	list<Item*> equippedItems;
	int health;
	int attack;
	int defense;
};
#endif