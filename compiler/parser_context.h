#pragma once

#include "_global.h"
#include "_llvm.h"

class ParserContext {
public:
  ParserContext() 
    : context(), builder(context), globalModule("global", context) {}

  llvm::LLVMContext                     context;
  llvm::IRBuilder<>                     builder;
  llvm::Module                          globalModule;
  std::map<std::string, llvm::Value*>   namedValues;
};