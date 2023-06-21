#include <iostream>
#include "Room.h"
#include "Exit.h"
#include "Global.h"
#include "Item.h"

// ----- Constructor -----
Item::Item(const char* name, const char* description, ItemType itemType) : Entity(EntityType::ITEM, name, description), itemType(itemType) {}

// ----- Deconstructor -----
Item::~Item() {
}