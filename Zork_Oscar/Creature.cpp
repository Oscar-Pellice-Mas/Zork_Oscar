#include <iostream>
#include "Global.h"
#include "Room.h"
#include "Exit.h"
#include "Item.h"
#include "Creature.h"

// ----- Constructor -----
Creature::Creature(const string& name, const string& description, Room* location) : Entity(EntityType::CREATURE, name, description), location(location) {}

// ----- Deconstructor -----
Creature::~Creature() {
}

Room* Creature::getLocation() const {
    return location;
}

void Creature::setLocation(Room* room) {
    location = room;
}
