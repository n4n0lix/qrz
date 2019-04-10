#include "Logger.h"

nullptr_t log_error(const string & str)
{
  std::cout << "error: " << str << "\n";
  return nullptr;
}

nullptr_t log_error(const token& tok, const string &str) {
  std::cout << "error: " << str << " @" << std::to_string(tok.line_no) << ":" << std::to_string(tok.line_pos) << "\n";
  return nullptr;
};