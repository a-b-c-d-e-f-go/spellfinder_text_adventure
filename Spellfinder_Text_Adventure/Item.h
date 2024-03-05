// Item.h : This file contains the Item class and all its subclasses.

#pragma once
#include <iostream>
#include "String.h"
#define usi unsigned short int
#define delete_s(target) if (target != nullptr) { delete target; } //Safe version. Delete target if it exists.
#define delete_arr(target) if (target != nullptr) { delete[] target; } //Safe version. Delete targeted array if it exists.
using namespace std;

class Item //Abstract class.
{
public:
	virtual int Damage() //Damage to anything else in the room.
	{
		return 0;
	}
	virtual int Self_Damage() //Damage to the player. Negative is healing.
	{
		return 0;
	}
	virtual bool Consumable() const { return false; } //Consumed on use?
	virtual String Name() const //For sorting & finding.
	{
		return String("ERROR");
	}
	virtual String Description() const //When inspected.
	{
		return String("ERROR");
	}
	virtual String RoomDescription() const //When found in a room.
	{
		return String("ERROR");
	}
	virtual void Use() //When used.
	{

	}
	virtual String Shorthand() const //When drawing the map.
	{
		return String("ITM");
	}
	
};
class Spear : public Item
{
public:
	String Name() const override //For sorting & finding.
	{
		return String("spear");
	}
	String Description() const override //When inspected.
	{
		return String("A spear with a wooden shaft and a rusted iron point.\nAlthough it was merely for decoration here,\nit seems to have seen use before being held by the statue.\n\nUSE:\n");
	}
	String RoomDescription() const override //When found in a room.
	{
		return String("The crumbled statue's spear is lying on the ground. Obtained the SPEAR.");
	}
	int Damage() override //Begginer weapon, but better than spark.
	{
		return 7;
	}
	Spear()
	{

	}
};
class Apple : public Item
{
public:
	bool Consumable() const override //Consumed on use.
	{
		return true;
	}
	String Name() const override //For sorting & finding.
	{
		return String("apple");
	}
	String Description() const override //When inspected.
	{
		return String("Apple.\n\nUSE:\n");
	}
	String RoomDescription() const override //When found in a room.
	{
		return String("Obtained the APPLE.");
	}
	void Use() override //When used.
	{

	}
	Apple()
	{

	}
};
class Bomb : public Item
{
public:
	bool Consumable() const override //Consumed on use.
	{
		return true;
	}
	String Name() const override //For sorting & finding.
	{
		return String("bomb");
	}
	String Description() const override //When inspected.
	{
		return String("High-yield explosive likely intended for mining. Handle with care.\n\nUSE:\n");
	}
	String RoomDescription() const override //When found in a room.
	{
		return String("Obtained the BOMB.");
	}
	int Damage() override //High damage.
	{
		return 15;
	}
	int Self_Damage() override //The kamikaze weapon. No reason not to use it if it kills remaining revenants but not you, but otherwise risky.
	{
		return 15;
	}
	Bomb()
	{

	}
};



class Spell : public Item //Implementation with variable name/damage.
{
private:
	String name = String("Mystery Spell");
	String desc = String("Mystery Spell Lore");
	int damage = 0;
	int self_damage = 0;
public:
	int Damage() override //Damage to anything else in the room.
	{
		return damage;
	}
	int Self_Damage() override //Damage to the player. Negative is healing.
	{
		return self_damage;
	}
	Spell()
	{

	}
	Spell(String _name, String _desc, int _damage = 0, int _self_damage = 0)
	{
		name = _name;
		desc = _desc;
		damage = _damage;
		self_damage = _self_damage;
	}
	String Name() const //For sorting & finding.
	{
		return name;
	}
	String Description() const override //When inspected.
	{
		String s = desc;
		s += String("\n\nCAST:\n");
		return s;
	}
	void Use() override
	{

	}
	String Shorthand() const override //When drawing the map.
	{
		return String("SPL");
	}
};
class Scroll : public Item
{
public:
	Spell* spell = nullptr;
	bool Consumable() const override //Consumed on use.
	{
		return true;
	}
	String Name() const override //For sorting & finding.
	{
		String s = spell->Name();
		s.ToLower();
		s.Append(" scroll");
		return s; //Eg. spark scroll
	}
	String Description() const override //When inspected.
	{
		String s = String("Parchment with enigmatic symbols written all over.\nIt will take time to copy this over to your spellbook.\n\nUse: Adds the spell '");
		s.Append(spell->Name()).Append("' to your spellbook.");
		return s;
	}
	String RoomDescription() const override //When found in a room.
	{
		String s = Name();
		s.ToUpper();
		s.Prepend(String("Obtained the "));
		s.Append(".");
		return s; //Eg. Obtained the SPARK SCROLL.
	}
	void Use() override //When used.
	{

	}
	Scroll()
	{
		
	}
	Scroll(Spell* _spell)
	{
		spell = _spell;
	}
	~Scroll()
	{
		delete spell;
		spell = nullptr;
	}
};

