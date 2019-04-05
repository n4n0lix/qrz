#pragma once

#include "_global.h"
#include "_llvm.h"

#include "token.h"

#include "ast.h"
#include "expr_ast.h"
#include "integer_expr_ast.h"
#include "variable_expr_ast.h"
#include "binary_expr_ast.h"
#include "call_expr_ast.h"
#include "prototype_expr_ast.h"
#include "function_ast.h"

class parser {
public:

  void    parse(deque<token> pTokens);


  unique<ExprAST> parse_integer_expr();
  unique<ExprAST> parse_parenthesis_expr();
  unique<ExprAST> parse_identifier_expr();
  unique<ExprAST> parse_primary();
  unique<ExprAST> parse_expr();
  unique<ExprAST> parse_binary_ops_rhs(int precedence, unique<ExprAST> lhs);
  
  unique<PrototypeAST> parse_prototype();
  unique<PrototypeAST> parse_extern();
  
  unique<FunctionAST> parse_function();
  unique<FunctionAST> parse_top_level_expr();

private:
  token                                 _curToken;
  deque<token>                          _tokens;
  unique<llvm::LLVMContext>             _context;
  unique<llvm::IRBuilder<>>             _builder;
  unique<llvm::Module>                  _globalModule;
  std::map<std::string, llvm::Value*>   _namedValues;

  void                  next_token();

  unique<ExprAST>       log_error(const token& tok, const string &str);
  unique<PrototypeAST>  log_error_p(const token& tok, const string &str);
  unique<FunctionAST>   log_error_f(const token& tok, const string &str);
  llvm::Value*          log_error_v(const string &str);
};