#pragma once

#include "_global.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/LambdaResolver.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/RTDyldMemoryManager.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Mangler.h"
#include "llvm/Support/DynamicLibrary.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace llvm;
using namespace llvm::orc;

class JIT {
public:
  using ObjLayerT = LegacyRTDyldObjectLinkingLayer;
  using CompileLayerT = LegacyIRCompileLayer<ObjLayerT, SimpleCompiler>;

  JIT();
  virtual ~JIT() = default;

  TargetMachine&  get_target_machine();
  VModuleKey      add_module(unique<Module> module);
  void            remove_module(VModuleKey key);
  JITSymbol       find_symbol(const string name);

private:
  std::string mangle(const string &name);
  JITSymbol   find_mangled_symbol(const string &name);

  ExecutionSession _execSession;
  shared<SymbolResolver> _resolver;
  unique<TargetMachine> _targetMachine;
  const DataLayout _dataLayout;
  ObjLayerT _objectLayer;
  CompileLayerT _compileLayer;
  std::vector<VModuleKey> _moduleKeys;
};