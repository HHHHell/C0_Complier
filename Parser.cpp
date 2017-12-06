#include "iostream"
#include "string"
#include "vector"
#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "error.h"

using namespace std;
extern std::string str2lower(std::string str);

Parser::Parser(Lexer &l, SymbolTable &table, Midcodes &m) 
	: lex(l), stable(table), midcodes(m), result(false)
{
	vector<Token> pretoken;
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
			}
		}
		else
			return false;

	}
	printresult("This is a program!");
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
	Token ntoken = gettoken();
	if (ntoken.getType() != INT && ntoken.getType() != CHAR)
	{
		return false;
	}
	if (ntoken.getType() == INT)
	{
		ntoken = gettoken();
		if (ntoken.getType() != IDENTITY)
			return false;

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
		if (ntoken.getType() == PLUS)
			ntoken = gettoken();

		if (ntoken.getType() != CONST_INT)
			return false;
		printresult("This is a Const Variable Define!");
		
		ntoken = gettoken(1);
		while (ntoken.getType() == COMMA)
		{
			pretoken.erase(pretoken.begin());
			ntoken = gettoken();
			if (ntoken.getType() != IDENTITY)
				return false;
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
			if (ntoken.getType() == PLUS)
				ntoken = gettoken();

			if (ntoken.getType() != CONST_INT)
				return false;
			printresult("This is a Const Variable Define!");
			ntoken = gettoken(1);
		}
	}
	else
	{
//		pretoken.erase(pretoken.begin());
		ntoken = gettoken();
		if (ntoken.getType() != IDENTITY)
			return false;

		ntoken = gettoken();
		if (ntoken.getType() != ASSIGN)
			return false;

		ntoken = gettoken();
		if (ntoken.getType() != CONST_CHAR)
			return false;
		printresult("This is a Const Variable Define!");

		ntoken = gettoken(1);
		while (ntoken.getType() == COMMA)
		{
			pretoken.erase(pretoken.begin());
			ntoken = gettoken();
			if (ntoken.getType() != IDENTITY)
				return false;
			ntoken = gettoken();
			if (ntoken.getType() != ASSIGN)
				return false;
			ntoken = gettoken();
			if (ntoken.getType() != CONST_CHAR)
				return false;
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
	Token ntoken = gettoken();
	if (ntoken.getType() != INT && ntoken.getType() != CHAR)
		return false;
	ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	ntoken = gettoken(1);
	if (ntoken.getType() == L_SQUARE)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken();
		if (ntoken.getType() != CONST_INT)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != R_SQUARE)
			return false;
	}
	printresult("This is a Variable Define!");
	ntoken = gettoken(1);

	while (ntoken.getType()  == COMMA)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken();
		if (ntoken.getType() != IDENTITY)
			return false;
		ntoken = gettoken(1);
		if (ntoken.getType() == L_SQUARE)
		{
			pretoken.erase(pretoken.begin());
			ntoken = gettoken();
			if (ntoken.getType() != CONST_INT)
				return false;
			ntoken = gettoken();
			if (ntoken.getType() != R_SQUARE)
				return false;
		}
		printresult("This is a Variable Define!");
		ntoken = gettoken(1);
	}
	return true;
}

bool Parser::retfunDef()
{
	bool re = funDefhead();
	if (!re)
		return false;
	Token ntoken = gettoken(1);
	if (ntoken.getType() == L_BRACK)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken(1);

		bool re = parameters();
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != R_BRACK)
			return false;
		ntoken = gettoken(1);
	}
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

	return true;
}

bool Parser::funDefhead()
{
	Token ntoken = gettoken();
	if (ntoken.getType() != INT && ntoken.getType() != CHAR)
		return false;
	ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	printresult("This is a Function Head Define!");

	ntoken = gettoken(1);

	return true;
}

bool Parser::parameters()
{
	Token ntoken = gettoken();
	if (ntoken.getType() != INT && ntoken.getType() != CHAR)
		return false;
	ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	ntoken = gettoken(1);
	while (ntoken.getType() == COMMA)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken();
		if (ntoken.getType() != INT && ntoken.getType() != CHAR)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != IDENTITY)
			return false;
		ntoken = gettoken(1);
	}
	printresult("This is a Parameters!");
//	pretoken.insert(pretoken.begin(), ntoken);
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
	Token ntoken = gettoken();
	if (ntoken.getType() != VOID)
		return false;
	ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	ntoken = gettoken();
	if (ntoken.getType() == L_BRACK)
	{
		if (pretoken.size() == 0)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.begin(), ntoken);
		}
		bool re = parameters();
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != R_BRACK)
			return false;
		ntoken = gettoken();
	}
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

	ntoken = gettoken();
	pretoken.insert(pretoken.begin(), ntoken);
	return true;
}

bool Parser::mainFun()
{
	Token ntoken = gettoken();
	if (ntoken.getType() != VOID)
		return false;
	
	ntoken = gettoken();
	if (ntoken.getType() != IDENTITY)
		return false;
	string idname = str2lower(ntoken.getStrValue());
	if (idname != "main")
		return false;

	ntoken = gettoken();
	if (ntoken.getType() != L_BRACK)
		return false;

	ntoken = gettoken();
	if (ntoken.getType() != R_BRACK)
		return false;

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
			re = funCall();
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
	if (ntoken.getType() != IDENTITY)
		return false;
	ntoken = gettoken();
	if (ntoken.getType() == L_SQUARE)
	{
		ntoken = gettoken(1);
		bool re = expression();
		ntoken = gettoken();
		if (ntoken.getType() != R_SQUARE)
			return false;
		ntoken = gettoken();
	}
	if (ntoken.getType() != ASSIGN)
		return false;
	ntoken = gettoken(1);
	bool re = expression();
	if (!re)
		return false;
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
	bool re = expression();
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
	re = suitationTab();
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

bool Parser::suitationTab()
{
	bool re = suitationSta();
	if (!re)
		return false;
	Token  ntoken = gettoken(1);
	while (ntoken.getType() == CASE)
	{
		re = suitationSta();
		if (!re)
			return false;
		ntoken = gettoken(1);
	}
	printresult("This is a Suitation Table!");
	return true;
}

bool Parser::suitationSta()
{
	Token ntoken = gettoken();
	if (ntoken.getType() != CASE)
		return false;
	
	ntoken = gettoken();
	if (ntoken.getType() != CONST_INT  && ntoken.getType() != CONST_CHAR)
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

	ntoken = gettoken(1);
	if (ntoken.getType() == L_BRACK)
	{
		ntoken = gettoken();
		if (pretoken.size() == 0)
		{
			ntoken = lex.nextsymbol();
			pretoken.insert(pretoken.begin(), ntoken);
		}
		bool re = valueParas();
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
	bool re = funCall();
	if (!re)
		return false;
	printresult("This is a Returned Function Call!");
}

bool Parser::unretfunCall()
{
	bool re = funCall();
	if (!re)
		return false;
	printresult("This is a Unreturned Function Call!");
}

bool Parser::valueParas()
{
	bool re = expression();
	if (!re)
		return false;
	
	Token ntoken = gettoken(1);
	while (ntoken.getType() == COMMA)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken(1);
		re = expression();
		if (!re)
			return false;

		ntoken = gettoken(1);
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
		bool re = expression();
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
	
			bool re = expression();
			if (!re)
				return false;
		}
	}
	else
	{
		bool re = expression();
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
	bool re = expression();
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
		re = expression();
		if (!re)
			return false;
	}
	printresult("This is a Condition Statement!");
	return true;
}

bool Parser::expression()
{
	Token ntoken = gettoken(1);
	if (ntoken.getType() == PLUS || ntoken.getType() == MINUS)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken(1);
	}

	bool re = item();
	if (!re)
		return false;

	ntoken = gettoken(1);
	while (ntoken.getType() == PLUS || ntoken.getType() == MINUS)
	{
		pretoken.erase(pretoken.begin());

		ntoken = gettoken(1);
		bool re = item();
		if (!re)
			return false;
		ntoken = gettoken(1);
	}
	printresult("This is an Expression!");
	return true;
}

bool Parser::item()
{
	bool re = factor();
	if (!re)
		return false;

	Token ntoken = gettoken(1);
	while (ntoken.getType() == MULTI || ntoken.getType() == DIVIDE)
	{
		pretoken.erase(pretoken.begin());
		ntoken = gettoken(1);
		re = factor();

		ntoken = gettoken(1);
	}

	printresult("This is an Item!");
	return true;
}

bool Parser::factor()
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
			pretoken.erase(pretoken.begin());
			pretoken.erase(pretoken.begin());

			ntoken = gettoken(1);
			re = expression();
			if (!re)
				return false;

			ntoken = gettoken();
			if (ntoken.getType() != R_SQUARE)
				return false;
		}
		else if (ntoken.getType() == L_BRACK)
		{
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
		re = expression();
		if (!re)
			return false;
		ntoken = gettoken();
		if (ntoken.getType() != R_BRACK)
			return false;
		break;
	case CONST_INT:
		re = integer();
		if (!re)
			return false;
		break;
	case CONST_CHAR:
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