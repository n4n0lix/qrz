#pragma once

#include "_global.h"
#include "_llvm.h"

#include "jit.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

#include "prototype_expr_ast.h"

class ParserContext {
public:
  ParserContext();
  virtual ~ParserContext() = default;

  llvm::Function* get_function(string funcName);
  void            new_module(string name);


  llvm::LLVMContext                         context;
  llvm::IRBuilder<>                         builder;

  std::map<std::string, llvm::Value*>           namedValues;
  std::map<std::string, unique<PrototypeAST>>   funcPrototypes;

  unique<JIT>                               jit;
  unique<llvm::Module>                      curModule;
  unique<llvm::legacy::FunctionPassManager> curModuleFPM;
};