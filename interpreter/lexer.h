#pragma once

#define ID_LEN_MAX 256
#define NUMBER_LEN_MAX 64
#define STRING_LEN_MAX 1024
#define OPERATOR_LEN_MAX 1024

#include <stdio.h>
#include <list>

enum class Token {
	TOKEN_ERR = -2,
	TOKEN_EOF,
	TOKEN_IDENTIFIER,
	TOKEN_INTEGER,
	TOKEN_FLOAT,
	TOKEN_KEYWORD,
	TOKEN_STRING,
	TOKEN_OPERATOR 
};

enum class LexerErr {
	TOO_LONG_IDENTIFIER,
	TOO_LONG_NUMBER,
	TOO_LONG_STRING,
	TOO_LONG_OPERATOR,
	MISUSE_ESCAPE_CHAR,
	UNRECOGNIZED_CHAR
};

enum class Keyword {
	DEFINECHARACTER,
	DEFINESCENE,
	WHOSE,
	NAME,
	TEXTURE,
	SAY,
	VOICEOVER,
	ATTACK,
	ENDDEFINE,
	USESCENE,
	ENTER,
	L,
	R,
	EXIT,
	DELAY,
	RETREAT,
	DEFINESOUND,
	FILENAME,
	LOOP,
	NOLOOP,
	PLAYSOUND

};

void LexerScan(FILE* fp);
Token GetToken(FILE* fp = stdin);
wchar_t Escape(wchar_t c);

struct TokenTableItem{
	Token type;
	void* data;
};

typedef std::list<TokenTableItem> TokenTableType;

wchar_t getWideChar(FILE* fp);