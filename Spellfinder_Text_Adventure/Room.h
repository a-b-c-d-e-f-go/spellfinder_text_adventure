#pragma once
#include <iostream>
#include "String.h"
#include "Item.h"
#define usi unsigned short int
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
	Room(String& _desc, Item* _item)
	{

	}
	~Room()
	{

	}
	String Description() const
	{

	}
};