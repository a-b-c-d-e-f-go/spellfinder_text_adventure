#pragma once
#include <iostream>
#include "String.h"
#include "Item.h"
#include "String.h"
#include "Item.h"
#define usi unsigned short int
#define delete_s(target) if (target != nullptr) { delete target; } //Safe version. Delete target if it exists.
#define delete_arr(target) if (target != nullptr) { delete[] target; } //Safe version. Delete targeted array if it exists.
using namespace std;

class Room
{
private:
	String description;
public:
	Item* item;
	Room()
	{
	}
	Room(String& _desc, Item* _item = nullptr)
	{
		description = _desc;
		item = _item;
	}
	Room(const char* _desc, Item* _item = nullptr) //For char arrays.
	{
		description = String(_desc); //Convert char array to custom string.
		item = _item;
	}
	~Room()
	{
		delete item;
		item = nullptr;
	}
	String Description() const
	{
		if (item != nullptr)
		{
			String s = description;
			return s.Append("\n").Append(item->RoomDescription());
		}
		return description;
	}
};