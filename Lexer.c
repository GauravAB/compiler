#include "Lexer.h"

extern const char* stream;
extern Token token;

const char* keyword_if;
const char* keyword_for;
const char* keyword_while;

void init_keywords()
{
	keyword_if = str_intern("if");
	keyword_while = str_intern("while");
	keyword_for = str_intern("for");

	//...
}

uint8_t char_to_digit[256] =
{
	['0'] = 0,
	['1'] = 1,
	['2'] = 2,
	['3'] = 3,
	['4'] = 4,
	['5'] = 5,
	['6'] = 6,
	['7'] = 7,
	['8'] = 8,
	['9'] = 9,
	['a'] = 10,  ['A'] = 10,
	['b'] = 11,  ['B'] = 11,
	['c'] = 12,  ['C'] = 12,
	['d'] = 13,  ['D'] = 13,
	['e'] = 14,  ['E'] = 14,
	['f'] = 15,  ['F'] = 15
};

void scan_int()
{
	//default decimal
	uint64_t base = 10;
	TokenMod mod;

	if (*stream == '0')
	{
		stream++;
		if (tolower(*stream) == 'x')
		{
			stream++;
			//hexadecimal
			base = 16;

		}
		else if (tolower(*stream) == 'b')
		{
			stream++;
			base = 2;
		}
		else if (isdigit(*stream))
		{
			base = 8;
			
		}
		else
		{
			syntax_error("Invalid integer literal suffix '%c' ", *stream);
			stream++;
		}

	}

	uint64_t val = 0;
	for (;;)
	{

		uint64_t digit = char_to_digit[*stream];
		
		//abnormal behavior
		if (digit == 0 && *stream != '0')
		{
			break;
		}

		if (digit >= base)
		{
			syntax_error("Digit '%c' out of range for base %llu", *stream, base);
			digit = 0;
		}

		if (val > (UINT64_MAX - digit) / 10)
		{
			syntax_error("Integer literal overflow");
			while (isdigit(*stream))
			{
				stream++;
			}
			val = 0;
		}
		val = val * base + digit;
		stream++;
	}
	
	token.intval = val;
	token.kind = TOKEN_INT;
}

//FLOAT = [0-9]*.[0-9]*([eE][+-]?[0-9]+)?
void scan_float()
{
	const char* start = stream;
	
	//consume the whole float and validate for strod
	while (isdigit(*stream))
	{
		stream++;
	}

	if (*stream == '.')
	{
		stream++;
	}

	while (isdigit(*stream))
	{
		stream++;
	}
	
	if (tolower(*stream) == 'e')
	{
		stream++;
		if (*stream == '+' || *stream == '-')
		{
			stream++;
		}

		if (!isdigit(*stream))
		{
			syntax_error("Expected digit after float literal exponent, found '%c' .", *stream);
		}
		while (isdigit(*stream))
		{
			stream++;
		}
	}
	const char* end = stream;
	double val = strtod(start, NULL);
	if (val == HUGE_VAL || val == -HUGE_VAL)
	{
		syntax_error("Float literal overflow");
	}
	token.floatval = val;
	token.kind = TOKEN_FLOAT;

}

char escape_to_char[256] =
 {
	['n'] = '\n',
	['r'] = '\r',
	['t'] = '\t',
	['v'] = '\v',
	['b'] = '\b',
	['a'] = '\a',
	['0'] = 0
};

void scan_char()
{
	assert(*stream == '\'');
	stream++;

	char val = 0;

	if (*stream == '\'')
	{
		syntax_error("Char literal cannot be empty");
		stream++;
	}
	else if (*stream == '\n')
	{
		syntax_error("Char literal cannot contain newline");

	}
	else if (*stream == '\\')
	{
		stream++;
		val = escape_to_char[*stream];
		if (val == 0 && *stream != '0')
		{
			syntax_error("Invalid char literal escape '\\%c'", *stream);
		}
		stream++;
	}
	else
	{
		val = *stream;
		stream++;
	}

	if (*stream != '\'')
	{
		syntax_error("Expecting closing char quote , got '%c'", *stream);
	}
	else
	{
		stream++;
	}

	token.kind = TOKEN_INT;
	token.intval = val;
	token.mod = TOKENMOD_CHAR;
}



void scan_str()
{
	
	assert(*stream == '"');
	stream++;
	char* str = NULL;

	while (*stream && *stream != '"')
	{
		char val = *stream;
		if (val == '\n')
		{
			syntax_error("String literal cannot contain newline");
			stream++;
		}
		else if (val == '\\')
		{
			stream++;
		    val = escape_to_char[*stream];
			if (val == 0 && *stream != '0')
			{
				syntax_error("Invalid string literal escape '\\%c'", *stream);
			};
		}
		buf_push(str, val);
		stream++;
	}

	if (*stream)
	{
		assert(*stream == '"');
		stream++;
	}
	else
	{
		syntax_error("Unexpected end of file within quoted string literal");
	}

	buf_push(str, 0);
	token.kind = TOKEN_STR;
	token.strval = str;
}


void next_token()
{
	
top:
	token.start = stream;
	token.mod = 0;

	switch (*stream)
	{
	case ' ': case '\n': case '\r': case '\t': case '\v':
		while (isspace(*stream))
		{
			stream++;
		}
		goto top;
		break;
	case '\'':
		scan_char();
		break;
	case '\"':
		scan_str();
		break;
	case '.':
		token.start = stream;
		scan_float();
		break;
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		//forward lookahead for '.'
		if (isdigit(*stream))
		{
			stream++;
		}
		if (*stream == '.' || tolower(*stream) == 'e')
		{
			stream = token.start;
			scan_float();
		}
		else
		{
			stream = token.start;
			scan_int();
		}
		
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
		printf("TOKEN VALUE : %llu\n", token.intval);
		break;
	case TOKEN_FLOAT:
		printf("TOKEN FLOAT : %d\n", token.floatval);
		break;
	case TOKEN_IDENTIFIER:
		printf("TOKEN NAME: %.*s\n", (int)(token.end - token.start), token.start);
		break;
	default:
		printf("TOKEN '%c' \n", token.kind);
		break;
	}
}


void syntax_error(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("Syntax Error: ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);

}


