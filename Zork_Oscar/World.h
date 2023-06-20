#ifndef __World__
#define __World__

#include <string>
#include <list>
#include <vector>

using namespace std;

class Entity;
class Player;

class World {
public:
	World();
	~World();

	void addEntity(Entity* entity);
	vector<Entity*> getEntities() const;

private:
	vector<Entity*> entities;
	Player* player;
};

#endif