#pragma once
#include "common.h"
#include "StretchyBuffer.h"
#include "StringIntern.h"
#include "Lexer.h"


typedef enum TokenKind
{
	//reserve first 128 values for one char tokens
	TOKEN_LAST_CHAR = 127,
	TOKEN_INT,
	TOKEN_IDENTIFIER,

}TokenKind;

//Token has to be of some kind
typedef struct Token
{
	TokenKind kind;

	const char* start;
	const char* end;

	union
	{
		int intval;
		const char* identifier_name;
	};
}Token;

#define assert_token(x) assert(match_token(x))
#define assert_token_name(x) assert(token.identifier_name == str_intern(x) && match_token(TOKEN_IDENTIFIER))
#define assert_token_int(x) assert(token.intval  == (x) && match_token(TOKEN_INT))
#define assert_token_eof() assert(is_token(0))



void next_token();
void print_token(Token token);
