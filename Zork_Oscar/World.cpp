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
    cout << player->getLocation()->getDescription() << endl;
    cout << "Type 'help' for instructions." << endl;

    string command;
    bool gameOver = false;
    while (!gameOver) {
        cout << "> ";
        getline(cin, command);

        if (command == "help") {
            cout << "Available commands:" << endl;
            cout << "- look: Look around the room." << endl;
            cout << "- take [item]: Take an item from the room." << endl;
            cout << "- drop [item]: Drop an item from your inventory." << endl;
            cout << "- move [direction]: Move to another room." << endl;
            cout << "- open [door]: Open a door in the room." << endl;
            cout << "- attack [creature]: Attack a creature in the room." << endl;
            cout << "- quit: Quit the game." << endl;
        } else if (command == "look") {
            Room* currentRoom = player->getLocation();
            cout << currentRoom->getDescription() << endl;
            cout << "Items in the room: ";
            list<Item*> items = currentRoom->getItems();
            if (items.empty()) {
                cout << "None";
            }
            else {
                for (Item* item : items) {
                    cout << item->getName() << " ";
                }
            }
            cout << endl;
            cout << "Creatures in the room: ";
            list<Creature*> creatures = currentRoom->getCreatures();
            if (creatures.empty()) {
                cout << "None";
            }
            else {
                for (Creature* creature : creatures) {
                    cout << creature->getName() << " ";
                }
            }
            cout << endl;
        } else if (command.substr(0, 4) == "take") {
            string itemName = command.substr(5);
            // Take item logic
            Room* currentRoom = player->getLocation();
            list<Item*> items = currentRoom->getItems();
            for (Item* item : items) {
                if (item->getName() == itemName) {
                    player->addItem(item);
                    currentRoom->removeItem(item);
                    cout << "You took the " << item->getName() << "." << endl;
                    break;
                }
            }
        }
        else if (command.substr(0, 4) == "drop") {
            string itemName = command.substr(5);
            // Drop item logic
            Room* currentRoom = player->getLocation();
            list<Item*> items = player->getItems();
            for (Item* item : items) {
                if (item->getName() == itemName) {
                    player->removeItem(item);
                    currentRoom->addItem(item);
                    cout << "You dropped the " << item->getName() << "." << endl;
                    break;
                }
            }
        }
        /*else if (command.substr(0, 4) == "move") {
            string direction = command.substr(5);
            // Move to another room logic
            Room* currentRoom = player->getLocation();
            list<Exit*> exits = currentRoom->getExits();
            for (Exit* exit : exits) {
                if (exit->getDirection() == direction) {
                    Room* nextRoom = exit->getDestination();
                    player->setLocation(nextRoom);
                    cout << "You moved to " << nextRoom->getName() << "." << endl;
                    break;
                }
            }
        }*/
        else if (command.substr(0, 4) == "open") {
            string doorName = command.substr(5);
            // Open door logic
            Room* currentRoom = player->getLocation();
            list<Exit*> exits = currentRoom->getExits();
            for (Exit* exit : exits) {
                if (exit->isLocked()) {
                    if (player->removeItem(exit->getKey())) {
                        exit->unlock(exit->getKey());
                        cout << "You unlocked the door using the " << exit->getKey()->getName() << "." << endl;
                    }
                    else {
                        cout << "The door is locked, and you don't have the key." << endl;
                    }
                }
                else {
                    cout << "The door is already open." << endl;
                }
            }
        }
        else if (command == "quit") {
            gameOver = true;
            cout << "Goodbye!" << endl;
        }
        else {
            cout << "Invalid command. Type 'help' for instructions." << endl;
        }
    }

    return 0;
}