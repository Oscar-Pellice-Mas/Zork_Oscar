#include <iostream>
#include "Global.h"
#include "Entity.h"

// ----- Constructor -----
Entity::Entity(const EntityType type, const char* name, const char* description) : type(type), name(name), description(description) {}

// ----- Deconstructor -----
Entity::~Entity() {
}

EntityType Entity::getEntityType() const {
    return type;
}

string Entity::getName() const {
    return name;
}

string Entity::getDescription() const {
    return description;
}

