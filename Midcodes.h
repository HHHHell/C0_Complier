#pragma once

#include "string"
#include "vector"

using namespace std;
class Midcodes
{
	vector<vector<string>> clist;
public:
	Midcodes();
	void insert(string* slist, int n);
	void output(string filename);
};