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

void Item::addEntity(Item* item) {
    contains.push_back(item);
}

bool Item::removeEntity(Item* item) {
    auto it = find(contains.begin(), contains.end(), item);
    if (it != contains.end()) {
        contains.erase(it);
        return true;
    }
    return false;
}

list<Item*> Item::getEntity() const {
    return contains;
}