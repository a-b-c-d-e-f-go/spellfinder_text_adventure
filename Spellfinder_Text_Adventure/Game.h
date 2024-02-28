#pragma once
#include <iostream>
#include "String.h"
#include "Room.h"
#include "Entity.h"
#include "Player.h"

#define usi unsigned short int //Most common int type in this program.
#define m(s) cout << s << endl //Write to map. Used in Run().
#define rg << "             " << //Gap between contents of each room. Used in Row_Contents().
#define loop(var, min, max) for (usi var = min; var < max; var++)
#define delete_s(target) if (target != nullptr) { delete target; } //Safe version. Delete target if it exists.
#define delete_arr(target) if (target != nullptr) { delete[] target; } //Safe version. Delete targeted array if it exists.
#define cont(x, y) rooms[x][y].Contents().CStr()
using namespace std;

const usi map_size = 4;
const char R_CLOSED[] =	"#############   #############   #############   #############"; //Top and bottom of the map.
const char R_OPEN[] =	"#####   #####   #####   #####   #####   #####   #####   #####"; //Top and bottom of rooms (besides R_CLOSED for the top and bottom of the map).
const char R_INSIDE[] =	"##         ##   ##         ##   ##         ##   ##         ##"; //Before and after a row of room's contents.

class Game
{
private:
	Room rooms[map_size][map_size] = {
		{Room(), Room(), Room(), Room()},
		{Room(), Room(), Room(), Room()},
		{Room(), Room(), Room(), Room()},
		{Room(), Room(), Room(), Room()},
	};
	Player* player;
	vector<Entity> entities = { };

	//Drawing
	const void Row_Contents(const usi r) const //Draws a row within the map that contains players and such.
	{
		m(R_INSIDE);
		m("##   " << cont(0, r) rg cont(1, r) rg cont(2, r) rg cont(3, r) << "   ##");
		m(R_INSIDE);
	}
	const void Draw_Map() const //Draws the entire map.
	{
		m(R_CLOSED); //Row 0
		Row_Contents(0);
		m(R_OPEN << endl);

		m(R_OPEN); //Row 1
		Row_Contents(1);
		m(R_OPEN << endl);

		m(R_OPEN); //Row 2
		Row_Contents(2);
		m(R_OPEN << endl);

		m(R_OPEN); //Row 3
		Row_Contents(3);
		m(R_CLOSED << endl);
	}

	bool Check_Command(String& input, String command) //If the input starts with this command, return true and set the input to just the parameter of the command.
	{
		if (input.Find(command) == 0) //Checks for a command.
		{
			input.Replace(command + " ", ""); //Remove command and space, leaving only the parameter. Eg. 'move south' becomes 'south'.
			return true;
		}
		return false;
	}

	//Effects
	const void Execute(String& input) //Process the player's command.
	{
		if (Check_Command(input, String("move"))) //move <north/south/east/west>. Moves 1 room in a given direction.
		{
			//Do something based on what input is (since it's now only the parameter).
			cout << "Moving " << input.CStr() << ".\n";
			
		}
		if (Check_Command(input, String("inspect"))) //inspect <item> - Describes a given item if the player has it. Otherwise says that they don't.
		{
			//Do something based on what input is (since it's now only the parameter).
		}
		if (Check_Command(input, String("spell"))) //spell <spell> - Describes a given spell if the player knows it. Otherwise says that they don't.
		{
			//Do something based on what input is (since it's now only the parameter).
		}
		if (Check_Command(input, String("use"))) //use <item> - Uses a given item, with an effect from its Use().
		{
			//Do something based on what input is (since it's now only the parameter).
		}
		if (Check_Command(input, String("cast"))) //cast <known spell> - Casts a given spell, with an effect from the Use().
		{
			//Do something based on what input is (since it's now only the parameter).
		}
	}
	
public:
	Game()
	{

	}
	~Game()
	{
		//Delete rooms
		loop(i, 0, map_size)
		{
			delete_arr(rooms[i]);
		}
		delete_arr(rooms);

		//Delete entities
		entities.clear();
		entities.shrink_to_fit();

		//Delete player
		delete_s(player);
		player = nullptr;
	}
	void Run()
	{
		player = new Player(0, 0); //Add player.
		Entity p_entity = (Entity)(*player); //Convert player to entity.
		entities.push_back(p_entity); //Add player to entities.
		Draw_Map();
		String input;
		do
		{
			input = String::ReadFromConsole(); //Waits for input from console.
			input.ToLower(); //Case insensitive.
			Execute(input); //Performs whatever command was input.
		} while (input != String("quit")); //End program after inputting "quit".
	}
};