#include "call_expr_ast.h"

CallExprAST::CallExprAST(string &pCallee, vector<unique<ExprAST>> pArgs)
	: _callee(pCallee), _args(std::move(pArgs)) 
{

}

Value* CallExprAST::generate_code(pdriver& driver) {
  // Look up the name in the global module table.
  Function* calleeFunc = driver.globalModule.getFunction( _callee );
  if (!calleeFunc)
    return (Value*)log_error("unknown function `" + _callee + "` referenced");

  // Argument amount missmatch
  if (calleeFunc->arg_size() != _args.size())
    return (Value*)log_error("expected #args: " + std::to_string(_args.size()) + ", but found: " + std::to_string(calleeFunc->arg_size()));

  // Generate list of argument values
  std::vector<Value*> argsV;
  for (int32 i = 0; i < _args.size(); i++) {
    auto value = _args[i]->generate_code(driver);

    if (value == nullptr)
      return nullptr;

    argsV.push_back( value );
  }

  // Create instruction
  return driver.builder.CreateCall(calleeFunc, argsV, "calltmp");
}
