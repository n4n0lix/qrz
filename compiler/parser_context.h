#pragma once

#include "_global.h"
#include "_llvm.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

class ParserContext {
public:
  ParserContext() 
    : context(), builder(context), globalModule("global", context), funcOptimizer(&globalModule)
  {

    // Do simple "peephole" optimizations and bit-twiddling optzns.
    funcOptimizer.add( llvm::createInstructionCombiningPass() );

    // Reassociate expressions.
    funcOptimizer.add( llvm::createReassociatePass() );

    // Eliminate Common SubExpressions.
    funcOptimizer.add( llvm::createGVNPass() );

    // Simplify the control flow graph (deleting unreachable blocks, etc).
    funcOptimizer.add( llvm::createCFGSimplificationPass() );

    funcOptimizer.doInitialization();
  }

  llvm::LLVMContext                     context;
  llvm::IRBuilder<>                     builder;
  llvm::Module                          globalModule;
  std::map<std::string, llvm::Value*>   namedValues;
  llvm::legacy::FunctionPassManager     funcOptimizer;
};