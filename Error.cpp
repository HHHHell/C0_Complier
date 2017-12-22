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
}

void Error::printerr()
{
	using namespace std;
	switch (errcode)
	{
	case INVALID_CHAR:
		cout << "Invalid characters in line: " << line_num;
		break;
	case UNMAtCH_SQUOTE:
		cout << "Unmatched sigle quote in line: " << line_num;
		break;
	case UNMATCH_DQUOTE:
		cout << "Unmatched double quote in line: " << line_num;
		break;
	default:
		break;
	}
}

int Error::skiperr(char* end_char)
{
	using namespace std;

	int i = 0;
	switch (errcode)
	{
	default:
		end_char[i++] = ' ';
		end_char[i++] = '\n';
		end_char[i++] = '\t';
		break;
	}
	return i;
}