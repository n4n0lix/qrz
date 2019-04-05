#include "variable_expr_ast.h"

VariableExprAST::VariableExprAST(const string pName)
 : _name(pName) 
{

}

llvm::Value* VariableExprAST::generate_code( llvm::LLVMContext& context ) {
	return nullptr;
}