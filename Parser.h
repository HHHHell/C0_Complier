#ifndef PARSER_H_
#define PARSER_H_
#include "vector"
#include "fstream"

#include "token.h"
#include "lexer.h"
#include "symbolitem.h"
#include "symboltable.h"
#include "midcodes.h"

class Parser {
private:
	Lexer &lex;
	Midcodes &midcodes;
	SymbolTable &stable;
	vector<Token> pretoken;
	Token gettoken(int mode);
	ofstream pout;
	void printresult(std::string str);
public:
	bool result;

	Parser(Lexer &l, SymbolTable &table, Midcodes &m);
	~Parser();
	bool program();
	bool constDeclare();
	bool constDef();
	bool variableDeclare();
	bool variableDef();
	bool retfunDef();
	bool funDefhead();
	bool parameters();
	bool compoundSta();
	bool unretfunDef();
	bool mainFun();
	bool statementList();
	bool statement();
	bool assignSta();
	bool ifSta();
	bool switchSta();
	bool suitationTab();
	bool suitationSta();
	bool defaultSta();
	bool whileSta();
	bool funCall();
	bool retfunCall();
	bool unretfunCall();
	bool valueParas();
	bool returnSta();
	bool printSta();
	bool scanSta();
	bool conditionSta();
	bool expression();
	bool item();
	bool factor();
	bool integer();
};

#endif // !PARSE_H_
