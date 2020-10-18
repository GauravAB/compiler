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
	TOKEN_FLOAT,
	TOKEN_STR,
	TOKEN_IDENTIFIER,

}TokenKind;

typedef enum TokenModifier
{
	TOKENMOD_NONE,
	TOKENMOD_CHAR,
	TOKENMON_HEX,
	TOKENMOD_OCT,
	TOKENMOD_BIN,

}TokenMod;

//Token has to be of some kind
typedef struct Token
{
	TokenKind kind;
	TokenMod mod;

	const char* start;
	const char* end;

	union
	{
		uint64_t intval;
		const char* identifier_name;
		const char* strval;
		double floatval;
	};
}Token;



void next_token();
void print_token(Token token);
void syntax_error(const char* fmt, ...);

#define assert_token(x) assert(match_token(x))
#define assert_token_name(x) assert(token.identifier_name == str_intern(x) && match_token(TOKEN_IDENTIFIER))
#define assert_token_int(x) assert(token.intval  == (x) && match_token(TOKEN_INT))
#define assert_token_eof() assert(is_token(0))
#define assert_token_float(x) assert(token.floatval == (x) && match_token(TOKEN_FLOAT))
#define assert_token_str(x) assert( strcmp(token.strval,(x)) == 0 && match_token(TOKEN_STR))