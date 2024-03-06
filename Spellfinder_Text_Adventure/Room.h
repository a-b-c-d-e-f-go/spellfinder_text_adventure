#pragma once
#include <iostream>
#include "String.h"
#include "Item.h"
#include "String.h"
#include "Item.h"
#define usi unsigned short int
using namespace std;

class Room
{
private:
	String description = String();
public:
	Item* item = nullptr;
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
		return description;
	}
};