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

	cout << "Please input your file path: " << endl;
	cin >> filename;
	ifstream infile(filename);
	while (!infile)
	{
		cout << "Cann't open file: " << filename << endl << endl;
		cout << "Please input file path again: " << endl;
		cin >> filename;
		infile.open(filename);
	}

	Lexer lex(filename);
	map<string, SymbolTable> tables;
	Midcodes mcodes(tables);

	SymbolTable ntable(0);	
	tables.insert(pair<string, SymbolTable>("#OverAll", ntable));

	SymbolTable ttable(0);
	tables.insert(pair<string, SymbolTable>("#StringConst", ttable));

	Parser parser(lex, tables, mcodes);

	bool re = parser.program();
	parser.result = re;
	if (!re)
	{
		cout << "Error!" << endl;
		return 0;
	}
	mcodes.output("result//mcodes.txt");
	mcodes.toMips("result//finalcode.asm", tables);
	return 0;
}
