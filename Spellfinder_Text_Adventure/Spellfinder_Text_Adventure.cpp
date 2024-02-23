// Spellfinder_Text_Adventure.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "String.h"
#include "Game.h"
#define usi unsigned short int
using namespace std;

int main()
{
    Game* g = new Game();
    g->Run();
}