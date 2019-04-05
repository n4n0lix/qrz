#pragma once

#include "_global.h"
#include "_llvm.h"

#include "token.h"
#include "expr_ast.h"
#include "prototype_expr_ast.h"
#include "function_ast.h"


unique<ExprAST>       log_error(const token& tok, const string &str);
unique<PrototypeAST>  log_error_p(const token& tok, const string &str);
unique<FunctionAST>   log_error_f(const token& tok, const string &str);
llvm::Value*          log_error_v(const string &str);

