#include "binary_expr_ast.h"

BinaryExprAST::BinaryExprAST(token pToken, unique<ExprAST> pLeft, unique<ExprAST> pRight)
 : _token(pToken), _left(std::move(pLeft)), _right(std::move(pRight)) 
{

}

Value* BinaryExprAST::generate_code(ParserContext& driver) {
  Value* left = _left->generate_code( driver );
  Value* right = _right->generate_code( driver );

  if (left == nullptr || right == nullptr)
    return nullptr;

  switch (_token.type) {
  case OP_PLUS: // +
    return driver.builder.CreateAdd(left, right, "addtmp");
  case OP_MINUS: // -
    return driver.builder.CreateSub(left, right, "subtmp");
  case OP_MULTIPLY: // *
    return driver.builder.CreateMul(left, right, "multmp");
  case OP_LESS_THAN: // <
    return driver.builder.CreateICmpULT(left, right, "cmptmp");
  default:
    return (Value*)log_error("invalid binary operator `" + _token.value + "`");
  }
}

