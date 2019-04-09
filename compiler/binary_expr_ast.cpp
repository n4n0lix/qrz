#include "binary_expr_ast.h"

BinaryExprAST::BinaryExprAST(token pToken, unique<ExprAST> pLeft, unique<ExprAST> pRight)
 : _token(pToken), _left(std::move(pLeft)), _right(std::move(pRight)) 
{

}

llvm::Value* BinaryExprAST::generate_code(pdriver& driver) {
  llvm::Value* left = _left->generate_code( driver );
  llvm::Value* right = _right->generate_code( driver );

  if (left == nullptr || right == nullptr)
    return nullptr;

  switch (_token.type) {
  case OP_PLUS: // +
    return driver.builder.CreateFAdd(left, right, "addtmp");
  case OP_MINUS: // -
    return driver.builder.CreateFSub(left, right, "subtmp");
  case OP_MULTIPLY: // *
    return driver.builder.CreateFMul(left, right, "multmp");
  case OP_LESS_THAN: // <
    return driver.builder.CreateFCmpULT(left, right, "cmptmp");
  default:
    return log_error_v("invalid binary operator `" + _token.value + "`");
  }
}

