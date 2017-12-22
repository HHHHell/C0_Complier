#ifndef DEFINE_H_
#define DEFINE_H_

#define MAXLENGTH	60

using namespace std;
enum symbol 
{
	NOTDEFINE,
	IDENTITY,			// a
	CONST_INT,			// 12
	CONST_CHAR,			// 'a'
	CONST_STRING,		// "string"

	RETURN,
	IF,
	ELSE,
	WHILE,
	SWITCH,
	CASE,
	DEFAULT,
	PRINTF,
	SCANF,
	VOID,
	INT,
	CHAR,
	CONST,
	
	PLUS,		// '+'
	MINUS,		// '-'
	MULTI,		// '*'
	DIVIDE,		// '/'
	SMALLER,	// '<'
	GREATER,	// '>'
	ASSIGN,		// '='
	NOLESS,		// '>='
	NOBIGGER,	// '<='
	EQUAL,		// '=='
	NOTEQUAL,	// '!='
	COMMA,		// ','
	SEMICOLON,	// ';'
	COLON,		// ':'
	QUOTE,		// '\\''
	DQUOTE,		// '"'
	L_BRACK,	// '('
	R_BRACK,	// ')'
	L_SQUARE,	// '['
	R_SQUARE,	// ']'
	L_CURLY,	// '{'
	R_CURLY		// '}'
};

enum type
{
	NOTYPE,
	INT_TYPE,
	CHAR_TYPE,
	VOID_TYPE
};

enum kind
{
	NOKIND,
	CONST_VAR,
	VARIABLE,
	VAR_LIST,
	PARAMETERS,
	FUNCTION
};

#endif // !DEFINE_H_