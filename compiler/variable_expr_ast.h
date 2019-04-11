#pragma once

#include "_global.h"
#include "_llvm.h"

#include "logger.h"

#include "expr_ast.h"
#include "parser_context.h"

using namespace llvm;

class VariableExprAST : public ExprAST {

public:
	VariableExprAST(const string pName);

	virtual llvm::Value* generate_code( ParserContext& ) override;
private:
	string _name;

};