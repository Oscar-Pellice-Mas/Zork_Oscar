#include <iostream>
#include "Global.h"
#include "Room.h"
#include "Exit.h"
#include "Item.h"
#include "Creature.h"

// ----- Constructor -----
Creature::Creature(const char* name, const char* description, Room* location, int health, int attack, int defense, string talk, Mood mood) :
    Entity(EntityType::CREATURE, name, description), location(location), health(health), attack(attack), defense(defense), talk(talk), mood(PASSIVE){}

// ----- Deconstructor -----
Creature::~Creature() {
}

string Creature::getDescription() const {
    return Entity::getDescription();
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

bool Creature::equipItem(Item* item) {
    if (item->isEquipable()) {
        equippedItems.push_back(item);
        inventory.remove(item);
        attack += item->getAttack();
        defense += item->getDefense();
        return true;
    }
        
    return false;
}

void Creature::unequipItem(Item* item) {
    equippedItems.remove(item);
    inventory.push_back(item);
    attack -= item->getAttack();
    defense -= item->getDefense();
}


list<Item*> Creature::getEquippedIems() {
    return equippedItems;
}

void Creature::takeDamage(int damage) {
    if (damage > 0) {
        health -= damage;
    }
}

int Creature::makeAttack(Creature* target) {
    int damage = attack - target->getDefense();
    if (damage > 0) {
        target->takeDamage(damage);
    }
    return damage;
}

string Creature::getTalk() {
    return talk;
}

Mood Creature::getMood() {
    return mood;
}

void Creature::setMood(Mood value) {
    mood = value;
}
