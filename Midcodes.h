#pragma once

#include "string"
#include "vector"
#include "SymbolTable.h"

using namespace std;
class Midcodes
{
	map<string, SymbolTable> &tables;
	vector<vector<string>> clist;
public:
	Midcodes(map<string, SymbolTable> &t);
	SymbolItem find(string name, string nkey);
	int size();
	void refill(string s, int index);
	void insert(vector<string> strs);
	void insert(vector<string> strs, int index);
	void output(string filename);
	void toMips(string filename, map<string, SymbolTable> &tables);
};