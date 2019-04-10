#include "function_ast.h"

FunctionAST::FunctionAST(unique<PrototypeAST> pProto, unique<ExprAST> pBody)
	: _funcPrototype(std::move(pProto)), _funcBody(std::move(pBody)) 
{

}

Function* FunctionAST::generate_code(pdriver& driver)
{
  // Check if function was defined extern
  Function* func = driver.globalModule.getFunction( _funcPrototype->get_name() );
  bool isExternal = (func != nullptr);

  // Define it if not defined extern
  if (!isExternal)
    func = _funcPrototype->generate_code( driver );
  
  // Something went wrong
  if (func == nullptr)
    return nullptr;

  if (!func->empty())
    return (Function*)log_error("function `" + _funcPrototype->get_name() + "` already defined!");


  // Create a new basic block to start insertion into.
  BasicBlock *basicBlock = BasicBlock::Create(driver.context, "entry", func);
  driver.builder.SetInsertPoint( basicBlock );

  // Record the function arguments in the NamedValues map.
  driver.namedValues.clear();
  for (auto& arg : func->args())
    driver.namedValues[arg.getName()] = &arg;

  if (Value *returnVal = _funcBody->generate_code( driver )) {
    // Finish off the function.
    driver.builder.CreateRet( returnVal );

    // Validate the generated code, checking for consistency.
    verifyFunction( *func );

    return func;
  }

  // Error reading body, remove function.
  func->eraseFromParent();
  return nullptr;
}
