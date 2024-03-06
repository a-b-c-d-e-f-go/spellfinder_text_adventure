// Room.h : This file contains the Room class, which can store a description and an item.
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
	Room() //Empty onstructor.
	{
	}
	Room(String& _desc, Item* _item = nullptr) //Constructor.
	{
		description = _desc;
		item = _item;
	}
	Room(const char* _desc, Item* _item = nullptr) //For char arrays.
	{
		description = String(_desc); //Convert char array to custom string.
		item = _item;
	}
	~Room() //Destructor.
	{
		delete item;
		item = nullptr;
	}
	String Description() const //Externally read-only.
	{
		return description;
	}
};