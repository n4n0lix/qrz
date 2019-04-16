#include "jit.h"

JIT::JIT()
  : _resolver(createLegacyLookupResolver(
      _execSession,
      [this](const std::string &Name) {
        return find_mangled_symbol(Name);
      },
      [](Error Err) {
        cantFail(std::move(Err), "lookupFlags failed");
      })),
    _targetMachine(EngineBuilder().selectTarget()), _dataLayout(_targetMachine->createDataLayout()),
    _objectLayer(_execSession,
      [this](VModuleKey) {
      return ObjLayerT::Resources{
        std::make_shared<SectionMemoryManager>(), _resolver
      };
    }),
    _compileLayer(_objectLayer, SimpleCompiler(*_targetMachine))
{
  llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
}


TargetMachine& JIT::get_target_machine() {
  return *_targetMachine; 
}

VModuleKey JIT::add_module(unique<Module> pModule) {
  auto key = _execSession.allocateVModule();
  cantFail( _compileLayer.addModule( key, std::move(pModule) ));
  _moduleKeys.push_back( key );
  return key;
}

void JIT::remove_module(VModuleKey pKey) {
  _moduleKeys.erase( find( _moduleKeys, pKey ));
  cantFail( _compileLayer.removeModule( pKey ));
}

JITSymbol JIT::find_symbol(const string pName) {
  return find_mangled_symbol( mangle( pName ));
}

string JIT::mangle(const string& name) {
  string MangledName;
  {
    raw_string_ostream MangledNameStream(MangledName);
    Mangler::getNameWithPrefix(MangledNameStream, name, _dataLayout);
  }
  return MangledName;
}

JITSymbol JIT::find_mangled_symbol(const string& pName) {
#ifdef _WIN32
  // The symbol lookup of ObjectLinkingLayer uses the SymbolRef::SF_Exported
  // flag to decide whether a symbol will be visible or not, when we call
  // IRCompileLayer::findSymbolIn with ExportedSymbolsOnly set to true.
  //
  // But for Windows COFF objects, this flag is currently never set.
  // For a potential solution see: https://reviews.llvm.org/rL258665
  // For now, we allow non-exported symbols on Windows as a workaround.
  const bool ExportedSymbolsOnly = false;
#else
  const bool ExportedSymbolsOnly = true;
#endif

  // Search modules in reverse order: from last added to first added.
  // This is the opposite of the usual search order for dlsym, but makes more
  // sense in a REPL where we want to bind to the newest available definition.
  for (auto h : make_range(_moduleKeys.rbegin(), _moduleKeys.rend()))
    if (auto sym = _compileLayer.findSymbolIn(h, pName, ExportedSymbolsOnly))
      return sym;

  // If we can't find the symbol in the JIT, try looking in the host process.
  if (auto symAddr = RTDyldMemoryManager::getSymbolAddressInProcess(pName))
    return JITSymbol(symAddr, JITSymbolFlags::Exported);

#ifdef _WIN32
  // For Windows retry without "_" at beginning, as RTDyldMemoryManager uses
  // GetProcAddress and standard libraries like msvcrt.dll use names
  // with and without "_" (for example "_itoa" but "sin").
  if (pName.length() > 2 && pName[0] == '_')
    if (auto SymAddr = RTDyldMemoryManager::getSymbolAddressInProcess(pName.substr(1)))
      return JITSymbol(SymAddr, JITSymbolFlags::Exported);
#endif

  return nullptr;
}
