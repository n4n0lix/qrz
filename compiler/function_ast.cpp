#include "function_ast.h"

FunctionAST::FunctionAST(unique<PrototypeAST> pProto, unique<ExprAST> pBody)
	: proto(std::move(pProto)), body(std::move(pBody)) 
{

}