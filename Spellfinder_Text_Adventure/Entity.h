// Entity.h : This file contains the Entity class and all its subclasses.
#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "Entity.h"
#include "Item.h"
#include "String.h"

#define usi unsigned short int
#define loop(var, min, max) for (usi var = min; var < max; var++)

using namespace std;

class Entity
{
public:
	usi x;
	usi y;
	short int health = 20;
};

class Player : public Entity //Uses x, y, and health same as revenants.
{
private:
	vector<Spell*> spells;
	vector<Item*> items;
	struct { //Compares 2 item's names alphabetically (so that the binary search has a sorted list to work with).
		bool operator()(const Item* a, const Item* b) const
		{
			return (a->Name() < b->Name());
		}
	} Compare;
	int BinarySearch(vector<Item*>& _vector, String _name) { //Binary search the item vector according to length of names.
		if (!_vector.empty()) //If the vector has contents.
		{
			int _low = 0;
			int _high = _vector.size() - 1; //Because vectors start at 0, the max is 1 less than the size.
			while (_low <= _high)
			{
				int _mid = _low + (_high - _low) / 2; //Set midpoint between low and high.
				//Compare mid to name, partitioning based on if it is higher or lower alphabetically.
				if (_name == _vector[_mid]->Name()) {
					return _mid;
				}

				if (_name > _vector[_mid]->Name()) {
					_low = _mid + 1;
				}

				if (_name < _vector[_mid]->Name()) {
					_high = _mid - 1;
				}
			}

		}
		return -1;
	}
	int BinarySearch_S(vector<Spell*>& _vector, String _name) { //Version for spells. Not familiar enough with templates yet, and there is only 1 different version needed.
		return BinarySearch((vector<Item*>&)_vector, _name); //The easy way.
	}
public:
	//Previous X & Y.
	short int px = -1;
	short int py = -1;
	Player() //Empty constructor.
	{

	}
	Player(usi _x, usi _y) //Constructor.
	{
		x = _x;
		y = _y;
	}
	~Player() //Destructor.
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
		int i = BinarySearch(items, _item);
		if (i >= 0) { return items[i]; } //Ignore -1.
		return nullptr;
	}
	Spell* FindSpell(String& _spell) //Finds a Spell using binary search.
	{
		int i = BinarySearch_S(spells, _spell);
		if (i >= 0) { return spells[i]; } //Ignore -1.
		return nullptr;
	}
	void AddItem(Item* _item)
	{
		items.push_back(_item); //Add item to items.
		std::sort(items.begin(), items.end(), Compare); //Sort the vector after adding this new item. Items must be in alphabetical order for binary search.
	}
	void AddSpell(Spell* _spell)
	{
		spells.push_back(_spell); //Add spell to spells.
		std::sort(spells.begin(), spells.end(), Compare); //Sort the vector after adding this new item. Spells must be in alphabetical order for binary search.
	}
	void RemoveItem(Item* _item) //For consumables. No need for a spell version. Once you know a spell, you know a spell.
	{
		loop(i, 0, items.size()) //Loops until finding the item. Then breaks the loop.
		{
			if (items[i] == _item) { items.erase(items.begin() + i); break; }
		}
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
};

class Revenant : public Entity
{
public:
	bool just_spawned = true; //If the revenant has been spawned this turn.
	Revenant() //Empty constructor.
	{

	}
	Revenant(usi _x, usi _y) //Constructor.
	{
		x = _x;
		y = _y;
	}
	~Revenant() //Destructor.
	{

	}
};
