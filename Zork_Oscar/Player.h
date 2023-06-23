#ifndef __Player__
#define __Player__

#include "Creature.h"
#include <algorithm>

using namespace std;

class Player : public Creature {
public:
	Player(const char* name, const char* description, Room* location, int health, int attack, int defense);
	~Player();

	string getDescription() const;

	void printBase();
	void helpCommand();
	void lookCommand();
	void takeCommand(string parameter);
	void takeCommand(list<string> parameter);
	void dropCommand(string parameter);
	void moveCommand(string parameter);
	void openCommand(string parameter);
	bool attackCommand(string parameter);
	void equipCommand(string parameter);
	void unequipCommand(string parameter);
	void inspectCommand(string parameter);
	void inventoryCommand();
private:

};

#endif