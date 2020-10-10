#include "StretchyBuffer.h"



void* buf__grow(const void* buf, size_t  new_len, size_t elem_size)
{
	//buffer overflow checks
	assert(buf_cap(buf) <= (SIZE_MAX - 1) / 2);

	//reallocate the whole structure
	size_t new_cap = MAX(1 + 2 * buf_cap(buf), new_len);
	
	assert(new_len <= new_cap);

	//buffer overflow check
	assert(new_cap <= (SIZE_MAX - offsetof(BufHdr, buf)) / elem_size);

	//size of header plus the payload
	size_t new_size = offsetof(BufHdr, buf) + new_cap * elem_size;

	BufHdr * new_hdr = NULL;
	if (buf)
	{
		new_hdr = realloc(buf__hdr(buf), new_size);
	}
	else
	{
		//one buf header
		new_hdr = xmalloc(new_size);
		new_hdr->len = 0;
	}

	//update increased capacity
	new_hdr->cap = new_cap;

	return new_hdr->buf;
}