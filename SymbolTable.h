#pragma once

#include "symbolitem.h"
#include "vector"
#include "map"

using namespace std;
class SymbolTable
{
	int level;
	int bias;

public:
	map<string, SymbolItem> symlist;
	SymbolTable();
	SymbolTable(int l);
	bool isexists(string name);
	void insert(SymbolItem item);
	SymbolItem find(string name);
	int alloc(int size);
	int getsize();
};
