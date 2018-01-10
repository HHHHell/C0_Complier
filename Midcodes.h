#pragma once

#include "string"
#include "vector"
#include "SymbolTable.h"

using namespace std;

class Midcodes
{
	map<string, SymbolTable> &tables;
	vector<vector<string>> clist;
	vector<string> mpcode;

public:
	Midcodes(map<string, SymbolTable> &t);
	SymbolItem find(string name, string nkey);
	int size();
	void refill(string s, int index);
	void insert(vector<string> strs);
	void insert(vector<string> strs, int index);
	void output(string filename);
	void loadword(string name, string nkey, string tar, string index = "");
	void saveword(string name, string nkey, string src, string index = "");
	void toMips(string filename, map<string, SymbolTable> &tables);
};