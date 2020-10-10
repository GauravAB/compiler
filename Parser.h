#pragma once
#include "common.h"
#include "Lexer.h"

int parse_expr();
int parse_expr1();
int parse_expr2();
int parse_expr3();



//aux fns
inline bool is_token(TokenKind kind);
inline bool match_token(TokenKind kind);
const char* token_kind_name(TokenKind kind);
inline bool expect_token(TokenKind kind);
void fatal(const char* fmt, ...);

