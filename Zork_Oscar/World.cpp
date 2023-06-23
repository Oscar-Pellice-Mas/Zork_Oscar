#include "World.h"

// ----- Constructor -----
World::World() {
    // Create items
    Item* key = new Item("Key", "A key", false, 0, 0);
    Item* box = new Item("Box", "A box", true, 1, 0);
    Item* pen = new Item("Pen", "A pen", true, 2, 0);
    Item* sword = new Item("Sword", "A sword", true, 10, 0);

    box->addItems(pen);

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