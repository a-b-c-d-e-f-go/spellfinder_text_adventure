// Item.h : This file contains the Item class and all its subclasses.

#pragma once
#include <iostream>
#include "String.h"
#define usi unsigned short int
using namespace std;

class Item //Abstract class.
{
public:
	virtual String Description() const;
	virtual void Use();
};

class Spell : Item //Implementation with variable name/damage.
{
	String name;
	int damage;
	Spell()
	{

	}
	Spell(String& _name, int _damage)
	{

	}
	String Description() const
	{

	}
	void Use()
	{

	}
	static bool Compare(Spell& a, Spell& b)
	{

	}
};

