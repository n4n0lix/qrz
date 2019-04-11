#pragma once

#include "_global.h"

#include <regex>

#include "token.h"
#include "str_utils.h"


struct LexerContext {
	int32 cur_line_no = 0;
	int32 cur_line_pos = 0;
  string cur_file = "";
};

class Lexer
{
public:
  static deque<token>  scan(string path, string location);

private:
  static bool		      handle(string pRegex, std::function<token_type(LexerContext&)> func, LexerContext&, string&, deque<token>&);
};