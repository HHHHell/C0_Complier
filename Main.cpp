#pragma once
#include "iostream"
#include "string"
#include "fstream"

#include "define.h"
#include "lexer.h"
#include "error.h"
#include "parser.h"

using namespace std;

int Error::errnum = 1;
bool flag = true;

void prefile()
{
	vector<string> files = { "result//error.txt","result//finalcode.asm",
		"result//lexer_result.txt","result//parser_result.txt","result//mcodes.txt" };
	ofstream file;
	for (int i = 0; i < files.size(); i++)
	{
		file.open(files[i]);
		file.close();
	}
}

int main(){
	prefile();
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
	if (!re || !flag)
	{
		cout << "Error occured!" << endl;
	}
	else
	{
		mcodes.output("result//mcodes.txt");
		mcodes.toMips("result//finalcode.asm", tables);

		cout << "Finished!" << endl;
	}
	return 0;
}