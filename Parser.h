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
	map<string, SymbolTable> &tables;
	vector<Token> pretoken;
	SymbolTable ntable;
	string nkey;
	ofstream pout;

	Token gettoken(int mode);
	void printresult(std::string str);
	bool isexists(SymbolItem item);
	bool isexists(string name);
	SymbolItem find(string name);
public:
	bool result;

	Parser(Lexer &l, map<string, SymbolTable> &tlist, Midcodes &m);
	~Parser();
	bool program();
	bool constDeclare();
	bool constDef();
	bool variableDeclare();
	bool variableDef();
	bool retfunDef();
	bool funDefhead(enum type &t, string &name, int &line);
	bool parameters(vector<enum type> &paras);
	bool compoundSta();
	bool unretfunDef();
	bool mainFun();
	bool statementList();
	bool statement();
	bool assignSta();
	bool ifSta();
	bool switchSta();
	bool suitationTab(bool ischar);
	bool suitationSta(bool ischar);
	bool defaultSta();
	bool whileSta();
	bool funCall();
	bool retfunCall();
	bool unretfunCall();
	bool valueParas(vector<enum type> paras);
	bool returnSta();
	bool printSta();
	bool scanSta();
	bool conditionSta();
	bool expression(bool &ischar);
	bool item(bool &ischar);
	bool factor(bool &iscahr);
	bool integer();
};

#endif // !PARSE_H_
