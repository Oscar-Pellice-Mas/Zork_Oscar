#ifndef __World__
#define __World__

#include <string>
#include <list>
#include <iostream>

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


private:
	list<Entity*> entities;
	Player* player;
};

#endif