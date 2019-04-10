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
#include "pdriver.h"
#include "logger.h"

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

  void                 handle_extern();
  unique<PrototypeAST> parse_extern();
  
  void                handle_function();
  unique<FunctionAST> parse_function();

  void                handle_top_level_expr();
  unique<FunctionAST> parse_top_level_expr();

private:
  token                                 _curToken;
  deque<token>                          _tokens;
  pdriver                               _driver;
  std::map<std::string, llvm::Value*>   _namedValues;

  void                  next_token();


};