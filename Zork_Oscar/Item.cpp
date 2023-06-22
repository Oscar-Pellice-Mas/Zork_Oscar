#include <iostream>
#include "Room.h"
#include "Exit.h"
#include "Global.h"
#include "Item.h"

// ----- Constructor -----
Item::Item(const char* name, const char* description, bool equipable, int attack, int defense) 
    : Entity(EntityType::ITEM, name, description), equipable(equipable), attack(attack), defense(defense) {}

// ----- Deconstructor -----
Item::~Item() {
}

int Item::getAttack() const {
    return attack;
}

int Item::getDefense() const {
    return defense;
}

bool Item::isEquipable() const {
    return equipable;
}
