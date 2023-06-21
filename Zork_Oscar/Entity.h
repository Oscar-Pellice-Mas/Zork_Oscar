#ifndef __Entity__
#define __Entity__

#include <string>
#include <list>

using namespace std;

enum EntityType {
	ENTITY,
	ROOM,
	EXIT,
	ITEM,
	CREATURE,
	PLAYER
};

class Entity {
public:
	Entity(const EntityType type, const char* name, const char* description);
	virtual ~Entity();

    EntityType getEntityType() const;
    string getName() const;
    string getDescription() const;

    void addItem(Entity* item);
    bool removeItem(Entity* item);
    list<Entity*> getItems() const;

private:
	EntityType type;
	std::string name;
	std::string description;
	list<Entity*> contains;
};

#endif