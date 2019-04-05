#pragma once

#include "_global.h"
#include "_llvm.h"

class ExprAST {
  public:
    virtual ~ExprAST() {}
    virtual llvm::Value* generate_code( llvm::LLVMContext& ) = 0;
};