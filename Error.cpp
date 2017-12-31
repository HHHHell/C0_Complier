#include "iostream"
#include "fstream"
#include "error.h"

extern bool flag;
Error::Error()
{
	eout.open("result//error.txt", ios::app);
	line_num = 1;
	errcode = 0;
	flag = false;
}

Error::Error(int line, int code) 
{

	eout.open("result//error.txt", ios::app);
	line_num = line;
	errcode = code;
	no = errnum++;
	flag = false;
}

void Error::printerr()
{
	using namespace std;

	cout << no << ": ";
	eout << no << ": ";
	switch (errcode)
	{
	case INVALID_CHAR:
		cout << "Invalid characters in line " << line_num << endl;
		eout << "Invalid characters in line " << line_num << endl;
		break;
	case UNMAtCH_SYMBOL:
		cout << "Unmatched symbols in line " << line_num << endl;
		eout << "Unmatched symbols in line " << line_num << endl;
		break;
	case SYNTAX_ERROR:
		cout << "Syntax error in line " << line_num << endl;
		eout << "Syntax error in line " << line_num << endl;
	default:
		break;
	}
	eout.flush();
}