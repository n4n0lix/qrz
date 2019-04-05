#pragma once

#include "_global.h"
#include "_llvm.h"

class pdriver {
public:
  pdriver() 
    : context(), builder(context), globalModule("global", context) {}

  llvm::LLVMContext                     context;
  llvm::IRBuilder<>                     builder;
  llvm::Module                          globalModule;
  std::map<std::string, llvm::Value*>   namedValues;
};