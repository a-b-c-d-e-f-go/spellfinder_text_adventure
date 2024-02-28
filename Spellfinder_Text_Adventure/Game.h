#pragma once
#include <iostream>
#include "String.h"
#include "Room.h"
#include "Entity.h"

#define usi unsigned short int //Most common int type in this program.
#define loop(var, min, max) for (usi var = min; var < max; var++)
#define delete_s(target) if (target != nullptr) { delete target; } //Safe version. Delete target if it exists.
#define delete_arr(target) if (target != nullptr) { delete[] target; } //Safe version. Delete targeted array if it exists.
#define newrev(x, y) revenants.push_back(Revenant(x, y))
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
	Room rooms[map_size][map_size] = { //Create room array.
		{
			Room("You enter what appears to be the room you came from.\nWhere the entrance used to be is now a solid wall."), //0,0: Top left. No items.
			Room("test"), //0,1: 
			Room(), //0,2: 
			Room() //0,3: Bottom left.
		},
		{
			Room(), //1,0: 
			Room(), //1,1: 
			Room(), //1,2: 
			Room() //1,3: 
		},
		{
			Room(), //2,0: 
			Room(), //2,1: 
			Room(), //2,2: 
			Room() //2,3: 
		},
		{
			Room(), //3,0: Top right.
			Room(), //3,1: 
			Room(), //3,2: 
			Room() //3,3: Bottom right.
		},
	};
	Player* player;
	vector<Revenant> revenants = { };
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
		usi count = 0;
		loop(i, 0, revenants.size()) //Loop through revenants vector.
		{
			if (revenants[i].x == x && revenants[i].y == y) //If the revenant is at this position.
			{
				count++; //Increase revenant count.
			}
		}
		switch (count) //Return based on count of revenants in this room. 0 revenants will show nothing.
		{
			case 0: return "   ";
			case 1: return "REV";
			case 2: return "Rx2";
			case 3: return "Rx3";
			case 4: return "Rx4";
			case 5: return "Rx5";
			case 6: return "Rx6";
			case 7: return "Rx7";
			case 8: return "Rx8";
			case 9: return "Rx9";
			default: return "Rx?";
		}
	}
	const char* Cont(usi x, usi y) const //Room contents.
	{
		if (player->x == x && player->y == y) { return "PLA"; } //If a player is present, the item will automatically get picked up (hence why they occupy the same space).
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
		return false; //Return false, checking for the next command.
	}

	//Effects
	const void Execute(String& input) //Do things based on the player's command.
	{
		if (Check_Command(input, String("move"))) //move <north/south/east/west> - Moves 1 room in a given direction.
		{
			//Do something based on what input is (since it's now only the parameter).
			output = String("Moved 1 room ") + input + String(". ") + rooms[player->x][player->y].Description();
			
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
		else if (Check_Command(input, String("cast"))) //cast <spell> - Casts a given spell, with an effect from the Use().
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
			cout << "move <north/south/east/west> Moves you 1 room in a given direction.\nThis will end your turn, allowing the revenants to act.\n\n";
			cout << "use <item> - Uses a given item, which might consume it depending on the item.\nThis will end your turn, allowing the revenants to act.\nUse 'inspect' for more detail on a specific item's effects.\n\n";
			cout << "cast <known spell> - Casts a given spell, which typically deals damage.\nThis will end your turn, allowing the revenants to act.\nUse 'spell' for more detail on a specific spell's effects.\n\n";

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

		//Delete revenants
		revenants.clear();
		revenants.shrink_to_fit();

		//Delete player
		delete_s(player);
		player = nullptr;
	}
	void Run()
	{
		player = new Player(0, 0); //Add player.
		newrev(3, 3);
		newrev(3, 3);
		newrev(3, 3);
		newrev(3, 3);
		newrev(2, 3);
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