#pragma once
#include <iostream>
#include "String.h"
#include "Room.h"
#include "Player.h"
#define usi unsigned short int
using namespace std;

const usi map_size = 12;

class Game
{
private:
	Room rooms[map_size][map_size];
	Player* player;
public:
	Game()
	{

	}
	~Game()
	{

	}
	void Run()
	{

	}
};