#include "integer_expr_ast.h"

IntegerExprAST::IntegerExprAST(string pValue) 
: _value(pValue) 
{

}
//... left of here: write tests with gtest/googletest
llvm::Value* IntegerExprAST::generate_code(pdriver& driver) {
  int64 value;
  try { 
    value = std::stoll( _value );
  } 
  catch (const std::out_of_range& e) {
    return log_error_v("int can only hold values inside [" + std::to_string(INT_MIN_VALUE) + "," + std::to_string(INT_MAX_VALUE) + "], but found value `" + _value + "`");
  }
  catch (const std::invalid_argument& e) {
    return log_error_v("`" + _value + "` is not a valid int");
  }
  
	return llvm::ConstantInt::get(driver.context, llvm::APSInt::get(value));
}