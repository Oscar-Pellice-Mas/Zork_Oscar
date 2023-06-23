#include "World.h"

// ----- Constructor -----
World::World() {
    // Create items
    Item* key = new Item("Key", "A small intricate key. Must be usefull to open a door.", false, 0, 0);
    Item* box = new Item("Box", "A metal box. It has something inside.", true, 1, 0);
    Item* stick = new Item("Stick", "A wooden stick. It must hurt to hit with this.", true, 2, 0);
    Item* sword = new Item("Sword", "A well forged sword. It will cut through enemies.", true, 10, 0);
    Item* dagger = new Item("Dagger", "A rusty dagger. It's about to break.", true, 5, 0);
    Item* armor = new Item("Armor", "A sturdy armor. It will ensure you don't get damaged.", true, 0, 10);
    Item* note = new Item("Note", "It reads: This sword is good but you should visit the armory.", false, 0, 0);
    Item* money = new Item("Money", "A good stack of money.", false, 0, 0);
    Item* goldKey = new Item("Gold key", "A key to freedom.", false, 0, 0);

    box->addItems(key);

    // Create rooms
    Room* cell = new Room("Cell", "This is an empty, ugly and small cell.");
    Room* room = new Room("Room", "A cozy living room with a table and a chair.");
    Room* armory = new Room("Armory", "A well-equipped goblin armory.");
    Room* hallway = new Room("Hallway", "A narrow hallway that connects rooms.");
    Room* study = new Room("Study", "A quiet study room filled with books.");
    Room* garden = new Room("Garden", "A beautiful garden with colorful flowers.");
    Room* outside = new Room("Outside", "You are out. You are free!");

    // Create exits
    Exit* cellDoor = new Exit("Cell Door", "A door with metal bars.", Direction::EAST, cell, room, DoorState::OPEN, NULL);
    Exit* cellDoor2 = new Exit("Cell Door", "A door with metal bars.", Direction::WEST, room, cell, DoorState::OPEN, NULL);
    Exit* metalDoor = new Exit("Metal Door", "A metal door with a lock. It reads: Armory.", Direction::EAST, room, armory, DoorState::CLOSED, key);
    Exit* metalDoor2 = new Exit("Metal Door", "A metal door.", Direction::WEST, armory, room, DoorState::OPEN, NULL);
    Exit* window = new Exit("Window", "A big window with views on the garden.", Direction::SOUTH, armory, garden, DoorState::OPEN, NULL);
    Exit* window2 = new Exit("Window", "A big window. It's a bit too high", Direction::NORTH, armory, garden, DoorState::CLOSED, NULL);
    Exit* passage = new Exit("Passage", "A passage that leads to a hallway.", Direction::SOUTH, room, hallway, DoorState::OPEN, NULL);
    Exit* passage2 = new Exit("Passage", "A passage that leads to a room.", Direction::NORTH, hallway, room, DoorState::OPEN, NULL);
    Exit* studyDoor = new Exit("Wood Door", "An intricate wooden door.", Direction::EAST, hallway, study, DoorState::OPEN, NULL);
    Exit* studyDoor2 = new Exit("Wood Door", "An intricate wooden door.", Direction::WEST, study, hallway, DoorState::OPEN, NULL);
    Exit* gardenDoor = new Exit("Glass Door", "The garden can be seen through the door glass.", Direction::EAST, study, garden, DoorState::OPEN, NULL);
    Exit* gardenDoor2 = new Exit("Glass Door", "The study can be seen through the door glass.", Direction::WEST, garden, study, DoorState::OPEN, NULL);
    Exit* exitDoor = new Exit("Exit Door", "The door to the outside. It has a lock.", Direction::WEST, garden, study, DoorState::CLOSED, goldKey);

    // Add exits to rooms
    cell->addExit(cellDoor);
    room->addExit(cellDoor2);
    room->addExit(metalDoor);
    room->addExit(passage);
    armory->addExit(metalDoor2);
    armory->addExit(window);
    hallway->addExit(passage2);
    hallway->addExit(studyDoor);
    study->addExit(studyDoor2);
    study->addExit(gardenDoor);
    garden->addExit(window2);
    garden->addExit(gardenDoor2);
    garden->addExit(exitDoor);

    // Add items to rooms
    room->addItem(box);
    hallway->addItem(note);
    armory->addItem(sword);
    armory->addItem(armor);
    study->addItem(money);

    // Create player and NPC
    Player* player = new Player("Player", "The main character.", cell, 20, 10, 5);
    Creature* goblin = new Creature("Goblin", "A small goblin with a knife.", hallway, 11, 10, 5);
    Creature* boss = new Creature("Boss", "A big and bad goblin.", garden, 20, 15, 10);
    hallway->addCreature(goblin);
    garden->addCreature(boss);

    // Add player and NPC to the world
    setPlayer(player);
    setEntity(goblin);
    setEntity(boss);

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

// Utility to lowercase a string
string stringToLowercase(string value) {
    string lowered = value;
    transform(lowered.begin(), lowered.end(), lowered.begin(), ::tolower);
    return lowered;
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
        { "inventory", { "inventory", "items" } },
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

        player->printBase();

        // Separates the input into command and parameters
        string command, parameter;
        list<string> parameters;
        size_t spacePos = input.find(' ');
        if (spacePos != string::npos) {
            command = input.substr(0, spacePos);
            parameter = input.substr(spacePos + 1);
            parameter = stringToLowercase(parameter);
            parameters = separateParameters(parameter);
        }
        else {
            command = input;
        }
        command = stringToLowercase(command);
 
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
                    player->helpCommand();
                }
                // Look action
                // Shows current room description or inspects an entity if given a parameter
                else if (command == "look") {
                    player->lookCommand();
                }
                // Take action
                // If given an item and its found on the room, add it to inventory
                else if (command == "take") {
                    if (parameters.size() == 1) player->takeCommand(parameter);
                    else player->takeCommand(parameters);
                }
                // Drop action
                // If given an item and its found on the inventory, add it to the room
                else if (command == "drop") {
                    player->dropCommand(parameter);
                }
                // Move action
                // If given a direction, move to the room if exit is existent
                else if (command == "move") {
                    player->moveCommand(parameter);
                }
                // Open action
                // If given a targer, open the exit if locked.
                else if (command == "open") {
                    player->openCommand(parameter);
                }
                // Attack action
                // Select a creature and makes an attack
                else if (command == "attack") {
                    player->attackCommand(parameter);
                }
                // Equip action
                // Select a item to equip
                else if (command == "equip") {
                    player->equipCommand(parameter);
                }
                // Unequip action
                // Select a item to equip
                else if (command == "unequip") {
                    player->unequipCommand(parameter);
                }
                // Investigate action
                // Select a item to investigate
                else if (command == "inspect") {
                    player->inspectCommand(parameter);
                }
                // Inventory action
                // Shows inventory of the player
                else if (command == "inventory") {
                    player->inventoryCommand();
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