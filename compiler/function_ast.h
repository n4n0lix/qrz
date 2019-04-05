#pragma once

#include "_global.h"
#include "_llvm.h"

#include "expr_ast.h"
#include "prototype_expr_ast.h"

class FunctionAST {
public:
	FunctionAST(unique<PrototypeAST> pProto, unique<ExprAST> pBody);
	
private:
	unique<PrototypeAST> proto;
	unique<ExprAST> body;
};