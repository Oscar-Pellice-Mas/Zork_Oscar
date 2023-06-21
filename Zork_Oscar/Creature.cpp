#include <iostream>
#include "Global.h"
#include "Room.h"
#include "Exit.h"
#include "Item.h"
#include "Creature.h"

// ----- Constructor -----
Creature::Creature(const char* name, const char* description, Room* location, int health, int attack, int defense) :
    Entity(EntityType::CREATURE, name, description), location(location), health(health), attack(attack), defense(defense){}

// ----- Deconstructor -----
Creature::~Creature() {
}

Room* Creature::getLocation() const {
    return location;
}

void Creature::setLocation(Room* room) {
    location = room;
}

int Creature::getHealth() const {
    return health;
}

int Creature::getAttack() const {
    return attack;
}

int Creature::getDefense() const {
    return defense;
}
