#pragma once

#include "_global.h"
#include "_llvm.h"

#include "pdriver.h"

class ExprAST {
  public:
    virtual ~ExprAST() {}
    virtual llvm::Value* generate_code( pdriver& ) = 0;
};