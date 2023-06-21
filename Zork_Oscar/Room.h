#ifndef __Room__
#define __Room__

#include <string>
#include <list>
#include "Entity.h"

class Exit;
class Item;
class Creature;

using namespace std;

class Room : public Entity {
public:
    Room(const char* name, const char* description);
	~Room();

    void addItem(Item* item);
    bool removeItem(Item* item);
    list<Item*> getItems() const;

    void addExit(Exit* exit);
    list<Exit*> getExits() const;

    void addCreature(Creature* creature);
    bool removeCreature(Creature* creature);
    list<Creature*> getCreatures() const;

private:
	list<Item*> items;
	list<Exit*> exits;
    list<Creature*> creatures;
};

#endif