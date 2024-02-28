#pragma once
#include <iostream>
#include "String.h"
#include "Room.h"
#include "Entity.h"

#define usi unsigned short int //Most common int type in this program.
#define loop(var, min, max) for (usi var = min; var < max; var++)
#define delete_s(target) if (target != nullptr) { delete target; } //Safe version. Delete target if it exists.
#define delete_arr(target) if (target != nullptr) { delete[] target; } //Safe version. Delete targeted array if it exists.
using namespace std;

#define m(s) cout << s << endl //Write to map. Used in Run().
#define cg << "             " << //Gap between contents of each room. Used in Row_Contents().
#define eg << "   ##   ##   " << //Gap between enemies in each room. Used in Row_Contents().
#define prompt String("Enter a command to proceed. 'help' will list all commands.") //Default output before a command is entered.
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
	String output = prompt;

	//Drawing
	const void Row_Contents(const usi r) const //Draws a row within the map that contains players and such.
	{
		m("##   " << Enem(0, r) eg Enem(1, r) eg Enem(2, r) eg Enem(3, r) << "   ##");
		m("##   " << Cont(0, r) cg Cont(1, r) cg Cont(2, r) cg Cont(3, r) << "   ##");
		m(R_INSIDE);
	}
	const char* Enem(usi x, usi y) const //Room enemy.
	{
		return "Gx3";
	}
	const char* Cont(usi x, usi y) const //Room contents.
	{
		if (player->x == x && player->y == y) { return "Px1"; } //If a player is present, the item will automatically get picked up (hence why they occupy the same space).
		if (rooms[x][y].item != nullptr) //If an item is present.
		{
			return rooms[x][y].item->Shorthand().CStr(); //Use item for room contents.
		}
		return "   "; //If nothing is present.
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
		if (Check_Command(input, String("move"))) //move <north/south/east/west> - Moves 1 room in a given direction.
		{
			//Do something based on what input is (since it's now only the parameter).
			output = String("Moved 1 room ") + input + String(".");
			
		}
		else if (Check_Command(input, String("inspect"))) //inspect <item> - Describes a given item if the player has it. Otherwise says that they don't.
		{
			//Do something based on what input is (since it's now only the parameter).
		}
		else if (Check_Command(input, String("spell"))) //spell <spell> - Describes a given spell if the player knows it. Otherwise says that they don't.
		{
			//Do something based on what input is (since it's now only the parameter).
		}
		else if (Check_Command(input, String("use"))) //use <item> - Uses a given item, with an effect from its Use().
		{
			//Do something based on what input is (since it's now only the parameter).
		}
		else if (Check_Command(input, String("cast"))) //cast <known spell> - Casts a given spell, with an effect from the Use().
		{
			//Do something based on what input is (since it's now only the parameter).
		}
		else if (Check_Command(input, String("help")))
		{
			output = prompt; //Reset command output.
			system("cls"); //Clear map to make way for command list.
			//List all commands.
			cout << "help - Lists all commands.\n\n";
			cout << "quit - Ends the game.\n\n";
			cout << "inspect <item> - Describes a given item.\n\n";
			cout << "spell <spell> - Describes a given spell.\n\n";
			cout << "move <north/south/east/west> Moves you 1 room in a given direction.\nThis will end your turn, allowing the ghosts to act.\n\n";
			cout << "use <item> - Uses a given item, which might consume it depending on the item.\nThis will end your turn, allowing the ghosts to act.\nUse 'inspect' for more detail on a specific item's effects.\n\n";
			cout << "cast <known spell> - Casts a given spell, which typically deals damage.\nThis will end your turn, allowing the ghosts to act.\nUse 'spell' for more detail on a specific spell's effects.\n\n";

			cout << "Press enter to return.\n"; getchar(); //Wait for enter key.
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
		//entities.push_back(p_entity); //Add player to entities.
		String input;
		do
		{
			system("cls"); Draw_Map(); output.WriteToConsole(); cout << endl; //Refresh every turn.
			input = String::ReadFromConsole(); //Waits for input from console.
			input.ToLower(); //Case insensitive.
			Execute(input); //Performs whatever command was input.
		} while (input != String("quit")); //End program after inputting "quit".
	}
};