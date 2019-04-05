#pragma once

#include "_global.h"
#include "_llvm.h"

#include "expr_ast.h"

class VariableExprAST : public ExprAST {

public:
	VariableExprAST(const string pName);

	virtual llvm::Value* generate_code( llvm::LLVMContext& ) override;
private:
	string _name;

};