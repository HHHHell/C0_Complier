#include "SymbolItem.h"
#include "Token.h"
#include "string"
#include "iostream"

using namespace std;

SymbolItem::SymbolItem() :name(""), ttype(NOTYPE), kkind(NOKIND)
{
	decline = -1;
	offset = -1;
	value = 0;
}

SymbolItem::SymbolItem(string n, enum type t, enum kind k, int d)
{
	name.assign(n);
	ttype = t;
	kkind = k;
	decline = d;
	offset = -1;
	value = 0;
}

SymbolItem::SymbolItem(string n, enum type t, enum kind k, int d, int off)
{
	name.assign(n);
	ttype = t;
	kkind = k;
	offset = off;
	decline = d;
	value = 0;
}

SymbolItem::SymbolItem(string n, enum type t, enum kind k, int d, int off, int v)
{
	name.assign(n);
	ttype = t;
	kkind = k;
	offset = off;
	decline = d;
	value = v;
}

SymbolItem::SymbolItem(const SymbolItem &item)
{
	name.assign(item.name);
	ttype = item.ttype;
	kkind = item.kkind;
	decline = item.decline;
	offset = item.offset;
	value = item.value;
	paras = item.paras;
}

void SymbolItem::setoffset(int s)
{
	offset = s;
}

void SymbolItem::setparas(vector<enum type> ps)
{
	paras = ps;
}

void SymbolItem::setvalue(int v)
{
	value = v;
}

string SymbolItem::getname()
{
	return name;
}

vector<enum type> SymbolItem::getparas()
{
	return paras;
}

enum type SymbolItem::gettype()
{
	return ttype;
}

enum kind SymbolItem::getkind()
{
	return kkind;
}

int SymbolItem::getoffset()
{
	return offset;
}

int SymbolItem::getdecline()
{
	return decline;
}