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

void Creature::addItem(Item* item) {
    inventory.push_back(item);
}

bool Creature::removeItem(Item* item) {
    auto it = find(inventory.begin(), inventory.end(), item);
    if (it != inventory.end()) {
        inventory.erase(it);
        return true;
    }
    return false;
}

list<Item*> Creature::getItems() const {
    return inventory;
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

bool Creature::equipItem(Item* value) {
    for (Item* item : inventory) {
        if (item = value) {
            if (item->isEquipable()) {
                equippedItems.push_back(item);
                inventory.remove(value);
                attack += item->getAttack();
                defense += item->getDefense();
                return true;
            }
        }
    }
    return false;
}

bool Creature::unequipItem(Item* value) {
    for (Item* item : equippedItems) {
        if (item = value) {
            equippedItems.remove(item);
            inventory.push_back(item);
            attack -= item->getAttack();
            defense -= item->getDefense();
            return true;
        }
    }
    return false;
}

void Creature::takeDamage(int damage) {
    int actualDamage = damage - defense;
    if (actualDamage > 0) {
        health -= actualDamage;
    }
}

void Creature::makeAttack(Creature* target) {
    int damage = attack - target->getDefense();
    if (damage > 0) {
        target->takeDamage(damage);
    }
}
