#pragma once
#include "common.h"
#include "StretchyBuffer.h"

typedef struct Intern
{
	size_t len;
	const char* str;
}Intern;


const char* str_intern_range(const char* start, const char* end);
const char* str_intern(const char* str);