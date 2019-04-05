#include "parser.h"

void parser::parse(deque<token> pTokens) {
  std::cout << "\t\tPARSER\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n";

  // Initializing
  _tokens = std::move(pTokens);

  /// top ::= definition | external | expression | ';'
  next_token(); // mvoe to first token
  do {
    std::cout << (_tokens.size()+1) << " tokens left\n";
    switch(_curToken.type) {
      case FUNC:
        parse_function();
        break;
      case EXTERN:
        parse_extern();
        break;
      default:
        parse_top_level_expr();
        break;
    }
  } while(!_tokens.empty());

  std::cout << "\nvalid input!\n";

  // TODO: delete pointers from _namedValues
};

void parser::next_token() {
  if (!_tokens.empty()) {
    _curToken = _tokens.front();
    _tokens.pop_front();
  } else {
    _curToken = token();
    _curToken.type = NONE;
  }
};


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                  Grammar                 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// int_expr ::= int
unique<ExprAST> parser::parse_integer_expr() {
  // int
  auto result = std::make_unique<IntegerExprAST>( _curToken.value );
  next_token();


  return std::move(result);
};

/// paren_expr ::= ( expr )
unique<ExprAST> parser::parse_parenthesis_expr() {
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
unique<ExprAST> parser::parse_identifier_expr() {
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
unique<ExprAST> parser::parse_primary() {
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
unique<ExprAST> parser::parse_expr() {
  std::cout << "~ expr\n"; 
  auto lhs = parse_primary();

  // error
  if (lhs == nullptr)
    return nullptr;

  return parse_binary_ops_rhs( 0, std::move(lhs) );
}

/// binoprhs
///   ::= ('+' primary)*
unique<ExprAST> parser::parse_binary_ops_rhs(int exprPrec, unique<ExprAST> lhs) {
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
unique<PrototypeAST> parser::parse_prototype() {
  // FUNC NAME
  if (_curToken.type != IDENTIFIER)
    return log_error_p(_curToken, "expected identifier in function declaration");

  string fnName = _curToken.value;
  next_token(); // Eat func name 'identifier'

  // '('
  if (_curToken.type != LEFT_PARENTHESIS)
    return log_error_p(_curToken, "expected '(' in function declaration");
  next_token(); 

  // FUNC ARG NAMES
  vector<string> argNames;
  while (_curToken.type != RIGHT_PARENTHESIS) {
    if (_curToken.type != IDENTIFIER)
      return log_error_p(_curToken, "expected identifier as function argument");
    
    auto argName = _curToken.value;
    argNames.push_back(argName);

    // FUNC ARG 'identifier'
    next_token();

    if (_curToken.type != COMMA && _curToken.type != RIGHT_PARENTHESIS) {
      return log_error_p(_curToken, "expected ',' or ')' in function argument list");
    }

    // ',' 
    if (_curToken.type == COMMA)
      next_token();
  }

  // ')'
  next_token();

  return std::make_unique<PrototypeAST>( fnName, argNames );
}

/// definition ::= 'func' prototype expression
unique<FunctionAST> parser::parse_function() {
  if (_curToken.type != FUNC)
    return log_error_f(_curToken, "parser error: only call `parse_function` when `func` token encountered!");

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

/// external ::= 'extern' prototype
unique<PrototypeAST> parser::parse_extern() {
  if (_curToken.type != EXTERN)
    return log_error_p(_curToken, "parser error: only call `parse_extern` when `extern` token encountered!");

  // 'extern'
  next_token();

  return parse_prototype();
}

unique<FunctionAST> parser::parse_top_level_expr() {
  auto expr = parse_expr();
  // error
  if (expr == nullptr)
    return nullptr;

  // Create anonymous function
  auto prototype = std::make_unique<PrototypeAST>( string(""), vector<string>() );
  return std::make_unique<FunctionAST>( std::move(prototype), std::move(expr) );
}
