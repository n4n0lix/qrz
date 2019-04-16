#include "function_ast.h"

FunctionAST::FunctionAST(unique<PrototypeAST> pProto, unique<ExprAST> pBody)
	: _funcPrototype(std::move(pProto)), _funcBody(std::move(pBody)) 
{

}

Function* FunctionAST::generate_code(ParserContext& driver)
{
  // Transfer ownership of the prototype to the FunctionProtos map, but keep a
  // reference to it for use below.
  auto funcName = _funcPrototype->get_name();
  driver.funcPrototypes[funcName] = std::move(_funcPrototype);
  Function *func = driver.get_function(funcName);
  if (!func)
    return nullptr;

  // Create a new basic block to start insertion into.
  BasicBlock *basicBlock = BasicBlock::Create( driver.context, "entry", func);
  driver.builder.SetInsertPoint( basicBlock );

  // Record the function arguments in the NamedValues map.
  driver.namedValues.clear();
  for (auto& arg : func->args())
    driver.namedValues[arg.getName()] = &arg;

  if (Value *body = _funcBody->generate_code( driver )) {
    // Finish off the function.
    driver.builder.CreateRet( body );

    // Validate the generated code, checking for consistency.
    llvm::verifyFunction( *func );

    driver.curModuleFPM->run( *func );

    return func;
  }

  // Error reading body, remove function.
  func->eraseFromParent();
  return nullptr;
}
