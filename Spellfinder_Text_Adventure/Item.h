// Item.h : This file contains the Item class and all its subclasses.

#pragma once
#include <iostream>
#include "String.h"
#define usi unsigned short int
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
	virtual void Use(String& _output) //When used.
	{
		if (Damage() > 0) //Can't be specific to revenants present because it's not in the Game class.
		{
			_output += String("\nAttacked for ");
			_output += String(to_string(Damage()));
			_output += String(" damage. ");
		}
		if (Self_Damage() > 0)
		{
			_output += String("\nDealt ");
			_output += String(to_string(Self_Damage()));
			_output += String(" damage to yourself. ");
		}
		else if (Self_Damage() < 0)
		{
			_output += String("\nHealed ");
			_output += String(to_string(Self_Damage() * -1));
			_output += String(" health. ");
		}
	}
	virtual usi UniqueEffect() //When used.
	{
		return 0;
	}
	virtual String Shorthand() const //When drawing the map.
	{
		return String("ITM");
	}
	
};

class Spear : public Item
{
public:
	void Use(String& _output) override //When used.
	{
		_output = String("You attack with the spear.");
		Item::Use(_output);
	}
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
		return String("The crumbled statue's spear is lying on the ground. Obtained the spear.");
	}
	int Damage() override //Begginer weapon, but better than spark.
	{
		return 7;
	}
	Spear() //Constructor.
	{

	}
};
class Glowfruit : public Item
{
public:
	void Use(String& _output) override //When used.
	{
		_output = String("You consume the glowfruit.");
		Item::Use(_output);
	}
	bool Consumable() const override //Consumed on use.
	{
		return true;
	}
	String Name() const override //For sorting & finding.
	{
		return String("glowfruit");
	}
	String Description() const override //When inspected.
	{
		return String("Strange bioluminescant fruit with an apple-ish scent.\n\nUSE:\n");
	}
	String RoomDescription() const override //When found in a room.
	{
		return String("There's something hanging from one of the plants. Obtained the glowfruit.");
	}
	int Self_Damage() override //Item gives a full heal. You'll need it.
	{
		return -20;
	}
	Glowfruit() //Constructor.
	{

	}
};
class Bomb : public Item
{
public:
	void Use(String& _output) override //When used.
	{
		_output = String("You detonate the bomb.");
		Item::Use(_output);
	}
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
		return String("An explosive device sits near the wall. Obtained the bomb.");
	}
	int Damage() override //High damage.
	{
		return 20;
	}
	int Self_Damage() override //The kamikaze weapon. No reason not to use it if it kills remaining revenants and not you, but otherwise risky.
	{
		return 15;
	}
	Bomb() //Constructor.
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
	void Use(String& _output) override //When used.
	{
		_output = String("You cast ").Append(name).Append(String(". "));
		Item::Use(_output);
	}
	usi UniqueEffect() override //When used.
	{
		if (Name() == String("vortex")) { return 1; } //Summons a revenant at 1,3.
		if (Name() == String("activate")) { return 3; } //Wins the game if at 2,0.
		return 0;
	}
	int Damage() override //Damage to anything else in the room.
	{
		return damage;
	}
	int Self_Damage() override //Damage to the player. Negative is healing.
	{
		return self_damage;
	}
	String Name() const //For sorting & finding.
	{
		return name;
	}
	String Description() const override //When inspected.
	{
		String s = desc;
		s += String("\n\nCAST:\n");
		if (name == String("activate"))
		{
			s += String("Activates a portal, if there is one in this room.\n");
		}
		if (name == String("vortex"))
		{
			s += String("Spawns 3 revenants in room (1,3).\n");
		}
		return s;
	}
	String Shorthand() const override //When drawing the map.
	{
		return String("SPL");
	}
	Spell() //Empty onstructor.
	{

	}
	Spell(String _name, String _desc, int _damage = 0, int _self_damage = 0) //Constructor.
	{
		name = _name;
		desc = _desc;
		damage = _damage;
		self_damage = _self_damage;
	}
};
class Scroll : public Item
{
public:
	Spell* spell = nullptr;
	void Use(String& _output) override //When used.
	{
		_output = String("You transcribe the ").Append(spell->Name()).Append(String(" scroll to your spellbook."));
		Item::Use(_output);
	}
	usi UniqueEffect() override //When used.
	{
		return 2; //Obtains spell.
	}
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
		String s = String("Parchment with enigmatic symbols written all over.\nIt will take time to copy this over to your spellbook,\nand doing so may bring more revenants.\n\nUSE:\nAdds the spell '");
		s.Append(spell->Name()).Append("' to your spellbook.\nSpawns a revenant in room (0,0).\n");
		return s;
	}
	String RoomDescription() const override //When found in a room.
	{
		String s = Name();
		s.ToLower();
		s.Prepend(String("Obtained the "));
		s.Append(".");
		return s; //Eg. Obtained the spark scroll.
	}
	Scroll() //Empty constructor.
	{
		
	}
	Scroll(Spell* _spell) //Constructor.
	{
		spell = _spell;
	}
	~Scroll() //Destructor.
	{
		delete spell;
		spell = nullptr;
	}
};