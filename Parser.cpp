#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "error.h"

#include "iostream"
#include "string"
#include "vector"
#include "sstream"

using namespace std;
extern std::string str2lower(std::string str);

string type2string(enum type ttype)
{
	string list[] = {"null", "int", "char", "void"};
	return list[ttype];
}

/*
string to_string(int num)
{
	ostringstream ostr;
	ostr << num;
	string tmp = ostr.str();
	return tmp;
}
*/

Parser::Parser(Lexer &l, map<string, SymbolTable> &tlist, Midcodes &m)
	: lex(l), tables(tlist), midcodes(m), result(false)
{
	varnum = 1;
	ntable = &(tables.find("#OverAll")->second);
	nkey = "#OverAll";
	parserout.open("result\\parser_result.txt");
}

Parser::~Parser()
{
	if (result)
		parserout << "\nFinished!" << endl;
	else
		parserout << "\nError!" << endl;
	parserout.flush();
	parserout.close();
}

void Parser::printresult(string str)
{
	parserout << str << endl;
//	cout << str << endl;
	parserout.flush();
}

Token Parser::gettoken(int mode = 0)
{
	Token ntoken;
	switch (mode)
	{
	case 0:
		if (pretoken.size() == 0)
		{
			ntoken = lex.nextsymbol();
		}
		else
		{
			ntoken = pretoken[0];
			pretoken.erase(pretoken.begin());
		}
		break;
	case 1:
		if (pretoken.size() == 0)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.end(), ntoken);
		}
		else
			ntoken = pretoken[0];
		break;
	}
	return ntoken;
}

bool Parser::isexists(string name)
{
	if (nkey != "#OverAll")
	{
		bool re = ntable->isexists(name);
		if (re == true)
			return true;
		map<string, SymbolTable>::iterator iter = tables.find("#OverAll");
		SymbolTable &ttable = iter->second;
		return ttable.isexists(name);
	}
	else
	{
		return ntable->isexists(name);
	}
}

SymbolItem Parser::pfind(string name)
{
	if (nkey != "#OverAll")
	{
		if (ntable->isexists(name))
		{
			SymbolItem item = ntable->find(name);
			return item;
		}
		map<string, SymbolTable>::iterator iter = tables.find("#OverAll");
		SymbolTable &ttable = iter->second;
		if (ttable.isexists(name))
		{
			SymbolItem item = ttable.find(name);
			return item;
		}
		return SymbolItem();
	}
	else
	{
		if (ntable->isexists(name))
		{
			SymbolItem item = ntable->find(name);
			return item;
		}
		return SymbolItem();
	}
}

string Parser::genvar()
{
	string name = "#var" + to_string(varnum++);
	int off = ntable->alloc(4);
	SymbolItem item(name, INT_TYPE, VARIABLE, -1, off);
	ntable->insert(item);
	return name;
}

string Parser::genvar(int value)
{
	string name = "#var" + to_string(varnum++);
	int off = ntable->alloc(4);
	SymbolItem item(name, INT_TYPE, VARIABLE, value, off);
	ntable->insert(item);
	return name;
}

string Parser::genvar(enum type t)
{
	string name = "#var" + to_string(varnum++);
	int off = ntable->alloc(4);
	SymbolItem item(name, t, VARIABLE, -1, off);
	ntable->insert(item);
	return name;
}

string Parser::genvar(int value, enum type t)
{
	string name = "#var" + to_string(varnum++);
	int off = ntable->alloc(4);
	SymbolItem item(name, t, VARIABLE, value, off);
	ntable->insert(item);
	return name;
}

string Parser::genvar(string str)
{
	static int strnum = 1;
	map<string, SymbolTable>::iterator iter = tables.find("#StringConst");

	map<string, SymbolItem>::iterator iter2 = iter->second.symlist.begin();
	while (iter2 != iter->second.symlist.end() && iter2->second.getstr() != str)
	{
		iter2++;
	}
	if (iter2 != iter->second.symlist.end())
	{
		return iter2->first;
	}

	string name = "_string" + to_string(strnum++);
	SymbolItem item(name, str);
	iter->second.insert(item);
	return name;
}

string Parser::genlabel()
{
	static int num = 1;
	return ".label" + to_string(num++);
}

bool Parser::program()
{
	using namespace std;

	Token ntoken = gettoken(1);

	if (ntoken.getType() == CONST)
	{
		bool re = constDeclare();
		if (!re)
			return false;
		ntoken = gettoken(1);
	}
	if (ntoken.getType() == INT || ntoken.getType() == CHAR)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.end(), ntoken);
		if (ntoken.getType() != IDENTITY)
		{
			cout << "Syntax errer in line: " << ntoken.getLinenum() << endl;
			return false;
		}
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.end(), ntoken);
		enum symbol type = ntoken.getType();
		if (type == L_BRACK || type == L_CURLY)
		{
			bool re = retfunDef();
			if (!re)
				return false;
			ntoken = gettoken(1);
			if (ntoken.getType() != VOID && ntoken.getType() != INT &&
				ntoken.getType() != CHAR)
				return false;
			while (pretoken[0].getType() == VOID || pretoken[0].getType() == INT ||
				pretoken[0].getType() == CHAR)
			{
				if (pretoken[0].getType() == VOID)
				{
					if (pretoken.size() == 1)
					{
						ntoken = lex.nextsymbol();
						pretoken.insert(pretoken.end(), ntoken);
					}
					else
						ntoken = pretoken[1];
					string idname = str2lower(ntoken.getStrValue());
					if (ntoken.getType() == IDENTITY && idname == "main")
					{
						bool re = mainFun();
						if (!re)
							return false;
						break;
					}
					else if (ntoken.getType() == IDENTITY && idname != "main")
					{
						bool re = unretfunDef();
						if (!re)
							return false;
					}
					else
						return false;
				}
				else if (pretoken[0].getType() == INT || pretoken[0].getType() == CHAR)
				{
					bool re = retfunDef();
					if (!re)
						return false;
				}
				else
					return false;
			}
		}
		else if (type == SEMICOLON || type == COMMA || type == L_SQUARE)
		{
			bool re = variableDeclare();
			if (!re)
				return false;
			while (pretoken[0].getType() == VOID || pretoken[0].getType() == INT ||
				pretoken[0].getType() == CHAR)
			{
				if (pretoken[0].getType() == VOID)
				{
					if (pretoken.size() == 1)
					{
						ntoken = lex.nextsymbol();
						pretoken.insert(pretoken.end(), ntoken);
					}
					else
						ntoken = pretoken[1];
					string idname = str2lower(ntoken.getStrValue());
					if (ntoken.getType() == IDENTITY && idname == "main")
					{
						bool re = mainFun();
						if (!re)
							return false;
						break;
					}
					else if (ntoken.getType() == IDENTITY)
					{
						bool re = unretfunDef();
						if (!re)
							return false;
					}
				}
				else if (pretoken[0].getType() == INT || pretoken[0].getType() == CHAR)
				{
					bool re = retfunDef();
					if (!re)
						return false;
				}
				else
					return false;
			}
		}
		else
			return false;
	}
	else if (ntoken.getType() == VOID)
	{
		if (pretoken.size() == 1)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.end(), ntoken);
		}
		else
			ntoken = pretoken[1];
		string idname = str2lower(ntoken.getStrValue());
		if (ntoken.getType() == IDENTITY && idname == "main")
		{
			bool re = mainFun();
			if (!re)
				return false;
		}
		else if (ntoken.getType() == IDENTITY)
		{
			bool re = unretfunDef();
			if (!re)
				return false;
			while (pretoken[0].getType() == VOID || pretoken[0].getType() == INT ||
				pretoken[0].getType() == CHAR)
			{

				if (pretoken[0].getType() == VOID)
				{
					if (pretoken.size() == 1)
					{
						ntoken = lex.nextsymbol();
						pretoken.insert(pretoken.end(), ntoken);
					}
					else
						ntoken = pretoken[1];
					string idname = str2lower(ntoken.getStrValue());
					if (ntoken.getType() == IDENTITY && idname == "main")
					{
						bool re = mainFun();
						if (!re)
							return false;
						break;
					}
					else if (ntoken.getType() == IDENTITY)
					{
						bool re = unretfunDef();
						if (!re)
							return false;
					}
					else
						return false;
				}
				else if (pretoken[0].getType() == INT || pretoken[0].getType() == CHAR)
				{
					bool re = retfunDef();
					if (!re)
						return false;
				}
				else
					return false;
			}
		}
		else
			return false;

	}
	printresult("This is a program!");
	return true;
}

bool Parser::constDeclare()
{
	Token ntoken = gettoken();

	if (ntoken.getType() != CONST)
		return false;
	ntoken = gettoken(1);
	bool re = constDef();
	if (!re)
		return false;
	ntoken = gettoken();
	if (ntoken.getType() != SEMICOLON)
		return false;
	printresult("This is a Const Variable Declare!");
	ntoken = gettoken(1);

	while (ntoken.getType() == CONST)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken(1);
		bool re = constDef();
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != SEMICOLON)
			return false;
		printresult("This is a Const Variable Declare!");
		ntoken = gettoken(1);
	}
	return true;
}

bool Parser::constDef()
{
	string name;
	enum type ttype;
	enum kind kkind = CONST_VAR;
	int value, line, offset, size;

	Token ntoken = gettoken();
	if (ntoken.getType() != INT && ntoken.getType() != CHAR)
	{
		return false;
	}

	if (ntoken.getType() == INT)
	{
		ttype = INT_TYPE;
		size = 4;

		ntoken = gettoken();
		if (ntoken.getType() != IDENTITY)
			return false;
		name = ntoken.getStrValue();
		line = ntoken.getLinenum();

		ntoken = gettoken();
		if (ntoken.getType() != ASSIGN)
			return false;

		ntoken = gettoken();
		int sign = 1;
		if (ntoken.getType() == MINUS)
		{
			ntoken = gettoken();
			sign = -1;
		}
		else if (ntoken.getType() == PLUS)
			ntoken = gettoken();

		if (ntoken.getType() != CONST_INT)
			return false;
		printresult("This is a Const Variable Define!");

		value = sign*ntoken.getIntValue();
		offset = ntable->alloc(4);
		SymbolItem nitem(name, ttype, kkind, line, offset, value);
		ntable->insert(nitem);

		ntoken = gettoken(1);
		while (ntoken.getType() == COMMA)
		{
			pretoken.erase(pretoken.begin());
			ntoken = gettoken();
			if (ntoken.getType() != IDENTITY)
				return false;

			name = ntoken.getStrValue();
			line = ntoken.getLinenum();

			ntoken = gettoken();
			if (ntoken.getType() != ASSIGN)
				return false;
			ntoken = gettoken();

			sign = 1;
			if (ntoken.getType() == MINUS)
			{
				ntoken = gettoken();
				sign = -1;
			}
			else if (ntoken.getType() == PLUS)
				ntoken = gettoken();

			if (ntoken.getType() != CONST_INT)
				return false;
			printresult("This is a Const Variable Define!");

			value = sign*ntoken.getIntValue();
			offset = ntable->alloc(4);
			SymbolItem nitem(name, ttype, kkind, line, offset, value);
			ntable->insert(nitem);

			ntoken = gettoken(1);
		}
	}
	else
	{
		ttype = CHAR_TYPE;

		ntoken = gettoken();
		if (ntoken.getType() != IDENTITY)
			return false;
		name = ntoken.getStrValue();
		line = ntoken.getLinenum();

		ntoken = gettoken();
		if (ntoken.getType() != ASSIGN)
			return false;

		ntoken = gettoken();
		if (ntoken.getType() != CONST_CHAR)
			return false;

		value = ntoken.getCharValue();
		offset = ntable->alloc(4);
		SymbolItem nitem(name, ttype, kkind, line, offset, value);
		ntable->insert(nitem);

		printresult("This is a Const Variable Define!");

		ntoken = gettoken(1);
		while (ntoken.getType() == COMMA)
		{
			pretoken.erase(pretoken.begin());
			ntoken = gettoken();
			if (ntoken.getType() != IDENTITY)
				return false;
			name = ntoken.getStrValue();
			line = ntoken.getLinenum();

			ntoken = gettoken();
			if (ntoken.getType() != ASSIGN)
				return false;
			ntoken = gettoken();
			if (ntoken.getType() != CONST_CHAR)
				return false;

			value = ntoken.getCharValue();
			offset = ntable->alloc(4);
			SymbolItem nitem(name, ttype, kkind, line, offset, value);

			ntable->insert(nitem);

			printresult("This is a Const Variable Define!");
			ntoken = gettoken(1);
		}
	}
	return true;
}

bool Parser::variableDeclare()
{
	bool re = variableDef();
	if (!re)
		return false;
	Token ntoken = gettoken();
	if (ntoken.getType() != SEMICOLON)
	{
		return false;
	}
	printresult("This is a Variable Declare!");

	ntoken = gettoken(1);

	while (true)
	{
		ntoken = gettoken(1);
		if (ntoken.getType() != INT && ntoken.getType() != CHAR)
			break;

		if (pretoken.size() == 1)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.end(), ntoken);
		}
		else
			ntoken = pretoken[1];
		if (ntoken.getType() != IDENTITY)
			break;

		if (pretoken.size() == 2)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.end(), ntoken);
		}
		else
			ntoken = pretoken[0];

		if (ntoken.getType() != SEMICOLON && ntoken.getType() != COMMA &&
			ntoken.getType() != L_SQUARE)
			break;
		re = variableDef();
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != SEMICOLON)
		{
			return false;
		}
		printresult("This is a Variable Declare!");
	}
	return true;
}

bool Parser::variableDef()
{
	string name;
	enum type ttype;
	enum kind kkind = VARIABLE;
	int line, offset, bsize, size;

	bsize = 4;
	Token ntoken = gettoken();
	if (ntoken.getType() != INT && ntoken.getType() != CHAR)
		return false;
	if (ntoken.getType() == INT)
	{
		ttype = INT_TYPE;
		size = bsize;
	}
	else
	{
		ttype = CHAR_TYPE;
		size = bsize;
	}
	ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	name = ntoken.getStrValue();
	line = ntoken.getLinenum();

	ntoken = gettoken(1);
	if (ntoken.getType() == L_SQUARE)
	{
		kkind = VAR_LIST;
		pretoken.erase(pretoken.begin());
		ntoken = gettoken();
		if (ntoken.getType() != CONST_INT)
			return false;
		size = bsize * ntoken.getIntValue();
		ntoken = gettoken();
		if (ntoken.getType() != R_SQUARE)
			return false;
	}

	offset = ntable->alloc(size);
	SymbolItem nitem(name, ttype, kkind, line, offset);
	ntable->insert(nitem);

	printresult("This is a Variable Define!");
	ntoken = gettoken(1);

	kkind = VARIABLE;
	while (ntoken.getType()  == COMMA)
	{
		size = bsize;

		pretoken.erase(pretoken.begin());
		ntoken = gettoken();
		if (ntoken.getType() != IDENTITY)
			return false;
		name = ntoken.getStrValue();
		line = ntoken.getLinenum();

		ntoken = gettoken(1);
		if (ntoken.getType() == L_SQUARE)
		{
			kkind = VAR_LIST;
			pretoken.erase(pretoken.begin());
			ntoken = gettoken();
			if (ntoken.getType() != CONST_INT)
				return false;
			size = bsize * ntoken.getIntValue();

			ntoken = gettoken();
			if (ntoken.getType() != R_SQUARE)
				return false;
		}

		offset = ntable->alloc(size);
		SymbolItem nitem(name, ttype, kkind, line, offset);

		ntable->insert(nitem);

		printresult("This is a Variable Define!");
		ntoken = gettoken(1);
	}
	return true;
}

bool Parser::retfunDef()
{
	vector<enum type> paras;
	string name;
	enum type ttype;
	enum kind kkind = FUNCTION;
	int line;

	bool re = funDefhead(ttype, name, line);
	if (!re)
		return false;

	midcodes.insert({ name, ":" });
	int index = midcodes.size();

	SymbolItem nitem(name, ttype, kkind, line);
	ntable->insert(nitem);
	tables.insert(pair<string, SymbolTable>(name, SymbolTable(1)));
	map<string, SymbolTable>::iterator iter = tables.find(name);
	if (iter == tables.end())
	{
		cout << "Insert error" << endl;
		return false;
	}
	ntable = &(iter->second);
	nkey = iter->first;

	vector<string> tmp = { "func", type2string(ttype), name };
	midcodes.insert(tmp);

	Token ntoken = gettoken(1);
	if (ntoken.getType() == L_BRACK)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken(1);

		bool re = parameters(paras);
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != R_BRACK)
			return false;
		ntoken = gettoken(1);
	}

	iter = tables.find("#OverAll");
	if (iter == tables.end())
	{
		cout << "Insert error" << endl;
		return false;
	}
	ntable = &(iter->second);
	nkey = iter->first;
	map<string, SymbolItem>::iterator iter2 = (ntable->symlist).find(name);
	iter2->second.setparas(paras);
	iter = tables.find(name);
	if (iter == tables.end())
	{
		cout << "Insert error" << endl;
		return false;
	}
	ntable = &(iter->second);
	nkey = iter->first;

	midcodes.refill(to_string(paras.size()), index);

	if (ntoken.getType() == L_CURLY)
	{
		pretoken.erase(pretoken.begin());
		re = compoundSta();
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != R_CURLY)
			return false;
	}
	else
		return false;
	printresult("This is a Returned Function Define!");
	ntoken = gettoken(1);

	iter = tables.find("#OverAll");
	ntable = &(iter->second);
	nkey = iter->first;

	return true;
}

bool Parser::funDefhead(enum type &ttype, string &name, int &line)
{
	Token ntoken = gettoken();
	if (ntoken.getType() != INT && ntoken.getType() != CHAR)
		return false;
	if (ntoken.getType() == INT)
		ttype = INT_TYPE;
	else
		ttype = CHAR_TYPE;

	ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	name = ntoken.getStrValue();
	line = ntoken.getLinenum();

	printresult("This is a Function Head Define!");
	ntoken = gettoken(1);

	return true;
}

bool Parser::parameters(vector<enum type> &paras)
{
	string name, fname;
	int line, offset, size;
	enum type ttype;
	enum kind kkind = PARAMETERS;

	size = 4;
	Token ntoken = gettoken();
	if (ntoken.getType() != INT && ntoken.getType() != CHAR)
		return false;
	if (ntoken.getType() == INT)
	{
		ttype = INT_TYPE;
	}
	else
	{
		ttype = CHAR_TYPE;
	}
	paras.insert(paras.end(), ttype);

	ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;

	name = ntoken.getStrValue();
	line = ntoken.getLinenum();
	offset = ntable->alloc(size);
	SymbolItem nitem(name, ttype, kkind, line, offset);
	ntable->insert(nitem);

	vector<string> tmp = { "para",type2string(ttype), name };
	midcodes.insert(tmp);

	ntoken = gettoken(1);
	while (ntoken.getType() == COMMA)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken();
		if (ntoken.getType() != INT && ntoken.getType() != CHAR)
			return false;
		if (ntoken.getType() == INT)
		{
			ttype = INT_TYPE;
			size = 4;
		}
		else
		{
			ttype = CHAR_TYPE;
			size = 4;
		}
		paras.insert(paras.end(), ttype);

		ntoken = gettoken();
		if (ntoken.getType() != IDENTITY)
			return false;
		name = ntoken.getStrValue();
		line = ntoken.getLinenum();
		offset = ntable->alloc(size);
		SymbolItem nitem(name, ttype, kkind, line, offset);
		ntable->insert(nitem);

		vector<string> tmp = { "para", type2string(ttype), name };
		midcodes.insert(tmp);

		ntoken = gettoken(1);
	}
	printresult("This is a Parameters!");
	return true;
}

bool Parser::compoundSta()
{
	Token ntoken = gettoken(1);
	if (ntoken.getType() == CONST)
	{
		bool re = constDeclare();
		if (!re)
			return false;
		ntoken = gettoken(1);
	}
	if (ntoken.getType() == INT || ntoken.getType() == CHAR)
	{
		bool re = variableDeclare();
		if (!re)
			return false;
	}

	bool re = statementList();
	if (!re)
		return false;

	printresult("This is a Compound Statement!");
	return true;
}

bool Parser::unretfunDef()
{
	string name;
	vector<enum type> paras;
	enum type ttype = VOID_TYPE;
	enum kind kkind = FUNCTION;
	int line;

	Token ntoken = gettoken();
	if (ntoken.getType() != VOID)
		return false;
	ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	name = ntoken.getStrValue();
	line = ntoken.getLinenum();

	midcodes.insert({ name, ":" });
	int index = midcodes.size();

	SymbolItem nitem(name, ttype, kkind, line);
	ntable->insert(nitem);

	tables.insert(pair<string, SymbolTable>(name, SymbolTable(1)));
	map<string, SymbolTable>::iterator iter = tables.find(name);
	if (iter == tables.end())
	{
		cout << "Insert error" << endl;
		return false;
	}
	ntable = &(iter->second);
	nkey = iter->first;

	vector<string> tmp = {"func", type2string(ttype), name };
	midcodes.insert(tmp);

	ntoken = gettoken();
	if (ntoken.getType() == L_BRACK)
	{
		if (pretoken.size() == 0)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.begin(), ntoken);
		}
		bool re = parameters(paras);
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != R_BRACK)
			return false;
		ntoken = gettoken();
	}

	iter = tables.find("#OverAll");
	if (iter == tables.end())
	{
		cout << "Insert error" << endl;
		return false;
	}
	ntable = &(iter->second);
	nkey = iter->first;
	map<string, SymbolItem>::iterator iter2 = (ntable->symlist).find(name);
	iter2->second.setparas(paras);
	iter = tables.find(name);
	if (iter == tables.end())
	{
		cout << "Insert error" << endl;
		return false;
	}
	ntable = &(iter->second);
	nkey = iter->first;

	midcodes.refill(to_string(paras.size()), index);

	if (ntoken.getType() == L_CURLY)
	{
		if (pretoken.size() == 0)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.begin(), ntoken);
		}
		bool re = compoundSta();
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != R_CURLY)
			return false;
	}
	else
		return false;
	midcodes.insert({ "ret" });
	printresult("This is a Unreturned Function Define!");

	iter = tables.find("#OverAll");
	ntable = &(iter->second);
	nkey = iter->first;

	ntoken = gettoken(1);
	return true;
}

bool Parser::mainFun()
{
	midcodes.insert({ "main", ":" });
	vector<string> tmp = { "func", "void", "main" , "0"};
	midcodes.insert(tmp);

	string name;
	enum type ttype = VOID_TYPE;
	enum kind kkind = FUNCTION;
	int line;
	vector<enum type> paras;

	Token ntoken = gettoken();
	if (ntoken.getType() != VOID)
		return false;

	ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	line = ntoken.getLinenum();
	name = ntoken.getStrValue();

	string idname = str2lower(ntoken.getStrValue());
	if (idname != "main")
		return false;

	ntoken = gettoken();
	if (ntoken.getType() != L_BRACK)
		return false;

	ntoken = gettoken();
	if (ntoken.getType() != R_BRACK)
		return false;

	SymbolItem nitem(name, ttype, kkind, line);
	nitem.setparas(paras);

	ntable->insert(nitem);

	tables.insert(pair<string, SymbolTable>(name, SymbolTable(1)));
	map<string, SymbolTable>::iterator iter = tables.find(name);
	if (iter == tables.end())
	{
		cout << "Insert error" << endl;
		return false;
	}
	ntable = &(iter->second);
	nkey = iter->first;

	ntoken = gettoken();
	if (ntoken.getType() != L_CURLY)
		return false;

	ntoken = gettoken(1);
	bool re = compoundSta();
	if (!re)
		return false;

	ntoken = gettoken();
	if (ntoken.getType() != R_CURLY)
		return false;

	midcodes.insert({ "ret" });
	printresult("This is a Main Function!");
	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}

	iter = tables.find("#OverAll");
	ntable = &(iter->second);
	nkey = iter->first;

	return true;
}

bool Parser::statementList()
{
	bool re = false;
	Token ntoken = gettoken(1);
	enum symbol type = ntoken.getType();
	while (type == IF || type == WHILE || type == L_CURLY || type == IDENTITY ||
		type == PRINTF || type == SCANF || type == SEMICOLON ||
		type == SWITCH || type == RETURN)
	{
		if (type == IDENTITY)
		{
			if (pretoken.size() == 1)
			{
				ntoken = lex.nextsymbol();
				pretoken.insert(pretoken.end(), ntoken);
			}
			else
				ntoken = pretoken[1];
			if (ntoken.getType() == SEMICOLON || ntoken.getType() == L_BRACK ||
				ntoken.getType() == L_SQUARE || ntoken.getType() == ASSIGN)
				re = statement();
			if (!re)
				return false;
		}
		else
		{
			re = statement();
			if (!re)
				return false;
		}
		ntoken = gettoken(1);
		type = ntoken.getType();
	}
	printresult("This is a Statement List!");
	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}
	return true;
}

bool Parser::statement()
{
	Token ntoken = gettoken(1);
	bool re = false;
	string name = ntoken.getStrValue();
	switch (ntoken.getType())
	{
	case IF:
		re = ifSta();
		if (!re)
			return false;
		break;
	case WHILE:
		re = whileSta();
		if (!re)
			return false;
		break;
	case L_CURLY:
		pretoken.erase(pretoken.begin());
		re = statementList();
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != R_CURLY)
			return false;
		break;
	case IDENTITY:
		if (pretoken.size() == 1)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.end(), ntoken);
		}
		else
			ntoken = pretoken[1];
		if (ntoken.getType() == SEMICOLON || ntoken.getType() == L_BRACK)
		{
			SymbolItem item = pfind(name);
			if (item.getkind() != FUNCTION)
			{
				cout << "Error_3" << endl;
				return false;
			}
			int index = midcodes.size();
			if (item.gettype() == VOID_TYPE)
				re = unretfunCall();
			else
				re = retfunCall(index);
			if (!re)
				return false;
			ntoken = gettoken();
			if (ntoken.getType() != SEMICOLON)
				return false;
			ntoken = gettoken(1);
		}
		else if (ntoken.getType() == ASSIGN || ntoken.getType() == L_SQUARE)
		{
			re = assignSta();
			if (!re)
				return false;
			ntoken = gettoken();
			if (ntoken.getType() != SEMICOLON)
				return false;
			ntoken = gettoken(1);
		}
		else
			return false;

		break;
	case PRINTF:
		re = printSta();
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != SEMICOLON)
			return false;
		ntoken = gettoken(1);
		break;
	case SCANF:
		re = scanSta();
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != SEMICOLON)
			return false;
		ntoken = gettoken(1);
		break;
	case SEMICOLON:
		ntoken = gettoken();
		if (ntoken.getType() != SEMICOLON)
			return false;
		ntoken = gettoken(1);
		break;
	case SWITCH:
		re = switchSta();
		if (!re)
			return false;
		break;
	case RETURN:
		re = returnSta();
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != SEMICOLON)
			return false;
		ntoken = gettoken(1);
		break;
	default:
		return false;
	}
	printresult("This is a Statement!");
	return true;
}

bool Parser::assignSta()
{
	int minusone = midcodes.size();
	Token ntoken = gettoken();
	bool ischar;
	if (ntoken.getType() != IDENTITY)
		return false;
	string name = ntoken.getStrValue();
	if (!isexists(name))
	{
		cout << "Error_3" << endl;
		return false;
	}
	SymbolItem item = pfind(name);
	enum type ttype = item.gettype();

	string index;
	bool islist = false;
	string result;

	ntoken = gettoken();
	if (ntoken.getType() == L_SQUARE)
	{
		islist = true;
		ntoken = gettoken(1);
		bool re = expression(ischar, index, minusone);

		ntoken = gettoken();
		if (ntoken.getType() != R_SQUARE)
			return false;
		ntoken = gettoken();
	}

	if (ntoken.getType() != ASSIGN)
		return false;

	ntoken = gettoken(1);
	bool re = expression(ischar, result, minusone);
	if (!re)
		return false;

	if (islist)
	{
		vector<string> tmp = { "[]=", name, index, result };
		midcodes.insert(tmp);
	}
	else
	{
		vector<string> tmp = { "=", result, name };
		midcodes.insert(tmp);
	}
	printresult("This is an Assign Statement!");
	return true;
}

bool Parser::ifSta()
{
	Token ntoken = gettoken();
	if (ntoken.getType() != IF)
		return false;
	ntoken = gettoken();
	if (ntoken.getType() != L_BRACK)
		return false;
	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}

	string ifbranch = genlabel(), elsebranch = genlabel(), endif = genlabel();
	vector<string> cond;
	bool re = conditionSta(cond);

	vector<string> tmp = {"bnz"};
	for (int i = 0; i < cond.size(); i++)
	{
		tmp.insert(tmp.end(), cond[i]);
	}
	tmp.insert(tmp.end(), ifbranch);
	midcodes.insert(tmp);

	tmp = { "goto", elsebranch };
	midcodes.insert(tmp);

	if (!re)
		return false;
	ntoken = gettoken();
	if (ntoken.getType() != R_BRACK)
		return false;
	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}

	midcodes.insert({ ifbranch, ":" });

	re = statement();
	if (!re)
		return false;
	midcodes.insert({ "goto", endif });

	ntoken = gettoken();
	if (ntoken.getType() != ELSE)
		return false;
	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}

	midcodes.insert({ elsebranch, ":" });

	re = statement();
	if (!re)
		return false;

	midcodes.insert({ endif, ":" });

	printresult("This is an If Statement!");
	return true;
}

bool Parser::switchSta()
{
	int minusone = midcodes.size();
	Token ntoken = gettoken();
	if (ntoken.getType() != SWITCH)
		return false;

	ntoken = gettoken();
	if (ntoken.getType() != L_BRACK)
		return false;

	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}

	bool ischar;
	string result;
	bool re = expression(ischar, result, minusone);

	if (!re)
		return false;

	ntoken = gettoken();
	if (ntoken.getType() != R_BRACK)
		return false;

	ntoken = gettoken();
	if (ntoken.getType() != L_CURLY)
		return false;

	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}

	int caseindex = midcodes.size();
	string endlabel = genlabel();

	re = suitationTab(ischar, result, caseindex, endlabel);
	if (!re)
		return false;

	ntoken = gettoken(1);
	if (ntoken.getType() == DEFAULT)
	{
		re = defaultSta(caseindex, endlabel);
		if (!re)
			return false;
	}
	else
	{
		midcodes.insert({ "goto", endlabel }, caseindex++);
	}
	ntoken = gettoken();
	if (ntoken.getType() != R_CURLY)
		return false;

	midcodes.insert({endlabel, ":"});
	printresult("This is a Switch Statement!");
	return true;
}

bool Parser::suitationTab(bool ischar, string flag, int &caseindex, string endlabel)
{
	bool re = suitationSta(ischar, flag, caseindex, endlabel);
	if (!re)
		return false;
	Token  ntoken = gettoken(1);
	while (ntoken.getType() == CASE)
	{
		re = suitationSta(ischar, flag, caseindex, endlabel);
		if (!re)
			return false;
		ntoken = gettoken(1);
	}
	printresult("This is a Suitation Table!");
	return true;
}

bool Parser::suitationSta(bool ischar, string flag, int &caseindex, string endlabel)
{
	Token ntoken = gettoken();
	int sign = 1;
	if (ntoken.getType() != CASE)
		return false;

	ntoken = gettoken();
	if (ntoken.getType() != CONST_INT  && ntoken.getType() != CONST_CHAR && ntoken.getType() != MINUS&& ntoken.getType() != PLUS)
		return false;
	if (ntoken.getType() == MINUS)
	{
		sign = -1;
		ntoken = gettoken();
	}

	int value = ntoken.getType() == CONST_INT ? ntoken.getIntValue()*sign : int(ntoken.getCharValue());
	string thisflag = genvar(value);
	midcodes.insert({"li", thisflag, to_string(value)}, caseindex++);
	string caselabel = genlabel();
	midcodes.insert({ "bnz", thisflag, "==", flag, caselabel }, caseindex++);

	ntoken = gettoken();
	if (ntoken.getType() != COLON)
		return false;

	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}

	midcodes.insert({caselabel, ":"});

	bool re = statement();
	if (!re)
		return false;
	midcodes.insert({"goto", endlabel});
	printresult("This is a Suitation Statement!");
	return true;
}

bool Parser::defaultSta(int &caseindex, string endlabel)
{
	Token ntoken = gettoken();
	if (ntoken.getType() != DEFAULT)
		return false;
	ntoken = gettoken();
	if (ntoken.getType() != COLON)
		return false;

	string defaultlabel = genlabel();
	midcodes.insert({ "goto", defaultlabel }, caseindex);

	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}
	midcodes.insert({ defaultlabel, ":" });
	bool re = statement();
	if (!re)
		return false;

	midcodes.insert({ "goto", endlabel });
	printresult("This is a Default Statement!");
	return true;

}

bool Parser::whileSta()
{
	Token ntoken = gettoken();
	if (ntoken.getType() != WHILE)
		return false;

	ntoken = gettoken();
	if (ntoken.getType() != L_BRACK)
		return false;

	int whileindex = midcodes.size();

	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}
	vector<string> cond;
	bool re = conditionSta(cond);
	if (!re)
		return false;

	string whilebegin = genlabel(), loop = genlabel(), endloop = genlabel();

	midcodes.insert({ whilebegin, ":" }, whileindex);

	vector<string> tmp = { "bnz" };
	for (int i = 0; i < cond.size(); i++)
	{
		tmp.insert(tmp.end(), cond[i]);
	}
	tmp.insert(tmp.end(), loop);
	midcodes.insert(tmp);

	tmp = { "goto", endloop };
	midcodes.insert(tmp);


	ntoken = gettoken();
	if (ntoken.getType() != R_BRACK)
		return false;

	midcodes.insert({loop, ":"});
	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}
	re = statement();
	if (!re)
		return false;
	midcodes.insert({ "goto", whilebegin });
	midcodes.insert({ endloop, ":" });
	printresult("This is a While Statement!");
	return true;
}

bool Parser::retfunCall(int &index)
{
	Token ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	string name = ntoken.getStrValue();
	SymbolItem item = pfind(name);
	vector<enum type> paras = item.getparas();

	midcodes.insert({ "call", name, to_string(paras.size()) }, index);

	ntoken = gettoken(1);
	if (ntoken.getType() == L_BRACK)
	{
		ntoken = gettoken();
		if (pretoken.size() == 0)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.begin(), ntoken);
		}
		bool re = false;
		re = valueParas(paras, index);

		if (!re)
			return false;

		ntoken = gettoken();
		if (ntoken.getType() != R_BRACK)
			return false;
	}

	index++;
	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}
	printresult("This is a Returned Function Call!");
	return true;
}

bool Parser::unretfunCall()
{
	Token ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	string name = ntoken.getStrValue();
	SymbolItem item = pfind(name);
	vector<enum type> paras = item.getparas();

	int index = midcodes.size();
	midcodes.insert({ "call", name, to_string(paras.size()) });

	ntoken = gettoken(1);
	if (ntoken.getType() == L_BRACK)
	{
		ntoken = gettoken();
		if (pretoken.size() == 0)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.begin(), ntoken);
		}
		bool re = valueParas(paras, index);
		if (!re)
			return false;

		ntoken = gettoken();
		if (ntoken.getType() != R_BRACK)
			return false;
	}

	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}
	printresult("This is a Unreturned Function Call!");
	return true;
}

bool Parser::valueParas(vector<enum type> paras, int &up)
{
	bool ischar;
	int count = 0;
	string result;
	bool re = expression(ischar, result, up);
	if (count == paras.size())
	{
		cout << "Error_3" << endl;
		return false;
	}

	count++;
	if (!re)
		return false;
	midcodes.insert({ "valpara", result }, up + count);

	Token ntoken = gettoken(1);
	while (ntoken.getType() == COMMA)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken(1);
		string result;
		re = expression(ischar, result, up);
		if (!re)
			return false;

		if (count == paras.size())
		{
			cout << "Error_3" << endl;
			return false;
		}
		count++;
		midcodes.insert({ "valpara", result }, up + count);

		ntoken = gettoken(1);
	}
	if (count != paras.size())
	{
		cout << "Error_3" << endl;
		return false;
	}

	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}

	up = up + count;
	printresult("This is a Value Parameters!");
	return true;
}

bool Parser::returnSta()
{
	int minusone = midcodes.size();
	bool ischar;
	Token ntoken = gettoken();
	if (ntoken.getType() != RETURN)
		return false;

	ntoken = gettoken(1);
	if (ntoken.getType() == L_BRACK)
	{
		ntoken = gettoken();
		if (pretoken.size() == 0)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.begin(), ntoken);
		}
		string result;
		bool re = expression(ischar, result, minusone);
		if (!re)
			return false;

		midcodes.insert({"ret", result});

		ntoken = gettoken();
		if (ntoken.getType() != R_BRACK)
			return false;
	}
	else
	{
		midcodes.insert({ "ret" });
	}

	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}
	printresult("This is a Return Statement!");
	return true;
}

bool Parser::printSta()
{
	int minusone = midcodes.size();
	Token ntoken = gettoken();
	if (ntoken.getType() != PRINTF)
		return false;

	ntoken = gettoken();
	if (ntoken.getType() != L_BRACK)
		return false;

	bool ischar;
	ntoken = gettoken(1);
	if (ntoken.getType() == CONST_STRING)
	{
		string str = ntoken.getStrValue();
		str = genvar(str);
		minusone = midcodes.size();

		ntoken = gettoken();

		ntoken = gettoken(1);
		if (ntoken.getType() == COMMA)
		{
			ntoken = gettoken();
			if (pretoken.size() == 0)
			{
				ntoken = lex.nextsymbol();
				pretoken.insert(pretoken.begin(), ntoken);
			}
			string result;
			bool re = expression(ischar, result, minusone);

			midcodes.insert({ "print", str });
			midcodes.insert({ "print", result });

			if (!re)
				return false;
		}
		else
		{
			midcodes.insert({ "print", str });
		}
	}
	else
	{
		string result;
		bool re = expression(ischar, result, minusone);
		if (!re)
			return false;
		midcodes.insert({ "print", result });
	}
	ntoken = gettoken();
	if (ntoken.getType() != R_BRACK)
		return false;


	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}
	printresult("This is a Print Statement!");
	return true;
}

bool Parser::scanSta()
{
	Token ntoken = gettoken();
	if (ntoken.getType() != SCANF)
		return false;

	ntoken = gettoken();
	if (ntoken.getType() != L_BRACK)
		return false;

	ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;

	midcodes.insert({"scan", ntoken.getStrValue()});

	ntoken = gettoken(1);
	while (ntoken.getType() == COMMA)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken();
		if (ntoken.getType() != IDENTITY)
			return false;
		midcodes.insert({ "scan", ntoken.getStrValue() });

		ntoken = gettoken(1);
	}

	pretoken.erase(pretoken.begin());
	if (ntoken.getType() != R_BRACK)
		return false;
	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}
	printresult("This is a Scanf Statement!");
	return true;
}

bool Parser::conditionSta(vector<string> &cond)
{
	bool ischar;
	string con1, op, con2;
	int minusone = midcodes.size();
	bool re = expression(ischar, con1, minusone);
	if (!re)
		return false;
	cond.insert(cond.end(), con1);

	Token ntoken = gettoken(1);
	if (ntoken.getType() == GREATER || ntoken.getType() == SMALLER ||
		ntoken.getType() == NOLESS || ntoken.getType() == NOBIGGER ||
		ntoken.getType() == EQUAL || ntoken.getType() == NOTEQUAL)
	{
		switch (ntoken.getType())
		{
		case GREATER:
			op = ">";
			break;
		case SMALLER:
			op = "<";
			break;
		case NOLESS:
			op = ">=";
			break;
		case NOBIGGER:
			op = "<=";
			break;
		case EQUAL:
			op = "==";
			break;
		default:
			op = "!=";
			break;
		}
		pretoken.erase(pretoken.begin());
		if (pretoken.size() == 0)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.begin(), ntoken);
		}
		re = expression(ischar, con2, minusone);
		if (!re)
			return false;
		cond.insert(cond.end(), op);
		cond.insert(cond.end(), con2);
	}

	printresult("This is a Condition Statement!");
	return true;
}

bool Parser::expression(bool &ischar, string &result, int &index)
{
	Token ntoken = gettoken(1);
	int sign = 1;

	if (ntoken.getType() == PLUS || ntoken.getType() == MINUS)
	{
		if (ntoken.getType() == MINUS)
			sign = -1;

		pretoken.erase(pretoken.begin());
		ntoken = gettoken(1);
	}

	string num2;
	bool re = item(ischar, num2, index);
	if (!re)
		return false;

	string num1 = num2;
	if (sign == -1)
	{
		num2 = genvar();
		vector<string> tmp = {"-", "0", num1, num2};
		if(index != -1)
			midcodes.insert(tmp, index++);
		else
			midcodes.insert(tmp);
	}

	ntoken = gettoken(1);
	while (ntoken.getType() == PLUS || ntoken.getType() == MINUS)
	{
		sign = 1;
		if (ntoken.getType() == MINUS)
			sign = -1;
		pretoken.erase(pretoken.begin());

		ntoken = gettoken(1);
		bool re = item(ischar,num1, index);
		if (!re)
			return false;

		if (sign == -1)
		{
			string tmpnum = num2;
			num2 = genvar();
			vector<string> tmp = { "-", tmpnum, num1, num2 };
			if (index != -1)
				midcodes.insert(tmp, index++);
			else
				midcodes.insert(tmp);
		}
		else
		{
			string tmpnum = num2;
			num2 = genvar();
			vector<string> tmp = { "+", tmpnum, num1, num2 };
			if (index != -1)
				midcodes.insert(tmp, index++);
			else
				midcodes.insert(tmp);
		}

		ntoken = gettoken(1);
	}

	result.assign(num2);
	printresult("This is an Expression!");
	return true;
}

bool Parser::item(bool &ischar, string &result, int &index)
{
	string num1, num2;
	bool re = factor(ischar, num2, index);
	if (!re)
		return false;

	Token ntoken = gettoken(1);
	while (ntoken.getType() == MULTI || ntoken.getType() == DIVIDE)
	{
		int sign = 1;
		if (ntoken.getType() == DIVIDE)
			sign = -1;
		pretoken.erase(pretoken.begin());
		ntoken = gettoken(1);
		re = factor(ischar, num1, index);

		if (sign == -1)
		{
			string tmpnum = num2;
			num2 = genvar();
			vector<string> tmp = { "/", tmpnum, num1, num2 };
			if (index != -1)
				midcodes.insert(tmp, index++);
			else
				midcodes.insert(tmp);
		}
		else
		{
			string tmpnum = num2;
			num2 = genvar();
			vector<string> tmp = { "*", tmpnum, num1, num2 };
			if (index != -1)
				midcodes.insert(tmp, index++);
			else
				midcodes.insert(tmp);
		}

		ntoken = gettoken(1);
	}

	result.assign(num2);
	printresult("This is an Item!");
	return true;
}

bool Parser::factor(bool &ischar, string &result, int &index)
{
	Token ntoken = gettoken(1);
	bool re = false;
	vector<string> tmp;
	enum type t;
	switch (ntoken.getType())
	{
	case IDENTITY:
		if (pretoken.size() == 1)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.end(), ntoken);
		}
		else
			ntoken = pretoken[1];

		if (ntoken.getType() == L_SQUARE)
		{

			Token stoken = pretoken[0];
			string name = stoken.getStrValue();
			SymbolItem item = pfind(name);
			if (item.getkind() != VAR_LIST ||
				(item.gettype() != INT_TYPE && item.gettype() != CHAR_TYPE))
			{
				cout << "Error_3" << endl;
				return false;
			}
			if (item.gettype() == INT_TYPE)
			{
				ischar = false;
				t = INT_TYPE;
			}
			else
			{
				ischar = true;
				t = CHAR_TYPE;
			}
			pretoken.erase(pretoken.begin());
			pretoken.erase(pretoken.begin());

			bool ichar = false;

			string num1, num2;
			num2 = genvar(t);

			ntoken = gettoken(1);
			re = expression(ichar, num1, index);
			if (!re || ichar)
				return false;

			tmp = {"[]", name, num1, num2 };
			if (index != -1)
				midcodes.insert(tmp, index++);
			else
				midcodes.insert(tmp);
			result.assign(num2);

			ntoken = gettoken();
			if (ntoken.getType() != R_SQUARE)
				return false;
		}
		else if (ntoken.getType() == L_BRACK)
		{
			Token stoken = pretoken[0];
			string name = stoken.getStrValue();
			SymbolItem item = pfind(name);
			if (item.getkind() != FUNCTION || item.gettype() == VOID_TYPE)
			{
				cout << "Error_3" << endl;
				return false;
			}
			if (item.gettype() == INT_TYPE)
				ischar = false;
			else
				ischar = true;

			re = retfunCall(index);
			if (!re)
				return false;

			string num1 = genvar();
			tmp = { "=", "ret", num1};
			if (index != -1)
				midcodes.insert(tmp, index++);
			else
				midcodes.insert(tmp);
			result.assign(num1);
		}
		else
		{
			string name = pretoken[0].getStrValue();
			SymbolItem item = pfind(name);
			if (item.getkind() != FUNCTION)
			{
				result = pretoken[0].getStrValue();
				pretoken.erase(pretoken.begin());
			}
			else
			{
				enum type t;
				if (item.gettype() == VOID_TYPE)
				{
					cout << "Error_3" << endl;
					return false;
				}
				else if (item.gettype() == INT_TYPE)
				{
					t = INT_TYPE;
					ischar = false;
				}
				else
				{
					t = CHAR_TYPE;
					ischar = true;
				}
				re = retfunCall(index);
				if (!re)
					return false;

				string num1 = genvar(t);
				tmp = { "=", "ret", num1 };
				if (index != -1)
					midcodes.insert(tmp, index++);
				else
					midcodes.insert(tmp);
				result.assign(num1);
			}
		}
		break;
	case L_BRACK:
		pretoken.erase(pretoken.begin());
		ntoken = gettoken(1);
		re = expression(ischar, result, index);
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != R_BRACK)
			return false;
		break;
	case PLUS:
	case MINUS:
	case CONST_INT:
		ischar = false;
		int value;
		re = integer(value);
		result = genvar(value);
		tmp = { "li", result, to_string(value) };
		if (index != -1)
			midcodes.insert(tmp, index++);
		else
			midcodes.insert(tmp);
		if (!re)
			return false;
		break;
	case CONST_CHAR:
		value = int(ntoken.getCharValue());
		result = genvar(value, CHAR_TYPE);
		ischar = true;
		tmp = { "li", result, to_string(int(ntoken.getCharValue())) };
		if (index != -1)
			midcodes.insert(tmp, index++);
		else
			midcodes.insert(tmp);
		pretoken.erase(pretoken.begin());
		break;
	default:
		return false;
	}
	ntoken = gettoken(1);
	printresult("This is a Factor!");
	return true;
}

bool Parser::integer(int& value)
{
	Token ntoken = gettoken();
	int sign = 1;
	if (ntoken.getType() == PLUS || ntoken.getType() == MINUS)
	{
		if (ntoken.getType() == MINUS)
			sign = -1;
		ntoken = gettoken();
	}
	if (ntoken.getType() != CONST_INT)
		return false;
	int tmp = ntoken.getIntValue();
	value = tmp*sign;
	ntoken = gettoken(1);
	return true;
}
