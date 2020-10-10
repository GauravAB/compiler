#include "Parser.h"

extern Token token;

void fatal(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("FATAL: ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
	exit(1);
}

/*
//left associative parser

expr3 = INT | '(' expr ')'
expr2 = [-]expr3
expr1 = expr2 ([/\*]) expr2)*
expr0 = expr1 ([+-]) expr1)*

expr = expr0

*/

int parse_expr3()
{
	int val;

	if (is_token(TOKEN_INT))
	{
		val = token.intval;
		next_token();
		return val;
	}
	else if (match_token('('))
	{
		
		val = parse_expr();
		expect_token(')');
		return val;
	}
	else
	{
		fatal("expected integer or (, got %s", token_kind_name(token.kind));
		return 0;
	}
}



int parse_expr2()
{
	if (match_token('-'))
	{
		return -parse_expr(2);
	}
	else
	{
		return parse_expr3();
	}
}


int parse_expr1()
{
	int val = parse_expr2();
	while (is_token('*') || is_token('/'))
	{
		char op = token.kind;
		next_token();
		int rval = parse_expr2();

		if (op == '*')
		{
			val *= rval;
		}
		else
		{
			assert(op == '/');
			assert(rval != 0);
			val /= rval;
		}

	}

	return val;
}

int parse_expr0()
{
	int val = parse_expr1();
	while (is_token('+') || is_token('-'))
	{
		char op = token.kind;
		next_token();
		int rval = parse_expr1();
		
		if (op == '+')
		{
			val += rval;
		}
		else
		{
			assert(op == '-');
			val -= rval;

		}
	}
	return val;
}

int  parse_expr()
{
	return parse_expr0();
}




//auxillary functions


inline bool is_token(TokenKind kind)
{
	return (token.kind == kind);
}

inline bool is_token_name(const char* name)
{
	//                                         perks of string intern
	return token.kind == TOKEN_IDENTIFIER && token.identifier_name == name;
}

inline bool match_token(TokenKind kind)
{
	if (is_token(kind))
	{
		next_token();
		return true;
	}
	else
	{
		return false;
	}
}

size_t copy_token_kind_name(char* dest , size_t dest_size, TokenKind kind)
{
	size_t n = 0;

	switch (kind)
	{
	case TOKEN_INT:
		n = snprintf(dest,dest_size, "integer");
		break;
	case TOKEN_IDENTIFIER:
		n = snprintf(dest,dest_size, "identifier");
		break;
	default:
		if (kind < 128 && isprint(kind))
		{
			n = snprintf(dest,dest_size, "%c", kind);
		}
		else
		{
			n = snprintf(dest,dest_size, "<ASCII %c", kind);
		}
		break;
	}

	return n;
}

//CAUTION: This returns pointer to a static internal buffer
const char* token_kind_name(TokenKind kind)
{
	static char buf[256];

	size_t n = copy_token_kind_name(buf, sizeof(buf), kind);
	assert(n + 1 <= sizeof(buf));

	return buf;
}

inline bool expect_token(TokenKind kind)
{
	if (is_token(kind))
	{
		next_token();
		return true;
	}
	else
	{
		fatal("expected Token: %s got this : %s\n", token_kind_name(kind), token_kind_name(token.kind));
		return false;
	}
}




