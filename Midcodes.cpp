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
		string str;
		str.assign(strs[i]);
		s.insert(s.end(), str);
	}
	clist.insert(clist.end(), s);
}

void Midcodes::insert(vector<string> strs, int index)
{
	vector<string> s;
	for (int i = 0; i < strs.size(); i++)
	{
		string str;
		str.assign(strs[i]);
		s.insert(s.end(), str);
	}
	vector<vector<string>>::iterator iter = clist.begin();
	if (index == clist.size())
		iter = clist.end();
	else
	{
		vector<string> tar = clist[index];
		while (iter != clist.end())
		{
			if (*iter == tar)
				break;
			iter++;
		}
	}
	clist.insert(iter, s);
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