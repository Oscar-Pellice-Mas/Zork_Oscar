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
    room2->addCreature(npc);

    // Add player and NPC to the world
    setPlayer(player);
    setEntity(npc);

}

// ----- Deconstructor -----
World::~World() {
	for (list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete* it;

	entities.clear();
}

// Entities setter
void World::setEntity(Entity* entity) {
    entities.push_back(entity);
}

// Entites getter
list<Entity*> World::getEntities() const {
    return entities;
}

// Plater setter
void World::setPlayer(Player* player) {
    World::player = player;
}

// Player getter
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

// Transforms string into Direction enum
Direction stringToDir(string parameter) {
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

// Transforms Direction enum into a string
inline const char* DirToString(Direction value) {
    switch (value) {
    case Direction::NORTH: return "North";
    case Direction::EAST: return "East"; 
    case Direction::WEST: return "West";
    case Direction::SOUTH: return "South";
    default: return "[Unknown Direction type]";
    }
}

int World::play() {
    
    // Create a list of commands synonims
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
        // Grab player input
        cout << endl;
        cout << "Enter your action: ";
        getline(cin, input);

        // Separates the input into command and parameters
        string command, parameter;
        size_t spacePos = input.find(' ');
        if (spacePos != string::npos) {
            command = input.substr(0, spacePos);
            parameter = input.substr(spacePos + 1);
        }
        else {
            command = input;
        }
        
        // Matches synonims giving the user input
        bool commandMatched = false;
        for (const Command& validCommand : commands) {
            if (matchesSynonyms(command, validCommand.synonyms)) {
                command = validCommand.name;
                commandMatched = true;
                break;
            }
        }

        // Command functionalities
        if (commandMatched) {
            if (commandMatched) {
                // Help action
                if (command == "help") {
                    cout << "Available commands:" << endl;
                    cout << "- look: Look around the room." << endl;
                    cout << "- take [item]: Take an item from the room." << endl;
                    cout << "- drop [item]: Drop an item from your inventory." << endl;
                    cout << "- move [direction]: Move to another room." << endl;
                    cout << "- open [door]: Open a door in the room." << endl;
                    cout << "- quit: Quit the game." << endl;
                }
                // Look action
                // Shows current room description or inspects an entity if given a parameter
                else if (command == "look") {
                    Room* currentRoom = player->getLocation();
                    //system("cls");
                    cout << currentRoom->getName() << endl;
                    cout << currentRoom->getDescription() << endl;
                    cout << endl;
                    cout << "Items in the room: ";
                    list<Item*> items = currentRoom->getItems();
                    if (items.empty()) {
                        cout << "None";
                    }
                    else {
                        for (Item* item : items) {
                            cout << item->getName() << ", ";
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
                            cout << creature->getName() << ", ";
                        }
                    }
                    cout << endl;
                    cout << "Exits on the room: ";
                    list<Exit*> exits = currentRoom->getExits();
                    if (exits.empty()) {
                        cout << "None";
                    }
                    else {
                        for (Exit* exit : exits) {
                            cout << exit->getName() << " on the ";
                            cout << DirToString(exit->getDirection()) << ", ";
                        }
                    }

                }
                // Take action
                // If given an item and its found on the room, add it to inventory
                else if (command == "take") {
                    if (parameter.empty()) {
                        cout << "Indicate an item to take." << endl;
                    }
                    else {
                        bool found = false;
                        string itemName = parameter;
                        Room* currentRoom = player->getLocation();
                        list<Item*> items = currentRoom->getItems();
                        for (Item* item : items) {
                            if (item->getName() == itemName) {
                                found = true;
                                player->addItem(item);
                                currentRoom->removeItem(item);
                                cout << "You took the " << item->getName() << "." << endl;
                                break;
                            }
                        }

                        if (!found) {
                            cout << "You couldn't take " << parameter << "." << endl;
                        }
                    }
                }
                // Drop action
                // If given an item and its found on the inventory, add it to the room
                else if (command == "drop") {
                    if (parameter.empty()) {
                        cout << "Indicate an item to drop." << endl;
                    }
                    else {
                        bool found = false;
                        string itemName = parameter;
                        Room* currentRoom = player->getLocation();
                        list<Item*> items = player->getItems();
                        for (Item* item : items) {
                            if (item->getName() == itemName) {
                                found = true;
                                player->removeItem(item);
                                currentRoom->addItem(item);
                                cout << "You dropped the " << item->getName() << "." << endl;
                                break;
                            }
                        }

                        if (!found) {
                            cout << "You couldn't drop " << parameter << "." << endl;
                        }
                    }
                    
                }
                // Move action
                // If given a direction, move to the room if exit is existent
                else if (command == "move") {
                    if (parameter.empty()) {
                        cout << "Indicate an direction to move." << endl;
                    }
                    else {
                        bool found = false;
                        Direction desiredDir = stringToDir(parameter);
                        if (desiredDir == Direction::INCORRECT) {
                            cout << "Indicate a valid direction to move." << endl;
                        }
                        else {
                            Room* currentRoom = player->getLocation();
                            list<Exit*> exits = currentRoom->getExits();
                            for (Exit* exit : exits) {
                                if (exit->getDirection() == desiredDir) {
                                    found = true;
                                    if (exit->isLocked()) {
                                        cout << "You can't move to " << parameter << ". The " << exit->getName() << " is locked." << endl;
                                    }
                                    else {
                                        Room* nextRoom = exit->getDestination();
                                        player->setLocation(nextRoom);
                                        cout << "You moved to " << nextRoom->getName() << "." << endl;
                                        break;
                                    }
                                }
                            }

                            if (!found) {
                                cout << "You couldn't move to " << parameter << "." << endl;
                            }
                        }
                        
                    }
                }
                // Open action
                // If given a targer, open the exit if locked.
                else if (command == "open") {
                    if (parameter.empty()) {
                        cout << "Indicate an exit to unlock." << endl;
                    }
                    else {
                        bool found = false;
                        Room* currentRoom = player->getLocation();
                        list<Exit*> exits = currentRoom->getExits();
                        for (Exit* exit : exits) {
                            if (exit->getName() == parameter) {
                                found = true;
                                if (exit->isLocked()) {
                                    if (player->removeItem(exit->getKey())) {
                                        exit->unlock(exit->getKey());
                                        cout << "You unlocked the door using the " << exit->getKey()->getName() << "." << endl;
                                    }
                                    else {
                                        cout << "The door is locked, and you couldn't open it." << endl;
                                    }
                                }
                                else {
                                    cout << "The door is already open." << endl;
                                }
                                break;
                            }
                        }

                        if (!found) {
                            cout << "There is no " << parameter << " to open." << endl;
                        }
                    }
                }
                // Exit action
                // Quits the game
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