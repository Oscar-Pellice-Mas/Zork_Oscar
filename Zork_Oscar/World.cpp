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
    // Create items
    Item* key = new Item("Key", "A key", ItemType::COMMON);
    Item* box = new Item("Box", "A box", ItemType::COMMON);
    Item* sword = new Item("Sword", "A sword", ItemType::WEAPON);

    // Create rooms
    Room* room1 = new Room("Room 1", "This is the first room.");
    Room* room2 = new Room("Room 2", "This is the second room.");
    Room* room3 = new Room("Room 3", "This is the third room.");

    // Create exits
    Exit* exit1 = new Exit("Door", "A wood door", Direction::NORTH, room1, room2, DoorState::CLOSED, key);
    Exit* exit2 = new Exit("Window", "An opened window", Direction::SOUTH, room2, room3, DoorState::OPEN, NULL);

    // Add exits to rooms
    room1->addExit(exit1);
    room2->addExit(exit2);

    // Add items to rooms
    room1->addItem(key);
    room2->addItem(box);
    room3->addItem(sword);

    // Create player and NPC
    Player* player = new Player("Player", "The main character", room1, 100, 10, 5);
    Creature* npc = new Creature("NPC", "A non-player character", room2, 80, 8, 4);

    // Add player and NPC to the world
    addPlayer(player);
    addEntity(npc);

}

// ----- Deconstructor -----
World::~World() {
	for (list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete* it;

	entities.clear();
}

void World::addEntity(Entity* entity) {
    entities.push_back(entity);
}

list<Entity*> World::getEntities() const {
    return entities;
}

void World::addPlayer(Player* player) {
    World::player = player;
}

Player* World::getPlayer() {
    return player;
}

int World::play() {

    // Game loop
    cout << "Welcome to the game!" << endl;
    cout << getPlayer()->getLocation()->getDescription()  << endl;
    cout << "Type 'help' for instructions." << endl;

    return 0;
}