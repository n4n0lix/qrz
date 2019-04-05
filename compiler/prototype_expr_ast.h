#pragma once

#include "_global.h"
#include "_llvm.h"

class PrototypeAST {
public:
	PrototypeAST(string pName, vector<string> pArgs);

private:
	string name;
	vector<string> args;
};