#include "vector"
#include "iostream"
#include "string"
#include "fstream"

#include "stdlib.h"

#include "lexer.h"
#include "define.h"
#include "error.h"
#include "token.h"

extern std::string str2lower(std::string str);

Lexer::Lexer() {
	lastch = '\0';
	line_num = 1;
}

Lexer::Lexer(char* filename)
{
	using namespace std;
	lastch = '\0';
	line_num = 1;
	fin = fopen(filename, "r");
	lexout.open("result\\lexer_result.txt");
	token_count = 1;
}

Lexer::~Lexer()
{
	lexout << "\nFinished!";
	lexout.flush();
	lexout.close();
	fclose(fin);
}

int Lexer::checkfile()
{
	if (!fin)
		return 0;
	if (feof(fin))
		return 0;
	return 1;
}

char Lexer::getLastchar()
{
	return lastch;
}

int Lexer::getline()
{
	return line_num;
}

Token Lexer::nextsymbol()
{
	Token ntoken = nextsym();
	if (ntoken.getType() != NOTDEFINE)
	{
		lexout << token_count++ << ' ' << ntoken.toString() << endl;
	}
	if (ntoken.getType() == NOTDEFINE && checkfile())
	{
		cout << "Not defined" << endl;
		lexout << "Not defined" << endl;

	}
	lexout.flush();
	return ntoken;
}

Token Lexer::nextsym()
{
	int n = 0;
	enum symbol type = NOTDEFINE;
	char word[MAXLENGTH];

	while (isspace(lastch) || lastch == '\0')
	{
		if (lastch == '\n')
			line_num++;
		lastch = fgetc(fin);
		continue;
	}
	if (!checkfile())
		return Token(NOTDEFINE, '\0', line_num);

	if (isalpha(lastch) || lastch == '_')
	{
		while (isalpha(lastch) || isdigit(lastch) || lastch == '_')
		{
			if (lastch == '_' || isdigit(lastch))
				word[n++] = lastch;
			else
			{
				word[n++] = islower(lastch) ? lastch : tolower(lastch);
			}
			lastch = fgetc(fin);
		}
		word[n] = '\0';
		std::string ss = word;
		std::string ts = str2lower(ss);
		if (ts == "return")
		{
			type = RETURN;
			return Token(type, ss, line_num);
		}
		else if (ts == "while")
		{
			type = WHILE;
			return Token(type, ss, line_num);
		}
		else if (ts == "if")
		{
			type = IF;
			return Token(type, ss, line_num);
		}
		else if (ts == "else")
		{
			type = ELSE;
			return Token(type, ss, line_num);
		}
		else if (ts == "switch")
		{
			type = SWITCH;
			return Token(type, ss, line_num);
		}
		else if (ts == "case")
		{
			type = CASE;
			return Token(type, ss, line_num);
		}
		else if (ts == "default")
		{
			type = DEFAULT;
			return Token(type, ss, line_num);
		}
		else if (ts == "printf")
		{
			type = PRINTF;
			return Token(type, ss, line_num);
		}
		else if (ts == "scanf")
		{
			type = SCANF;
			return Token(type, ss, line_num);
		}
		else if (ts == "void")
		{
			type = VOID;
			return Token(type, ss, line_num);
		}
		else if (ts == "int")
		{
			type = INT;
			return Token(type, ss, line_num);
		}
		else if (ts == "char")
		{
			type = CHAR;
			return Token(type, ss, line_num);
		}
		else if (ts == "const")
		{
			type = CONST;
			return Token(type, ss, line_num);
		}
		else
		{
			type = IDENTITY;
			return Token(type, ss, line_num);
		}
	}

	else if (isdigit(lastch))
	{
		while (isdigit(lastch))
		{
			word[n++] = lastch;
			lastch = fgetc(fin);
		}
		word[n] = '\0';
		type = CONST_INT;
		return Token(type, atoi(word), line_num);
	}

	else if (lastch == '>')
	{
		word[n++] = lastch;
		lastch = fgetc(fin);
		if (lastch == '=')
		{
			word[n++] = lastch;
			lastch = fgetc(fin);

			word[n] = '\0';
			type = NOLESS;
			return Token(type, ">=", line_num);
		}
		word[n] = '\0';
		type = GREATER;
		return Token(type, ">", line_num);
	}

	else if (lastch == '<')
	{
		word[n++] = lastch;
		lastch = fgetc(fin);
		if (lastch == '=')
		{
			word[n++] = lastch;
			lastch = fgetc(fin);

			word[n] = '\0';
			type = NOBIGGER;
			return Token(type, "<=", line_num);
		}
		word[n] = '\0';
		type = SMALLER;
		return Token(type, "<", line_num);
	}

	else if (lastch == '!')
	{
		word[n++] = lastch;
		lastch = fgetc(fin);
		if (lastch == '=')
		{
			word[n++] = lastch;
			lastch = fgetc(fin);

			word[n] = '\0';
			type = NOTEQUAL;
			return Token(type, "!=", line_num);
		}

		Error err(line_num, INVALID_CHAR);
		err.printerr("!");
		return Token(type, "!=", line_num);
	}

	else if (lastch == '=')
	{
		word[n++] = lastch;
		lastch = fgetc(fin);
		if (lastch == '=')
		{
			word[n++] = lastch;
			lastch = fgetc(fin);

			word[n] = '\0';
			type = EQUAL;
			return Token(type, "==", line_num);
		}
		word[n] = '\0';
		type = ASSIGN;
		return Token(type, "=", line_num);
	}

	else if (lastch == '"')
	{
		word[n++] = lastch;
		lastch = fgetc(fin);
		while (lastch != '"')
		{
			if (lastch < 32 || lastch > 127)
			{
				Error err(line_num, INVALID_CHAR);
				err.printerr("" + lastch);
				if (lastch == '\n')
				{
					lastch = '"';
					break;
				}
				lastch = fgetc(fin);
				continue;
			}
			word[n++] = lastch;
			lastch = fgetc(fin);
		}
		word[n++] = lastch;
		lastch = fgetc(fin);

		word[n] = '\0';
		type = CONST_STRING;
		return Token(type, word, line_num);
	}

	else if (lastch == '\'')
	{
		word[n++] = lastch;
		lastch = fgetc(fin);
		while (lastch != '\'' && n < 3)
		{
			if (!isdigit(lastch) && !isalpha(lastch) && lastch != '+'&& lastch != '-' && lastch != '*' && lastch != '/' && lastch != '_')
			{
				Error err(line_num, INVALID_CHAR);
				err.printerr(lastch + "");
				lastch = '_';
			}
			word[n++] = lastch;
			lastch = fgetc(fin);
		}
		if (lastch != '\'')
		{
			Error err(line_num, UNMATCH_SYMBOL);
			err.printerr("'");
			lastch = '\'';
		}
		word[n++] = lastch;
		lastch = fgetc(fin);

		word[n] = '\0';
		type = CONST_CHAR;
		return Token(type, word[1], line_num);
	}

	else if (lastch == '+')
	{
		type = PLUS;
		lastch = fgetc(fin);
		return Token(type, "+", line_num);
	}
	else if (lastch == '-')
	{
		type = MINUS;
		lastch = fgetc(fin);
		return Token(type, "-", line_num);
	}
	else if (lastch == '*')
	{
		type = MULTI;
		lastch = fgetc(fin);
		return Token(type, "*", line_num);
	}
	else if (lastch == '/')
	{
		type = DIVIDE;
		lastch = fgetc(fin);
		return Token(type, "/", line_num);
	}

	else if (lastch == ';')
	{
		type = SEMICOLON;
		lastch = fgetc(fin);
		return Token(type, ";", line_num);
	}
	else if (lastch == ',')
	{
		type = COMMA;
		lastch = fgetc(fin);
		return Token(type, ",", line_num);

	}
	else if (lastch == ':')
	{
		type = COLON;
		lastch = fgetc(fin);
		return Token(type, ":", line_num);
	}

	else if (lastch == '(')
	{
		type = L_BRACK;
		lastch = fgetc(fin);
		return Token(type, "(", line_num);
	}
	else if (lastch == ')')
	{
		type = R_BRACK;
		lastch = fgetc(fin);
		return Token(type, ")", line_num);
	}
	else if (lastch == '[')
	{
		type = L_SQUARE;
		lastch = fgetc(fin);
		return Token(type, "[", line_num);
	}
	else if (lastch == ']')
	{
		type = R_SQUARE;
		lastch = fgetc(fin);
		return Token(type, "]", line_num);
	}
	else if (lastch == '{')
	{
		type = L_CURLY;
		lastch = fgetc(fin);
		return Token(type, "{", line_num);
	}
	else if (lastch == '}')
	{
		type = R_CURLY;
		lastch = fgetc(fin);
		return Token(type, "}", line_num);
	}
	else
	{
		Error err(line_num, INVALID_CHAR);
		err.printerr(lastch + "");
		
		lastch = fgetc(fin);
		return nextsym();
	}
}

bool Lexer::skip(vector<char> end_char)
{
	while (checkfile())
	{
		if (lastch == '\n')
			line_num++;
		for (int i = 0; i < end_char.size(); i++)
		{
			if (lastch == end_char[i])
			{
				return true;
			}
		}
		if (checkfile())
		{
			lastch = fgetc(fin);
		}
		else
			return false;
	}
	return false;
}
