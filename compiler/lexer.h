#pragma once

#include "_global.h"

#include <fstream>
#include <sstream>
#include <regex>

#include "token.h"


struct ldriver {
	int32 cur_line_no = 0;
	int32 cur_line_pos = 0;
  string cur_file = "";
};

class lexer
{
public:
	deque<token>  scan(string path);

private:
	deque<token>	_tokens;
	ldriver			  _driver;
	string			  _input;

	bool		      handle(string pRegex, std::function<token_type(ldriver&)> func);
	std::string		load_file(string path);
	bool			    str_ends_with(const string &str, const string &ending);
};