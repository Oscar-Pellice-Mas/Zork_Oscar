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