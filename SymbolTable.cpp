#include "symbolitem.h"
#include "symboltable.h"

#include "iostream"
#include "vector"

using namespace std;

SymbolTable::SymbolTable()
{
	level = -1;
	bias = 64;
}

SymbolTable::SymbolTable(int l)
{
	level = l;
	bias = 64;
}

bool SymbolTable::isexists(string name)
{
	map<string, SymbolItem>::iterator iter = symlist.find(name);
	return iter != symlist.end();
}

void SymbolTable::insert(SymbolItem item)
{
	symlist.insert(pair<string, SymbolItem>(item.getname(), item));
	cout << symlist.size() << " Insert " << item.getname() << endl;
}

SymbolItem SymbolTable::find(string name)
{
	map<string, SymbolItem>::iterator iter = symlist.find(name);
	if (iter == symlist.end())
		return SymbolItem();
	return iter->second;
}

int SymbolTable::alloc(int size)
{
	int tmp = bias;
	bias += size;
	return tmp;
}

int SymbolTable::getsize()
{
	return bias;
}