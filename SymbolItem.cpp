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

SymbolItem::SymbolItem(const SymbolItem &item)
{
	name.assign(item.name);
	ttype = item.ttype;
	kkind = item.kkind;
	offset = item.offset;
	value = item.value;
}

void SymbolItem::setoffset(int s)
{
	offset = s;
}

void SymbolItem::setvalue(int v)
{
	value = v;
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