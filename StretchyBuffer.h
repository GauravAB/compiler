#pragma once
#include "common.h"

#define MAX(x,y) ( (x) >= (y) ? (x) : (y))

//stretchy buffers ( like vectors in cpp)

typedef struct BufHdr
{
	size_t len;
	size_t cap;
	char buf[0]; //actual pointer to buffer

}BufHdr;


#define buf__hdr(b) ( (BufHdr*)((char*)b - offsetof(BufHdr,buf)))
#define buf__fits(b,n) (buf_len(b) + (n) <= buf_cap(b))
#define buf__fit(b,n) (buf__fits(b,n) ? 0 : ( (b) = buf__grow((b), buf_len(b) + (n), sizeof(*(b)))))

#define buf_len(b) ((b) ? buf__hdr(b)->len:0)
#define buf_cap(b) ((b) ? buf__hdr(b)->cap:0)
#define buf_push(b,...) (buf__fit(b,1), (b)[buf__hdr(b)->len++] = (__VA_ARGS__))

#define buf_free(b) ((b) ? free(buf__hdr(b)), (b) = NULL : 0)

#define buf_end(b) ((b) + buf_len(b))

void* buf__grow(const void* buf, size_t  new_len, size_t elem_size);