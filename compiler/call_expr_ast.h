#pragma once

#include "_global.h"
#include "_llvm.h"

#include "expr_ast.h"
#include "pdriver.h"

class CallExprAST : public ExprAST {
public:
	CallExprAST(string &pCallee, vector<unique<ExprAST>> pArgs);

	llvm::Value* ExprAST::generate_code( pdriver& ) override;
private:
	string _callee;
	vector<unique<ExprAST>> _args;
};
