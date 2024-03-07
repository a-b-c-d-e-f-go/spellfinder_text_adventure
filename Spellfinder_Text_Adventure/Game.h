// Game.h : This file contains the Game class, which controls the map as well as all room/position-dependent interactions.
#pragma once
#include <iostream>
#include "String.h"
#include "Room.h"
#include "Entity.h"

#define ef else if //Makes both types of if 2 characters, so else if chains look cleaner.
#define usi unsigned short int //Most common int type in this program.
#define loop(var, min, max) for (usi var = min; var < max; var++)
using namespace std;

#define m(s, y) cout << s << Side_Contents(y).CStr() << endl //Write to map. Used in Run().
#define cg << "             " << //Gap between players/items in each room. Used in Row_Contents().
#define eg << "   ##   ##   " << //Gap between enemies in each room. Used in Row_Contents().
#define prompt String("Enter a command to proceed. 'help' will list all commands.") //Default output before a command is entered.
#define pos_desc(_x, _y, _string) if (player->x == _x && player->y == _y) {output += String(_string); }
const usi map_size = 4;
const char R_CLOSED[] =	"#############   #############   #############   #############"; //Top and bottom of the map.
const char R_OPEN[] =	"#####   #####   #####   #####   #####   #####   #####   #####"; //Top and bottom of rooms (besides R_CLOSED for the top and bottom of the map).
const char R_INSIDE[] =	"##         ##   ##         ##   ##         ##   ##         ##"; //Before and after a row of room's contents.
const char R_GAP[] =	"                                                             "; //Between rooms.
#define newrev(x, y) revenants.push_back(Revenant(x, y)) //Spawns in a revenant at the given coordinates.
#define inspect_valid_item output = prompt; system("cls"); input.ToUpper().WriteToConsole(); cout << "\n---------------\n"; inspected->Description().WriteToConsole();//Clear console and write item details to console.
#define valid_item_stats if (inspected->Damage() > 0) { cout << "Deals " << to_string(inspected->Damage()) << " damage to any revenants in the same room.\n"; } if (inspected->Self_Damage() > 0) { cout << "Deals " << to_string(inspected->Self_Damage()) << " damage to yourself.\n"; } if (inspected->Self_Damage() < 0) { cout << "Heals you for " << to_string(inspected->Self_Damage() * -1) << " health.\n"; } //Shown below USE:

class Game
{
private:
	short int gamestate = 0; //0 for mid-game, -1 for failure, 1 for success.
	const void wait_for_enter() const
	{
		cout << "\nPress enter to return.\n"; String().ReadFromConsole(); //Wait for enter key.
	}
	Room rooms[map_size][map_size] = { //Create room array.
		{
			//0,0: ENTRANCE (Top Left)
			Room("You enter what appears to be the room you came from.\nWhere the entrance used to be is now a solid wall."),
			//0,1: MOSS
			Room("You enter a room with moss-covered walls."),
			//0,2: BANAL
			Room("You enter a room with a creaking wooden floor."),
			//0,3: EDGE (Bottom Left)
			Room("You enter a room with cracked walls and a hole in the south east corner.", new Bomb())
		},
		{
			//1,0: STATUES
			Room("You enter a room with a line of armored statues holding spears on the north side.", new Spear()),
			//1,1: PILLARS
			Room("You enter a room with 4 stone pillars at each corner."),
			//1,2: FIREBALL
			Room("You enter a room with a scorched floor covered in burnt scrolls.", new Scroll(new Spell(String("fireball"), String("Creates an explosion at your position,\ndamaging everything nearby and (to a lesser extent) yourself."), 12, 4))),
			//1,3: PASTA
			Room("You enter room with no distinguishing features.")
		},
		{
			//2,0: PORTAL
			Room("You enter a room with a staircase leading north,\nup towards an empty tungsten frame about the size of a doorway."),
			//2,1: GARDEN
			Room("You enter a small garden filled with luminescent flora.", new Glowfruit()),
			//2,2: HEAL
			Room("You enter a room with diagonal tiles and an obsidian pedestal at its center.", new Scroll(new Spell(String("heal"), String("Quickly mends most non-lethal wounds."), 0, -6))),
			//2,3: WEST REVENANT EXIT
			Room("As you enter, a thick gray fog permeates your vision.\nAdjusting your eyes, you notice what appear to be operating tables.")
		},
		{
			//3,0: INDEX (Top Right)
			Room("You enter a room with bookshelves adorning the northern and eastern walls."),
			//3,1: VORTEX
			Room("You enter a room furnished with a wooden desk, drawer, and chair.\nPaper notes written in incomprehensible symbols are strewn across the floor.", new Scroll(new Spell(String("vortex"), String("Uses a fracture in spacetime to deal immense damage.\nUnfortunately, it also summons more revenants elsewhere."), 999, -3))),
			//3,2: NORTH REVENANT EXIT
			Room("As you enter, a thick gray fog permeates your vision.\nAdjusting your eyes, you notice what appear to be lockers on the eastern wall."),
			//3,3: BOTTOMLESS PIT (Bottom Right)
			Room("The fog dissapates as you enter what can only be described as a bottomless pit.\nIt looks like there's something emerging from it.", new Scroll(new Spell(String("activate"), String("Designed as a key for some form of portal."), 0, 0)))
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
	String output = prompt; //Gets written to the console below the map.

	//Drawing
	const String Side_Contents(const usi y) const
	{
		String s = "\t";
		if (y == 1)
		{
			s += String("Your Health: ");
			s += String(player->health);
			s += String("/20");
		}

		if (y > 2 && y <= 2 + rev_count[player->x][player->y]) //From 3 to 3+count.
		{
			s += String("Revenant: ");
			usi j = y - 2; //3 is the start.
			loop(i, 0, revenants.size())
			{
				if ((revenants[i].x == player->x) && (revenants[i].y == player->y))
				{
					j--;
					if (j <= 0)
					{
						s += String(revenants[i].health);
						break;
					}
				}
			}
			s += String("/20");
		}
		//if (y == 1) { s += String("Your Health: "); }
		//return String("test").Append(String(to_string(y)));
		return s;
	}
	const void Row_Contents(const usi r) const //Draws a row within the map that contains players and such.
	{
		//Enemy layer.
		m("##   " << Enem(0, r) eg Enem(1, r) eg Enem(2, r) eg Enem(3, r) << "   ##", r * 6 + 1);
		//Player/item layer.
		m("##   " << Cont(0, r).CStr() cg Cont(1, r).CStr() cg Cont(2, r).CStr() cg Cont(3, r).CStr() << "   ##", r * 6 + 2);
		m(R_INSIDE, r * 6 + 3);
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
	const String Cont(usi x, usi y) const //Room contents.
	{
		if (player->x == x && player->y == y) { return String("YOU"); } //If a player is present, the item will automatically get picked up (hence why they occupy the same space).
		if (rooms[x][y].item != nullptr) //If an item is present.
		{
			return rooms[x][y].item->Shorthand(); //Use item for room contents.
		}
		return String("   "); //If nothing is present.
	}
	const void Draw_Map() const //Draws the entire map.
	{
		m(R_CLOSED, 0); //Row 0
		Row_Contents(0);
		m(R_OPEN, 4);
		m(R_GAP, 5);

		m(R_OPEN, 6); //Row 1
		Row_Contents(1);
		m(R_OPEN, 10);
		m(R_GAP, 11);

		m(R_OPEN, 12); //Row 2
		Row_Contents(2);
		m(R_OPEN, 16);
		m(R_GAP, 17);

		m(R_OPEN, 18); //Row 3
		Row_Contents(3);
		m(R_CLOSED, 22);
		cout << endl;
	}

	//Revenants
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
	const void Move_Revenant(Revenant& rev, short int _x, short int _y) //Move a revenant and update its position to other revenants.
	{
		rev_count[rev.x][rev.y] -= 1; //Decrease revenant count where the revenant moved from.
		rev.x += _x;
		rev.y += _y;
		rev_count[rev.x][rev.y] += 1; //Increase revenant count where the revenant moved to.
	}
	const void Revenants_Turn() //Happens when you wait, move, cast a spell, or use an item. It's here because otherwise the revenant class needs access to the rooms array.
	{
		if (revenants.size() > 0) //If there are any remaining revenants.
		{
			usi attacks = 0;
			usi recovers = 0;
			output += String("The revenants take their turn.\n");

			loop(i, 0, revenants.size()) //For each revenant that exists.
			{
				if (revenants[i].just_spawned) //On the revenants first turn after being spawned.
				{
					revenants[i].just_spawned = false; //Disable this flag to allow actions next turn. Otherwise do nothing.
				}
				else //If the revenant has already been spawned.
				{
					if ((revenants[i].x == player->x) && (revenants[i].y == player->y)) //At player's position. Attempt to attack.
					{
						attacks++;
					}
					ef((revenants[i].x == player->px) && (revenants[i].y == player->py) && revenants[i].health < 20) //The player left the revenant's room this turn, and the revenant is damaged. Recover health and give a head start.
					{
						recovers++;
						revenants[i].health += 4; //Heal by 4 and don't move.
						if (revenants[i].health > 20) { revenants[i].health = 20; } //Cap health at 20.
					}
					else //Not at player's position. Attempt to close in.
					{
						int weight_n = 0; int weight_s = 0; int weight_e = 0; int weight_w = 0; //Weights for different directions.

						//Affect weights based on X/Y difference to player. The further away on an axis, the more it is favoured.
						weight_n += (revenants[i].y - player->y); weight_s -= (revenants[i].y - player->y);
						weight_e -= (revenants[i].x - player->x); weight_w += (revenants[i].x - player->x);

						//If the distance to the player is further than 1 room.
						if (abs(weight_n) > 1 || abs(weight_s) > 1 || abs(weight_e) > 1 || abs(weight_w) > 1)
						{
							//Affect weights based on if there are other revenants occupying that direction, encouraging the revenants to spread out when distant from the player.
							weight_n -= rev_count[revenants[i].x][revenants[i].y - 1] * 2;
							weight_s -= rev_count[revenants[i].x][revenants[i].y + 1] * 2;
							weight_e -= rev_count[revenants[i].x + 1][revenants[i].y] * 2;
							weight_w -= rev_count[revenants[i].x - 1][revenants[i].y] * 2;
						}
						//Prevent revenants from pathfinding outside the map.
						if ((revenants[i].x + 1) >= map_size) { weight_e = -999; } if ((revenants[i].x - 1) < 0) { weight_w = -999; }
						if ((revenants[i].y + 1) >= map_size) { weight_s = -999; } if ((revenants[i].y - 1) < 0) { weight_n = -999; }

						//Choose a direction to move in based on the highest weight.
						if (weight_n >= weight_s && weight_n >= weight_e && weight_n >= weight_w) { Move_Revenant(revenants[i], 0, -1); } //If weight_n is at least tied for highest.
						ef(weight_s >= weight_n && weight_s >= weight_e && weight_s >= weight_w) { Move_Revenant(revenants[i], 0, 1); } //If weight_s is at least tied for highest.
						ef(weight_e >= weight_n && weight_e >= weight_s && weight_e >= weight_w) { Move_Revenant(revenants[i], 1, 0); } //If weight_e is at least tied for highest.
						ef(weight_w >= weight_n && weight_w >= weight_s && weight_w >= weight_e) { Move_Revenant(revenants[i], -1, 0); } //If weight_w is at least tied for highest.
					}
				}
			}
			if (attacks > 0) //Process revenant attacks based on how many were in the same room as a player.
			{
				if (attacks == 1)
				{
					output += String("A revenant attacks for 3 damage.\n");
				}
				else
				{
					output += String(attacks).Append(String(" revenants attack for ").Append(attacks * 3).Append(String(" damage.\n")));
				}
				player->health -= (3 * attacks); //Damage the player.
			}
			if (recovers > 0) //Announce revenant recoveries.
			{
				if (recovers == 1)
				{
					output += String("A revenant recovered 4 health.\n");
				}
				else
				{
					output += String(recovers).Append(String(" revenants recovered 4 health each.\n"));
				}
			}
			
		}
		Update_Revenant_Count(); //Should update after they move.
	}
	const void Damage_Revenants(int damage) //Damages any revenants in the same room as the player by a given amount, automatically killing them at 0 health.
	{
		if (damage > 0) //If it has zero damage, it's not an attack, and shouldn't be treated as missed.
		{
			usi hits = 0; //Amount of revenants damaged by the attack.
			usi kills = 0; //Amount of revenants kills by the attack.
			if (rev_count[player->x][player->y] > 0) //If there is more than one revenant.
			{
				loop(i, 0, revenants.size()) //For each revenant.
				{
					if ((revenants[i].x == player->x) && (revenants[i].y == player->y)) //If position matches the player.
					{
						revenants[i].health -= damage;
						hits++;
						if (revenants[i].health <= 0)
						{
							kills++;
							revenants.erase(revenants.begin() + i); //Remove revenant.
							i--; //Move i to the previous revenant to avoid skipping one after deletion.
						}
					}
				}
			}
			if (hits > 0) //If there are revenants in the room.
			{
				output += String("\nHit ");
				output += String(to_string(hits));
				output += String(" revenant(s). ");
				if (kills > 0)
				{
					output += String(" Killed ");
					output += String(to_string(kills));
					output += String(" revenant(s). ");
					rev_count[player->x][player->y] -= kills; //Faster than doing a full revenant count.
				}
			}
			else //If there are no revenants in the room.
			{
				output += String("\nMissed! ");
			}
			Item_Check(); //Check for items after attacking. This will only do anything if there are no revenants in the room.
		}
	}

	//Items
	const void Item_Check() //Waiting, clearing a room, or moving into an empty room.
	{
		if ((rev_count[player->x][player->y] <= 0) && (rooms[player->x][player->y].item != nullptr)) //If the room is clear and contains an item.
		{
			output += String("\n");
			output += rooms[player->x][player->y].item->RoomDescription(); //Announce item pickup.
			player->AddItem(rooms[player->x][player->y].item); //Give the item to the player.
			rooms[player->x][player->y].item = nullptr; //Remove item from room.
		}
	}
	const void Use_Item(Item* item) //All of the things that happen when you use an item or spell, depending on the item or spell.
	{
		item->Use(output); //Use item

		//Damage & self damage of item.
		player->health -= item->Self_Damage();
		Damage_Revenants(item->Damage());
		if (player->health > 20) { player->health = 20; } //Prevent overheal.
		Scroll* s = ((Scroll*)item); //Used by case 2.
		switch (item->UniqueEffect())
		{
			case 1: //Vortex.
				loop(i, 0, 3) //Spawn 3 revenants at the PASTA room
				{
					newrev(1, 3);
				}
				break;
			case 2: //Spell scroll.
				if (s != nullptr) { player->AddSpell(s->spell); } //Add spell from scroll.
				newrev(0, 0); //Spawn 1 revenant at 0,0.
				break;
			case 3: //Activate.
				if (player->x == 2 && player->y == 0) //If the player is in the portal room.
				{
					gamestate = 1; //Win the game.
				}
				ef(player->x == 1 && player->y == 0) //If the player is in the statue room.
				{
					output += String("\nThe statues don't do anything... ");
				}
				ef(player->x == 2 && player->y == 1) //If the player is in the garden room.
				{
					output += String("\nThe plants are now activated???????\n");
				}
				ef (player->x == 0 && player->y == 0) //If the player is in the start room.
				{
					output += String("\nThe solid wall remains solid. Seems like this isn't the way out.\n");
				}
				ef(player->x == 1 && player->y == 3) //If the player is in the pasta room.
				{
					output += String("\nSomething seems to take notice. You should leave...?\n");
				}
				ef(player->x == 3 && player->y == 3) //If the player is at the bottomless pit.
				{
					output += String("\nYou found out what the bottomless pit does.\nA revenant emerges... ");
					newrev(3, 3);
				}
				ef (player->x == 1 && player->y == 1) //If the player is in the pillars room.
				{
					output += String("\nThe pillars are displeased with your aimless shenanigains!\n"); //In my defense, it was funny.
					loop(i, 0, 4) //Fills the map with revenants. Good luck!
					{
						loop(j, 0, 4)
						{
							newrev(i, j);
						}
					}
				}
				else
				{
					output += String("\nNothing happened... ");
				}
			default:
				break;
		}
		if (item->Consumable())
		{
			player->RemoveItem(item);
			output += String("\nItem was consumed. ");
		}
	}

	//Commands
	bool Check_Command(String& input, const String command) //If the input starts with this command, return true and set the input to just the parameter of the command.
	{
		if (input.Find(command) == 0) //Checks for a command.
		{
			input.Replace(command + " ", ""); //Remove command and space, leaving only the parameter. Eg. 'move south' becomes 'south'.
			return true;
		}
		return false; //Return false, checking for the next command.
	}
	const void Execute(String& input) //Do things based on the player's command.
	{
		player->px = -1; player->py = -1; //Reset previous position if not moving.
		if (Check_Command(input, String("wait"))) //wait - Does nothing, allowing the revenants to take their turn.
		{
			output = String("Skipping your turn. ");
			Revenants_Turn();
			Item_Check();
		}
		ef (Check_Command(input, String("move"))) //move <north/south/east/west> - Moves 1 room in a given direction.
		{
			//Initialize variables.
			short int xv = 0;
			short int yv = 0;
			bool valid = true;
			//X and Y modifier based on direction.
			if (input == String("north")) { yv = -1; }
			ef (input == String("south")) { yv = 1; }
			ef (input == String("west")) { xv = -1; }
			ef (input == String("east")) { xv = 1; }
			else //If no cardinal direction is input.
			{
				output = String("Cannot move in direction ").Append(input).Append(String(". Please use north, south, east, or west.")); //Error message.
				return; //Don't perform move command if it is not one of the cardinal directions.
			}
			if ((player->x + xv) >= map_size || (player->x + xv) < 0) { valid = false; } //Outside map x bounds.
			if ((player->y + yv) >= map_size || (player->y + yv) < 0) { valid = false; } //Outside map y bounds.

			if (valid)
			{
				player->px = player->x; player->py = player->y; //Update previous position when moved.
				player->x += xv; player->y += yv; //Move player.
				output = String("Moved 1 room ") + input + String(". "); //Announce new room the player has entered.

				Revenants_Turn(); //This should happen before revenant count for best results.
				output += rooms[player->x][player->y].Description(); //Describe room after letting revenants take their turn.

				usi revs = rev_count[player->x][player->y]; //Revenants in the room with the player.
				if (revs > 1) //If there are revenants in the room, announce this too.
				{
					output += String("\nThere are ");
					output += String(to_string(revs));
					output += String(" revenants in the room with you.");
				}
				ef (revs == 1) //Different version for 1 revenant.
				{
					output += String("\nThere is a revenant in the room with you.");
				}
				else //Items and certain flavor text can only be accessed when there are no revenants in a room.
				{
					//Specific lines that only happen with no revenants.
					pos_desc(0, 3, "\nThrough it, you can see a deep cavern. Wouldn't want to fall down there.");
					pos_desc(1, 0, "\nOne of the statues has almost crumbled completely.");
					pos_desc(1, 3, "\nYou shouldn't linger here.");
					pos_desc(2, 1, "\nIt doesn't seem to have been kempt in a while.");
					pos_desc(2, 3, "\nOne of the tables shudders for a moment, but there's nobody on it.");
					pos_desc(3, 2, "\nSomething is watching you.");
					//Items are found after these lines.
					Item_Check();
				}
			}
			else //Do not let revenants act here. It'd be pretty frustrating if you gave them a free turn for forgetting there's a wall in the way.
			{
				output = String("Cannot move ") + input + String(" as it there is a solid wall in the way.");
			}
		}
		ef (Check_Command(input, String("inspect"))) //inspect <item> - Describes a given item if the player has it. Otherwise says that they don't.
		{
			Item* inspected = player->FindItem(input);
			if (inspected == nullptr) //Invalid item.
			{
				output = String("You don't have an item called ").Append(input).Append(String(".\nUse 'inventory' for a list of items and spells.")); //Error message.
			}
			else //Valid item.
			{
				inspect_valid_item; valid_item_stats;
				//Write if the item is consumable or not in its description.
				if (inspected->Consumable()) { cout << "Item is consumed on use.\n\n"; }
				else { cout << "Item is not consumed on use.\n\n"; }
				wait_for_enter();
			}
		}
		ef (Check_Command(input, String("spell"))) //spell <spell> - Describes a given spell if the player knows it. Otherwise says that they don't.
		{
			Spell* inspected = player->FindSpell(input);
			if (inspected == nullptr) //Invalid spell.
			{
				output = String("You don't know a spell called ").Append(input).Append(String(".\nUse 'inventory' for a list of items and spells.")); //Error message.
			}
			else //Valid spell.
			{
				inspect_valid_item; valid_item_stats;
				wait_for_enter();
			}
		}
		ef(Check_Command(input, String("use"))) //use <item> - Uses a given item, with an effect from its Use().
		{
			Item* use = player->FindItem(input);
			if (use == nullptr) //Invalid item.
			{
				output = String("You don't have an item called ").Append(input).Append(String(".\nUse 'inventory' for a list of items and spells.")); //Error message.
			}
			else //Valid item. Attempt to use.
			{
				Use_Item(use);
				Revenants_Turn(); //Since it's an action with effects (such as an attack), the revenants should act afterwards.
			}
		}
		ef(Check_Command(input, String("cast"))) //cast <spell> - Casts a given spell, with an effect from the Use().
		{
			Spell* cast = player->FindSpell(input);
			if (cast == nullptr) //Invalid spell.
			{
				output = String("You don't know a spell called ").Append(input).Append(String(".\nUse 'inventory' for a list of items and spells.")); //Error message.
			}
			else //Valid spell. Attempt to cast.
			{
				output = String("Casting spell ").Append(input).Append(String(".")); //Announce casting.
				Use_Item((Item*)cast);
				Revenants_Turn(); //Since it's an action with effects (such as an attack), the revenants should act afterwards.
			}
		}
		ef (Check_Command(input, String("inventory"))) //cast <spell> - Casts a given spell, with an effect from the Use().
		{
			output = prompt; //Reset command output.
			system("cls"); //Clear map to make way for command list.
			player->Inventory().WriteToConsole(); //List inventory.
			wait_for_enter();
		}
		ef (Check_Command(input, String("help"))) //help - Lists all commands.
		{
			output = prompt; //Reset command output.
			system("cls"); //Clear map to make way for command list.
			//List all commands.
			cout << "help - Lists all commands.\n\n";
			cout << "quit - Ends the game.\n\n";
			cout << "inspect <item> - Describes a given item.\n\n";
			cout << "spell <spell> - Describes a given spell.\n\n";
			cout << "inventory - Lists all items and spells you have.\n\n";
			cout << "wait - Does nothing and ends your turn, allowing the revenants to act.\n\n";
			cout << "move <north/south/east/west> Moves you 1 room in a given direction.\nThis will end your turn, allowing the revenants to act.\n\n";
			cout << "use <item> - Uses a given item, which might consume it depending on the item.\nThis will end your turn, allowing the revenants to act.\nUse 'inspect' for more detail on a specific item's effects.\n\n";
			cout << "cast <known spell> - Casts a given spell, which typically deals damage.\nThis will end your turn, allowing the revenants to act.\nUse 'spell' for more detail on a specific spell's effects.\n\n";
			wait_for_enter();
		}
		ef(input == String("")) //Empty input
		{
			output = prompt;
		}
		else
		{
			output = String("Command does not exist. Enter 'help' to see a list of commands.");
		}
	}
	
public:
	Game() //Constructor
	{
		player = new Player(0, 0); //Add player.
		player->AddSpell(new Spell(String("spark"), String("Shoots out a short-ranged spark of energy."), 5, 0)); //Starter attack.
		//Add revenants.
		loop(i, 0, 2)
		{
			newrev(3, 3);
		}
	}
	~Game() //Destructor
	{
		//Delete rooms.
		loop(i, 0, map_size)
		{
			delete[] rooms[i];
		}
		delete[] rooms;

		//Delete revenants.
		revenants.clear();
		revenants.shrink_to_fit();

		//Delete player.
		delete player;
		player = nullptr;
	}
	void Run() //Where the magic happens.
	{
		cout << "#################\n## SPELLFINDER ##\n#################\n\n";
		cout << "As you enter the dungeon, you look back.\nBehind you, where the entrance used to be, is a solid wall.\nThere has to be a way out somehow.\nPress enter to begin...\n";
		String().ReadFromConsole(); //Wait for enter key.
		String input;
		do
		{
			Update_Revenant_Count(); //Do this before drawing the map.
			system("cls"); Draw_Map(); output.WriteToConsole(); cout << endl; //Refresh every turn.
			if (player->health <= 0) { gamestate = -1; } //Player dies at 0 health.
			if (gamestate != -1)
			{
				input = String::ReadFromConsole(); //Waits for input from console.
				input.ToLower(); //Case insensitive.
				Execute(input); //Performs whatever command was input.
			}
		} while (input != String("quit") && gamestate == 0); //End program after inputting "quit".
		switch (gamestate) //End message based on what happened.
		{
			case -1: cout << "\nYOU DIED!"; break;
			case 0: cout << "\nQuitting game..."; break;
			case 1: cout << "\nThe portal opens...\nYOU WIN!"; break;
		}
	}
};