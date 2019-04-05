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
    ... left of here: llvm expects always two operands have same type, and result has same type as well
        but how are we then handling for example  var1(4) < var2(5) = true?
  case OP_LESS_THAN: // <
    left = driver.builder.CreateFCmpULT(left, right, "cmptmp");
    // Convert bool 0/1 to double 0.0 or 1.0
    return driver.builder.CreateUIToFP(left, llvm::Type::getDoubleTy(driver.context), "booltmp");
  default:
    return log_error_v("invalid binary operator `" + _token.value + "`");
  }

	return nullptr; 
}

