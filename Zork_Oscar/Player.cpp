#include <iostream>
#include "Global.h"
#include "Room.h"
#include "Exit.h"
#include "Item.h"
#include "Player.h"

// ----- Constructor -----
Player::Player(const string& name, const string& description, Room* location) : Creature(name, description, location) {}

// ----- Deconstructor -----
Player::~Player() {
}