#include "vector"
#include "string"
#include "iostream"
#include "fstream"

#include "midcodes.h"

using namespace std;

Midcodes::Midcodes()
{
	vector<string[5]> clist;
}

void Midcodes::insert(string* slist, int n)
{
	string s[5];
	for (int i = 0; i < 5; i++)
	{
		if (i < 5)
			s[i].assign(slist[i]);
		s[i] = "";
	}
	clist.insert(clist.end(), s);
}

void Midcodes::output(string filename)
{
	ofstream fout;
	fout.open(filename);
	for (int i = 0; i < clist.size(); i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (clist[i][j] == "")
				continue;
			fout << clist[i][j] << " ";
		}
		fout << endl;
	}
	fout.flush();
	fout.close();
}