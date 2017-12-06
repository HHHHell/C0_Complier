#pragma once
#include "token.h"
#include "string"

using namespace std;
class SymbolItem
{
	string name;
	enum type ttype;
	enum kind kkind;
	int value;		//only for const_var
	int offset;
	int decline;
public:
	SymbolItem();
	SymbolItem(string n, enum type t, enum kind k, int d);
	SymbolItem(string n, enum type t, enum kind k, int d, int off);
	SymbolItem(const SymbolItem &item);
	void setoffset(int s);
	void setvalue(int v);
	enum type gettype();
	enum kind getkind();
	int getoffset();
};