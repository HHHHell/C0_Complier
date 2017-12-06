#pragma once

#include "symbolitem.h"
#include "vector"

using namespace std;
class SymbolTable
{
public:
	int level;
	vector<SymbolItem> symlist;

	SymbolTable();
	SymbolTable(int l);
};