#ifndef ERROR_H_
#define ERROR_H_
#include "fstream"
#include "iostream"


#define NDEFERROR		0

// lex error
#define INVALID_CHAR	1
#define UNMATCH_SYMBOL	2

//syntax error
#define SYNTAX_ERROR	101
#define NO_L_BRACK		102
#define NO_R_BRACK		103
#define NO_L_SQUARE		104
#define NO_R_SQUARE		105
#define NO_L_CURLY		106
#define NO_R_CURLY		107
#define NO_COMMA		108
#define NO_COLON		111
#define NO_SEMICOLON	109
#define INVALID_OP		110
#define NO_SYMBOL		112

//semantic error
#define IDENT_REDIFN	201
#define UNMATCH_TYPE	202
#define UNMATCH_IFSTA	203
#define IDENT_NODIFN	204
#define WRONG_PARANUM	205
#define WRONG_INDEX		206
#define SAME_CASEVALUE	207

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
	void printerr(string c = "");
};


#endif // !ERROR_H_