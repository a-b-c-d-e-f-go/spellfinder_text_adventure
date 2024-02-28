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
	String Contents() const
	{
		return String("   ");
	}
	Room()
	{
	}
	Room(String& _desc, Item* _item)
	{
		
	}
	~Room()
	{
		delete item;
		item = nullptr;
	}
	String Description() const
	{

	}
};