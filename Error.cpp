#include "iostream"
#include "string"
#include "fstream"
#include "error.h"

using namespace std;
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

void Error::printerr(string c)
{
	using namespace std;

	cout << no << ": ";
	eout << no << ": ";
	switch (errcode)
	{
	case INVALID_CHAR:
		cout << "Invalid characters " << c << " in line " << line_num << endl;
		eout << "Invalid characters " << c << " in line " << line_num << endl;
		break;
	case UNMATCH_SYMBOL:
		cout << "Unmatched symbols " << c << " in line " << line_num << endl;
		eout << "Unmatched symbols " << c << " in line " << line_num << endl;
		break;

	case SYNTAX_ERROR:
		cout << "Syntax error in line " << line_num << endl;
		eout << "Syntax error in line " << line_num << endl;
		break;
	case NO_L_BRACK:
		cout << "No left brack in line " << line_num << endl;
		eout << "No left brack in line " << line_num << endl;
		break;
	case NO_R_BRACK:
		cout << "No right brack in line " << line_num << endl;
		eout << "No right brack in line " << line_num << endl;
		break;
	case NO_L_SQUARE:
		cout << "No left square brack in line " << line_num << endl;
		eout << "No left square brack in line " << line_num << endl;
		break;
	case NO_R_SQUARE:
		cout << "No right square brack in line " << line_num << endl;
		eout << "No right square brack in line " << line_num << endl;
		break;
	case NO_L_CURLY:
		cout << "No left curly brack in line " << line_num << endl;
		eout << "No left curly brack in line " << line_num << endl;
		break;
	case NO_R_CURLY:
		cout << "No right curly brack in line " << line_num << endl;
		eout << "No right curly brack in line " << line_num << endl;
		break;
	case NO_COMMA:
		cout << "No comma brack in line " << line_num << endl;
		eout << "No comma curly brack in line " << line_num << endl;
		break;
	case NO_COLON:
		cout << "No colon brack in line " << line_num << endl;
		eout << "No colon curly brack in line " << line_num << endl;
		break;
	case NO_SEMICOLON:
		cout << "No semicolon in line " << line_num << endl;
		eout << "No semicolon in line " << line_num << endl;
		break;
	case INVALID_OP:
		cout << "Invalid operator " << c << " in line " << line_num << endl;
		eout << "Invalid operator " << c << " in line " << line_num << endl;
		break;
	case NO_SYMBOL:
		cout << "No symbol " << c << " in line " << line_num << endl;
		eout << "No symbol " << c << " in line " << line_num << endl;
		break;

	case IDENT_REDIFN:
		cout << "Identity " << c << " redifined in line " << line_num << endl;
		eout << "Identity " << c << " redifined in line " << line_num << endl;
		break;
	case IDENT_NODIFN:
		cout << "Identity " << c << "referred before declared in line " << line_num << endl;
		eout << "Identity " << c << "referred before declared in line " << line_num << endl;
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
	case SAME_CASEVALUE:
		cout << "Same case value in line " << line_num << endl;
		eout << "Same case value in line " << line_num << endl;
		break;
	default:
		break;
	}

	eout.flush();
}