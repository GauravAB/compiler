#include "StringIntern.h"

extern InternStr* interns;

//string interning using a string buffer in range
const char* str_intern_range(const char* start, const char* end)
{
	size_t len = end - start;

	//TODO: change the linear time to hash time
	for (size_t i = 0; i < buf_len(interns); ++i)
	{
		//linear time compare
		if (interns[i].len == len && (strncmp(interns[i].str, start, len) == 0))
		{
			return (interns[i].str);
		}
	}

	//consider the null termination when interning
	char* str = xmalloc(len + 1);
	memcpy(str, start, len);

	str[len] = 0;

	buf_push(interns, ((InternStr){ len, str }));

	return str;
}

const char* str_intern(const char* str)
{
	return str_intern_range(str, str + strlen(str));
}

