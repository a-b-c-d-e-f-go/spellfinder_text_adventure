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
			Room("You enter what appears to be the room you came from.\nWhere the entrance used to be is now a solid wall."), //0,0: Top left (START).
			Room("DESC"), //0,1: 
			Room("DESC"), //0,2: 
			Room("DESC") //0,3: Bottom left.
		},
		{
			Room("DESC"), //1,0: 
			Room("You enter a room with 4 stone pillars at each corner."), //1,1: 
			Room("You enter a room with moss-covered walls."), //1,2: 
			Room("") //1,3: 
		},
		{
			Room("DESC"), //2,0: 
			Room("DESC"), //2,1: 
			Room("DESC"), //2,2: 
			Room("As you enter, a thick gray fog permeates your vision.\nAdjusting your eyes, you notice what appear to be operating tables.") //2,3: Endgame room.
		},
		{
			Room("You enter a room with bookshelves covering the northern and eastern walls."), //3,0: Top right.
			Room("You enter a room furnished with a wooden desk, drawer, and chair.\nPaper notes written in incomprehensible symbols are strewn across the floor."), //3,1: 
			Room("As you enter, a thick gray fog permeates your vision.\nAdjusting your eyes, you notice what appear to be lockers on the eastern wall."), //3,2: Endgame room.
			Room("The fog dissapates as you enter what can only be described as a bottomless pit.") //3,3: Bottom right (END).
		},
	};
	usi rev_count[map_size][map_size] = { //For the purposes of revenant AI and the map display.
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
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
		switch (rev_count[x][y]) //Return based on count of revenants in this room. 0 revenants will show nothing.
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
			default: return "Rx?"; //For more than 9 revenants in one spot.
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
	const void Update_Revenant_Count()
	{
		loop(i, 0, map_size) //Loop through rev_count and reset it.
		{
			loop(j, 0, map_size)
			{
				rev_count[i][j] = 0;
			}
		}
		loop(i, 0, revenants.size()) //Loop through revenants vector.
		{
			rev_count[revenants[i].x][revenants[i].y]++; //Increase counter at its coordinates.
		}
	}

	const void Revenants_Turn() //Happens when you move, cast a spell, or 
	{
		if (revenants.size() > 0) //If there are any remaining revenants.
		{
			output += String("The revenants take their turn.\n");
			Update_Revenant_Count(); //Should update after they move.
		}
	}

	//Effects
	const void Execute(String& input) //Do things based on the player's command.
	{

		if (Check_Command(input, String("move"))) //move <north/south/east/west> - Moves 1 room in a given direction.
		{
			//Initialize variables.
			short int xv = 0;
			short int yv = 0;
			bool valid = true;
			//X and Y modifier based on direction.
			if (input == String("north")) { yv = -1; }
			if (input == String("south")) { yv = 1; }
			if (input == String("west")) { xv = -1; }
			if (input == String("east")) { xv = 1; }
			if ((player->x + xv) >= map_size || (player->x + xv) < 0) { valid = false; } //Outside map x bounds.
			if ((player->y + yv) >= map_size || (player->y + yv) < 0) { valid = false; } //Outside map y bounds.

			if (valid)
			{
				player->x += xv; player->y += yv; //Move player.
				output = String("Moved 1 room ") + input + String(". "); //Announce new room the player has entered.

				Revenants_Turn(); //This should happen before revenant count for best results.
				output += rooms[player->x][player->y].Description(); //Describe room after letting revenants take their turn.

				usi revs = rev_count[player->x][player->y]; //Revenants in the room with the player.
				if (revs > 1) //If there are revenants in the room, announce this too.
				{
					output.Append(String("\nThere are "));
					output.Append(String(to_string(revs)));
					output.Append(String(" revenants in the room with you."));
				}
				else if (revs == 1) //Different version for 1 revenant.
				{
					output.Append(String("\nThere is a revenant in the room with you."));
				}
				else if (revs <= 0) //Specific room descriptions that only happen with no revenants.
				{

				}
			}
			else //Do not let revenants act here. It'd be pretty frustrating if you gave them a free turn for forgetting there's a wall in the way.
			{
				output = String("Cannot move ") + input + String(" as it there is a solid wall in the way.");
			}
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
		else if (Check_Command(input, String("help"))) //help - Lists all commands.
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
		else
		{
			output = String("Command does not exist. Enter 'help' to see a list of commands.");
		}
	}
	
public:
	Game()
	{

	}
	~Game()
	{
		//Delete rooms.
		loop(i, 0, map_size)
		{
			delete_arr(rooms[i]);
		}
		delete_arr(rooms);

		//Delete revenants.
		revenants.clear();
		revenants.shrink_to_fit();

		//Delete player.
		delete_s(player);
		player = nullptr;
	}
	void Run()
	{
		player = new Player(0, 0); //Add player.
		newrev(3, 3);
		newrev(3, 3);
		newrev(3, 3);
		newrev(2, 3);
		newrev(3, 2);
		String input;
		do
		{
			Update_Revenant_Count(); //Do this before drawing the map.
			system("cls"); Draw_Map(); output.WriteToConsole(); cout << endl; //Refresh every turn.
			input = String::ReadFromConsole(); //Waits for input from console.
			input.ToLower(); //Case insensitive.
			Execute(input); //Performs whatever command was input.
		} while (input != String("quit")); //End program after inputting "quit".
	}
};