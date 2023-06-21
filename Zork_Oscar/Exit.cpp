#include <iostream>
#include "Room.h"
#include "Exit.h"

// ----- Constructor -----
Exit::Exit(const char* name, const char* description, Direction direction, Room* source, Room* destination, DoorState state, Item* key) :
	Entity(EntityType::EXIT, name, description), direction(direction), source(source), destination(destination), state(state), key(key) {}

// ----- Deconstructor -----
Exit::~Exit() {
}

Direction Exit::getDirection() const {
    return direction;
}

Item* Exit::getKey() const {
    return key;
}

Room* Exit::getSource() const {
    return source;
}

Room* Exit::getDestination() const {
    return destination;
}

bool Exit::isLocked() const {
    return state == DoorState::CLOSED;
}

bool Exit::unlock(Item* item) {
    if (key == item) {
        state = DoorState::OPEN;
        return true;
    }
    return false;
}