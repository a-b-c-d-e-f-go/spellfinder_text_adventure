#pragma once
#include <iostream>
#include <list>
#include "Entity.h"
#include "String.h"
#define usi unsigned short int
#define delete_s(target) if (target != nullptr) { delete target; } //Safe version. Delete target if it exists.
#define delete_arr(target) if (target != nullptr) { delete[] target; } //Safe version. Delete targeted array if it exists.
using namespace std;

class Entity
{
public:
	usi x;
	usi y;
};

class Player : public Entity //Different layer, but still uses x and y.
{
private:
	list<String> spells;
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
	bool FindSpell(String& _spell)
	{

	}
};

class Ghost : public Entity
{
public:

};