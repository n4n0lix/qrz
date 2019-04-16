#include "parser.h"

Parser::Parser()
{
}

void Parser::parse(deque<token> pTokens, not_null<ParserContext*> pContext) {
  log_parser() << "initializing with " << std::to_string(pTokens.size()) << " tokens\n";

  // Initializing
  _tokens = std::move(pTokens);
  _context = pContext;

  /// top ::= definition | external | expression | ';'
  next_token(); // move to first token
  do {
    //std::cout << (_tokens.size()+1) << " tokens left\n";
    switch(_curToken.type) {
      case FUNC:
        handle_function();
        break;
      case EXTERN:
        handle_extern();
        break;
      default:
        handle_top_level_expr();
        break;
    }
  } while(!_tokens.empty());

  _context->curModule->print(log_parser(), nullptr);
  _context->curModule->print(llvm::errs(), nullptr);
}

void Parser::next_token() {
  if (!_tokens.empty()) {
    _curToken = _tokens.front();
    _tokens.pop_front();
    log_parser() << "token: " << _curToken.type_name() << "\n";
  } else {
    _curToken = token();
    _curToken.type = NONE;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                  Grammar                 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// int_expr ::= int
unique<ExprAST> Parser::parse_integer_expr() {
  // int
  auto result = std::make_unique<IntegerExprAST>( _curToken.value );
  next_token();


  return std::move(result);
};

/// paren_expr ::= ( expr )
unique<ExprAST> Parser::parse_parenthesis_expr() {
  // '('
  auto lhsParen = _curToken;
  next_token(); 

  // expr
  auto expr  = parse_expr();
  if (!expr) return nullptr;
  
  // ')'
  if (_curToken.type != RIGHT_PARENTHESIS) {
    return log_error(_curToken, "closing parenthesis missing");
  }
  next_token();
    

  return std::move(expr);
};

/// identifier_expr
///   ::= identifier
///   ::= identifier '(' expr* ')'
unique<ExprAST> Parser::parse_identifier_expr() {
  // identifier
  string idName = _curToken.value;
  next_token();

  // simple var reference
  if (_curToken.type != LEFT_PARENTHESIS)
    return std::make_unique<VariableExprAST>( idName );

  // '('
  next_token();

  // expr*
  vector<unique<ExprAST>> args;
  while (_curToken.type != RIGHT_PARENTHESIS) {
    auto arg = parse_expr();

    // error happened during parsing expr
    if (arg == nullptr) {
      return nullptr;
    }

    args.push_back( std::move(arg) );

    if (_curToken.type != COMMA && _curToken.type != RIGHT_PARENTHESIS) {
      return log_error(_curToken, "expected ',' or ')' in argument list");
    }

    // eat ',' 
    if (_curToken.type == COMMA)
      next_token();
  }

  // ')'
  next_token();

  return std::make_unique<CallExprAST>( idName, std::move(args) );
}

/// primary
///   ::= identifier_expr
///   ::= int_expr
///   ::= paren_expr
unique<ExprAST> Parser::parse_primary() {
  switch (_curToken.type) {
    case IDENTIFIER:
      return parse_identifier_expr();
    case INTEGER:
      return parse_integer_expr();
    case LEFT_PARENTHESIS:
      return parse_parenthesis_expr();
    default:
      return log_error(_curToken, "expected expression but found " + _curToken.value + " (" + _curToken.type_name() + ")");
  };
}

/// expression
///   ::= primary binary_ops_rhs
///
unique<ExprAST> Parser::parse_expr() {
  auto lhs = parse_primary();

  // error
  if (lhs == nullptr)
    return nullptr;

  return parse_binary_ops_rhs( 0, std::move(lhs) );
}

/// binoprhs
///   ::= ('+' primary)*
unique<ExprAST> Parser::parse_binary_ops_rhs(int exprPrec, unique<ExprAST> lhs) {
  while(1) {
    auto tokenPrec = _curToken.get_precedence();
    
    // if this binary_ops binds at least as tightly as the current binary_ops
    // consume it, otherwise we are done
    if ( tokenPrec < exprPrec )
      return lhs;

    auto binaryOpsToken = _curToken;
    next_token();

    auto rhs = parse_primary();
    // error
    if (rhs == nullptr)
      return nullptr;

    auto nextPrec = _curToken.get_precedence();

    if (tokenPrec < nextPrec) {
      rhs = parse_binary_ops_rhs(tokenPrec+1, std::move(rhs));
      // error
      if (rhs == nullptr)
        return nullptr;
    }

    lhs = std::make_unique<BinaryExprAST>(binaryOpsToken, std::move(lhs), std::move(rhs));
  }
}

/// prototype
///   ::= id '(' id* ')'
unique<PrototypeAST> Parser::parse_prototype() {
  // FUNC NAME
  if (_curToken.type != IDENTIFIER)
    return log_error(_curToken, "expected identifier in function declaration");

  string fnName = _curToken.value;
  next_token(); // Eat func name 'identifier'

  // '('
  if (_curToken.type != LEFT_PARENTHESIS)
    return log_error(_curToken, "expected '(' in function declaration");
  next_token(); 

  // FUNC ARG NAMES
  vector<string> argNames;
  while (_curToken.type != RIGHT_PARENTHESIS) {
    if (_curToken.type != IDENTIFIER)
      return log_error(_curToken, "expected identifier as function argument");
    
    auto argName = _curToken.value;
    argNames.push_back(argName);

    // FUNC ARG 'identifier'
    next_token();

    if (_curToken.type != COMMA && _curToken.type != RIGHT_PARENTHESIS) {
      return log_error(_curToken, "expected ',' or ')' in function argument list");
    }

    // ',' 
    if (_curToken.type == COMMA)
      next_token();
  }

  // ')'
  next_token();

  return std::make_unique<PrototypeAST>( fnName, argNames );
}

void Parser::handle_function()
{
  if (auto funcAst = parse_function()) {
    if (auto* irFunc = funcAst->generate_code( *_context )) {
      fprintf(stderr, "Read function definition:");
      irFunc->print(errs());
      fprintf(stderr, "\n");
    }
  }
  else {
    // Something went wrong: skip this token
    next_token();
  }
}

/// definition ::= 'func' prototype expression
unique<FunctionAST> Parser::parse_function() {
  if (_curToken.type != FUNC)
    return log_error(_curToken, "parser error: only call `parse_function` when `func` token encountered!");

  // 'func'
  next_token();

  auto prototype = parse_prototype();
  // error
  if (prototype == nullptr)
    return nullptr;

  auto expr = parse_expr();
  // error
  if (expr == nullptr)
    return nullptr;

  return std::make_unique<FunctionAST>( std::move(prototype), std::move(expr) );
}

void Parser::handle_extern()
{
  if (auto protoAST = parse_extern()) {
    if (auto *ir = protoAST->generate_code( *_context )) {
      fprintf(stderr, "Read extern: ");
      ir->print(errs());
      fprintf(stderr, "\n");
    }
  }
  else {
    // Something went wrong: skip this token
    next_token();
  }
}

/// external ::= 'extern' prototype
unique<PrototypeAST> Parser::parse_extern() {
  if (_curToken.type != EXTERN)
    return log_error(_curToken, "parser error: only call `parse_extern` when `extern` token encountered!");

  // 'extern'
  next_token();

  return parse_prototype();
}

void Parser::handle_top_level_expr()
{
  // Evaluate a top-level expression into an anonymous function.
  if (auto funcAST = parse_top_level_expr()) {
    if (auto *ir = funcAST->generate_code( *_context )) {
      //fprintf(stderr, "Read top-level expression:");
      //ir->print(errs());
      //fprintf(stderr, "\n");



      // JIT the module containing the anonymous expression, keeping a handle so
      // we can free it later.
      auto moduleKey = _context->jit->add_module( std::move(_context->curModule) );
      _context->new_module("jit");

      // Search the JIT for the __anon_expr symbol.
      auto exprSymbol = _context->jit->find_symbol("__anon_expr");
      assert(exprSymbol && "function not found");

      // Get the symbol's address and cast it to the right type (takes no
      // arguments, returns a double) so we can call it as a native function.
      uint64 address = exprSymbol.getAddress().get();
      intptr_t addressPtr = (intptr_t)address;
      int64(*func)() = (int64(*)())addressPtr;
      auto result = func();
      std::cout << "=> " << std::to_string( result ) << "\n";

      // Delete the anonymous expression module from the JIT.
      _context->jit->remove_module(moduleKey);
    }
  }
  else {
    // Something went wrong: skip this token
    next_token();
  }
}

unique<FunctionAST> Parser::parse_top_level_expr() {
  auto expr = parse_expr();
  // error
  if (expr == nullptr)
    return nullptr;

  // Create anonymous function
  auto prototype = std::make_unique<PrototypeAST>( string("__anon_expr"), vector<string>() );
  return std::make_unique<FunctionAST>( std::move(prototype), std::move(expr) );
}