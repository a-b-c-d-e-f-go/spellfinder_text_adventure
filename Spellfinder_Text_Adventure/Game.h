#pragma once
#include <iostream>
#include "String.h"
#include "Room.h"
#include "Player.h"

#define usi unsigned short int //Most common int type in this program.
#define m(s) cout << s << endl //Write to map. Used in Run().
#define rg << "             " << //Gap between contents of each room. Used in Row_Contents().
using namespace std;

const usi map_size = 3;
const char R_CLOSED[] =	"#############   #############   #############"; //Top and bottom of the map.
const char R_OPEN[] =	"#####   #####   #####   #####   #####   #####"; //Top and bottom of rooms (besides R_CLOSED for the top and bottom of the map).
const char R_INSIDE[] =	"##         ##   ##         ##   ##         ##"; //Before and after a row of room's contents.

class Game
{
private:
	Room rooms[map_size][map_size];
	Player* player;

	const void Row_Contents(const usi r) const //Draws a row within the map that contains players and such.
	{
		m(R_INSIDE);
		m("##   " << C(0, r) rg C(1, r) rg C(2, r) << "   ##");
		m(R_INSIDE);
	}
	const char* C(const usi x, const usi y) const //Get 3 characters to place in a room.
	{
		return "[S]";
	}
	
public:
	Game()
	{

	}
	~Game()
	{

	}
	void Run()
	{
		//Draws the entire map.
		m(R_CLOSED); //Row 0
		Row_Contents(0);
		m(R_OPEN << endl);

		m(R_OPEN); //Row 1
		Row_Contents(1);
		m(R_OPEN << endl);

		m(R_OPEN); //Row 2
		Row_Contents(2);
		m(R_CLOSED << endl);
	}
};