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
	/*int binarySearch(list<Item> _list, String _low, String _high, String _name) {

		while (_low <= _high) {
			int _mid = low + (high - low) / 2;

			if (number_to_search_for == _list[mid]) {
				return _mid;
			}

			if (number_to_search_for > _list[mid]) {
				_low = _mid + 1;
			}

			if (number_to_search_for < _list[mid]) {
				_high = _mid - 1;
			}

		}

		return -1;
	}*/
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
		String s = String("ITEMS:\n");
		for (auto const& i : items) {
			s += i.Name();
		}
		//String("ITEMS:\nLamp\nBox\nCat\n\nSPELLS:\nEruption\nBlizzard\nPetrify\n\n")
		return s;
	}
	void AddItem(Item* _item)
	{

		//Sort the list after adding this new item. Items must be in alphabetical order for binary search.
		/*sort(items.begin(), items.end(),
		[](const Item& a, const Item& b) {
			return a < b;
		});*/
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