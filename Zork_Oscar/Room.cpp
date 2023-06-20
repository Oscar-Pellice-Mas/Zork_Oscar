#include <iostream>
#include "Global.h"
#include "Exit.h"
#include "Item.h"
#include "Creature.h"
#include "Room.h"

// ----- Constructor -----
Room::Room(const string& name, const string& description) : Entity(EntityType::ROOM, name, description) {}

// ----- Deconstructor -----
Room::~Room() {
}

void Room::addItem(Item* item) {
    items.push_back(item);
}

bool Room::removeItem(Item* item) {
    auto value = find(items.begin(), items.end(), item);
    if (value != items.end()) {
        items.erase(value);
        return true;
    }
    return false;
}

list<Item*> Room::getItems() const {
    return items;
}

void Room::addExit(Exit* exit) {
    exits.push_back(exit);
}

list<Exit*> Room::getExits() const {
    return exits;
}