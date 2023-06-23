#include "World.h"

// ----- Constructor -----
World::World() {
    // Create items
    Item* key = new Item("Key", "A key", false, 0, 0);
    Item* box = new Item("Box", "A box", true, 1, 0);
    Item* sword = new Item("Sword", "A sword", true, 10, 0);

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
    Player* player = new Player("Player", "The main character", room1, 20, 10, 5);
    Creature* npc = new Creature("NPC", "A non-player character", room2, 20, 10, 5);
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
    for (const string& synonym : synonyms) {
        if (command == synonym) {
            return true;
        }
    }
    return false;
}

// Transforms string into Direction enum
Direction stringToDir(string value) {
    transform(value.begin(), value.end(), value.begin(), ::tolower);
    if (value == "north") {
        return Direction::NORTH;
    } 
    else if (value == "east") {
        return Direction::EAST;
    }
    else if (value == "west") {
        return Direction::WEST;
    }
    else if (value == "south") {
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

// Prints the Base of the UI
void printBase(Player* player) {
    system("CLS");
    cout << "_-- Zork --_" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Location: " << player->getLocation()->getName() << endl;
    cout << player->getLocation()->getDescription() << endl;
    cout << endl;
}

// Utility to lowercase a string
string stringToLower(string value) {
    string lowered = value;
    transform(lowered.begin(), lowered.end(), lowered.begin(), ::tolower);
    return lowered;
}

list<string> separateParameters(const string& input) {
    list<string> parameters;
    string parameter;
    size_t startIndex = 0;
    size_t spacePos = input.find(' ');

    while (spacePos != string::npos) {
        parameter = input.substr(startIndex, spacePos - startIndex);
        parameters.push_back(parameter);
        startIndex = spacePos + 1;
        spacePos = input.find(' ', startIndex);
    }

    parameter = input.substr(startIndex);
    parameters.push_back(parameter);

    return parameters;
}

void World::helpCommand() {
    cout << "Available commands:" << endl;
    cout << "- look: Look around the room." << endl;
    cout << "- take [item]: Take an item from the room." << endl;
    cout << "- drop [item]: Drop an item from your inventory." << endl;
    cout << "- move [direction]: Move to another room." << endl;
    cout << "- open [exit]: Open a door in the room." << endl;
    cout << "- attack [creature]: Attack a creature." << endl;
    cout << "- talk [creature]: Talk with a creature." << endl;
    cout << "- quit: Quit the game." << endl;
}

void World::lookCommand() {
    Room* currentRoom = player->getLocation();
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
            if (creature->getHealth() > 0) cout << creature->getName() << ", ";
            else cout << "The corpse of " << creature->getName() << ", ";
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
    cout << endl;
}

void World::takeCommand(string parameter) {
    if (parameter.empty()) {
        cout << "Indicate an item to take." << endl;
    }
    else {
        bool found = false;
        string itemName = parameter;
        Room* currentRoom = player->getLocation();
        list<Item*> items = currentRoom->getItems();
        for (Item* item : items) {
            if (stringToLower(item->getName()) == itemName) {
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

void World::takeCommand(list<string> parameters) {
    if (parameters.size() < 3) {
        cout << "Wrong command. Check \"help\" for instructions." << endl;
    }
    else {
        bool recipientFound, itemFound = false;

        // First parameter is container
        list<string>::iterator it = parameters.begin();
        string itemContainer = *it;
        // Second will be ignored for simplicity
        // Third is the item
        advance(it, 5);
        string itemName = *it;

        // Make the search from inventory and room
        Room* currentRoom = player->getLocation();
        list<Item*> items = currentRoom->getItems();
        for (Item* item : items) {
            if (stringToLower(item->getName()) == stringToLower(itemContainer)) {
                recipientFound = true;
                list<Item*> containerItems = item->getItems();
                for (Item* containerItem : containerItems) {
                    if (stringToLower(containerItem->getName()) == stringToLower(itemName)) {
                        player->addItem(item);
                        containerItem->removeItems(item);
                        cout << "You took the " << containerItem->getName() << " from the " << item->getName() << "." << endl;
                        break;
                    }
                }
                
            }
        }

        if (!recipientFound || !itemFound) {
            list<Item*> items = player->getItems();
            for (Item* item : items) {
                if (stringToLower(item->getName()) == stringToLower(itemContainer)) {
                    recipientFound = true;
                    list<Item*> containerItems = item->getItems();
                    for (Item* containerItem : containerItems) {
                        if (stringToLower(containerItem->getName()) == stringToLower(itemName)) {
                            player->addItem(item);
                            containerItem->removeItems(item);
                            cout << "You took the " << containerItem->getName() << " from the " << item->getName() << "." << endl;
                            break;
                        }
                    }

                }
            }
        }

        // Output if items not found
        if (recipientFound && !itemFound) {
            cout << "You couldn't find " << itemName << " in " << itemContainer << "." << endl;
        } else if (!recipientFound) {
            cout << "You couldn't find " << itemContainer << "." << endl;
        }
    }
}

void World::dropCommand(string parameter) {
    if (parameter.empty()) {
        cout << "Indicate an item to drop." << endl;
    }
    else {
        bool found = false;
        string itemName = parameter;
        Room* currentRoom = player->getLocation();
        list<Item*> items = player->getItems();
        for (Item* item : items) {
            if (stringToLower(item->getName()) == itemName) {
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

void World::moveCommand(string parameter) {
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
                        printBase(player);
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

void World::openCommand(string parameter) {
    if (parameter.empty()) {
        cout << "Indicate an exit to unlock." << endl;
    }
    else {
        bool found = false;
        Room* currentRoom = player->getLocation();
        list<Exit*> exits = currentRoom->getExits();
        for (Exit* exit : exits) {
            if (stringToLower(exit->getName()) == parameter) {
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

void World::attackCommand(string parameter) {
    if (parameter.empty()) {
        cout << "Indicate a target to attack." << endl;
    }
    else {
        bool found = false;
        Room* currentRoom = player->getLocation();
        list<Creature*> creatures = currentRoom->getCreatures();
        for (Creature* creature : creatures) {
            if (stringToLower(creature->getName()) == parameter) {
                found = true;
                int damage = getPlayer()->makeAttack(creature);
                cout << "You deal " << damage << " points of damage to " << creature->getName() << endl;
                if (creature->getHealth() > 0) {
                    cout << "The creature is still standing." << creature->getHealth() << endl;
                }
                else {
                    cout << "You slayed the creature." << endl;
                }
            }
        }

        if (!found) {
            cout << "There is no " << parameter << " in the room." << endl;
        }
    }
}

void World::equipCommand(string parameter) {
    if (parameter.empty()) {
        cout << "Indicate an item to equip." << endl;
    }
    else {
        bool found = false;
        list<Item*> items = player->getItems();
        for (Item* item : items) {
            if (stringToLower(item->getName()) == parameter) {
                found = true;
                if (getPlayer()->equipItem(item)) {
                    cout << "You equipped " << item->getName() << endl;
                }
                else {
                    cout << "You can't equip " << item->getName() << endl;
                }
            }
        }

        if (!found) {
            cout << "There is no " << parameter << " in your inventory." << endl;
        }
    }
}

void World::unequipCommand(string parameter) {
    if (parameter.empty()) {
        cout << "Indicate an item to unequip." << endl;
    }
    else {
        bool found = false;
        list<Item*> items = player->getEquippedIems();
        for (Item* item : items) {
            if (stringToLower(item->getName()) == parameter) {
                found = true;
                getPlayer()->unequipItem(item);
                cout << "You unequipped " << item->getName() << endl;
            }
        }

        if (!found) {
            cout << "There is no " << parameter << " in your inventory." << endl;
        }
    }
}

void World::inspectCommand(string parameter) {
    if (parameter.empty()) {
        cout << "Indicate an entity to inspect." << endl;
    }
    else {
        bool found = false;

        // Equipped items
        if (!found) {
            list<Item*> items = player->getEquippedIems();
            for (Item* item : items) {
                if (stringToLower(item->getName()) == parameter) {
                    found = true;
                    cout << "You inspect the " << item->getName() << " you have equiped." << endl;
                    cout << item->getDescription() << endl;
                    break;
                }
            }
        }

        // Inventory items
        if (!found) {
            list<Item*> items = player->getItems();
            for (Item* item : items) {
                if (stringToLower(item->getName()) == parameter) {
                    found = true;
                    cout << "You inspect the " << item->getName() << " on your inventory." << endl;
                    cout << item->getDescription() << endl;
                    break;
                }
            }
        }

        // Room exits
        if (!found) {
            Room* currentRoom = player->getLocation();
            list<Exit*> exits = currentRoom->getExits();
            for (Exit* exit : exits) {
                if (stringToLower(exit->getName()) == parameter) {
                    found = true;
                    cout << "You inspect the " << exit->getName() << " on the room." << endl;
                    cout << exit->getDescription() << endl;
                    break;
                }
            }
        }

        // Room items
        if (!found) {
            Room* currentRoom = player->getLocation();
            list<Item*> items = currentRoom->getItems();
            for (Item* item : items) {
                if (stringToLower(item->getName()) == parameter) {
                    found = true;
                    cout << "You inspect the " << item->getName() << " on the room." << endl;
                    cout << item->getDescription() << endl;
                    break;
                }
            }
        }

        // Creatures
        if (!found) {
            Room* currentRoom = player->getLocation();
            list<Creature*> creatures = currentRoom->getCreatures();
            for (Creature* creature : creatures) {
                if (stringToLower(creature->getName()) == parameter) {
                    found = true;
                    cout << "You inspect the " << creature->getName() << " on the room." << endl;
                    cout << creature->getDescription() << endl;
                    break;
                }
            }
        }

        // Not found
        if (!found) {
            cout << "There is no " << parameter << " to inspect." << endl;
        }
    }
}

int World::play() {
    
    // Create a list of commands synonims
    vector<Command> commands = {
        { "help", { "help", "info" } },
        { "look", { "look", "observe", "see"}},
        { "take", { "take", "grab", "get"}},
        { "drop", { "drop", "discard", "throw"}},
        { "move", { "move", "go", "enter" } },
        { "open", { "open", "unlock" } },
        { "attack", { "attack", "fight", "hit"}},
        { "equip", { "equip", "wear" } },
        { "unequip", { "unequip", "remove" } },
        { "inspect", { "inspect", "investigate" } },
        { "quit", { "quit", "exit" } }
    };

    // Game loop
    system("CLS");
    cout << "_-- Zork --_" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Location: " << player->getLocation()->getName() << endl;
    cout << player->getLocation()->getDescription() << endl;

    string input;
    bool gameOver = false;
    while (!gameOver) {

        // Grab player input
        cout << "---------------------------------------" << endl;
        cout << "Enter your action:";
        getline(cin, input);

        printBase(player);

        // Separates the input into command and parameters
        string command, parameter;
        list<string> parameters;
        size_t spacePos = input.find(' ');
        if (spacePos != string::npos) {
            command = input.substr(0, spacePos);
            parameter = input.substr(spacePos + 1);
            parameter = stringToLower(parameter);
            parameters = separateParameters(parameter);
        }
        else {
            command = input;
        }
        command = stringToLower(command);
 
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
                    helpCommand();
                }
                // Look action
                // Shows current room description or inspects an entity if given a parameter
                else if (command == "look") {
                    lookCommand();
                }
                // Take action
                // If given an item and its found on the room, add it to inventory
                else if (command == "take") {
                    takeCommand(parameter);
                }
                // Drop action
                // If given an item and its found on the inventory, add it to the room
                else if (command == "drop") {
                    dropCommand(parameter);
                }
                // Move action
                // If given a direction, move to the room if exit is existent
                else if (command == "move") {
                    moveCommand(parameter);
                }
                // Open action
                // If given a targer, open the exit if locked.
                else if (command == "open") {
                    openCommand(parameter);
                }
                // Attack action
                // Select a creature and makes an attack
                else if (command == "attack") {
                    attackCommand(parameter);
                }
                // Equip action
                // Select a item to equip
                else if (command == "equip") {
                    equipCommand(parameter);
                }
                // Unequip action
                // Select a item to equip
                else if (command == "unequip") {
                    unequipCommand(parameter);
                }
                // Investigate action
                // Select a item to investigate
                else if (command == "inspect") {
                    inspectCommand(parameter);
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