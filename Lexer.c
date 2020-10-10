#include "Lexer.h"

extern const char* stream;
extern Token token;



void next_token()
{
	token.start = stream;
	int val = 0;

	switch (*stream)
	{
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		while (isdigit(*stream))
		{
			val *= 10;
			val += *stream++ - '0';
		}
		token.kind = TOKEN_INT;
		token.intval = val;
		break;
	case 'A': case 'B': case 'C': case 'D':	case 'E': case 'F':	case 'G': case 'H':	case 'I': case 'J':	case 'K':
	case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
	case 'W': case 'X': case 'Y': case 'Z': case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
	case 'h': case 'i': case 'j': case 'k':	case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
	case 's': case 't':	case 'u': case 'v': case 'w': case 'x':	case 'y': case 'z': case '_':

		while (isalnum(*stream) || *stream == '_')
		{
			stream++;
		}
		token.kind = TOKEN_IDENTIFIER;
		token.identifier_name = str_intern_range(token.start, stream);
		break;
	default:
		token.kind = *stream++;
		break;
	}

	token.end = stream;
}

void print_token(Token token)
{
	switch (token.kind)
	{
	case TOKEN_INT:
		printf("TOKEN VALUE : %d\n", token.intval);
		break;
	case TOKEN_IDENTIFIER:
		printf("TOKEN NAME: %.*s\n", (int)(token.end - token.start), token.start);
		break;
	default:
		printf("TOKEN '%c' \n", token.kind);
		break;
	}
}

