#ifndef __Room__
#define __Room__

#include <string>
#include <list>
#include "Entity.h"

class Exit;
class Item;

using namespace std;

class Room : public Entity {
public:
    Room(const string& name, const string& description);
	~Room();

    void addItem(Item* item);
    bool removeItem(Item* item);
    list<Item*> getItems() const;
    void addExit(Exit* exit);
    list<Exit*> getExits() const;

private:
	list<Item*> items;
	list<Exit*> exits;
};

#endif