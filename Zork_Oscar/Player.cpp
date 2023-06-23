#include <iostream>
#include "Global.h"
#include "Room.h"
#include "Exit.h"
#include "Item.h"
#include "Player.h"

// ----- Constructor -----
Player::Player(const char* name, const char* description, Room* location, int health, int attack, int defense)
    : Creature(name, description, location, health, attack, defense) {}

// ----- Deconstructor -----
Player::~Player() {
}

string Player::getDescription() const {
    return Entity::getDescription();
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
void Player::printBase() {
    system("CLS");
    cout << "_-- Zork --_" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Health: " << getHealth() << endl;
    cout << "Attack: " << getAttack() << " | Defense: " << getDefense() << endl;
    cout << "---------------------------------------" << endl << endl;
    cout << "Location: " << getLocation()->getName() << endl;
    cout << getLocation()->getDescription() << endl;
    cout << endl;
}

// Utility to lowercase a string
string stringToLower(string value) {
    string lowered = value;
    transform(lowered.begin(), lowered.end(), lowered.begin(), ::tolower);
    return lowered;
}

void Player::helpCommand() {
    cout << "Available commands:" << endl;
    cout << "- look: Look around the room." << endl;
    cout << "- take [item]: Take an item from the room." << endl;
    cout << "- take [item] from [item]: Take an item from another item." << endl;
    cout << "- drop [item]: Drop an item from your inventory." << endl;
    cout << "- drop [item] into [item]: Drop an item into anotther item." << endl;
    cout << "- move [direction]: Move to another room." << endl;
    cout << "- open [exit]: Open a door in the room." << endl;
    cout << "- attack [creature]: Attack a creature." << endl;
    cout << "- inspect [entity]: Inspect an entity." << endl;
    cout << "- inventory : Shows the items of the player." << endl;
    cout << "- quit: Quit the game." << endl;
}

void Player::lookCommand() {
    Room* currentRoom = getLocation();
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

void Player::takeCommand(string parameter) {
    if (parameter.empty()) {
        cout << "Indicate an item to take." << endl;
    }
    else {
        bool found = false;
        string itemName = parameter;
        Room* currentRoom = getLocation();
        list<Item*> items = currentRoom->getItems();
        for (Item* item : items) {
            if (stringToLower(item->getName()) == itemName) {
                found = true;
                addItem(item);
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

void Player::takeCommand(list<string> parameters) {
    if (parameters.size() < 3) {
        cout << "Wrong command. Check \"help\" for instructions." << endl;
    }
    else {
        bool recipientFound = false;
        bool itemFound = false;

        // First parameter is container
        list<string>::iterator it = parameters.begin();
        string itemContainer = *it;
        // Second will be ignored for simplicity
        // Third is the item
        advance(it, 2);
        string itemName = *it;

        // Make the search from inventory and room
        Room* currentRoom = getLocation();
        list<Item*> items = currentRoom->getItems();
        for (Item* item : items) {
            if (stringToLower(item->getName()) == stringToLower(itemName)) {
                recipientFound = true;
                list<Item*> containerItems = item->getItems();
                for (Item* containerItem : containerItems) {
                    if (stringToLower(containerItem->getName()) == stringToLower(itemContainer)) {
                        itemFound = true;
                        addItem(containerItem);
                        containerItem->removeItems(item);
                        cout << "You took the " << containerItem->getName() << " from the " << item->getName() << "." << endl;
                        break;
                    }
                }

            }
        }

        if (!recipientFound || !itemFound) {
            list<Item*> items = getItems();
            for (Item* item : items) {
                if (stringToLower(item->getName()) == stringToLower(itemName)) {
                    recipientFound = true;
                    list<Item*> containerItems = item->getItems();
                    for (Item* containerItem : containerItems) {
                        if (stringToLower(containerItem->getName()) == stringToLower(itemContainer)) {
                            itemFound = true;
                            addItem(containerItem);
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
            cout << "You couldn't find " << itemContainer << " in " << itemName << "." << endl;
        }
        else if (!recipientFound) {
            cout << "You couldn't find " << itemName << "." << endl;
        }
    }
}

void Player::dropCommand(string parameter) {
    if (parameter.empty()) {
        cout << "Indicate an item to drop." << endl;
    }
    else {
        bool found = false;
        string itemName = parameter;
        Room* currentRoom = getLocation();
        list<Item*> items = getItems();
        for (Item* item : items) {
            if (stringToLower(item->getName()) == itemName) {
                found = true;
                removeItem(item);
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

bool Player::moveCommand(string parameter) {
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
            Room* currentRoom = getLocation();
            list<Exit*> exits = currentRoom->getExits();
            for (Exit* exit : exits) {
                if (exit->getDirection() == desiredDir) {
                    found = true;
                    if (exit->isLocked()) {
                        cout << "You can't move to " << parameter << ". The " << exit->getName() << " is locked." << endl;
                    }
                    else {
                        Room* nextRoom = exit->getDestination();
                        setLocation(nextRoom);
                        printBase();
                        
                        // If player gets to the exit
                        if (stringToLower(nextRoom->getName()) == "outside") {
                            printBase();
                            cout << "Congratulations!" << endl;
                            cout << "---------------------------------------" << endl;
                            return true;
                        }

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
    return false;
}

void Player::openCommand(string parameter) {
    if (parameter.empty()) {
        cout << "Indicate an exit to unlock." << endl;
    }
    else {
        bool found = false;
        Room* currentRoom = getLocation();
        list<Exit*> exits = currentRoom->getExits();
        for (Exit* exit : exits) {
            if (stringToLower(exit->getName()) == parameter) {
                found = true;
                if (exit->isLocked()) {
                    if (removeItem(exit->getKey())) {
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

bool Player::attackCommand(string parameter) {
    if (parameter.empty()) {
        cout << "Indicate a target to attack." << endl;
    }
    else {
        bool found = false;
        Room* currentRoom = getLocation();
        list<Creature*> creatures = currentRoom->getCreatures();
        for (Creature* creature : creatures) {
            if (stringToLower(creature->getName()) == parameter) {
                found = true;
                // Check if creature is alive
                if (creature->getHealth() <= 0) {
                    cout << "The " << parameter << " is already dead." << endl;
                    return false;
                }

                // Apply damage
                int damage = makeAttack(creature);
                if (creature->getHealth() > 0) {
                    damage = creature->makeAttack(this);
                    printBase();
                    cout << "You deal " << damage << " DMG to " << creature->getName() << endl;
                    cout << "The creature is still standing with " << creature->getHealth() << " Health." << endl;
                    cout << "The " << creature->getName() << " attacks you for " << damage << " DMG." << endl;
                    if (getHealth() <= 0) {
                        cout << "You have been slayed!" << endl;
                        cout << "---------------------------------------" << endl;
                        return true;
                    }
                }
                else {
                    cout << "You deal " << damage << " DMG to " << creature->getName() << endl;
                    cout << "You slayed the creature." << endl;
                }
                break;
            }
        }

        if (!found) {
            cout << "There is no " << parameter << " in the room." << endl;
        }
        return false;
    }
}

void Player::equipCommand(string parameter) {
    if (parameter.empty()) {
        cout << "Indicate an item to equip." << endl;
    }
    else {
        bool found = false;
        list<Item*> items = getItems();
        for (Item* item : items) {
            if (stringToLower(item->getName()) == parameter) {
                found = true;
                if (equipItem(item)) {
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

void Player::unequipCommand(string parameter) {
    if (parameter.empty()) {
        cout << "Indicate an item to unequip." << endl;
    }
    else {
        bool found = false;
        list<Item*> items = getEquippedIems();
        for (Item* item : items) {
            if (stringToLower(item->getName()) == parameter) {
                found = true;
                unequipItem(item);
                cout << "You unequipped " << item->getName() << endl;
            }
        }

        if (!found) {
            cout << "There is no " << parameter << " in your inventory." << endl;
        }
    }
}

void Player::inspectCommand(string parameter) {
    if (parameter.empty()) {
        cout << "Indicate an entity to inspect." << endl;
    }
    else {
        bool found = false;

        // Equipped items
        if (!found) {
            list<Item*> items = getEquippedIems();
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
            list<Item*> items = getItems();
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
            Room* currentRoom = getLocation();
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
            Room* currentRoom = getLocation();
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
            Room* currentRoom = getLocation();
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

void Player::inventoryCommand() {
    cout << "Inventory: " << endl;
    list<Item*> items = getItems();
    if (items.empty()) {
        cout << "No items." << endl;
    }
    else {
        for (Item* item : items) {
            cout << " - " << item->getName() << "." << endl;
        }
    }

    cout << "Equipped: " << endl;
    list<Item*> equippedItems = getEquippedIems();
    if (equippedItems.empty()) {
        cout << "No items equipped." << endl;
    }
    else {
        for (Item* item : equippedItems) {
            cout << " - " << item->getName() << "." << endl;
        }
    }
}