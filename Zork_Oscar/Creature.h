#ifndef __Creature__
#define __Creature__

#include <string>
#include "Entity.h"

class Room;
class Item;

using namespace std;

class Creature : public Entity {
public:
	Creature(const string& name, const string& description, Room* location);
	~Creature();

    Room* getLocation() const;
	void setLocation(Room* room);

private:
	Room* location;
};

#endif