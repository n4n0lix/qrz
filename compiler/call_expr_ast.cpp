#include "call_expr_ast.h"

CallExprAST::CallExprAST(string &pCallee, vector<unique<ExprAST>> pArgs)
	: _callee(pCallee), _args(std::move(pArgs)) 
{

}

llvm::Value* CallExprAST::generate_code( llvm::LLVMContext& context ) { 
	 return nullptr; 
}
