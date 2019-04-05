#pragma once

#include "_global.h"
#include "_llvm.h"

#include "expr_ast.h"

class IntegerExprAST : public ExprAST {
public:
  IntegerExprAST(string pValue);

  llvm::Value* ExprAST::generate_code( llvm::LLVMContext& ) override;

private:
  string _value;
};