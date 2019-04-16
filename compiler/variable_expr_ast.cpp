#include "variable_expr_ast.h"

VariableExprAST::VariableExprAST(const string pName)
 : _name(pName) 
{

}

Value* VariableExprAST::generate_code(ParserContext& driver) {
  Value* var = driver.namedValues[_name];

  if (var == nullptr)
    (Value*)log_error("unkown variable `" + _name + "`");

	return var;
}