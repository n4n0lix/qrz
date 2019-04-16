#pragma once

#include "_global.h"
#include "_llvm.h"

#include "parser_context.h"

using namespace llvm;

class ExprAST {
  public:
    virtual ~ExprAST() {}
    virtual Value* generate_code( ParserContext& ) = 0;
};