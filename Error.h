#ifndef ERROR_H_
#define ERROR_H_

#define NDEFERROR		0

// lexer error
#define INVALID_CHAR	1
#define UNMAtCH_SYMBOL	2

//parser error
#define SYNTAX_ERROR	11

class Error
{
private:
	int line_num;
	int errcode;
	int no;
	static int errnum;
public:
	Error();
	Error(int line, int code);
	void printerr();
};

#endif // !ERROR_H_