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

// Struct to represent a command and its synonyms
struct Command {
    string name;
    vector<string> synonyms;
};

// Function to check if a command matches any of the given synonyms
bool matchesSynonyms(string command, const vector<string>& synonyms) {
    transform(command.begin(), command.end(), command.begin(), ::tolower);
    for (const string& synonym : synonyms) {
        if (command == synonym) {
            return true;
        }
    }
    return false;
}

Direction translateDirection(string parameter) {
    transform(parameter.begin(), parameter.end(), parameter.begin(), ::tolower);
    if (parameter == "north") {
        return Direction::NORTH;
    } 
    else if (parameter == "east") {
        return Direction::EAST;
    }
    else if (parameter == "west") {
        return Direction::WEST;
    }
    else if (parameter == "south") {
        return Direction::SOUTH;
    }
    else {
        return Direction::INCORRECT;
    }
}

int World::play() {
    
    vector<Command> commands = {
        { "help", { "help", "info" } },
        { "look", { "look", "observe" } },
        { "take", { "take", "grab" } },
        { "drop", { "drop", "discard" } },
        { "move", { "move", "go", "enter" } },
        { "open", { "open", "unlock" } },
        { "attack", { "attack", "fight" } },
        { "inspect", { "inspect", "examine" } },
        { "interact", { "interact", "talk" } },
        { "equip", { "equip", "wear" } },
        { "unequip", { "unequip", "remove" } },
        { "search", { "search", "look for" } },
        { "quit", { "quit", "exit" } }
    };

    // Game loop
    cout << "Welcome to the game!" << endl;
    cout << player->getLocation()->getDescription() << endl;
    cout << "Type 'help' for instructions." << endl;

    string input;
    bool gameOver = false;
    while (!gameOver) {
        cout << endl;
        cout << "Enter your action: ";
        getline(cin, input);


        string command, parameter;
        size_t spacePos = input.find(' ');
        if (spacePos != string::npos) {
            command = input.substr(0, spacePos);
            parameter = input.substr(spacePos + 1);
        }
        else {
            command = input;
        }
        

        bool commandMatched = false;
        for (const Command& validCommand : commands) {
            if (matchesSynonyms(command, validCommand.synonyms)) {
                command = validCommand.name;
                commandMatched = true;
                break;
            }
        }

        if (commandMatched) {
            if (commandMatched) {
                if (command == "help") {
                    cout << "Available commands:" << endl;
                    cout << "- look: Look around the room." << endl;
                    cout << "- take [item]: Take an item from the room." << endl;
                    cout << "- drop [item]: Drop an item from your inventory." << endl;
                    cout << "- move [direction]: Move to another room." << endl;
                    cout << "- open [door]: Open a door in the room." << endl;
                    cout << "- quit: Quit the game." << endl;
                }
                else if (command == "look") {
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
                }
                else if (command == "take") {
                    string itemName = parameter;
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
                else if (command == "drop") {
                    string itemName = parameter;
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
                else if (command == "move") {
                    string direction = parameter;
                    Room* currentRoom = player->getLocation();
                    list<Exit*> exits = currentRoom->getExits();
                    for (Exit* exit : exits) {
                        if (exit->getDirection() == translateDirection(direction)) {
                            Room* nextRoom = exit->getDestination();
                            player->setLocation(nextRoom);
                            cout << "You moved to " << nextRoom->getName() << "." << endl;
                            break;
                        }
                    }
                }
                else if (command == "open") {
                    string doorName = parameter;
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
            }
        }
        else {
            cout << "Invalid command. Type 'help' for a list of commands." << endl;
        }
    }

    return 0;
}