#include "parser_context.h"

ParserContext::ParserContext()
  : context(), builder(context), jit(make_unique<JIT>())
{
  new_module("global");
}

llvm::Function* ParserContext::get_function(std::string funcName)
{
  // First, see if the function has already been added to the current module.
  if (auto *func = curModule->getFunction(funcName))
    return func;

  // If not, check whether we can codegen the declaration from some existing
  // prototype.
  auto prototype = funcPrototypes.find(funcName);
  if (prototype != funcPrototypes.end())
    return prototype->second->generate_code( *this );

  // If no existing prototype exists, return null.
  return nullptr;
}

void ParserContext::new_module(string name)
{
  // Open a new module.
  curModule = std::make_unique<llvm::Module>( name, context );
  curModule->setDataLayout( jit->get_target_machine().createDataLayout() );

  // Create a new pass manager attached to it.
  curModuleFPM = std::make_unique<llvm::legacy::FunctionPassManager>( curModule.get() );

  curModuleFPM->add(llvm::createInstructionCombiningPass());
  curModuleFPM->add(llvm::createReassociatePass());
  curModuleFPM->add(llvm::createGVNPass());
  curModuleFPM->add(llvm::createCFGSimplificationPass());

  curModuleFPM->doInitialization();
}


