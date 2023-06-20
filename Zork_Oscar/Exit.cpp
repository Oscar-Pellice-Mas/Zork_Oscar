#include <iostream>
#include "Room.h"
#include "Exit.h"

// ----- Constructor -----
Exit::Exit(const string& name, const string& description, Direction direction, Room* source, Room* destination) :
	Entity(EntityType::EXIT, name, description), direction(direction), source(source), destination(destination) {}

// ----- Deconstructor -----
Exit::~Exit() {
}

Direction Exit::getDirection() const {
    return direction;
}

Room* Exit::getSource() const {
    return source;
}

Room* Exit::getDestination() const {
    return destination;
}