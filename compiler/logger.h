#pragma once

#include "_global.h"
#include "_llvm.h"

#include "llvm/Support/raw_ostream.h"
#include <fstream>

#include "token.h"

nullptr_t       log_error(const string &str);
nullptr_t       log_error(const token& tok, const string &str);

llvm::raw_fd_ostream&  log_parser();

