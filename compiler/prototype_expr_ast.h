#pragma once

#include "_global.h"
#include "_llvm.h"

#include "parser_context.h"

using namespace llvm;

class PrototypeAST {
public:
	PrototypeAST(string pName, vector<string> pArgs);

  llvm::Function* generate_code(ParserContext&);
  string get_name();

private:
	string _name;
	vector<string> _argNames;
};