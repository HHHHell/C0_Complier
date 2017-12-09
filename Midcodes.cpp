#include "vector"
#include "string"
#include "iostream"
#include "fstream"

#include "midcodes.h"

using namespace std;

void Midcodes::insert(vector<string> strs)
{
	vector<string> s;
	for (int i = 0; i < strs.size(); i++)
	{
		s[i].assign(strs[i]);
	}
	clist.insert(clist.end(), s);
}

void Midcodes::refill(string s, int index)
{
	clist[index].insert(clist[index].end(), s);
}

int Midcodes::size()
{
	return clist.size();
}

void Midcodes::output(string filename)
{
	ofstream fout;
	fout.open(filename);
	for (int i = 0; i < clist.size(); i++)
	{
		for (int j = 0; j < clist[i].size(); j++)
		{
			fout << clist[i][j] << " ";
		}
		fout << endl;
	}
	fout.flush();
	fout.close();
}