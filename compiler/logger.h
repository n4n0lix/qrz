#pragma once

#include "_global.h"
#include "_llvm.h"

#include "token.h"

nullptr_t       log_error(const string &str);
nullptr_t       log_error(const token& tok, const string &str);

