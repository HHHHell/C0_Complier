#ifndef ERROR_H_
#define ERROR_H_
#include "fstream"
#include "iostream"


#define NDEFERROR		0

// lexer error
#define INVALID_CHAR	1
#define UNMATCH_SYMBOL	2

//parser error
#define SYNTAX_ERROR	11
#define IDENT_REDIFN	12
#define UNMATCH_TYPE	13
#define UNMATCH_IFSTA	14
#define IDENT_NODIFN	15
#define WRONG_PARANUM	16
#define WRONG_INDEX		17

using namespace std;

class Error
{
private:
	int line_num;
	int errcode;
	int no;
	ofstream eout;
	static int errnum;

public:
	Error();
	Error(int line, int code);
	void printerr();
};

#endif // !ERROR_H_