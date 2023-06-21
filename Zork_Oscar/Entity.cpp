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

void Entity::addEntity(Entity* item) {
    contains.push_back(item);
}

bool Entity::removeEntity(Entity* item) {
    auto it = find(contains.begin(), contains.end(), item);
    if (it != contains.end()) {
        contains.erase(it);
        return true;
    }
    return false;
}

list<Entity*> Entity::getEntity() const {
    return contains;
}