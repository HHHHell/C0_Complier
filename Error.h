#ifndef ERROR_H_
#define ERROR_H_

#define NDEFERROR		0
// lex error
#define INVALID_CHAR	1
#define UNMAtCH_SQUOTE	2
#define UNMATCH_DQUOTE	3

class Error
{
private:
	int line_num;
	int errcode;
public:
	Error();
	Error(int line, int code);
	void printerr();
	int skiperr(char* end_char);
};

#endif // !ERROR_H_