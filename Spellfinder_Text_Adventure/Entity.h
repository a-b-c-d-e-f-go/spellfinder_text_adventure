#pragma once
#include <iostream>
#include <list>
#include "Entity.h"
#include "String.h"
#define usi unsigned short int
#define loop(var, min, max) for (usi var = min; var < max; var++)
#define delete_s(target) if (target != nullptr) { delete target; } //Safe version. Delete target if it exists.
#define delete_arr(target) if (target != nullptr) { delete[] target; } //Safe version. Delete targeted array if it exists.
using namespace std;

class Entity
{
public:
	usi x;
	usi y;
	usi health;
};

class Player : public Entity //Different layer, but still uses x and y.
{
private:
	list<Spell> spells;
	list<Item> items;
public:
	Player()
	{

	}
	Player(usi _x, usi _y)
	{
		x = _x;
		y = _y;
	}
	~Player()
	{

	}
	Item* FindItem(String& _item) //Finds an Item using binary search.
	{
		return nullptr;
	}
	Spell* FindSpell(String& _spell) //Finds a Spell using binary search.
	{
		return nullptr;
	}
	String Inventory() //Returns a complete list of items and spells.
	{
		/*for (auto const& i : items) {
			std::cout << i.name;
		}*/
		return String("ITEMS:\nLamp\nBox\nCat\n\nSPELLS:\nEruption\nBlizzard\nPetrify");
	}
};

class Revenant : public Entity
{
public:
	Revenant()
	{

	}
	Revenant(usi _x, usi _y)
	{
		x = _x;
		y = _y;
	}
	~Revenant()
	{

	}
};