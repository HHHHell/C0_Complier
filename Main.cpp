#pragma once
#include "iostream"
#include "string"
#include "fstream"

#include "define.h"
#include "lexer.h"
#include "error.h"
#include "parser.h"

int main(){
	using namespace std;
	char filename[MAXLENGTH];

	cout << "Please input your file path:" << endl;
	cin >> filename;
	Lexer lex(filename);
	if (!lex.checkfile())
	{
		cout << "Cann't open file: " << filename << endl;
		return 0;
	}
	Midcodes mcodes;
	map<string, SymbolTable> tables;
	SymbolTable ntable(0);	
	tables.insert(pair<string, SymbolTable>("OverAll", ntable));

	Parser parser(lex, tables, mcodes);

	bool re = parser.program();
	parser.result = re;
	if (!re)
		cout << "Error!" << endl;
	return 0;
}
