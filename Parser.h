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
	SymbolTable *ntable;
	string nkey;
	ofstream pout;

	int varnum;

	Token gettoken(int mode);
	void printresult(std::string str);
	bool isexists(SymbolItem item);
	bool isexists(string name);
	SymbolItem pfind(string name);
	string genvar();
	string genvar(int value);
	string genvar(enum type t);
	string genvar(int value, enum type t);
	string genvar(string str);
	string genlabel();
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
	bool suitationTab(bool ischar, string flag, int &caseindex, string endlabel);
	bool suitationSta(bool ischar, string flag, int &caseindex, string endlabel);
	bool defaultSta(int &caseindex, string endlabel);
	bool whileSta();
	bool retfunCall(int &index);
	bool unretfunCall();
	bool valueParas(vector<enum type> paras, int &index);
	bool returnSta();
	bool printSta();
	bool scanSta();
	bool conditionSta(vector<string> &cond);
	bool expression(bool &ischar, string& result, int &index);
	bool item(bool &ischar, string& result, int &index);
	bool factor(bool &iscahr, string& result, int &index);
	bool integer(int& value);
};

#endif // !PARSE_H_
