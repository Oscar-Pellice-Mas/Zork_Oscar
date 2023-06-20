#ifndef __Exit__
#define __Exit__

#include <string>
#include "Entity.h"

class Room;

using namespace std;

enum class Direction {
	NORTH,
	SOUTH,
	EAST,
	WEST
};


class Exit : public Entity {
public:
	Exit(const string& name, const string& description, Direction direction, Room* source, Room* destination);
	~Exit();

	Direction getDirection() const;
	Room* getSource() const;
	Room* getDestination() const;

public:
	Direction direction;
	Room* source;
	Room* destination;
};

#endif