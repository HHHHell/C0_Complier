#pragma once
#include "token.h"
#include "define.h"
#include "vector"
#include "string"

using namespace std;
class SymbolItem
{
	string name;
	enum type ttype;
	enum kind kkind;
	int value;					//only for const_var
	int offset;
	int decline;
	string str;
	vector<enum type> paras;			//only for functions

public:
	SymbolItem();
	SymbolItem(string n, enum type t, enum kind k, int line);
	SymbolItem(string n, enum type t, enum kind k, int line, int off);
	SymbolItem(string n, enum type t, enum kind k, 
		int line, int off, int v);
	SymbolItem(string name, string strvalue);
	SymbolItem(const SymbolItem &item);
	void setoffset(int s);
	void setvalue(int v);
	void setparas(vector<enum type> ps);

	string getname();
	vector<enum type> getparas();
	enum type gettype();
	enum kind getkind();
	int getoffset();
	int getdecline();
	int getvalue();
	string getstr();
};