#include "Source.h"

//prototypes
void buf_test();
void lex_test();
void str_intern_test();
void parse_test();


//globals
Intern* interns = NULL;
Token token;
const char* stream;

void init_stream(const char* src)
{
	stream = src;
	next_token();
}


int main(int argc, char** argv)
{
	buf_test();
	str_intern_test();

	lex_test();
	
	return 0;	
}



void* xmalloc(size_t num_bytes)
{
	void* ptr = malloc(num_bytes);

	if (!ptr)
	{
		perror("xmalloc failed");
		exit(1);
	}
	return ptr;
}

void lex_test()
{

	//Integer literal tests
	//make sure UINT_MAX does'nt trigger overflow
	init_stream("18446744073709551615 0xffffffffffffffff 042 0b1111");
	assert_token_int(18446744073709551615ull);
	assert_token_int(0xffffffffffffffffull);
	assert_token_int(042);
	assert_token_int(0xF);


	//float literal tests
	init_stream("3.14 .42 3e10");
	assert_token_float(3.14);
	assert_token_float(.42);
	assert_token_float(3e10);
	assert_token_eof();

	//Char literal test
	init_stream("'a' '\\n' ");
	assert_token_int('a');
	assert_token_int('\n');
	assert_token_eof();

	//String literal test
	init_stream("\"foo\" \"a\\nb\" ");
	assert_token_str("foo");
	assert_token_str("a\nb");
	assert_token_eof();

	//Generic tests
	const char* str = "XY+(XY)_HELLOW1,234+882";
	init_stream(str);
	assert_token_name("XY");
	assert_token('+');
	assert_token('(');
	assert_token_name("XY");
	assert_token(')');
	assert_token_name("_HELLOW1");
	assert_token(',');
	assert_token_int(234);
	assert_token('+');
	assert_token_int(882);
	assert_token_eof();
}


void str_intern_test()
{
	char x[] = "hello";
	char y[] = "hello";

	assert(x != y);
	const char* px = str_intern(x);
	const char* py = str_intern(y);

	assert(px == py);

	char z[] = "gaurav";

	const char* pz = str_intern(z);
	assert(px != pz);

	//	buf_free(interns);
}


void buf_test()
{
	int* asdf = NULL;
	enum { N = 1024 };

	for (int i = 0; i < N; i++)
	{
		buf_push(asdf, i);
	}

	assert(buf_len(asdf) == N);

	for (unsigned int i = 0; i < buf_len(asdf); i++)
	{
		assert(asdf[i] == i);
	}

	buf_free(asdf);
	assert(asdf == NULL);
	assert(buf_len(asdf) == 0);
}



int test_parse_expr(const char* expr)
{
	init_stream(expr);
	return parse_expr();
}

#define TEST_EXPR(x) assert(test_parse_expr(#x) == (x))

void parse_test()
{
	TEST_EXPR(1);
	TEST_EXPR((1));
	TEST_EXPR(-1);
	TEST_EXPR(1-2-3);
	TEST_EXPR(2*3+4*5);
	TEST_EXPR(2+-3);
}

#undef TEXT_EXPR


