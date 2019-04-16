#include "prototype_expr_ast.h"

PrototypeAST::PrototypeAST(string pName, vector<string> pArgs)
	: _name(pName), _argNames(pArgs) 
{

}

Function * PrototypeAST::generate_code(ParserContext& driver)
{
  // Create function
  std::vector<Type*> argTypes(_argNames.size(), Type::getInt64Ty( driver.context ));
  Type* returnType = Type::getInt64Ty( driver.context );

  FunctionType* funcType = FunctionType::get( returnType, argTypes, false );
  Function* func = Function::Create(funcType, Function::ExternalLinkage, _name, *driver.curModule );

  // Set names for arguments
  assert(_argNames.size() == func->arg_size());
  int32 i = 0;
  for (auto& arg : func->args()) {
    arg.setName( _argNames[i] );
    i++;
  }

  return func;
}

string PrototypeAST::get_name()
{
  return _name;
}
