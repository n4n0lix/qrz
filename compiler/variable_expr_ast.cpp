#include "variable_expr_ast.h"

VariableExprAST::VariableExprAST(const string pName)
 : _name(pName) 
{

}

llvm::Value* VariableExprAST::generate_code(pdriver& driver) {
  llvm::Value* var = driver.namedValues[_name];

  if (var == nullptr)
    log_error_v("unkown variable `" + _name + "`");

	return var;
}