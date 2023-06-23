#ifndef __Exit__
#define __Exit__

#include <string>
#include "Entity.h"

class Room;
class Item;

using namespace std;

enum class Direction {
	NORTH,
	SOUTH,
	EAST,
	WEST,
	INCORRECT
};

enum class DoorState {
	OPEN,
	CLOSED
};


class Exit : public Entity {
public:
	Exit(const char* name, const char* description, Direction direction, Room* source, Room* destination, DoorState state, Item* key);
	~Exit();

	string getDescription() const;

	Direction getDirection() const;
	Room* getSource() const;
	Room* getDestination() const;
	Item* getKey() const;
	bool isLocked() const;
	bool unlock(Item* item);

private:
	Direction direction;
	Room* source;
	Room* destination; 
	
	DoorState state;
	Item* key;
};

#endif