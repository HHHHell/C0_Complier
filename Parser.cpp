#include "iostream"
#include "string"
#include "vector"
#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "error.h"

using namespace std;
extern std::string str2lower(std::string str);

Parser::Parser(Lexer &l, map<string, SymbolTable> &tlist, Midcodes &m)
	: lex(l), tables(tlist), midcodes(m), result(false)
{
	ntable = &(tables.find("OverAll")->second);
	nkey = "OverAll";
	pout.open("result\\parser_result.txt");
}

Parser::~Parser()
{
	if (result)
		pout << "\nFinished!" << endl;
	else
		pout << "\nError!" << endl;
	pout.flush();
	pout.close();
}

void Parser::printresult(string str)
{
	pout << str << endl;
	cout << str << endl;
	pout.flush();
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

bool Parser::isexists(SymbolItem item)
{
	string name = item.getname();
	return isexists(name);
}

bool Parser::isexists(string name)
{
	if (nkey != "OverAll")
	{
		bool re = ntable->isexists(name);
		if (re == true)
			return true;
		map<string, SymbolTable>::iterator iter = tables.find("OverAll");
		SymbolTable &ttable = iter->second;
		return ttable.isexists(name);
	}
	else
	{
		return ntable->isexists(name);
	}
}

SymbolItem Parser::find(string name)
{
	if (nkey != "OverAll")
	{
		if (ntable->isexists(name))
		{
			SymbolItem item = ntable->find(name);
			return item;
		}
		map<string, SymbolTable>::iterator iter = tables.find("OverAll");
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
		if (type == L_BRACK)
		{
			bool re = retfunDef();
			if (!re)
				return false;
			ntoken = gettoken(1);
			if (ntoken.getType() != VOID && ntoken.getType() != INT &&
				ntoken.getType() != CHAR)
				return false;
			while (ntoken.getType() == VOID || ntoken.getType() == INT ||
				ntoken.getType() == CHAR)
			{
				if (ntoken.getType() == VOID)
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
		offset = ntable->alloc(size);
		SymbolItem nitem(name, ttype, kkind, line, offset, value);	
		if (isexists(nitem)) 
		{
			cout << "Error_3" << endl;
			return false;
		}
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
			offset = ntable->alloc(size);
			SymbolItem nitem(name, ttype, kkind, line, offset, value);
			if (isexists(nitem))
			{
				cout << "Error_3" << endl;
				return false;
			}
			ntable->insert(nitem);

			ntoken = gettoken(1);
		}
	}
	else
	{
		ttype = CHAR_TYPE;
		size = 2;

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
		
		value = ntoken.getIntValue();
		offset = ntable->alloc(size);
		SymbolItem nitem(name, ttype, kkind, line, offset, value);
		if (isexists(name))
		{
			cout << "Error_3" << endl;
			return false;
		}
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

			value = ntoken.getIntValue();
			offset = ntable->alloc(size);
			SymbolItem nitem(name, ttype, kkind, line, offset, value);
			if (isexists(nitem))
			{
				cout << "Error_3" << endl;
				return false;
			}
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

	Token ntoken = gettoken();
	if (ntoken.getType() != INT && ntoken.getType() != CHAR)
		return false;
	if (ntoken.getType() == INT)
	{
		bsize = 4;
		ttype = INT_TYPE;
		size = bsize;
	}
	else
	{
		ttype = CHAR_TYPE;
		bsize = 2;
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
	if (isexists(nitem))
	{
		cout << "Error_3" << endl;
		return false;
	}
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
		if (isexists(nitem))
		{
			cout << "Error_3" << endl;
			return false;
		}
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

	SymbolItem nitem(name, ttype, kkind, line);
	if (isexists(nitem))
	{
		cout << "Error_3" << endl;
		return false;
	}
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

	iter = tables.find("OverAll");
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

	iter = tables.find("OverAll");
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

	Token ntoken = gettoken();
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
		size = 2;
	}
	paras.insert(paras.end(), ttype);

	ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	name = ntoken.getStrValue();
	line = ntoken.getLinenum();
	offset = ntable->alloc(size);
	SymbolItem nitem(name, ttype, kkind, line, offset);
	if (isexists(nitem))
	{
		cout << "Error_3" << endl;
		return false;
	}
	ntable->insert(nitem);

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
			size = 2;
		}
		paras.insert(paras.end(), ttype);

		ntoken = gettoken();
		if (ntoken.getType() != IDENTITY)
			return false;
		name = ntoken.getStrValue();
		line = ntoken.getLinenum();
		offset = ntable->alloc(size);
		SymbolItem nitem(name, ttype, kkind, line, offset);
		if (isexists(nitem))
		{
			cout << "Error_3" << endl;
			return false;
		}
		ntable->insert(nitem);

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

	SymbolItem nitem(name, ttype, kkind, line);
	if (isexists(nitem))
	{
		cout << "Error_3" << endl;
		return false;
	}
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

	iter = tables.find("OverAll");
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
	printresult("This is a Unreturned Function Define!");

	iter = tables.find("OverAll");
	ntable = &(iter->second);
	nkey = iter->first;

	ntoken = gettoken(1);
	return true;
}

bool Parser::mainFun()
{
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
	if (isexists(nitem))
	{
		cout << "Error_3" << endl;
		return false;
	}
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

	printresult("This is a Main Function!");
	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}

	iter = tables.find("OverAll");
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
			SymbolItem item = find(name);
			if (item.getkind() != FUNCTION)
			{
				cout << "Error_3" << endl;
				return false;
			}
			if (item.gettype() == VOID_TYPE)
				re = unretfunCall();
			else
				re = retfunCall();
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
	SymbolItem item = find(name);
	enum type ttype = item.gettype();

	ntoken = gettoken();
	if (ntoken.getType() == L_SQUARE)
	{
		ntoken = gettoken(1);
		bool re = expression(ischar);
		if (ischar)
		{
			cout << "Error_3" << endl;
			return false;
		}

		ntoken = gettoken();
		if (ntoken.getType() != R_SQUARE)
			return false;
		ntoken = gettoken();
	}

	if (ntoken.getType() != ASSIGN)
		return false;

	ntoken = gettoken(1);
	bool re = expression(ischar);
	if (!re)
		return false;
/*	if (ttype == CHAR_TYPE && !ischar)
	{
		cout << "Error_3" << endl;
		return false;
	}
*/
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
	bool re = conditionSta();
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
	re = statement();
	if (!re)
		return false;
	ntoken = gettoken();
	if (ntoken.getType() != ELSE)
		return false;
	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}
	re = statement();
	if (!re)
		return false;

	printresult("This is an If Statement!");
	return true;
}

bool Parser::switchSta()
{
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
	bool re = expression(ischar);
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
	re = suitationTab(ischar);
	if (!re)
		return false;

	ntoken = gettoken(1);
	if (ntoken.getType() == DEFAULT)
	{
		re = defaultSta();
		if (!re)
			return false;
	}
	ntoken = gettoken();
	if (ntoken.getType() != R_CURLY)
		return false;

	printresult("This is a Switch Statement!");
	return true;
}

bool Parser::suitationTab(bool ischar)
{
	bool re = suitationSta(ischar);
	if (!re)
		return false;
	Token  ntoken = gettoken(1);
	while (ntoken.getType() == CASE)
	{
		re = suitationSta(ischar);
		if (!re)
			return false;
		ntoken = gettoken(1);
	}
	printresult("This is a Suitation Table!");
	return true;
}

bool Parser::suitationSta(bool ischar)
{
	Token ntoken = gettoken();
	if (ntoken.getType() != CASE)
		return false;
	
	ntoken = gettoken();
	if (ntoken.getType() != CONST_INT  && ntoken.getType() != CONST_CHAR)
		return false;
/*	if ((ischar && ntoken.getType() == CONST_INT) ||
		(!ischar && ntoken.getType() == CONST_CHAR))
	{
		cout << "Error_3" << endl;
		return false;
	}
*/
	ntoken = gettoken();
	if (ntoken.getType() != COLON)
		return false;

	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}
	bool re = statement();
	if (!re)
		return false;
	printresult("This is a Suitation Statement!");
	return true;
}

bool Parser::defaultSta()
{
	Token ntoken = gettoken();
	if (ntoken.getType() != DEFAULT)
		return false;
	ntoken = gettoken();
	if (ntoken.getType() != COLON)
		return false;

	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}
	bool re = statement();
	if (!re)
		return false;

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

	if (pretoken.size() == 0)
	{
		ntoken = lex.nextsymbol();
		pretoken.insert(pretoken.begin(), ntoken);
	}
	bool re = conditionSta();
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
	re = statement();
	if (!re)
		return false;

	printresult("This is a While Statement!");
	return true;
}

bool Parser::funCall()
{
	Token ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	string name = ntoken.getStrValue();
	SymbolItem item = find(name);
	vector<enum type> paras = item.getparas();

	ntoken = gettoken(1);
	if (ntoken.getType() == L_BRACK)
	{
		ntoken = gettoken();
		if (pretoken.size() == 0)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.begin(), ntoken);
		}
		bool re = valueParas(paras);
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
	printresult("This is a Function Call Statement!");
	return true;
}

bool Parser::retfunCall()
{
	Token ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	string name = ntoken.getStrValue();
	SymbolItem item = find(name);
	vector<enum type> paras = item.getparas();

	ntoken = gettoken(1);
	if (ntoken.getType() == L_BRACK)
	{
		ntoken = gettoken();
		if (pretoken.size() == 0)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.begin(), ntoken);
		}
		bool re = valueParas(paras);
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
	printresult("This is a Returned Function Call!");
	return true;
}

bool Parser::unretfunCall()
{
	Token ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	string name = ntoken.getStrValue();
	SymbolItem item = find(name);
	vector<enum type> paras = item.getparas();

	ntoken = gettoken(1);
	if (ntoken.getType() == L_BRACK)
	{
		ntoken = gettoken();
		if (pretoken.size() == 0)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.begin(), ntoken);
		}
		bool re = valueParas(paras);
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

bool Parser::valueParas(vector<enum type> paras)
{
	bool ischar;
	int count = 0;
	bool re = expression(ischar);
	if (count == paras.size() || (paras[count] == INT_TYPE && ischar) ||
		(paras[count] == CHAR_TYPE && !ischar))
	{
		cout << "Error_3" << endl;
		return false;
	}

	count++;
	if (!re)
		return false;
	
	Token ntoken = gettoken(1);
	while (ntoken.getType() == COMMA)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken(1);
		re = expression(ischar);
		if (!re)
			return false;

		if (count == paras.size()||(paras[count]==INT_TYPE && ischar)||
			(paras[count] == CHAR_TYPE && !ischar))
		{
			cout << "Error_3" << endl;
			return false;
		}
		count++;
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

	printresult("This is a Value Parameters!");
	return true;
}

bool Parser::returnSta()
{
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
		bool re = expression(ischar);
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
	printresult("This is a Return Statement!");
	return true;
}

bool Parser::printSta()
{
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
			bool re = expression(ischar);
			if (!re)
				return false;
		}
	}
	else
	{
		bool re = expression(ischar);
		if (!re)
			return false;
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

	ntoken = gettoken(1);
	while (ntoken.getType() == COMMA)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken();		
		if (ntoken.getType() != IDENTITY)
			return false;
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

bool Parser::conditionSta()
{
	bool ischar;
	bool re = expression(ischar);
	if (!re)
		return false;
	Token ntoken = gettoken(1);
	if (ntoken.getType() == GREATER || ntoken.getType() == SMALLER ||
		ntoken.getType() == NOLESS || ntoken.getType() == NOBIGGER ||
		ntoken.getType() == EQUAL || ntoken.getType() == NOTEQUAL)
	{
		pretoken.erase(pretoken.begin());
		if (pretoken.size() == 0)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.begin(), ntoken);
		}
		re = expression(ischar);
		if (!re)
			return false;
	}
	printresult("This is a Condition Statement!");
	return true;
}

bool Parser::expression(bool &ischar)
{
	Token ntoken = gettoken(1);
	if (ntoken.getType() == PLUS || ntoken.getType() == MINUS)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken(1);
	}

	bool re = item(ischar);
	if (!re)
		return false;

	ntoken = gettoken(1);
	while (ntoken.getType() == PLUS || ntoken.getType() == MINUS)
	{
		pretoken.erase(pretoken.begin());

		ntoken = gettoken(1);
		bool re = item(ischar);
		if (!re)
			return false;
		ntoken = gettoken(1);
	}
	printresult("This is an Expression!");
	return true;
}

bool Parser::item(bool &ischar)
{
	bool re = factor(ischar);
	if (!re)
		return false;

	Token ntoken = gettoken(1);
	while (ntoken.getType() == MULTI || ntoken.getType() == DIVIDE)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken(1);
		re = factor(ischar);

		ntoken = gettoken(1);
	}

	printresult("This is an Item!");
	return true;
}

bool Parser::factor(bool &ischar)
{
	Token ntoken = gettoken(1);
	bool re = false;
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
			SymbolItem item = find(name);
			if (item.getkind() != VAR_LIST ||
				(item.gettype() != INT_TYPE && item.gettype() != CHAR_TYPE))
			{
				cout << "Error_3" << endl;
				return false;
			}
			if (item.gettype() == INT_TYPE)
				ischar = false;
			else
				ischar = true;

			pretoken.erase(pretoken.begin());
			pretoken.erase(pretoken.begin());

			bool ichar = false;
			ntoken = gettoken(1);
			re = expression(ichar);
			if (!re || ichar)
				return false;

			ntoken = gettoken();
			if (ntoken.getType() != R_SQUARE)
				return false;
		}
		else if (ntoken.getType() == L_BRACK)
		{
			Token stoken = pretoken[0];
			string name = stoken.getStrValue();
			SymbolItem item = find(name);
			if (item.getkind() != FUNCTION || item.gettype() == VOID_TYPE)
			{
				cout << "Error_3" << endl;
				return false;
			}
			if (item.gettype() == INT_TYPE)
				ischar = false;
			else
				ischar = true;

			re = retfunCall();
			if (!re)
				return false;
		}
		else
			pretoken.erase(pretoken.begin());
		break;
	case L_BRACK:
		pretoken.erase(pretoken.begin());
		ntoken = gettoken(1);
		re = expression(ischar);
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != R_BRACK)
			return false;
		break;
	case CONST_INT:
		ischar = false;
		re = integer();
		if (!re)
			return false;
		break;
	case CONST_CHAR:
		ischar = true;
		pretoken.erase(pretoken.begin()); 
		break;
	default:
		return false;
	}
	ntoken = gettoken(1);
	printresult("This is a Factor!");
	return true;
}

bool Parser::integer()
{
	Token ntoken = gettoken();
	if (ntoken.getType() == PLUS || ntoken.getType() == MINUS)
		ntoken = gettoken();
	if (ntoken.getType() != CONST_INT)
		return false;
	ntoken = gettoken(1);
	return true;
}