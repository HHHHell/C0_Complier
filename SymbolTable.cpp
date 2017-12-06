#include "symbolitem.h"
#include "symboltable.h"

#include "iostream"
#include "vector"

using namespace std;

SymbolTable::SymbolTable()
{
	level = -1;
	vector<SymbolItem> symlist;
}

SymbolTable::SymbolTable(int l)
{
	level = l;
	vector<SymbolItem> symlist;
}