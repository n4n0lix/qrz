#include "integer_expr_ast.h"

IntegerExprAST::IntegerExprAST(string pValue) 
: _value(pValue) 
{

}

llvm::Value* IntegerExprAST::generate_code(pdriver& driver) {
	return llvm::ConstantInt::get(driver.context, llvm::APInt(INTEGER_BIT_SIZE, _value, INTEGER_RADIX));
}