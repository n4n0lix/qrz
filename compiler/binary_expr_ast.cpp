#include "binary_expr_ast.h"

BinaryExprAST::BinaryExprAST(token pToken, unique<ExprAST> pLeft, unique<ExprAST> pRight)
 : _token(pToken), _left(std::move(pLeft)), _right(std::move(pRight)) 
{

}

llvm::Value* BinaryExprAST::generate_code( llvm::LLVMContext& context ) {
	return nullptr; 
}

