#include "token.h"

string token::type_name() {
  switch (type)
  {
  case _EOF: return "<<EOF>>";
  case FUNC: return "func";
  case EXTERN: return "extern";
  case IDENTIFIER: return "identifier";
  case INTEGER: return "integer";
  case OP_PLUS: return "operator+";
  case OP_MINUS: return "operator-";
  case OP_MULTIPLY: return "operator*";
  case OP_DIVIDE: return "operator/";
  case OP_LESS_THAN: return "<";
  case LEFT_PARENTHESIS: return "(";
  case RIGHT_PARENTHESIS: return ")";
  case COMMA: return ",";
  default:
    return "<UNKOWN>";
  }
}

int32 token::get_precedence() {
  switch (type)
  {
  case OP_LESS_THAN: return 10;
  case OP_PLUS: return 20;
  case OP_MINUS: return 20;
  case OP_MULTIPLY: return 40;
  case OP_DIVIDE: return 40;
  default: return -1;
  }
}