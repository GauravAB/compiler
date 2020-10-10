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
	lex_test();
	str_intern_test();
	parse_test();
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
	char* source = "XY+(XY)HELLOW123456+789ORLD";
	init_stream(source);

	while (token.kind)
	{
		print_token(token);
		next_token();
	}

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


