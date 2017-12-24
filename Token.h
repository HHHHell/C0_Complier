#pragma once

#include "string"
#include "vector"
#include "string"
#include "Error.h"
#include "define.h"

using namespace std;
class Token
{
private:
	enum symbol type;
	int int_value;
	char char_value;
	string str_value;
	int line_num;

public:
	Token();
	Token(const Token &atoken);
	Token(enum symbol _type, int _num, int _line);
	Token(enum symbol _type, char _char, int _line);
	Token(enum symbol _type, string _str, int _line);

	enum symbol getType();
	int getIntValue();
	char getCharValue();
	string getStrValue();
	int getLinenum();
	string toString();
};
