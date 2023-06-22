#ifndef __World__
#define __World__

#include <string>
#include <list>
#include <iostream>
#include <algorithm>

#include "Global.h"
#include "Entity.h"
#include "Creature.h"
#include "Item.h"
#include "Exit.h"
#include "Room.h"
#include "Player.h"

using namespace std;

class Entity;
class Player;

class World {
public:
	World();
	~World();

	void setEntity(Entity* entity);
	list<Entity*> getEntities() const;

	void setPlayer(Player* player);
	Player* getPlayer();

	int play();

	void helpCommand();
	void lookCommand();
	void takeCommand(string parameter);
	void dropCommand(string parameter);
	void moveCommand(string parameter);
	void openCommand(string parameter);
	void attackCommand(string parameter);
	void equipCommand(string parameter);
	void unequipCommand(string parameter);
	void inspectCommand(string parameter);
	void talkCommand(string parameter);

private:
	list<Entity*> entities;
	Player* player;
};

#endif