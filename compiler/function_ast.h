#pragma once

#include "_global.h"
#include "_llvm.h"
#include "logger.h"

#include "expr_ast.h"
#include "prototype_expr_ast.h"

using namespace llvm;

class FunctionAST {
public:
	FunctionAST(unique<PrototypeAST> pProto, unique<ExprAST> pBody);
	
  Function* generate_code(ParserContext&);

private:
	unique<PrototypeAST> _funcPrototype;
	unique<ExprAST> _funcBody;
};