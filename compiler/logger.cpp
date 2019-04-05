#include "Logger.h"

unique<ExprAST> log_error(const token& tok, const string &str) {
  std::cout << "error: " << str << " @" << std::to_string(tok.line_no) << ":" << std::to_string(tok.line_pos) << "\n";
  return nullptr;
};

unique<PrototypeAST> log_error_p(const token& tok, const string &str) {
  log_error(tok, str);
  return nullptr;
};

unique<FunctionAST> log_error_f(const token& tok, const string &str) {
  log_error(tok, str);
  return nullptr;
};

llvm::Value* log_error_v(const string &str) {
  std::cout << "error: " << str << "\n";
  return nullptr;
}