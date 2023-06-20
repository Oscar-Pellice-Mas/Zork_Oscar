#include <iostream>
#include "Global.h"
#include "Entity.h"
#include "Creature.h"
#include "Item.h"
#include "Exit.h"
#include "Room.h"
#include "Player.h"
#include "World.h"

// ----- Constructor -----
World::World() {
    World world;

    // Create items
    Item* key = new Item("Key", "A key", ItemType::COMMON);
    Item* box = new Item("Box", "A box", ItemType::COMMON);
    Item* sword = new Item("Sword", "A sword", ItemType::WEAPON);

    // Create rooms
    Room* room1 = new Room("Room 1", "This is the first room.");
    Room* room2 = new Room("Room 2", "This is the second room.");
    Room* room3 = new Room("Room 3", "This is the third room.");

    // Add items to rooms
    room1->addItem(key);
    room2->addItem(box);
    room3->addItem(sword);

    // Create player and NPC
    Player* player = new Player("Player", "The main character", room1);
    Creature* npc = new Creature("NPC", "A non-player character", room2);

    // Add player and NPC to the world
    world.addEntity(player);
    world.addEntity(npc);

    // Create exits
    Exit* exit1 = new Exit("Door", "A wood door", Direction::NORTH, room1, room2);
    Exit* exit2 = new Exit("Window", "An opened window", Direction::SOUTH, room2, room3);

    // Add exits to rooms
    room1->addExit(exit1);
    room2->addExit(exit2);
}

// ----- Deconstructor -----
World::~World() {
	for (vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete* it;

	entities.clear();
}

void World::addEntity(Entity* entity) {
    entities.push_back(entity);
}

vector<Entity*> World::getEntities() const {
    return entities;
}