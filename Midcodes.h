#pragma once

#include "string"
#include "vector"

using namespace std;
class Midcodes
{
	vector<vector<string>> clist;
public:
	int size();
	void refill(string s, int index);
	void insert(vector<string> strs);
	void insert(vector<string> strs, int index);
	void output(string filename);
};