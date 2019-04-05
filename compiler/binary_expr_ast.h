#pragma once

#include "_global.h"
#include "_llvm.h"

#include "logger.h"

#include "token.h"
#include "expr_ast.h"
#include "pdriver.h"

class BinaryExprAST : public ExprAST {
public:
	BinaryExprAST(token pToken, unique<ExprAST> pLeft, unique<ExprAST> pRight);

	llvm::Value* ExprAST::generate_code( pdriver& ) override;
private:
	token _token;
	unique<ExprAST> _left;
	unique<ExprAST> _right;
};