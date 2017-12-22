#pragma once
#include "iostream"
#include "string"
#include "lexer.h"
#include "define.h"
#include "error.h"
#include "token.h"

string symstring[] = {
	"NOTDEFINE",
	"IDENTITY",
	"CONST_INT",
	"CONST_CHAR",
	"CONST_STRING",

	"RETURNSY",
	"IFSY",
	"ELSESY",
	"WHILESY",
	"SWITCHSY",
	"CASESY",
	"DEFAULTSY",
	"PRINTFSY",
	"SCANFSY",
	"VOIDSY",
	"INTSY",
	"CHARSY",
	"CONSTSY",

	"PLUS",
	"MINUS",
	"MULTI",
	"DIVIDE",
	"SMALLER",
	"GREATER",
	"ASSIGN",
	"NOLESS",
	"NOBIGGER",
	"EQUAL",
	"NOTEQUAL",
	"COMMA",
	"SEMICOLON",
	"COLON",
	"QUOTE",
	"DQUOTE",
	"L_BRACK",
	"R_BRACK",
	"L_SQUARE",
	"R_SQUARE",
	"L_CURLY",
	"R_CURLY"
};
std::string str2lower(std::string str)
{
	using namespace std;
	char *p = const_cast<char*>(str.c_str());
	int i = 0;
	for (i = 0; i < str.length(); i++)
	{
		if (p[i] >= 'A' && p[i] <= 'Z')
			p[i] = tolower(p[i]);
	}
	return string(p);
}

string id2symbol(enum symbol id)
{
	return symstring[id];
}

Token::Token() {
	type = NOTDEFINE;
	int_value = 0;
	char_value = '\0';
	str_value = "";
	line_num = -1;
}

Token::Token(const Token &atoken)
{
	type = atoken.type;
	int_value = atoken.int_value;
	char_value = atoken.char_value;
	str_value = std::string(atoken.str_value.c_str());
	line_num = atoken.line_num;
}

Token::Token(enum symbol _type, int _num, int _line)
{
	type = _type;
	int_value = _num;
	char_value = '\0';
	str_value = "";
	line_num = _line;
}

Token::Token(enum symbol _type, char _char, int _line)
{
	type = _type;
	int_value = 0;
	char_value = _char;
	str_value = "";
	line_num = _line;
}

Token::Token(enum symbol _type, std::string _str, int _line)
{
	type = _type;
	int_value = 0;
	char_value = '\0';
	str_value = _str;
	line_num = _line;

}


enum symbol Token::getType()
{
	return type;
}

int Token::getIntValue()
{
	return int_value;
}

char Token::getCharValue()
{
	return char_value;
}

std::string Token::getStrValue()
{
	return str_value;
}

int Token::getLinenum()
{
	return line_num;
}

string Token::toString()
{
	using namespace std;
	string str = id2symbol(type) + ' ';
	switch (type)
	{
	case CONST_INT:
		char x[100];
		itoa(getIntValue(), x, 10);
		str += x;
		break;
	case  CONST_CHAR:
		str += getCharValue();
		break;
	default:
		str += getStrValue();
	}
	return str;
}
