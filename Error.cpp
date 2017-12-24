#include "iostream"
#include "error.h"

Error::Error()
{
	line_num = 1;
	errcode = 0;
}

Error::Error(int line, int code) 
{
	line_num = line;
	errcode = code;
	no = errnum++;
}

void Error::printerr()
{
	using namespace std;
	cout << no << " ";
	switch (errcode)
	{
	case INVALID_CHAR:
		cout << "Invalid characters in line: " << line_num << endl;
		break;
	case UNMAtCH_SYMBOL:
		cout << "Unmatched symbols in line: " << line_num << endl;
		break;
	case SYNTAX_ERROR:
		cout << "Syntax error in line: " << line_num << endl;
	default:
		break;
	}
}