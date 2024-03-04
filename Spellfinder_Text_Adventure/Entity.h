#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
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
	vector<Spell*> spells;
	vector<Item*> items;
	int BinarySearch(vector<Item*>& _vector, int _low, int _high, String _name) { //Binary search the item vector according to length of names.

		while (_low <= _high) {
			int _mid = _low + (_high - _low) / 2;

			if (_name.Length() == _vector[_mid]->Name().Length()) {
				return _mid;
			}

			if (_name.Length() > _vector[_mid]->Name().Length()) {
				_low = _mid + 1;
			}

			if (_name.Length() < _vector[_mid]->Name().Length()) {
				_high = _mid - 1;
			}

		}
		return -1;
	}
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
		while (!items.empty()) //Continuously delete and remove item pointers until empty.
		{
			delete items.back();
			items.pop_back();
		}
		while (!spells.empty()) //Continuously delete and remove spell pointers until empty.
		{
			delete spells.back();
			spells.pop_back();
		}
	}
	Item* FindItem(String& _item) //Finds an Item using binary search.
	{
		//return items[BinarySearch(items, items.begin(), items.end(), _item)];
		return nullptr;
	}
	Spell* FindSpell(String& _spell) //Finds a Spell using binary search.
	{
		//return spells[BinarySearch(spells, spells.begin(), spells.end(), _spell)];
		return nullptr;
	}
	String Inventory() //Returns a complete vector of items and spells.
	{
		String s = String("ITEMS:\n"); //List items.
		for (auto const& i : items) {
			s += i->Name();
			s += String("\n");
		}
		s += String("\nSPELLS:\n"); //List spells.
		for (auto const& i : spells) {
			s += i->Name();
			s += String("\n");
		}
		s += String("\n");
		return s;
	}
	void AddItem(Item* _item)
	{
		items.push_back(_item); //Add item to items.
		std::sort(items.begin(), items.end(), Compare); //Sort the vector after adding this new item. Items must be in alphabetical order for binary search.
	}
	void AddSpell(Spell* _spell)
	{
		spells.push_back(_spell); //Add spell to spells.
		std::sort(spells.begin(),spells.end(),Compare); //Sort the vector after adding this new item. Spells must be in alphabetical order for binary search.
	}
	struct { //Compares 2 item's names by length (for binary search).
		bool operator()(const Item* a, const Item* b) const
		{
			return a->Name().Length() < b->Name().Length();
		}
	} Compare;
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