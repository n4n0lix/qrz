#pragma once

#include "_global.h"
#include "_llvm.h"

#include "expr_ast.h"

class CallExprAST : public ExprAST {
public:
	CallExprAST(string &pCallee, vector<unique<ExprAST>> pArgs);

	llvm::Value* ExprAST::generate_code( llvm::LLVMContext& ) override;
private:
	string _callee;
	vector<unique<ExprAST>> _args;
};
