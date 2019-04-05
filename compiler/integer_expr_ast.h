#pragma once

#include "_global.h"
#include "_llvm.h"
#include "_defs.h"

#include "expr_ast.h"
#include "pdriver.h"

class IntegerExprAST : public ExprAST {
public:
  IntegerExprAST(string pValue);

  llvm::Value* ExprAST::generate_code( pdriver& ) override;

private:
  string _value;
};