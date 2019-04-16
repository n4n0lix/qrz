#pragma once

#include "_global.h"
#include "_llvm.h"
#include "logger.h"

#include "gsl/gsl"

#include "token.h"

#include "ast.h"
#include "expr_ast.h"
#include "integer_expr_ast.h"
#include "variable_expr_ast.h"
#include "binary_expr_ast.h"
#include "call_expr_ast.h"
#include "prototype_expr_ast.h"
#include "function_ast.h"
#include "parser_context.h"
#include "logger.h"
#include "jit.h"

#include "llvm/Support/TargetSelect.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

using namespace gsl;

class Parser {
public:

  Parser();
  virtual ~Parser() = default;

  void    parse( deque<token>, not_null<ParserContext*> );

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

  void                next_token();

private:
  token                           _curToken;
  deque<token>                    _tokens;
  ParserContext*                  _context;
};