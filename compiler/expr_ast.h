#pragma once

#include "_global.h"
#include "_llvm.h"

#include "parser_context.h"

class ExprAST {
  public:
    virtual ~ExprAST() {}
    virtual llvm::Value* generate_code( ParserContext& ) = 0;
};