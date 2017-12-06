#ifndef LEXER_H_
#define LEXER_H_

#include "vector"
#include "fstream"
#include "Error.h"
#include "define.h"
#include "Token.h"

class Lexer {
private:
	char lastch;
	std::FILE* fin;
	int line_num;

	ofstream lout;
	int token_count;
	Token nextsym();
public:
	Lexer();
	~Lexer();
	Lexer(char* filename);
	char getLastchar();
	int checkfile();
	Token nextsymbol();
	bool skip(char* end_char);
};

#endif // !LEXER_H_
