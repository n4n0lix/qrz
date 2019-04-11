#pragma once

#include "_global.h"
#include "_llvm.h"
#include "logger.h"

#include "expr_ast.h"
#include "parser_context.h"

using namespace llvm;

class CallExprAST : public ExprAST {
public:
	CallExprAST(string &pCallee, vector<unique<ExprAST>> pArgs);

	llvm::Value* ExprAST::generate_code( ParserContext& ) override;
private:
	string _callee;
	vector<unique<ExprAST>> _args;
};
