#pragma once

#include "symbolitem.h"
#include "vector"
#include "map"

using namespace std;
class SymbolTable
{
public:
	int level;
	map<string, SymbolItem> symlist;

	SymbolTable();
	SymbolTable(int l);
	SymbolTable(const SymbolTable& st);
	bool isexists(string name);
	void insert(SymbolItem item);
	SymbolItem find(string name);
	int alloc(int size);
};