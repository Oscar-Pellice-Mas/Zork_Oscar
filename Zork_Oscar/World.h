#ifndef __World__
#define __World__

#include <string>
#include <list>
#include <iostream>
#include <algorithm>

using namespace std;

class Entity;
class Player;

class World {
public:
	World();
	~World();

	void addEntity(Entity* entity);
	list<Entity*> getEntities() const;

	void addPlayer(Player* player);
	Player* getPlayer();

	int play();

private:
	list<Entity*> entities;
	Player* player;
};

#endif