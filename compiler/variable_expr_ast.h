#pragma once

#include "_global.h"
#include "_llvm.h"

#include "logger.h"

#include "expr_ast.h"
#include "pdriver.h"

using namespace llvm;

class VariableExprAST : public ExprAST {

public:
	VariableExprAST(const string pName);

	virtual llvm::Value* generate_code( pdriver& ) override;
private:
	string _name;

};