#include "integer_expr_ast.h"

IntegerExprAST::IntegerExprAST(string pValue) 
: _value(pValue) 
{

}

llvm::Value* IntegerExprAST::generate_code(llvm::LLVMContext& context) {
	return llvm::ConstantInt::get(context, llvm::APInt(64, _value, 10));
}