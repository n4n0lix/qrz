#include "call_expr_ast.h"

CallExprAST::CallExprAST(string &pCallee, vector<unique<ExprAST>> pArgs)
	: _callee(pCallee), _args(std::move(pArgs)) 
{

}

llvm::Value* CallExprAST::generate_code(pdriver& driver) {
  // Look up the name in the global module table.
  llvm::Function* calleeFunc = driver.globalModule.getFunction( _callee );
  if (!calleeFunc)
    return log_error_v("unknown function `" + _callee + "` referenced");

  // Argument amount missmatch
  if (calleeFunc->arg_size() != _args.size())
    return log_error_v("expected #args: " + std::to_string(_args.size()) + ", but found: " + std::to_string(calleeFunc->arg_size()));

  // Generate list of argument values
  std::vector<llvm::Value*> argsV;
  for (int32 i = 0; i < _args.size(); i++) {
    auto value = _args[i]->generate_code(driver);

    if (value == nullptr)
      return nullptr;

    argsV.push_back( value );
  }

  // Create instruction
  return driver.builder.CreateCall(calleeFunc, argsV, "calltmp");
}
