#pragma once
#include "_global.h"

enum token_type {
  _EOF,

  // keywords
  FUNC,
  EXTERN,

  // primary
  IDENTIFIER,
  INTEGER,

  // operators
  OP_PLUS,
  OP_MINUS,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_LESS_THAN,

  // misc
  COMMA,
  LEFT_PARENTHESIS,
  RIGHT_PARENTHESIS,

  // token that gets skipped during lexing
  NONE,
};

struct token {
  token_type  type = NONE;
  int32       line_no = -1;
  int32       line_pos = -1;
  string      value = "";

  string      type_name();
  int32       get_precedence();
};