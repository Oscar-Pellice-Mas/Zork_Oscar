#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

// Enum for item types
enum class EntityType {
    CREATURE,
    EXIT,
    ROOM,
    ITEM
};

enum class ItemType {
    ITEM,
    CONTAINER,
    WEAPON,
    ARMOR
};

enum class Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

// Forward declaration of classes
class World;
class Entity;
class Creature;
class NPC;
class Player;
class Exit;
class Room;
class Item;

// World class
class World {
private:
    vector<Entity*> entities;

public:
    void addEntity(Entity* entity) {
        entities.push_back(entity);
    }

    vector<Entity*> getEntities() const {
        return entities;
    }
};

// Entity class
class Entity {
protected:
    EntityType type;
    string name;
    string description;
    vector<Item*> contains;

public:
    Entity(const EntityType type, const string& name, const string& description) : type(type), name(name), description(description) {}

    virtual ~Entity() {
        for (Item* item : contains) {
            delete item;
        }
    }

    EntityType getEntityType() const {
        return type;
    }

    string getName() const {
        return name;
    }

    string getDescription() const {
        return description;
    }

    void addItem(Item* item) {
        contains.push_back(item);
    }

    bool removeItem(Item* item) {
        auto it = find(contains.begin(), contains.end(), item);
        if (it != contains.end()) {
            contains.erase(it);
            return true;
        }
        return false;
    }

    vector<Item*> getItems() const {
        return contains;
    }
};

// Creature class
class Creature : public Entity {
protected:
    Room* location;

public:
    Creature(const string& name, const string& description, Room* location) : Entity(EntityType::CREATURE, name, description), location(location) {}

    Room* getLocation() const {
        return location;
    }

    void setLocation(Room* room) {
        location = room;
    }
};

// Player class
class Player : public Creature {
public:
    Player(const string& name, const string& description, Room* location) : Creature(name, description, location) {}
};

// NPC class
class NPC : public Creature {
public:
    NPC(const string& name, const string& description, Room* location) : Creature(name, description, location) {}
};

// Exit class
class Exit : public Entity {
private:
    Direction direction;
    Room* source;
    Room* destination;

public:
    Exit(const string& name, const string& description, Direction direction, Room* source, Room* destination) : Entity(EntityType::EXIT, name, description), direction(direction), source(source), destination(destination) {}

    Direction getDirection() const {
        return direction;
    }

    Room* getSource() const {
        return source;
    }

    Room* getDestination() const {
        return destination;
    }
};

// Room class
class Room : public Entity {
private:
    vector<Item*> items;
    vector<Exit*> exits;

public:
    Room(const string& name, const string& description) : Entity(EntityType::ROOM , name, description) {}

    void addItem(Item* item) {
        items.push_back(item);
    }

    bool removeItem(Item* item) {
        auto value = find(items.begin(), items.end(), item);
        if (value != items.end()) {
            items.erase(value);
            return true;
        }
        return false;
    }

    vector<Item*> getItems() const {
        return items;
    }

    void addExit(Exit* exit) {
        exits.push_back(exit);
    }

    vector<Exit*> getExits() const {
        return exits;
    }
};

// Item class
class Item : public Entity {
private:
    ItemType itemType;

public:
    Item(const string& name, const string& description, ItemType type) : Entity(EntityType::ITEM, name, description), itemType(itemType) {}

    string getName() const {
        return name;
    }

    string getDescription() const {
        return description;
    }

    ItemType getItemType() const {
        return itemType;
    }
};

int main() {
    World world;

    // Create items
    Item* key = new Item("Key", "A key", ItemType::ITEM);
    Item* box = new Item("Box", "A box", ItemType::CONTAINER);
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
    NPC* npc = new NPC("NPC", "A non-player character", room2);

    // Add player and NPC to the world
    world.addEntity(player);
    world.addEntity(npc);

    // Create exits
    Exit* exit1 = new Exit("Door", "A wood door", Direction::NORTH, room1, room2);
    Exit* exit2 = new Exit("Window", "An opened window", Direction::SOUTH, room2, room3);

    // Add exits to rooms
    room1->addExit(exit1);
    room2->addExit(exit2);

    return 0;
}