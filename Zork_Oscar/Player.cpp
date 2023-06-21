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