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
	case UNMATCH_SYMBOL:
		cout << "Unmatched symbols in line " << line_num << endl;
		eout << "Unmatched symbols in line " << line_num << endl;
		break;
	case SYNTAX_ERROR:
		cout << "Syntax error in line " << line_num << endl;
		eout << "Syntax error in line " << line_num << endl;
		break;
	case IDENT_REDIFN:
		cout << "Identity redifined in line " << line_num << endl;
		eout << "Identity redifined in line " << line_num << endl;
		break;
	case IDENT_NODIFN:
		cout << "Identity not decliared is referred in line " << line_num << endl;
		eout << "Identity not decliared is referred in line " << line_num << endl;
		break;
	case UNMATCH_TYPE:
		cout << "Unmatched type in line " << line_num << endl;
		eout << "Unmatched type in line " << line_num << endl;
		break;
	case UNMATCH_IFSTA:
		cout << "Unmatched if statement in line " << line_num << endl;
		eout << "Unmatched if statement in line " << line_num << endl;
		break;
	case WRONG_PARANUM:
		cout << "Wrong parameters number in function call in line " << line_num << endl;
		eout << "Wrong parameters number in function call in line " << line_num << endl;
		break;
	case WRONG_INDEX:
		cout << "Wrong index of list in line " << line_num << endl;
		eout << "Wrong index of list in line " << line_num << endl;
		break;
	default:
		break;
	}
	eout.flush();
}