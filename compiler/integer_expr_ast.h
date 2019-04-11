#pragma once

#include "_global.h"
#include "_llvm.h"
#include "_defs.h"
#include "logger.h"

#include "expr_ast.h"
#include "parser_context.h"

class IntegerExprAST : public ExprAST {
public:
  IntegerExprAST(string pValue);

  llvm::Value* ExprAST::generate_code( ParserContext& ) override;

private:
  string _value;
};